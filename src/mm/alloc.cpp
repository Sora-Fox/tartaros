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

#include "alloc.hpp"

#include <stdint.h>

namespace {
  char* heap_start = nullptr;
  char* heap_end = nullptr;
}

bool mm::init_alloc(void* const base, const size_t size)
{
  heap_start = static_cast<char*>(base);
  heap_end = heap_start + size;
  return base != nullptr && size != 0;
}

void* mm::alloc(const size_t size, const size_t align)
{
  if (!heap_start || !size) {
    return nullptr;
  }
  /* TODO: should allocator be responsible for memeory pages mapping or not */
  const uintptr_t current = reinterpret_cast<uintptr_t>(heap_start);
  const uintptr_t aligned = (current + align - 1) & ~(align - 1);
  char* const ptr = reinterpret_cast<char*>(aligned);
  if (ptr + size > heap_end) {
    return nullptr;
  }
  heap_start = ptr + size;
  return ptr;
}

void mm::free(const void*) {}
