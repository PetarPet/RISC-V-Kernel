#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"

// Implementing __mem_alloc and __mem_free functions from lib.h
void *__mem_alloc(size_t size) {
    return MemoryAllocator::AllocateMemory(size);
}

int __mem_free(void *ptr) {
    return MemoryAllocator::FreeMemory(ptr);
}

//Overriding operator with call to syscall_c.hpp function
void *operator new(size_t n) {
    return mem_alloc(n);
}

void *operator new[](size_t n) {
    return mem_alloc(n);
}

void operator delete(void *p) {
    mem_free(p);
}

void operator delete[](void *p) noexcept {
mem_free(p);
}