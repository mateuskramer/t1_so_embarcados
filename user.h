#ifndef USER_H
#define USER_H

#include "types.h"

#define ID_BLINK1  2
#define ID_BLINK2  3
#define ID_BLINK3  4
#define ID_BLINK4  5

void config_user(void);
TASK task_blink1(void);
TASK task_blink2(void);
TASK task_blink3(void);
TASK task_blink4(void);

#endif /* USER_H */
