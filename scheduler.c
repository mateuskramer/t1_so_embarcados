#include "scheduler.h"
#include "types.h"
#include "kernel.h"

// Fila de aptos
extern ready_queue_t r_queue;


void scheduler()
{
  r_queue.pos_task_running = RR_scheduler();
  r_queue.task_running     = &r_queue.TASKS[r_queue.pos_task_running];
}

uint8_t RR_scheduler()
{
    uint8_t prox = r_queue.pos_task_running;
    
    do {
        prox = (prox+1) % r_queue.size;                
    } while (r_queue.TASKS[prox].task_state != READY ||
             r_queue.TASKS[prox].task_ptr == idle);
    
    return prox;
}
