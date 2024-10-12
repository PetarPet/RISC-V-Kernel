#include "../h/MemoryAllocator.hpp"
#include "../test/printing.hpp"

void MemoryAllocator::InitializeMemory() {
    pFreePieces = (MemoryPiece *) HEAP_START_ADDR;

    MemoryPiece *memory = pFreePieces;
    memory->pPrevPiece = memory->pNextPiece = nullptr;

    memory->piece_size = (size_t) ((char *) HEAP_END_ADDR - (char *) HEAP_START_ADDR - MEM_BLOCK_SIZE);

    memory->status = FREE;
}

void *MemoryAllocator::AllocateMemory(size_t requestedSize) {
    // Round up the requested size if it's not a multiple of block size
    if (requestedSize % MEM_BLOCK_SIZE != 0)
        requestedSize = (requestedSize / MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;

    // Attempt to allocate memory, return starting address of allocated piece

    for (MemoryPiece *current = MemoryAllocator::pFreePieces; current; current = current->pNextPiece) {
        if (current->piece_size == requestedSize) {
            RemovePieceFromFreePieceList(&pFreePieces, current);
            InsertPieceToFreePieceList(&pAllocatedPieces, current, ALLOCATED);
            total_size_allocated += requestedSize;
            return (void *) ((char *) current + sizeof(MemoryPiece));
        }
            // Free segment is bigger than requestedSize, so we split it in two pieces
            // One piece is returned to free piece list and other is returned to caller
        else if (current->piece_size > requestedSize) {

            RemovePieceFromFreePieceList(&pFreePieces, current);

            //New piece starts where requested piece ends
            MemoryPiece *newBlock = (MemoryPiece *) ((char *) current + sizeof(MemoryPiece) + requestedSize);

            newBlock->pPrevPiece = newBlock->pNextPiece = nullptr;
            newBlock->piece_size = (current->piece_size - sizeof(MemoryPiece) - requestedSize);

            current->piece_size = requestedSize;

            InsertPieceToFreePieceList(&pFreePieces, newBlock, FREE);
            InsertPieceToFreePieceList(&pAllocatedPieces, current, ALLOCATED);

            total_size_allocated += requestedSize;
            return (void *) ((char *) current + sizeof(MemoryPiece));
        }
    }
    return nullptr; //Memory segment not found

}

int MemoryAllocator::FreeMemory(void *memoryToFree) {
    if (!memoryToFree)
        return -1;

    MemoryPiece *pieceToFree = (MemoryPiece *) ((char *) memoryToFree - sizeof(MemoryPiece));

    if (pieceToFree->status != ALLOCATED)
        return -1;

    total_size_deallocated += pieceToFree->piece_size;

    RemovePieceFromFreePieceList(&pAllocatedPieces, pieceToFree);
    InsertPieceToFreePieceList(&pFreePieces, pieceToFree, FREE);

    // Call a method to merge pieces after each memory deallocation to avoid fragmentation
    MergeAdjacentPieces(pieceToFree);

    return 1;
}

MemoryPiece *MemoryAllocator::pFreePieces = nullptr;
MemoryPiece *MemoryAllocator::pAllocatedPieces = nullptr;
uint64 MemoryAllocator::total_size_allocated = 0;
uint64 MemoryAllocator::total_size_deallocated = 0;
