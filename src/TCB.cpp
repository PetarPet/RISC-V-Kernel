#include "../h/TCB.hpp"
#include "../h/Riscv.hpp"

thread_t TCB::running = nullptr;
thread_t TCB::main = nullptr;
thread_t TCB::idle = nullptr;

thread_t TCB::CreateThread(Body body, void *arg, uint64 *stack, bool ready) {
    return new TCB(body, arg, stack, ready);
}

void TCB::ThreadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->SetThreadState(FINISHED);

    //ecall and context switch
    TCB::Yield();
}

thread_t TCB::CreateMainThread() {
    if(!main){
        main = CreateThread(nullptr, nullptr, nullptr, false);
        main->sys_thread = true;
        running = main;
    }
    return main;
}

thread_t TCB::CreateIdleThread() {
    if(!idle){
        uint64* stack = (uint64*) __mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
        idle = CreateThread(IdleThreadBody, nullptr, stack, false);
        idle->sys_thread = true;
        idle->SetThreadState(IDLE);
    }
    return idle;
}

void TCB::IdleThreadBody(void *) {
    while (true){
        thread_dispatch();
    }
}

void TCB::Yield() {
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::Dispatch(){
    TCB* old = running;
    if(old->thread_state == RUNNING){
        old->thread_state = READY;
        Scheduler::Put(old);
    }
    running = Scheduler::Get();

    // If running is nullptr choose idle thread
    if(running){
        running->thread_state = RUNNING;
    } else running = idle;

    Riscv::SetPrivilegeLevelForRunningThread();
    TCB::contextSwitch(&old->context, &running->context);
}

int TCB::Exit(){
    if(running->GetThreadState() != RUNNING) {
        return -1;
    }
    running->SetThreadState(FINISHED);
    Dispatch();
    return 0;
}

int TCB::Start() {
    if(this->GetThreadState()!=CREATED){
        return -2;
    }
    this->SetThreadState(READY);
    Scheduler::Put(this);
    return 0;
}