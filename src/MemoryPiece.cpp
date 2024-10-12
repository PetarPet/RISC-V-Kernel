#include "../h/MemoryPiece.hpp"

void InsertPieceToFreePieceList(MemoryPiece **ppHead, MemoryPiece *pieceToInsert, enum MemoryPieceStatus status) {
    if (!pieceToInsert) return;

    pieceToInsert->status = status;

    if (!(*ppHead)) {
        *ppHead = pieceToInsert;
        return;
    }

    // Insertion is done before the head of the linked list
    MemoryPiece *head = *ppHead;
    if (pieceToInsert < head) {
        pieceToInsert->pNextPiece = head;
        head->pPrevPiece = pieceToInsert;
        *ppHead = pieceToInsert;

        return;
    }

    // Insertion is done in the middle or at the end of the linked list
    MemoryPiece *prev = head;
    for (MemoryPiece *current = head->pNextPiece; current; current = current->pNextPiece) {
        if (pieceToInsert < current) {
            prev->pNextPiece = pieceToInsert;
            pieceToInsert->pPrevPiece = prev;

            current->pPrevPiece = pieceToInsert;
            pieceToInsert->pNextPiece = current;
            return;
        }
        prev = current;
    }

    // Insertion is done at the end of linked list
    prev->pNextPiece = pieceToInsert;
    pieceToInsert->pPrevPiece = prev;
}

void RemovePieceFromFreePieceList(MemoryPiece **ppHead, MemoryPiece *pieceToRemove) {
    if (!ppHead || !pieceToRemove) return;

    // Removing from the beginning
    MemoryPiece *head = *ppHead;
    if (head == pieceToRemove) {
        *ppHead = head->pNextPiece;
        if (*ppHead) //New head not last in list
        {
            (*ppHead)->pPrevPiece = nullptr;
        }
        pieceToRemove->pNextPiece = pieceToRemove->pPrevPiece = nullptr;
        return;
    }


    // Removing from the middle
    MemoryPiece *prev = pieceToRemove->pPrevPiece;
    MemoryPiece *next = pieceToRemove->pNextPiece;

    prev->pNextPiece = next;
    if (next)
        next->pPrevPiece = prev;

    pieceToRemove->pPrevPiece = pieceToRemove->pNextPiece = nullptr;
}

void MergeAdjacentPieces(MemoryPiece *freePiece) {
    if (!freePiece || freePiece->status != FREE) return;

    MemoryPiece *nextPiece, *currentPiece;

    if (freePiece->pPrevPiece)
        currentPiece = freePiece->pPrevPiece;
    else
        currentPiece = freePiece;

    // Merge adjacent free blocks
    while (currentPiece->pNextPiece &&
           (((char *) currentPiece + currentPiece->piece_size + sizeof(MemoryPiece)) >= ((char *) (currentPiece->pNextPiece)))) {
        nextPiece = currentPiece->pNextPiece;

        currentPiece->piece_size += sizeof(MemoryPiece) + nextPiece->piece_size;
        currentPiece->pNextPiece = currentPiece->pNextPiece->pNextPiece;

        if (currentPiece->pNextPiece)
            currentPiece->pNextPiece->pPrevPiece = currentPiece;
    }

}