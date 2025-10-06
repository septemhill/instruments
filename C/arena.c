#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    void* ptr;
    size_t size;
} PtrInfo;

typedef struct {
    PtrInfo* info_ptrs;        // Array of allocation records
    size_t num_info_ptrs;    // Number of allocations
    size_t capacity_info_ptrs; // Capacity of the info_ptrs array

    size_t size;               // Total size of the memory block
    void* start_pos;           // The fixed start of the memory block
    void* current_pos;         // The moving pointer to the next free spot
    void* end_pos;             // The fixed end of the memory block

    bool using;
} Arena;

const size_t initial_info_capacity = 10;

/**
 * @brief Ensures the info_ptrs array has enough capacity for at least one more element.
 *
 * If the array is full, it doubles its capacity.
 * @param arena The arena to check.
 * @return 0 on success, -1 on memory allocation failure.
 */
static int arena_ensure_info_capacity(Arena* arena) {
    if (arena->num_info_ptrs >= arena->capacity_info_ptrs) {
        size_t new_capacity = (arena->capacity_info_ptrs == 0) ? 8 : arena->capacity_info_ptrs * 2;
        PtrInfo* new_info_ptrs = (PtrInfo*)realloc(arena->info_ptrs, new_capacity * sizeof(PtrInfo));
        if (new_info_ptrs == NULL) {
            return -1; // Reallocation failed
        }
        arena->info_ptrs = new_info_ptrs;
        arena->capacity_info_ptrs = new_capacity;
    }
    return 0; // Success
}

void* arena_alloc(Arena* arena, size_t size) {
    // Check if there is enough space
    if ((char*)arena->current_pos + size > (char*)arena->end_pos) {
        return NULL;
    }

    // Ensure there is space to record this allocation
    if (arena_ensure_info_capacity(arena) != 0) {
        return NULL; // Failed to get space for metadata
    }

    // Allocation is successful, get the pointer and move the current position
    void* ptr = arena->current_pos;
    arena->current_pos = (void*)((char*)arena->current_pos + size);

    // Add the allocation info to the array
    arena->info_ptrs[arena->num_info_ptrs].ptr = ptr;
    arena->info_ptrs[arena->num_info_ptrs].size = size;
    arena->num_info_ptrs++;

    return ptr;
}

Arena* arena_create(size_t size) {
    Arena* arena = (Arena*)malloc(sizeof(Arena));
    if (arena == NULL) {
        return NULL;
    }

    // Pre-allocate space for 10 PtrInfo objects
    arena->info_ptrs = (PtrInfo*)malloc(initial_info_capacity * sizeof(PtrInfo));
    if (arena->info_ptrs == NULL) {
        free(arena);
        return NULL;
    }

    arena->num_info_ptrs = 0;
    arena->capacity_info_ptrs = initial_info_capacity;

    // Allocate the main memory block for the arena
    arena->start_pos = malloc(size);
    if (arena->start_pos == NULL) {
        free(arena->info_ptrs);
        free(arena);
        return NULL;
    }

    arena->size = size;
    arena->current_pos = arena->start_pos;
    arena->end_pos = (void*)((char*)arena->start_pos + size);

    return arena;
}

void arena_destroy(Arena* arena) {
    if (arena != NULL) {
        // The main memory block was allocated at start_pos
        free(arena->start_pos); // Must free using the original start pointer
        free(arena->info_ptrs);
        free(arena);
    }
}

void* arena_realloc(Arena* arena, void* ptr, size_t new_size) {
    // If ptr is NULL, realloc behaves like malloc.
    if (ptr == NULL) {
        return arena_alloc(arena, new_size);
    }

    // If new_size is 0, it's like a free, which we don't support.
    // We return NULL, and the original pointer is not invalidated,
    // as per the arena model.
    if (new_size == 0) {
        return NULL;
    }

    // Find the original allocation record.
    PtrInfo* old_info = NULL;
    for (int i = 0; i < arena->num_info_ptrs; i++) {
        if (arena->info_ptrs[i].ptr == ptr) {
            old_info = &arena->info_ptrs[i];
            break;
        }
    }

    // If the pointer was not found in this arena, we can't do anything.
    if (old_info == NULL) {
        return NULL;
    }

    size_t old_size = old_info->size;

    // Optimization: If the pointer is the most recent allocation,
    // we can try to resize it in-place.
    if ((char*)ptr + old_size == (char*)arena->current_pos) {
        ssize_t diff = new_size - old_size;
        // Check if there's enough space to grow. Shrinking is always possible.
        if ((char*)arena->current_pos + diff <= (char*)arena->end_pos) {
            arena->current_pos = (char*)arena->current_pos + diff;
            old_info->size = new_size;
            return ptr; // The pointer address doesn't change.
        }
    }

    // General case: Allocate a new block at the end of the arena,
    // copy the data, and update the original PtrInfo record.

    // 1. Manually allocate space from the end of the arena.
    if ((char*)arena->current_pos + new_size > (char*)arena->end_pos) {
        return NULL; // Not enough space.
    }
    void* new_ptr = arena->current_pos;
    arena->current_pos = (char*)arena->current_pos + new_size;

    // 2. Copy data from the old block to the new one.
    size_t copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);

    // 3. Update the original PtrInfo record to point to the new location and size.
    //    This reuses the existing record instead of creating a new one.
    old_info->ptr = new_ptr;
    old_info->size = new_size;

    // The old memory block at `ptr` is now considered abandoned within the arena.

    return new_ptr;
}
