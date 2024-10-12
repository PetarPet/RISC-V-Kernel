#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "List.hpp"

class TCB;

class Scheduler {
private:
    static List<TCB> sReadyThreadQueue;

public:
    static TCB *Get();

    static void Put(TCB *tcb);
};


#endif //SCHEDULER_HPP