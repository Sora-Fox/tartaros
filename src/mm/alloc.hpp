#ifndef MM_ALLOC_HPP
#define MM_ALLOC_HPP

#include <stddef.h>

namespace mm {
  bool init_alloc(void*, size_t);

  void* alloc(size_t, size_t = 8);
  void free(const void*);
}

#endif
