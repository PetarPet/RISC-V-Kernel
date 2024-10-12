#include "../h/Riscv.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

using Body = void (*)(void *);

void Riscv::popSppSpie() {
    SetPrivilegeLevelForRunningThread();

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

uint64 Riscv::Syscall(uint64 *sys_call_arguments) {
    uint64 syscall_op_code = sys_call_arguments[0];
    uint64 ret_val = 0;
    switch (syscall_op_code) {
        case MEM_ALLOC: {
            size_t volatile size = sys_call_arguments[1] * MEM_BLOCK_SIZE;
            ret_val = (uint64) __mem_alloc(size);
            break;
        }
        case MEM_FREE: {
            void *ptr = (void *) sys_call_arguments[1];
            ret_val = (uint64) __mem_free(ptr);
            break;
        }
        case THREAD_CREATE: {
            thread_t *handle = (thread_t *) sys_call_arguments[1];
            Body body = (Body) sys_call_arguments[2];
            void *arg = (void *) sys_call_arguments[3];
            uint64 *stack = (uint64 *) sys_call_arguments[4];

            *handle = TCB::CreateThread(body, arg, stack, true);

            if (*handle) {
                ret_val = 0;
            } else ret_val = -1;
            break;
        }
        case THREAD_EXIT: {
            ret_val = TCB::Exit();
            break;
        }
        case THREAD_DISPATCH: {
            TCB::Dispatch();
            break;
        }
        case THREAD_DELETE:{
            thread_t handle = (thread_t) sys_call_arguments[1];
            delete handle;
            break;
        }
        case THREAD_START:{
            thread_t handle = (thread_t) sys_call_arguments[1];
            ret_val = handle->Start();
            break;
        }
        case THREAD_PREPARE:{
            thread_t *handle = (thread_t *) sys_call_arguments[1];
            Body routine = (Body) sys_call_arguments[2];
            void *arguments = (void *) sys_call_arguments[3];
            uint64 *stack = (uint64 *) sys_call_arguments[4];

            *handle = TCB::CreateThread(routine, arguments, stack, false);

            if (*handle)
                ret_val = 0;
            else
                ret_val = -1;
            break;
        }
        case SEM_OPEN: {
            sem_t *handle = (sem_t*) sys_call_arguments[1];
            uint init = (uint) sys_call_arguments[2];

            if (Sem::CreateSemaphore(handle, init)) {
                ret_val = 0;
            } else {
                ret_val = -1;
            }
            break;
        }
        case SEM_CLOSE:{
            sem_t handle = (sem_t) sys_call_arguments[1];
            ret_val = handle->Close();
            break;
        }
        case SEM_WAIT:{
            sem_t handle = (sem_t) sys_call_arguments[1];
            ret_val = handle->Wait();
            break;
        }
        case SEM_SIGNAL:{
            sem_t handle = (sem_t) sys_call_arguments[1];
            ret_val = handle->Signal();
            break;
        }
        case SEM_TRYWAIT:{
            sem_t handle = (sem_t) sys_call_arguments[1];
            ret_val = handle->TryWait();
            break;
        }
        case GET_C:{
            char c = __getc();
            ret_val = (uint64) c;
            break;
        }
        case PUT_C:{
            char c = (char) sys_call_arguments[1];
            __putc(c);
            break;
        }
        default: {
            break;
        }
    }
    return ret_val;
}

void Riscv::handleSupervisorTrap() {
    uint64 sys_call_arguments[5];

    // Extract arguments from registers
    __asm__ volatile ("mv %0, a0" : "=r" (sys_call_arguments[0]));
    __asm__ volatile ("mv %0, a1" : "=r" (sys_call_arguments[1]));
    __asm__ volatile ("mv %0, a2" : "=r" (sys_call_arguments[2]));
    __asm__ volatile ("mv %0, a3" : "=r" (sys_call_arguments[3]));
    __asm__ volatile ("mv %0, a4" : "=r" (sys_call_arguments[4]));

    uint64 scause = r_scause();

    if (scause == USER_ECALL || scause == SUPERVISOR_ECALL) {
        //  Add 4 because ecall saves instruction which caused ecall not next one
        uint volatile sepc = r_sepc() + 4;
        uint volatile sstatus = r_sstatus();

        WriteReturnedValue(Syscall(sys_call_arguments));

        w_sepc(sepc);
        w_sstatus(sstatus);
    } else if (scause == TIMER) {
        mc_sip(SIP_STIP);
    } else if (scause == CONSOLE) {
        console_handler();
        mc_sip(SIP_SEIP);
    } else {
        uint64 scause = r_scause();
        uint64 sepc = r_sepc();
        uint64 stvec = r_stvec();
        uint64 stval = r_stval();

        printString("scause: ");
        printInt(scause);
        printString("\n");

        printString("sepc: ");
        printInt(sepc);
        printString("\n");

        printString("stvec: ");
        printInt(stvec);
        printString("\n");

        printString("stval: ");
        printInt(stval);
        printString("\n");
    }
}