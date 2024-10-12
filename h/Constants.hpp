#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "../lib/hw.h"
// Supervisor interrupt codes
static const uint64 USER_ECALL = 0x0000000000000008UL;
static const uint64 SUPERVISOR_ECALL = 0x0000000000000009UL;
static const uint64 TIMER = 0x8000000000000001UL;
static const uint64 CONSOLE = 0x8000000000000009UL;

//  Syscall codes
static const uint64 MEM_ALLOC = 0x01;
static const uint64 MEM_FREE = 0x02;

static const uint64 THREAD_CREATE = 0x11;
static const uint64 THREAD_EXIT = 0x12;
static const uint64 THREAD_DISPATCH = 0x13;
static const uint64 THREAD_DELETE = 0x14;
static const uint64 THREAD_START = 0x15;
static const uint64 THREAD_PREPARE = 0x16;

static const uint64 SEM_OPEN = 0x21;
static const uint64 SEM_CLOSE = 0x22;
static const uint64 SEM_WAIT = 0x23;
static const uint64 SEM_SIGNAL = 0x24;
static const uint64 SEM_TRYWAIT = 0x26;

static const uint64 TIME_SLEEP = 0x31;

static const uint64 GET_C = 0x41;
static const uint64 PUT_C = 0x42;
#endif //CONSTANTS_HPP