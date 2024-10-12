#include "../h/Scheduler.hpp"

List<TCB> Scheduler::sReadyThreadQueue;

TCB *Scheduler::Get() {
    return sReadyThreadQueue.removeFirst();
}

void Scheduler::Put(TCB *tcb) {
    sReadyThreadQueue.addLast(tcb);
}