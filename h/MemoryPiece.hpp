#ifndef MEMORY_PIECE_HPP
#define MEMORY_PIECE_HPP

#include "../lib/hw.h"

enum MemoryPieceStatus {
    FREE,
    ALLOCATED
};

typedef struct MemoryPiece {
    size_t piece_size;
    struct MemoryPiece *pNextPiece;
    struct MemoryPiece *pPrevPiece;
    enum MemoryPieceStatus status;

} MemoryPiece;

void InsertPieceToFreePieceList(MemoryPiece **ppHead, MemoryPiece *pieceToInsert, enum MemoryPieceStatus status);

void RemovePieceFromFreePieceList(MemoryPiece **ppHead, MemoryPiece *pieceToRemove);

void MergeAdjacentPieces(MemoryPiece *freePiece);


#endif //MEMORY_PIECE_HPP