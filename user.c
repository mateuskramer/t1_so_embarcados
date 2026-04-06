#include "user.h"
#include <xc.h>
#include "kernel.h"

void config_user()
{
    TRISCbits.RC6       = 0;
    TRISCbits.RC7       = 0;
    TRISDbits.RD0       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELCbits.ANSC6    = 0;
    ANSELCbits.ANSC7    = 0;
    
    asm("global _LED_1, _LED_2, _LED_3");
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
        PORTCbits.RC6 = ~PORTCbits.RC6;
    }    
}

TASK LED_2()
{
    while (1) {
        PORTCbits.RC7 = ~PORTCbits.RC7;
        os_delay(100);
    }    
}

TASK LED_3()
{
    while (1) {
        PORTDbits.RD0 = ~PORTDbits.RD0;
    }    
}
       


