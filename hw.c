#include "hw.h"
#include "kernel.h"
#include "scheduler.h"

void setup_hardware(void)
{
    // Configuração do timer
    INTCONbits.TMR0IE   = 1;
    INTCONbits.TMR0IF   = 0;
    T0CONbits.T08BIT    = 1; // 8 bits
    T0CONbits.T0CS      = 0; // Instrução interna
    T0CONbits.PSA       = 0; // Ativa preescaler
    T0CONbits.T0PS      = 0b111; // 1:256
    T0CONbits.TMR0ON    = 1; // Ativa timer
    TMR0                = 0;
}

void __interrupt() ISR(void)
{
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;
        SAVE_CONTEXT(READY);
        scheduler();
        RESTORE_CONTEXT();
    }
}
