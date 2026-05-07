#include "io.h"

void adc_config(void)
{
    ADCON0bits.CHS      = 0b00000;  // Canal zero (AN0)
    ADCON1bits.PVCFG    = 0b00;     // VDD
    ADCON1bits.NVCFG    = 0b00;     // ...
    ADCON2bits.ADFM     = 1;
    ADCON2bits.ACQT     = 0b110;
    ADCON2bits.ADCS     = 0b100;
    TRISAbits.RA0       = 1;
    ANSELAbits.ANSA0    = 1;
}

void adc_on(void)
{
    ADCON0bits.ADON = 1;
}

uint16_t adc_read(void)
{
    uint16_t dados_adc;
    ADCON0bits.GO = 1;
    while (ADCON0bits.GODONE);
    dados_adc = ADRES;
    return dados_adc;
}


 
 
void pwm_config(uint8_t period)
{
    /* RC2 como saida LOW antes de qualquer coisa: evita EN1 flutuar HIGH */
    LATCbits.LATC2   = 0;
    TRISCbits.RC2    = 0;

    T2CONbits.TMR2ON = 0;
    PR2              = period;
    CCP1CON          = 0x00;   /* CCP desabilitado; pwm_set_duty ativa quando necessario */
    CCPR1L           = 0;
    CCP1CONbits.DC1B = 0;
    T2CONbits.T2CKPS = 0b01;
}
 
void pwm_on(void)
{

    TMR2             = 0;
    T2CONbits.TMR2ON = 1;
    TRISCbits.RC2 = 0;
}
 
void pwm_off(void)
{
    T2CONbits.TMR2ON = 0; 
    TRISCbits.RC2    = 1;  
}
 
void pwm_set_duty(uint16_t duty)
{
    if (duty > 1023) duty = 1023;

    if (duty == 0) {
        /* CCP1 duty=0 ainda gera um pulso curto no PIC18; desabilitar
         * o modulo e forcar o pino LOW para realmente parar o motor.  */
        CCP1CON = 0x00;
        TRISCbits.RC2 = 0;
        LATCbits.LATC2 = 0;
    } else {
        CCP1CON = 0x0C;
        CCPR1L = (uint8_t)(duty >> 2);
        CCP1CONbits.DC1B = (uint8_t)(duty & 0x03);
    }
}
 
 

static ext_int_callback_t ext_int_user_callback = 0;
 
void ext_int_config(ext_int_edge_t edge, ext_int_callback_t callback)
{

    ext_int_user_callback = callback;
 
    /* 2. RB0 como entrada digital. */
    TRISBbits.RB0    = 1;
    ANSELBbits.ANSB0 = 0;   /* garante modo digital (sem anal�gico) */
 
    /* 3. Seleciona a borda de disparo.
     *    INTCON2bits.INTEDG0 = 1 ? subida, 0 ? descida.             */
    INTCON2bits.INTEDG0 = (edge == EXT_INT_RISING) ? 1 : 0;
 
    /* 4. Limpa flag antes de habilitar para n�o disparar spuriously.  */
    INTCONbits.INT0IF = 0;
}
 
void ext_int_enable(void)
{
    INTCONbits.INT0IF = 0;   /* Limpa flag residual         */
    INTCONbits.INT0IE = 1;   /* Habilita interrup��o INT0   */
}
 
void ext_int_disable(void)
{
    INTCONbits.INT0IE = 0;   /* Desabilita interrup��o INT0 */
}
 
/**
 * Chamada internamente pela ISR em hw.c quando INT0IF � detectado.
 * N�o deve ser chamada diretamente pelo usu�rio.
 */
void ext_int_isr_handler(void)
{
    INTCONbits.INT0IF = 0;           /* Limpa flag na ISR              */
    if (ext_int_user_callback != 0) {
        ext_int_user_callback();     /* Executa callback do usu�rio    */
    }
}
