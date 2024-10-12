#include "../h/syscall_c.h"
#include "../h/Constants.hpp"
#include "../lib/console.h"

// ============================= Memory ==========================

void *mem_alloc(size_t size) {
    size_t blocks = (size % MEM_BLOCK_SIZE == 0) ?
                    size / MEM_BLOCK_SIZE :
                    1 + size / MEM_BLOCK_SIZE;

    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (blocks));
    __asm__ volatile ("mv a0, %0" : : "r" (MEM_ALLOC));

    __asm__ volatile ("ecall");

    // Get return value
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return (void *) ret_val;
}

int mem_free(void *ptr) {
    if (!ptr) return 0;

    // Set arguments for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (ptr));
    __asm__ volatile ("mv a0, %0" : : "r" (MEM_FREE));
    __asm__ volatile ("ecall");

    // Get return value
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

// ================================ Thread ======================================

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    if (!handle)
        return -1;
    if (!start_routine)
        return -2;

    // Allocate stack
    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);

    if (!stack)
        return -3;

    // Set arguments for syscall
    __asm__ volatile ("mv a4, %0" : : "r" (stack));
    __asm__ volatile ("mv a3, %0" : : "r" (arg));
    __asm__ volatile ("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_CREATE));

    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int thread_exit() {
    // Set arguments for syscall
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_EXIT));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

void thread_dispatch() {
    // Set arguments for syscall
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

int thread_delete(thread_t handle){
    if(!handle) return -1;

    // Set arguments for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DELETE));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int thread_start(thread_t handle){
    if(!handle) return -1;

    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_START));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int thread_prepare(thread_t *handle, void (*start_routine)(void *), void *arg) {
    if (!handle)
        return -1;
    if (!start_routine)
        return -2;

    // Allocate stack
    void *stack = mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);

    if (!stack)
        return -3;

    // Set arguments for syscall
    __asm__ volatile ("mv a4, %0" : : "r" (stack));
    __asm__ volatile ("mv a3, %0" : : "r" (arg));
    __asm__ volatile ("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_PREPARE));

    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

// ========================== Semaphore ===================================

int sem_open(sem_t *handle, unsigned init) {
    if (!handle) return -1;

    // Set arguments for syscall
    __asm__ volatile ("mv a2, %0" : : "r" (init));
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_OPEN));

    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int sem_close(sem_t handle) {
    if (!handle) return -1;

    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (handle));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int sem_wait(sem_t id) {
    if (!id) return -1;

    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_WAIT));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int sem_signal(sem_t id) {
    if (!id) return -1;

    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_SIGNAL));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int sem_trywait(sem_t id) {
    if (!id) return -1;

    // Set arguments for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (id));
    __asm__ volatile ("mv a0, %0" : : "r" (SEM_TRYWAIT));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return ret_val;
}

int time_sleep(time_t) {
    return 0;
}

// ========================== Console ===================================

char getc() {
    // Set argument for syscall
    __asm__ volatile ("mv a0, %0" : : "r" (GET_C));
    __asm__ volatile ("ecall");

    // Get return value from syscall
    uint64 volatile ret_val;
    __asm__ volatile ("mv %0, a0" : "=r" (ret_val));
    return (char) ret_val;
}

void putc(char c) {
    // Set argument for syscall
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    __asm__ volatile ("mv a0, %0" : : "r" (PUT_C));
    __asm__ volatile ("ecall");
}
