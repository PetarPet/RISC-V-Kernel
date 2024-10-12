#include "../h/Riscv.hpp"
#include "../test/printing.hpp"

extern void userMain();

void user_wrapper(void *arg) {
    printString("userMain() started!\n");
    userMain();
    printString("userMain() finished!\n");

    //Wrapper parameter is actually pointer to Sem
    sem_signal((sem_t) arg);
}

int main(){
    // Initialise MemoryAllocator and create threads
    MemoryAllocator::InitializeMemory();
    thread_t mainThread = TCB::CreateMainThread();
    thread_t idleThread = TCB::CreateIdleThread();

    // Set supervisor trap
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);

    // Mask just timer interrupt (for asynchronous context switch)
    Riscv::mc_sie(Riscv::SIP_SSIP);

    //Enable interrupts globally
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    printString("main() started!\n");
    sem_t userSem;
    sem_open(&userSem, 0);

    thread_t userThread;
    thread_create(&userThread, user_wrapper, userSem);

    sem_wait(userSem);
    printString("main() ended!\n");

    // Mask interrupts globally
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);

    delete mainThread;
    delete userThread;
    delete idleThread;

    sem_close(userSem);
    return 0;
}
