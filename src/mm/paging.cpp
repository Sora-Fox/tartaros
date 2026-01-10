#include "mm/paging.hpp"
#include <stddef.h>

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
constexpr size_t pd_size = 1024;
alignas(page_size) page_struct page_directory[pd_size];

namespace {
  page_struct make_page_struct(uintptr_t);
  void flush_tlb(uintptr_t);
}

void mm::map_page(const uintptr_t virt, const uintptr_t phys)
{
  const auto pd_idx = virt >> 22 & 0x3FF;
  const auto pt_idx = virt >> 12 & 0x3FF;
  if (!page_directory[pd_idx].present) {
    const page_struct* const pt = /*TODO: Implement page catalog */ nullptr;
    const auto pt_addr = reinterpret_cast<uintptr_t>(pt);
    page_directory[pd_idx] = make_page_struct(pt_addr);
  }
  const uintptr_t pt_addr = page_directory[pd_idx].base << 12;
  const auto pt = reinterpret_cast<page_struct*>(pt_addr);
  pt[pt_idx] = make_page_struct(phys);
  flush_tlb(virt);
}

void mm::map_region(uintptr_t vbeg, const uintptr_t vend, uintptr_t phys)
{
  while (vbeg < vend) {
    map_page(vbeg, phys);
    vbeg += page_size;
    phys += page_size;
  }
}

void mm::unmap_page(const uintptr_t virt)
{
  const auto pd_idx = virt >> 22 & 0x3FF;
  const auto pt_idx = virt >> 12 & 0x3FF;
  if (!page_directory[pd_idx].present) {
    return;
  }
  const uintptr_t pt_addr = page_directory[pd_idx].base << 12;
  const auto pt = reinterpret_cast<page_struct*>(pt_addr);
  pt[pt_idx] = {};
  flush_tlb(virt);
}

void mm::unmap_region(uintptr_t vbeg, const uintptr_t vend)
{
  while (vbeg < vend) {
    unmap_page(vbeg);
    vbeg += page_size;
  }
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

  void flush_tlb(const uintptr_t virt)
  {
    asm volatile("invlpg %0" : : "m"(virt) : "memory");
  }

}
