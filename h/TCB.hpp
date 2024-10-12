#ifndef TCB_HPP
#define TCB_HPP

#include "syscall_c.h"
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../h/Scheduler.hpp"

class TCB {
public:
    using Body = void (*)(void *);

    ~TCB() {
        delete[] stack;
    }

    enum State {
        CREATED, READY, RUNNING, // runnable
        WAITING, SLEEPING, FINISHED, //blocked
        IDLE // idle thread
    }; // Queues: ready, suspended, sleeping

    State GetThreadState() const {
        return thread_state;
    }

    void SetThreadState(State s) { thread_state = s; }

    bool IsFinished() { return thread_state == FINISHED; }

    bool IsSysThread() const { return sys_thread; }

    static void Yield();

    // Can't be static because of (this) pointer
    int Start();

    static thread_t CreateMainThread();

    static thread_t CreateIdleThread();

    // Supervisor mode memory allocation
    void *operator new(size_t size) {
        return __mem_alloc(size);
    }

    void operator delete(void *ptr) {
        __mem_free(ptr);
    }

    static thread_t running;
    static thread_t main;
    static thread_t idle;

private:
    TCB(Body body, void *arg, uint64 *stack, bool ready) :
            body(body),
            arg(arg),
            stack(body != nullptr ? stack : nullptr),
            context({body != nullptr ? (uint64) &ThreadWrapper : 0,
                     body != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0}),
            thread_state(body != nullptr ? CREATED : RUNNING),
            sys_thread(false) {
        if (body != nullptr && ready) {
            Scheduler::Put(this);
            this->thread_state = READY;
        }
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };
    Body body;
    void *arg;
    uint64 *stack;
    Context context;
    State thread_state;
    bool sys_thread;

    static thread_t CreateThread(Body body, void *arg, uint64 *stack, bool ready);

    static void ThreadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void IdleThreadBody(void *);

    static void Dispatch();

    static int Exit();

    friend class Riscv;
    friend class Sem;
};

#endif //TCB_HPP