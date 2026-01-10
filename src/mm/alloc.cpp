#include "alloc.hpp"

#include <stdint.h>

namespace {
  char* heap_start = nullptr;
  char* heap_end = nullptr;
}

bool mm::init_alloc(void* const base, const size_t size)
{
  heap_start = static_cast<char*>(base);
  heap_end = heap_start + size;
  return base != nullptr && size != 0;
}

void* mm::alloc(const size_t size, const size_t align)
{
  if (!heap_start || !size) {
    return nullptr;
  }
  /* TODO: should allocator be responsible for memeory pages mapping or not */
  const uintptr_t current = reinterpret_cast<uintptr_t>(heap_start);
  const uintptr_t aligned = (current + align - 1) & ~(align - 1);
  char* const ptr = reinterpret_cast<char*>(aligned);
  if (ptr + size > heap_end) {
    return nullptr;
  }
  heap_start = ptr + size;
  return ptr;
}

void mm::free(const void*) {}
