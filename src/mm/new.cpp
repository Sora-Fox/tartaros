#include "mm/new.hpp"
#include "mm/alloc.hpp"

void* operator new(size_t size)
{
  return mm::alloc(size);
}

void operator delete(void* ptr)
{
  mm::free(ptr);
}

void operator delete(void* ptr, size_t)
{
  mm::free(ptr);
}
