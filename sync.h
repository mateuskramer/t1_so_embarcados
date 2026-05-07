#ifndef SYNC_H
#define	SYNC_H

#include <stdint.h>
#include "os_config.h"
#include "types.h"

// Padrão POSIX
typedef struct sem {
    int contador;
    uint8_t fila[MAX_USER_TASKS];
    uint8_t pos_input;
    uint8_t pos_output;
} sem_t;

void sem_init(sem_t *sem, uint8_t valor);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);

// Funções para o mutex
void mutex_init(mutex_t *mutex);
uint8_t mutex_lock(mutex_t *mutex);
uint8_t mutex_unlock(mutex_t *mutex);
uint8_t mutex_trylock(mutex_t *mutex);

#endif	/* SYNC_H */

