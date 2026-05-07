#include "sync.h"
#include "kernel.h"
#include "scheduler.h"

// Fila de aptos
extern ready_queue_t r_queue;


void sem_init(sem_t *sem, uint8_t valor)
{
    sem->contador           = valor;
    sem->pos_input          = 0;
    sem->pos_output         = 0;
}

void sem_wait(sem_t *sem)
{
    DISABLE_ALL_INTERRUPTS();
    
    sem->contador--;
    if (sem->contador < 0) {
        sem->fila[sem->pos_input] = r_queue.pos_task_running;
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
    
    sem->contador++;
    if (sem->contador <= 0) {
        // Libera o processo bloqueado a mais tempo
        r_queue.TASKS[sem->fila[sem->pos_output]].task_state = READY;
        sem->pos_output = (sem->pos_output + 1) % MAX_USER_TASKS;
    }
    
    ENABLE_ALL_INTERRUPTS();    
}


// Mutex


//Inicializa um mutex
void mutex_init(mutex_t *mutex)
{
    mutex->locked       = 0;            // Desbloqueado
    mutex->owner_id     = 255;          // Nenhum proprietário (255 = inválido)
    mutex->waiting_count = 0;
    mutex->pos_input    = 0;
    mutex->pos_output   = 0;
}

//Adquire um mutex
 uint8_t mutex_lock(mutex_t *mutex)
{
    DISABLE_ALL_INTERRUPTS();
    
    // Se o mutex está desbloqueado, bloqueia e retorna sucesso
    if (mutex->locked == 0) {
        mutex->locked   = 1;
        mutex->owner_id = r_queue.pos_task_running;
        ENABLE_ALL_INTERRUPTS();
        return 0;
    }
    
    // Mutex já está bloqueado - adiciona a tarefa atual a fila de espera
    if (mutex->waiting_count < MAX_USER_TASKS) {
        mutex->waiting_queue[mutex->pos_input] = r_queue.pos_task_running;
        mutex->pos_input = (mutex->pos_input + 1) % MAX_USER_TASKS;
        mutex->waiting_count++;
        
        // Salva contexto e muda estado da tarefa para WAITING_MUTEX
        SAVE_CONTEXT(WAITING_MUTEX);
        scheduler();
        RESTORE_CONTEXT();
        
        ENABLE_ALL_INTERRUPTS();
        return 0;
    }
    
    ENABLE_ALL_INTERRUPTS();
    return 1;
}

//mutex_unlock - Libera um mutex
uint8_t mutex_unlock(mutex_t *mutex)
{
    DISABLE_ALL_INTERRUPTS();
    
    // Verifica se a tarefa atual é o proprietário do mutex
    if (mutex->owner_id != r_queue.pos_task_running) {
        ENABLE_ALL_INTERRUPTS();
        return 1;
    }
    
    // Verifica se tem tarefas na fila de espera
    if (mutex->waiting_count > 0) {
        // Transfere propriedade para a próxima tarefa da fila
        uint8_t next_owner = mutex->waiting_queue[mutex->pos_output];
        mutex->pos_output = (mutex->pos_output + 1) % MAX_USER_TASKS;
        mutex->waiting_count--;
        
        // Muda o estado da tarefa para READY
        r_queue.TASKS[next_owner].task_state = READY;
        mutex->owner_id = next_owner;
    } 
    else {
        // Desbloqueia o mutex
        mutex->locked = 0;
        mutex->owner_id = 255;
    }
    
    ENABLE_ALL_INTERRUPTS();
    return 0;
}

// Tenta adquirir um mutex sem bloquear
uint8_t mutex_trylock(mutex_t *mutex)
{
    DISABLE_ALL_INTERRUPTS();
    
    if (mutex->locked == 0) {
        mutex->locked   = 1;
        mutex->owner_id = r_queue.pos_task_running;
        ENABLE_ALL_INTERRUPTS();
        return 0; 
    }
    
    ENABLE_ALL_INTERRUPTS();
    return 1; 
}
