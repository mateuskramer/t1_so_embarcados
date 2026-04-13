#include "user.h"
#include <xc.h>
#include "kernel.h"
#include "sync.h"

sem_t s;

void config_user()
{
    TRISCbits.RC6       = 0;
    TRISCbits.RC7       = 0;
    TRISDbits.RD0       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELCbits.ANSC6    = 0;
    ANSELCbits.ANSC7    = 0;
    
    asm("global _LED_1, _LED_2, _LED_3");
    
    sem_init(&s, 0);    
}

TASK acionaMotor()
{
    while (1) {
        
    }
}

TASK ligaLed()
{
    while (1) {
        
    }    
}

TASK apagaLed()
{
    while (1) {
        
    }    
}

TASK LED_1()
{
    while (1) {
        //sem_post(&controle_tomada_decisao);
        PORTCbits.RC6 = ~PORTCbits.RC6;
        sem_wait(&s);
    }    
}

TASK LED_2()
{
    while (1) {
        
        //sem_post(&controle_leitura);
        PORTCbits.RC7 = ~PORTCbits.RC7;
        sem_post(&s);
        //os_delay(5);
    }    
}

TASK LED_3()
{
    while (1) {
        PORTDbits.RD0 = ~PORTDbits.RD0;
        //os_delay(1);
        //os_task_change_state(WAITING, NULL);
    }    
}
