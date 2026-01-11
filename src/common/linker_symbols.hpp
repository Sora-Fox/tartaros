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
