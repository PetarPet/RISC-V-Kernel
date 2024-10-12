#ifndef SEM_HPP
#define SEM_HPP

#include "TCB.hpp"
#include "List.hpp"
#include "../lib/mem.h"
#include "syscall_c.h"

typedef Sem *sem_t;

class Sem {
public:
    static Sem *CreateSemaphore(sem_t *handle, unsigned init);

    ~Sem() {
        Close();
    }

    int Close();

    int Wait();

    int Signal();

    int TryWait();

    int Value() const {
        return this->val;
    }

    //Supervisor memory allocation
    void *operator new(size_t size) { return __mem_alloc(size); }

    void operator delete(void *ptr) { __mem_free(ptr); }

protected:
    void Block();

    void Unblock();

private:
    int val;
    List<TCB> blocked;
    bool closed;

    Sem(unsigned init);
};

#endif //SEM_HPP