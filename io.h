#ifndef IO_H
#define	IO_H

#include <stdint.h>
#include <xc.h>

void adc_config(void);
void adc_on(void);
uint16_t adc_read(void);

void pwm_config(uint8_t period);
void pwm_on(void);
void pwm_off(void);
void pwm_set_duty(uint16_t duty);


/** Borda de disparo da interrupção externa. */
typedef enum {
    EXT_INT_RISING  = 0,   /**< Dispara na borda de subida  */
    EXT_INT_FALLING = 1    /**< Dispara na borda de descida */
} ext_int_edge_t;

typedef void (*ext_int_callback_t)(void);

void ext_int_config(ext_int_edge_t edge, ext_int_callback_t callback);
void ext_int_enable(void);
void ext_int_disable(void);

#endif	/* IO_H */