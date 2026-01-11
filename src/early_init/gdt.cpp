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

#include "gdt.hpp"
#include <stddef.h>

namespace {
  struct [[gnu::packed]] gdt_entry
  {
    uint16_t limit_low : 16;
    uint32_t base_low : 24;
    uint8_t type : 4;
    uint8_t s : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high : 8;
  };

  struct [[gnu::packed]] gdtr_value
  {
    uint16_t size;
    uintptr_t addr;
  };

  consteval gdt_entry
  make_flat_gdt_entry(const uint8_t type, const uint8_t dpl, const uint8_t flags)
  {
    return gdt_entry{
      .limit_low = 0xFFFF,
      .base_low = 0x0,
      .type = type,
      .s = 0x1,
      .dpl = dpl,
      .present = 0x1,
      .limit_high = 0xF,
      .flags = flags,
      .base_high = 0x0,
    };
  }

  constexpr uint8_t exrd = 0xA;
  constexpr uint8_t rdwr = 0x2;
  constexpr uint8_t flags = 0xC;

  constexpr size_t null_idx = 0;
  constexpr size_t kcode_idx = 1;
  constexpr size_t kdata_idx = 2;
  constexpr size_t ucode_idx = 3;
  constexpr size_t udata_idx = 4;

  constexpr size_t ring0 = 0;
  constexpr size_t ring3 = 3;
}

constexpr uint16_t early::k_cs = (kcode_idx << 3) | ring0;
constexpr uint16_t early::k_ds = (kdata_idx << 3) | ring0;
constexpr uint16_t early::u_cs = (ucode_idx << 3) | ring3;
constexpr uint16_t early::u_ds = (udata_idx << 3) | ring3;

void early::init_gdt()
{
  alignas(16) static gdt_entry gdt[5];
  gdt[null_idx] = {};
  gdt[kcode_idx] = make_flat_gdt_entry(exrd, ring0, flags);
  gdt[kdata_idx] = make_flat_gdt_entry(rdwr, ring0, flags);
  gdt[ucode_idx] = make_flat_gdt_entry(exrd, ring3, flags);
  gdt[udata_idx] = make_flat_gdt_entry(rdwr, ring3, flags);
  const static gdtr_value gdtr{
    .size = sizeof(gdt) - 1,
    .addr = reinterpret_cast<uintptr_t>(gdt),
  };
  asm volatile("lgdt  %[gdtr]\n"
               "ljmp %[kernel_cs], $1f\n"
               "1:\n"
               "movw %[kernel_ds], %%ds\n"
               "movw %[kernel_ds], %%ss\n"
               "movw %[kernel_ds], %%es\n"
               "movw %[kernel_ds], %%gs\n"
               "movw %[kernel_ds], %%fs\n"
      :
      : [gdtr] "m"(gdtr), [kernel_cs] "i"(k_cs), [kernel_ds] "r"(k_ds)
      : "memory");
}
