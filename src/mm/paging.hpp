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

#include <stdint.h>

namespace mm {
  void map_page(uintptr_t virt, uintptr_t phys);
  void map_region(uintptr_t vbegin, uintptr_t vend, uintptr_t phys);

  void unmap_page(uintptr_t virt);
  void unmap_region(uintptr_t vbegin, uintptr_t vend);
}

#endif
