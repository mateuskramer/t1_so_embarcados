#include "user.h"
#include "kernel.h"
#include "io.h"
#include "sync.h"
#include "com.h"
#include <xc.h>

static mutex_t adc_mutex;
static pipe_t  adc_pipe;

void config_user(void)
{
    asm("global _task_blink1, _task_blink2, _task_blink3, _task_blink4");

    TRISCbits.RC1 = 0;  LATCbits.LATC1 = 0;
    TRISCbits.RC2 = 0;  LATCbits.LATC2 = 0;
    TRISCbits.RC3 = 0;  LATCbits.LATC3 = 0;
    TRISCbits.RC4 = 0;  LATCbits.LATC4 = 0;

    adc_config();
    adc_on();

    pwm_config(99);
    pwm_on();

    mutex_init(&adc_mutex);
    pipe_init(&adc_pipe);
}

TASK task_blink1(void)
{
    while (1) { LATCbits.LATC1 ^= 1; }
}

TASK task_blink2(void)
{
    while (1) { LATCbits.LATC2 ^= 1; }
}

/* Produtora: le ADC e envia para o pipe */
TASK task_blink3(void)
{
    while (1)
    {
        mutex_lock(&adc_mutex);
        uint16_t adc = adc_read();
        mutex_unlock(&adc_mutex);

        pipe_write(&adc_pipe, (char)(adc >> 2));  /* escala 0-1023 para 0-255 */
    }
}

/* Consumidora: recebe do pipe e ajusta PWM */
TASK task_blink4(void)
{
    while (1)
    {
        char dado;
        pipe_read(&adc_pipe, &dado);
        pwm_set_duty((uint16_t)((uint8_t)dado * 4));  /* escala 0-255 para 0-1020 */
    }
}
