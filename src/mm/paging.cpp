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

#include "mm/paging.hpp"
#include <stddef.h>
#include "libk/string.hpp"
#include "mm/phys_alloc.hpp"

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

constexpr size_t pd_size = 1024;
alignas(mm::page_size) page_struct page_directory[pd_size];

namespace {
  [[nodiscard]] page_struct* get_page_table(size_t);
  [[nodiscard]] page_struct make_page_struct(uintptr_t);
  [[nodiscard]] bool is_page_aligned(uintptr_t);
  void flush_tlb(uintptr_t);
}

bool mm::map_page(const uintptr_t vaddr, const uintptr_t paddr)
{
  if (!is_page_aligned(paddr)) {
    return false;
  }
  const auto pd_idx = vaddr >> 22 & 0x3FF;
  const auto pt_idx = vaddr >> 12 & 0x3FF;
  if (!page_directory[pd_idx].present) {
    const auto new_pt = alloc_phys();
    if (!new_pt) {
      return false;
    }
    page_directory[pd_idx] = make_page_struct(new_pt);
    std::memset(get_page_table(pd_idx), 0, page_size);
  }
  page_struct* pt = get_page_table(pd_idx);
  pt[pt_idx] = make_page_struct(paddr);
  flush_tlb(vaddr);
  return true;
}

bool mm::map_region(const uintptr_t vbegin, const uintptr_t vend, const uintptr_t pbegin)
{
  uintptr_t vcurr = vbegin;
  uintptr_t pcurr = pbegin;
  while (vcurr < vend && map_page(vcurr, pcurr)) {
    vcurr += page_size;
    pcurr += page_size;
  }
  if (vcurr < vend) {
    unmap_region(vbegin, vcurr);
    return false;
  }
  return true;
}

void mm::unmap_page(const uintptr_t vaddr)
{
  const auto pd_idx = vaddr >> 22 & 0x3FF;
  const auto pt_idx = vaddr >> 12 & 0x3FF;
  if (!page_directory[pd_idx].present) {
    return;
  }
  page_struct* pt = get_page_table(pd_idx);
  pt[pt_idx] = {};
  flush_tlb(vaddr);
}

void mm::unmap_region(uintptr_t vbegin, uintptr_t vend)
{
  vbegin &= ~0xFFF;
  vend &= ~0xFFF;
  while (vbegin < vend) {
    unmap_page(vbegin);
    vbegin += page_size;
  }
}

namespace {
  page_struct* get_page_table(const size_t index)
  {
    constexpr size_t recursive_index = pd_size - 1;
    const uintptr_t virt_addr = (recursive_index << 22) | (index << 12);
    return reinterpret_cast<page_struct*>(virt_addr);
  }

  page_struct make_page_struct(const uintptr_t base)
  {
    return page_struct{
      .present = 1,
      .read_write = 1,
      .base = base >> 12,
    };
  };

  bool is_page_aligned(const uintptr_t addr)
  {
    return addr % mm::page_size == 0;
  }

  void flush_tlb(const uintptr_t vaddr)
  {
    asm volatile("invlpg %0" : : "m"(vaddr) : "memory");
  }
}
