#ifndef MM_NEW_HPP
#define MM_NEW_HPP

#include <stddef.h>

void* operator new(size_t);
void operator delete(void*);
void operator delete(void*, size_t);

#endif
