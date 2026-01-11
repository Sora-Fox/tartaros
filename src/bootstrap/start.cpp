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

#include "bootstrap/enter_virtual.hpp"
#include "common/multiboot.hpp"

#if !defined(__i386__)
#  error "This needs i386-elf compiler"
#endif

constexpr uint32_t align = 1 << 0;
constexpr uint32_t meminfo = 1 << 1;
constexpr uint32_t framebuffer = 1 << 2;
constexpr uint32_t flags = align | meminfo | framebuffer;
constexpr uint32_t checksum = -flags - MULTIBOOT_HEADER_MAGIC;

[[using gnu: aligned(MULTIBOOT_HEADER_ALIGN), used,
    section(".multiboot")]] const static multiboot_header mbh{
  .magic = MULTIBOOT_HEADER_MAGIC,
  .flags = flags,
  .checksum = checksum,

  .header_addr = 0,
  .load_addr = 0,
  .load_end_addr = 0,
  .bss_end_addr = 0,
  .entry_addr = 0,

  .mode_type = MULTIBOOT_FRAMEBUFFER_TYPE_RGB,
  .width = 1024,
  .height = 768,
  .depth = 32,
};

extern "C" [[using gnu: naked, section(".bootstrap.text"), noreturn]] void _start(void)
{
  asm volatile("movl $_stack_top, %%esp\n"
               "subl $_kernel_virtual_base, %%esp\n"
               "pushl %%ebx\n"
               "pushl %%eax\n"
               "call %P0\n"
      :
      : "i"(bootstrap::enter_virtaul)
      :);
}
