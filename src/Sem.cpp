#include "../h/Sem.hpp"

Sem *Sem::CreateSemaphore(sem_t *handle, unsigned init) {
    *handle = new Sem(init);
    return *handle;
}

// Constructor
Sem::Sem(unsigned init) : val(init), closed(false) {}

void Sem::Block() {
    TCB::running->SetThreadState(TCB::WAITING);
    blocked.addLast(TCB::running);
    TCB::Dispatch();
}

void Sem::Unblock(){
    thread_t blocked_thread = blocked.removeFirst();
    blocked_thread->SetThreadState(TCB::READY);
    Scheduler::Put(blocked_thread);
}

int Sem::Close() {
    // Check if Sem has already been closed
    if (closed) return -2;
    closed = true;

    while (blocked.peekLast()) {
        thread_t blocked_thread = blocked.removeFirst();
        blocked_thread->SetThreadState(TCB::READY);
        Scheduler::Put(blocked_thread);
    }
    return 0;
}

int Sem::Wait() {
    if (closed) return -1;

    if (--this->val < 0)
        Block();

    /*
    // Check if Sem has been closed in meanwhile
    if (closed) return -2;
     */
    return 0;

}

int Sem::Signal() {
    if (closed) return -1;

    if (++val <= 0)
        Unblock();

    return 0;
}

int Sem::TryWait() {
    if (closed) return -2;

    if(val>0){
        // If semaphore is not taken decrement value and return
        val--;
        return 1;
    } else {
        // If semaphore is taken just return;
        return 0;
    }
}

