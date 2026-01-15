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

#include "phys_alloc.hpp"
#include "libk/utility.hpp"
#include "paging.hpp"

namespace {
  uintptr_t heap_start = 0;
  uintptr_t heap_end = 0;
}

bool mm::init_phys_alloc(uintptr_t const base, const size_t size)
{
  heap_start = base;
  heap_end = heap_start + size;
  return base && size;
}

uintptr_t mm::alloc_phys(const size_t num_pages)
{
  if (!heap_start || !num_pages) {
    return 0;
  }
  const uintptr_t new_heap_start = heap_start + page_size * num_pages;
  if (new_heap_start > heap_end) {
    return 0;
  }
  return std::exchange(heap_start, new_heap_start);
}

void mm::free_phys(const uintptr_t, const size_t) {}
