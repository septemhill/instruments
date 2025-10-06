#ifndef ARENA_H
#define ARENA_H

#include <stddef.h> // For size_t

/**
 * @brief An opaque type representing the Arena memory allocator.
 *
 * The internal structure is hidden from the user to enforce encapsulation.
 * All interactions should be done through the provided arena_* functions.
 */
typedef struct Arena Arena;

/**
 * @brief Creates and initializes a new memory arena.
 *
 * @param size The total size of the memory block to be managed by the arena.
 * @return A pointer to the newly created Arena, or NULL if allocation fails.
 */
Arena* arena_create(size_t size);

/**
 * @brief Destroys an arena and frees all associated memory.
 *
 * This includes the main memory block and any internal tracking structures.
 * All pointers allocated from this arena will be invalidated.
 *
 * @param arena A pointer to the Arena to be destroyed.
 */
void arena_destroy(Arena* arena);

/**
 * @brief Allocates a block of memory from the arena.
 *
 * This is a fast, linear allocation. Memory is not initialized.
 *
 * @param arena The arena from which to allocate.
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory, or NULL if the arena is full.
 */
void* arena_alloc(Arena* arena, size_t size);

/**
 * @brief Re-allocates a block of memory within the arena.
 *
 * If the block is the most recent allocation, it may be resized in-place.
 * Otherwise, a new block is allocated at the end of the arena, data is
 * copied, and the old block's tracking record is updated. The old memory
 * region becomes "dead space" until the entire arena is destroyed or compacted.
 *
 * @param arena The arena where the memory resides.
 * @param ptr A pointer to a previously allocated block from the same arena.
 * @param new_size The new size for the memory block.
 * @return A pointer to the reallocated memory, or NULL on failure.
 */
void* arena_realloc(Arena* arena, void* ptr, size_t new_size);

#endif // ARENA_H