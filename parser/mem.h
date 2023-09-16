#ifndef MEM_H
#define MEM_H

#include "types.h"

typedef struct memory {
  void *data;
  usize size;
  usize pos;
  usize free;
} memory;

/* Allocates a new memory block, "arena" style */
memory *memory_new(usize max_size);

/* Allocates some memory in the memory block, returns an address to the memory */
void *memory_allocate(memory* mem, usize size);

/* Grows the memory block by `size` bytes */
void *memory_reallocate(memory* mem, usize size);

/* Initialize memory to 0 */
memory memory_init(void *data, usize size);

/* Frees up memory from block */
void memory_free(memory *mem, usize size);

/* Resets `pos` and free to 0 */
void memory_clear(memory *mem);

#endif
