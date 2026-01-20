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
#include "early_init/framebuffer.hpp"
#include "early_init/gdt.hpp"
#include "early_init/idt.hpp"
#include "early_init/panic.hpp"
#include "early_init/vga.hpp"
#include "kernel/main.hpp"
#include "mb_parser.hpp"
#include "mm/paging.hpp"
#include "mm/phys_alloc.hpp"

namespace {
  void setup_framebuffer(early::framebuffer_info);
  void setup_vga();
}

void early::enter_kernel(uint32_t magic, const multiboot_info* mbi)
{
  puts("Hello from higher half");
  mb_info out;
  if (!parse_multiboot(magic, mbi, &out)) {
    panic("Failed to parse multiboot info");
  }
  if (!mm::init_phys_alloc(out.heap_phys_addr, out.heap_size_bytes)) {
    panic("Failed to init allocator");
  }
  if (out.framebuffer_present) {
    setup_framebuffer(out.fb_info);
    puts("Framebuffer output initialized");
  } else {
    setup_vga();
    puts("VGA output initialized");
  }
  printf("Bootloader: %s\n", out.bootloader_name);
  const size_t heap_mb = out.heap_size_bytes / (1024 * 1024);
  printf("Alloc initialized: 0x%x, %zuMB\n", out.heap_phys_addr, heap_mb);
  {
    mm::unmap_region(linker::bootstrap_start, linker::bootstrap_end);
    mm::unmap_page(reinterpret_cast<uintptr_t>(mbi));
    const size_t size_kb = 4096 * 2; /* TODO: Hardcode */
    printf("Unmap unused regions: %zuKB\n", size_kb);
  }
  init_gdt();
  puts("GDT Initialized successfully");
  init_idt();
  puts("IDT Initialized successfully");
  call_constructors();
  puts("Constructors called successfully");
  kernel::main();
  call_destructors();
  puts("Destructors called successfully");
  panic("Unexpected finish");
}

namespace {
  void setup_framebuffer(const early::framebuffer_info fb_info)
  {
    const uintptr_t fb_phys = fb_info.addr;
    const uintptr_t fb_virt = 0x6000'0000;          /* TODO: Hardcode */
    const uintptr_t fb_size = mm::page_size * 1024; /* TODO: Hardcode */
    mm::map_region_or_panic(fb_virt, fb_virt + fb_size, fb_phys);
    const auto fb_ptr = reinterpret_cast<void*>(fb_virt);
    early::init_framebuffer(fb_ptr, fb_info.pitch, fb_info.width, fb_info.height);
    early::init_console(early::framebuffer_out_char);
  }

  void setup_vga()
  {
    constexpr uintptr_t vga_phys = 0x000B'8000;
    const auto vga_virt = vga_phys + linker::kvbase_int;
    mm::map_page_or_panic(vga_virt, vga_phys);
    early::init_vga(reinterpret_cast<void*>(vga_virt));
    early::init_console(early::vga_out_char);
  }
}
