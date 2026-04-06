#include "sync.h"
#include "kernel.h"
#include "scheduler.h"

// Fila de aptos
extern ready_queue_t r_queue;


void sem_init(sem_t *sem, uint8_t valor)
{
    sem->contador           = valor;
    sem->qtd_tasks_fila     = 0;
    sem->pos_input          = 0;
    sem->pos_output         = 0;
}

void sem_wait(sem_t *sem)
{
    DISABLE_ALL_INTERRUPTS();
    
    sem->contador--;
    if (sem->contador < 0) {
        sem->fila[sem->pos_input] = r_queue.pos_task_running;
        sem->qtd_tasks_fila++;
        sem->pos_input = (sem->pos_input + 1) % MAX_USER_TASKS;
        // Troca de contexto
        SAVE_CONTEXT(WAITING_SEM);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    ENABLE_ALL_INTERRUPTS();
}

void sem_post(sem_t *sem)
{
    DISABLE_ALL_INTERRUPTS();
    
    ENABLE_ALL_INTERRUPTS();    
}

