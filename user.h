#ifndef USER_H
#define USER_H

#include "types.h"

#define ID_MONITOR   2
#define ID_ALARM     3
#define ID_PRODUCER  4
#define ID_CONSUMER  5

void config_user(void);
TASK task_monitor(void);
TASK task_alarm(void);
TASK task_producer(void);
TASK task_consumer(void);

#endif /* USER_H */
