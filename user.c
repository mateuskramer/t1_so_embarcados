#include "user.h"
#include "kernel.h"
#include "io.h"
#include "sync.h"
#include "com.h"
#include <xc.h>

static mutex_t adc_mutex;
static pipe_t  adc_pipe;
static sem_t   alarm_sem;

static void on_int0(void)
{
    static uint8_t i;
    for (i = 0; i < r_queue.size; i++) {
        if (r_queue.TASKS[i].task_id == ID_ALARM) {
            r_queue.TASKS[i].task_stack.stack_size = 0;
            r_queue.TASKS[i].task_state = READY;
            return;
        }
    }
    if (r_queue.size <= MAX_USER_TASKS) {
        r_queue.TASKS[r_queue.size].task_id               = ID_ALARM;
        r_queue.TASKS[r_queue.size].task_delay             = 0;
        r_queue.TASKS[r_queue.size].task_priority          = 2;
        r_queue.TASKS[r_queue.size].task_ptr               = task_alarm;
        r_queue.TASKS[r_queue.size].task_state             = READY;
        r_queue.TASKS[r_queue.size].task_stack.stack_size  = 0;
        r_queue.size++;
    }
}

void config_user(void)
{
    asm("global _task_monitor, _task_alarm, _task_producer, _task_consumer, _on_int0");

    TRISCbits.RC2 = 0;  LATCbits.LATC2 = 0;
    TRISCbits.RC3 = 0;  LATCbits.LATC3 = 0;
    TRISCbits.RC4 = 0;  LATCbits.LATC4 = 0;

    adc_config();
    adc_on();

    pwm_config(99);
    pwm_on();

    mutex_init(&adc_mutex);
    pipe_init(&adc_pipe);
    sem_init(&alarm_sem, 0);

    ext_int_config(EXT_INT_RISING, on_int0);
    ext_int_enable();
}

/* Luz de alerta: bloqueia no semaforo, pisca RC4 quando alarme dispara */
TASK task_monitor(void)
{
    while (1)
    {
        sem_wait(&alarm_sem);

        /* pisca RC4 rapidamente 6 vezes */
        static uint8_t blink;
        static volatile uint16_t d;
        for (blink = 0; blink < 6; blink++)
        {
            LATCbits.LATC4 ^= 1;
            for (d = 0; d < 40000; d++);
        }
        LATCbits.LATC4 = 0;
    }
}

/* One-shot: criada/reativada pelo INT0, executa uma vez e dorme */
TASK task_alarm(void)
{
    mutex_lock(&adc_mutex);
    uint16_t adc = adc_read();
    mutex_unlock(&adc_mutex);

    LATCbits.LATC3 = (adc > 512) ? 1 : 0;
    sem_post(&alarm_sem);

    os_task_change_state(WAITING, NULL);
}

/* Produtora: le ADC e envia para o pipe */
TASK task_producer(void)
{
    while (1)
    {
        mutex_lock(&adc_mutex);
        uint16_t adc = adc_read();
        mutex_unlock(&adc_mutex);

        pipe_write(&adc_pipe, (char)(adc >> 2));
    }
}

/* Consumidora: recebe do pipe e ajusta PWM */
TASK task_consumer(void)
{
    while (1)
    {
        char dado;
        pipe_read(&adc_pipe, &dado);
        pwm_set_duty((uint16_t)((uint8_t)dado * 4));
    }
}
