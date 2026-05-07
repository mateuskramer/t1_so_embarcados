#include "kernel.h"
#include "user.h"

int main()
{
    os_config();
    os_create_task(ID_BLINK1, task_blink1, 1);
    os_create_task(ID_BLINK2, task_blink2, 1);
    os_create_task(ID_BLINK3, task_blink3, 2);
    os_create_task(ID_BLINK4, task_blink4, 2);
    
    os_start();
    while (1){};
    return 0;
}
