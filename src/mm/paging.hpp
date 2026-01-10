#ifndef MM_PAGING_HPP
#define MM_PAGING_HPP

#include <stdint.h>

namespace mm {
  void map_page(uintptr_t virt, uintptr_t phys);
  void map_region(uintptr_t vbegin, uintptr_t vend, uintptr_t phys);

  void unmap_page(uintptr_t virt);
  void unmap_region(uintptr_t vbegin, uintptr_t vend);
}

#endif
