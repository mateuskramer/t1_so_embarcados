# 1 "scheduler.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "scheduler.c" 2
# 1 "./scheduler.h" 1



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 5 "./scheduler.h" 2

void scheduler();
uint8_t RR_scheduler();
# 2 "scheduler.c" 2
# 1 "./types.h" 1




# 1 "./os_config.h" 1
# 6 "./types.h" 2

typedef void TASK;

typedef enum {READY = 0,
              WAITING,
              RUNNING
             } state_t;

typedef void (*f_ptr)(void);

typedef struct hw_stack {
    uint8_t TOSL_REG;
    uint8_t TOSH_REG;
    uint8_t TOSU_REG;
} hw_stack_t;

typedef struct sw_stack {
    hw_stack_t stack[31];
    uint8_t stack_size;
} sw_stack_t;

typedef struct tcb {
    uint8_t task_id;
    state_t task_state;

    f_ptr task_ptr;
    uint8_t task_delay;
    uint8_t task_priority;

    uint8_t W_REG;
    uint8_t STATUS_REG;
    uint8_t BSR_REG;
    uint8_t PRODL_REG;
    uint8_t PRODH_REG;
    uint8_t FSR0L_REG;
    uint8_t FSR0H_REG;
    uint8_t FSR1L_REG;
    uint8_t FSR1H_REG;
    uint8_t FSR2L_REG;
    uint8_t FSR2H_REG;
    uint8_t TABLAT_REG;
    uint8_t TBLPTRL_REG;
    uint8_t TBLPTRH_REG;
    uint8_t TBLPTRU_REG;
    uint8_t PCLATH_REG;
    uint8_t PCLATU_REG;


    sw_stack_t task_stack;
} tcb_t;


typedef struct ready_queue {
    tcb_t TASKS[3 +1];
    uint8_t size;
    tcb_t *task_running;
    uint8_t pos_task_running;
} ready_queue_t;
# 3 "scheduler.c" 2


extern ready_queue_t r_queue;


void scheduler()
{
  r_queue.pos_task_running = RR_scheduler();
  r_queue.task_running = &r_queue.TASKS[r_queue.pos_task_running];
}

uint8_t RR_scheduler()
{
    uint8_t prox = r_queue.pos_task_running;

    do {
        prox = (prox+1) % (3 +1);
    } while (r_queue.TASKS[prox].task_state != READY);

    return prox;
}
