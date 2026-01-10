#ifndef BOOTSTRAP_UTILS_HPP
#define BOOTSTRAP_UTILS_HPP

#include <stdint.h>
#include "common/linker_symbols.hpp"

namespace bootstrap {
  template <typename T>
  [[gnu::section(".bootstrap.text")]] T* to_phys(T* virt)
  {
    const auto virt_int = reinterpret_cast<uintptr_t>(virt);
    return reinterpret_cast<T*>(virt_int - linker::kvbase_int);
  }

  template <typename T>
  [[gnu::section(".bootstrap.text")]] T* to_virt(T* phys)
  {
    const auto phys_int = reinterpret_cast<uintptr_t>(phys);
    return reinterpret_cast<T*>(phys_int + linker::kvbase_int);
  }

  [[noreturn, gnu::section(".bootstrap.text")]] void panic(const char*);
}

#endif
