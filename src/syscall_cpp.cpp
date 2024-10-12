#include "../h/syscall_cpp.hpp"

// ================================ Thread ===================================

Thread::Thread(void (*body)(void *), void *arg):body(body),arg(arg) {
    myHandle = nullptr;
    thread_create(&myHandle,body,arg);
}

Thread::Thread() {
    myHandle = nullptr;
    thread_prepare(&myHandle, Thread::wrapper, (void *) this);
}

Thread::~Thread(){
    thread_delete(myHandle);
}

int Thread::start() {
    return thread_start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::wrapper(void *thread) {
    ((Thread *) thread)->run();
}

// ============================== Semaphore =========================

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore(){
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

// ================================ Console ==============================

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}


