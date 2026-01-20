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

#include "mb_parser.hpp"
#include "common/multiboot.hpp"

bool early::parse_multiboot(uint32_t magic, const multiboot_info* mbi, mb_info* out)
{
  constexpr uint32_t expected_magic = 0x2BADB002;
  if (!out || !mbi || magic != expected_magic) {
    return false;
  }
  const bool has_memory = mbi->flags & MULTIBOOT_INFO_MEMORY;
  const bool has_mmap = mbi->flags & MULTIBOOT_INFO_MEM_MAP;
  const bool is_valid = has_memory && has_mmap;
  if (!is_valid) {
    return false;
  }
  const auto lower = static_cast<size_t>(mbi->mem_lower) * 1024;
  const auto upper = static_cast<size_t>(mbi->mem_upper) * 1024;
  *out = {};
  out->total_ram_bytes = lower + upper + 1024 * 1024;
  out->framebuffer_present = mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO;
  if (out->framebuffer_present) {
    out->fb_info.addr = mbi->framebuffer_addr;
    out->fb_info.pitch = mbi->framebuffer_pitch;
    out->fb_info.width = mbi->framebuffer_width;
    out->fb_info.height = mbi->framebuffer_height;
  }
  if (mbi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
    out->bootloader_name = reinterpret_cast<const char*>(mbi->boot_loader_name);
  }

  uintptr_t best_addr = 0;
  size_t best_len = 0;
  const auto mmap_begin = reinterpret_cast<multiboot_mmap_entry*>(mbi->mmap_addr);
  const auto mmap_end_addr = mbi->mmap_addr + mbi->mmap_length;
  const auto mmap_end = reinterpret_cast<multiboot_mmap_entry*>(mmap_end_addr);
  for (auto p = mmap_begin; p != mmap_end; ++p) {
    const auto addr = static_cast<uintptr_t>(p->addr);
    const auto len = static_cast<size_t>(p->len);
    if (p->type != MULTIBOOT_MEMORY_AVAILABLE || len < best_len) {
      continue;
    }
    best_len = len;
    best_addr = addr;
  }
  out->heap_phys_addr = best_addr;
  out->heap_size_bytes = best_len;
  return best_len != 0;
}
