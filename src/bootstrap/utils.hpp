/*
 * This file is part of the Tartaros project.
 *
 * Copyright (C) 2026 Sora Fox [github.com/Sora-Fox]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
