#ifndef HW_H
#define	HW_H

#include <xc.h>

void setup_hardware(void);
void __interrupt() ISR(void);

void ext_int_isr_handler(void);



#endif	/* HW_H */

