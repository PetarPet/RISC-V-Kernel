#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"

// =========================== Memory ====================================
void *mem_alloc(size_t size);
int mem_free(void*);

// =========================== Thread ========================================
class TCB;
typedef TCB *thread_t;
int thread_create(
        thread_t *handle,
        void(*start_routine)(void*),
        void *arg
);
int thread_exit();
void thread_dispatch ();
int thread_delete(thread_t handle);
int thread_start(thread_t handle);
int thread_prepare(thread_t *handle,
                   void(*start_routine)(void*),
                   void *arg);

// ========================== Semaphore ===================================

class Sem;
typedef Sem *sem_t;

int sem_open(
        sem_t *handle,
        unsigned init
);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
int sem_trywait(sem_t id);

typedef unsigned long time_t;
int time_sleep(time_t);

// ========================== Console ===================================

const int EOF = -1;
char getc();
void putc(char);

#endif //SYSCALL_C_HPP