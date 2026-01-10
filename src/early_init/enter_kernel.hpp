#ifndef HIGHER_HALF_HPP
#define HIGHER_HALF_HPP

#include <stdint.h>

struct multiboot_info;

namespace early {
  [[noreturn]] void enter_kernel(uint32_t, const multiboot_info*);
}

#endif
