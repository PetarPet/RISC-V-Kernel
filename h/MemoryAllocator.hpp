#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "MemoryPiece.hpp"

class MemoryAllocator {
public:
    static void InitializeMemory();

    static void *AllocateMemory(size_t requestedSize);

    static int FreeMemory(void *);

    static uint64 total_size_allocated;
    static uint64 total_size_deallocated;

    static MemoryPiece *pFreePieces, *pAllocatedPieces;
};


#endif //MEMORY_ALLOCATOR_HPP