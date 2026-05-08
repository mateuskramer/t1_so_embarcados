#include "kernel.h"
#include "user.h"

int main()
{
    os_config();
    os_create_task(ID_MONITOR,  task_monitor,  2);
    os_create_task(ID_PRODUCER, task_producer, 2);
    os_create_task(ID_CONSUMER, task_consumer, 2);

    os_start();
    while (1){};
    return 0;
}
