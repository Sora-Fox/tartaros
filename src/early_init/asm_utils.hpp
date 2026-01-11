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

#ifndef ASM_UTILS_HPP
#define ASM_UTILS_HPP

#include <stdint.h>

namespace assembly {
  inline void hlt();
  inline void sti();
  inline void cli();

  inline uint32_t get_cr2();
  inline uint32_t get_cr3();

  inline uint8_t inb(const uint16_t);
  inline void outb(const uint16_t, const uint8_t);
}

/*
 * TODO: Move asm utils to libk: no kernel logic here
 * TODO: Move definitions to .cpp file to allow cpu store
 * funcntions bodys in cache to prevent cache misses
 */

void assembly::hlt()
{
  asm volatile("hlt");
}

void assembly::cli()
{
  asm volatile("cli");
}

void assembly::sti()
{
  asm volatile("sti");
}

uint32_t assembly::get_cr2()
{
  uint32_t result = 0;
  asm volatile("movl %%cr2, %0" : "+r"(result));
  return result;
}

uint32_t assembly::get_cr3()
{
  uint32_t result = 0;
  asm volatile("movl %%cr3, %0" : "+r"(result));
  return result;
}

uint8_t assembly::inb(const uint16_t port)
{
  uint8_t value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void assembly::outb(const uint16_t port, const uint8_t value)
{
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif
