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

#ifndef MM_PAGING_HPP
#define MM_PAGING_HPP

#include <stddef.h>
#include <stdint.h>

namespace mm {
  constexpr size_t page_size = 4096;

  [[nodiscard]] bool map_page(uintptr_t vaddr, uintptr_t paddr);
  [[nodiscard]] bool map_region(uintptr_t vbegin, uintptr_t vend, uintptr_t pbegin);

  void map_page_or_panic(uintptr_t vaddr, uintptr_t paddr);
  void map_region_or_panic(uintptr_t vbegin, uintptr_t vend, uintptr_t pbegin);

  void unmap_page(uintptr_t vaddr);
  void unmap_region(uintptr_t vbegin, uintptr_t vend);
}

#endif
