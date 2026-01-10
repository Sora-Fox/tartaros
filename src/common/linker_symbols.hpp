#ifndef LINKER_SYMBOLS_HPP
#define LINKER_SYMBOLS_HPP

#include <stdint.h>

namespace linker {
  extern "C" const char _kernel_virtual_base[];
  extern "C" const char _kernel_phys_start[];
  extern "C" const char _kernel_phys_end[];
  extern "C" const char _kernel_virt_start[];
  extern "C" const char _kernel_virt_end[];

  extern "C" char _boostrap_start[];
  extern "C" char _boostrap_end[];

  const auto bootstrap_start = reinterpret_cast<uintptr_t>(_boostrap_start);
  const auto bootstrap_end = reinterpret_cast<uintptr_t>(_boostrap_end);

  const auto kvbase_int = reinterpret_cast<uintptr_t>(_kernel_virtual_base);
  const auto kpstart_int = reinterpret_cast<uintptr_t>(_kernel_phys_start);
  const auto kvstart_int = reinterpret_cast<uintptr_t>(_kernel_virt_start);
  const auto kpend_int = reinterpret_cast<uintptr_t>(_kernel_phys_end);
  const auto kvend_int = reinterpret_cast<uintptr_t>(_kernel_virt_end);
}

#endif
