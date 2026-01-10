#ifndef BOOTSTRAP_ENTER_VIRTUAL_HPP
#define BOOTSTRAP_ENTER_VIRTUAL_HPP

#include <stdint.h>

struct multiboot_info;

namespace bootstrap {
  [[gnu::section(".bootstrap.text"), noreturn]]
  void enter_virtaul(uint32_t, const multiboot_info*);
  /* TODO: type: virtaul -> virtual */
}

#endif
