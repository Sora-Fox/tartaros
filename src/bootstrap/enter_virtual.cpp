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
#include <stddef.h>
#include "bootstrap/utils.hpp"
#include "early_init/enter_kernel.hpp"

struct [[gnu::packed]] page_struct
{
  uint8_t present : 1 = 0;
  uint8_t read_write : 1 = 0;
  uint8_t user_supervisor : 1 = 0;
  uint8_t write_through : 1 = 0;
  uint8_t cache_disable : 1 = 0;
  uint8_t accessed : 1 = 0;
  uint8_t dirty_avl : 1 = 0;
  uint8_t page_size : 1 = 0;
  uint8_t avl : 4 = 0;
  uint32_t base : 20 = 0;
};

constexpr size_t page_size = 4096;
extern page_struct page_directory[];

namespace {
  [[gnu::section(".bootstrap.text")]] void map_page(uintptr_t virt, uintptr_t phys);
  [[gnu::section(".bootstrap.text")]] void map_region(uintptr_t, uintptr_t, uintptr_t);
  [[gnu::section(".bootstrap.text")]] page_struct make_page_struct(uintptr_t);
  [[gnu::section(".bootstrap.text")]] page_struct* alloc_pt();
  [[gnu::section(".bootstrap.text")]] void enable_paging();
  [[gnu::section(".bootstrap.text")]] void map_recursivly();
}

void bootstrap::enter_virtual(uint32_t magic, const multiboot_info* mbi)
{
  using namespace linker;
  const auto vga_phys = static_cast<uintptr_t>(0x000B'8000);
  const auto vga_virt = vga_phys + kvbase_int;
  const auto mbi_int = reinterpret_cast<uintptr_t>(mbi);
  map_region(bootstrap_start, bootstrap_end, bootstrap_start);
  map_region(kvstart_int, kvend_int, kvstart_int - kvbase_int);
  map_page(vga_virt, vga_phys);
  map_page(mbi_int, mbi_int);
  map_recursivly();
  enable_paging();
  early::enter_kernel(magic, mbi);
}

namespace {
  page_struct make_page_struct(const uintptr_t base)
  {
    return page_struct{
      .present = 1,
      .read_write = 1,
      .base = base >> 12,
    };
  };

  page_struct* alloc_pt()
  {
    constexpr static size_t pt_size = 1024;
    constexpr static size_t pt_pool_size = 16;
    alignas(page_size) static page_struct pt_pool[pt_size * pt_pool_size];
    static size_t used = 0;
    if (used == pt_pool_size) {
      using bootstrap::panic;
      using bootstrap::to_phys;
      panic(to_phys("Bootstrap panic: Failed to allocate page table"));
    }
    return pt_pool + used++ * pt_size;
  }

  void map_page(const uintptr_t virt, const uintptr_t phys)
  {
    using bootstrap::to_phys;
    const auto pd_idx = virt >> 22 & 0x3FF;
    const auto pt_idx = virt >> 12 & 0x3FF;
    if (!to_phys(page_directory)[pd_idx].present) {
      const page_struct* const pt = to_phys(alloc_pt());
      const auto pt_addr = reinterpret_cast<uintptr_t>(pt);
      to_phys(page_directory)[pd_idx] = make_page_struct(pt_addr);
    }
    const uintptr_t pt_addr = to_phys(page_directory)[pd_idx].base << 12;
    const auto pt = reinterpret_cast<page_struct*>(pt_addr);
    pt[pt_idx] = make_page_struct(phys);
  }

  void map_region(uintptr_t vbeg, uintptr_t vend, uintptr_t phys)
  {
    while (vbeg < vend) {
      map_page(vbeg, phys);
      vbeg += page_size;
      phys += page_size;
    }
  }

  void enable_paging()
  {
    const auto pd_phys = bootstrap::to_phys(page_directory);
    asm volatile("movl %[pd], %%cr3\n"
                 "movl %%cr0, %%eax\n"
                 "orl $0x80010000, %%eax\n"
                 "movl %%eax, %%cr0\n"
                 "addl %[base], %%esp\n"
        :
        : [pd] "r"(pd_phys), [base] "i"(linker::kvbase_int)
        : "eax");
  }

  void map_recursivly()
  {
    size_t recursive_index = 1023;
    auto pd_phys = bootstrap::to_phys(page_directory);
    const auto pd_addr = reinterpret_cast<uintptr_t>(pd_phys);
    pd_phys[recursive_index] = make_page_struct(pd_addr);
  }
}
