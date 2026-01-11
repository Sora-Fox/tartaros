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

#include "early_init/enter_kernel.hpp"
#include <stdint.h>
#include "common/linker_symbols.hpp"
#include "common/multiboot.hpp"
#include "constructors.hpp"
#include "early_init/console.hpp"
#include "early_init/gdt.hpp"
#include "early_init/idt.hpp"
#include "early_init/panic.hpp"
#include "early_init/vga.hpp"
#include "mb_parser.hpp"
#include "mm/alloc.hpp"
#include "mm/paging.hpp"

void early::enter_kernel(uint32_t magic, const multiboot_info* mbi)
{
  puts("Hello from higher half");
  using namespace linker;
  init_vga(reinterpret_cast<void*>(0x000B'8000 + kvbase_int));
  init_console(vga_out_char);
  mb_info out;
  if (!parse_multiboot(magic, mbi, &out)) {
    panic("Failed to parse multiboot info");
  }
  {
    mm::unmap_region(bootstrap_start, bootstrap_end);
    mm::unmap_page(reinterpret_cast<uintptr_t>(mbi));
    const size_t size_kb = (bootstrap_end - bootstrap_start + 4096) / 1024;
    printf("Unmap unused regions: %zuKB\n", size_kb);
  }
  {
    const bool has_name = mbi->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME;
    const auto mbi_name = reinterpret_cast<const char*>(mbi->boot_loader_name);
    const auto name = has_name ? mbi_name : "UNKNOWN";
    printf("Bootloader: %s\n", name);
  }
  {
    const auto heap_ptr = reinterpret_cast<void*>(out.heap_phys_addr);
    if (!mm::init_alloc(heap_ptr, out.heap_size_bytes)) {
      panic("Failed to init allocator");
    }
    const size_t heap_mb = out.heap_size_bytes / (1024 * 1024);
    printf("Alloc initialized: 0x%x, %zuMB\n", out.heap_phys_addr, heap_mb);
  }
  init_gdt();
  puts("GDT Initialized successfully");
  init_idt();
  puts("IDT Initialized successfully");
  call_constructors();
  puts("Constructors called successfully");
  /* TODO: Call kernel main */
  while (true) {
    asm volatile("hlt");
  } // TODO: Double fault after idt init
  call_destructors();
  puts("Destructors called successfully");
  panic("Unexpected finish");
}
