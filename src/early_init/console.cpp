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

#include "early_init/console.hpp"
#include "early_init/asm_utils.hpp"
#include "libk/vsnprintf.hpp"

namespace {
  early::output_engine_t out_char = nullptr;

  void serial_print_char(char);
  void print_char(char);
  void print_str(const char*);
}

void early::init_console(output_engine_t engine)
{
  out_char = engine;
}

void early::puts(const char* str)
{
  print_str(str);
  print_char('\n');
}

void early::vprintf(const char* fmt, va_list args)
{
  constexpr static size_t buffer_size = 512;
  char buffer[buffer_size];
  std::vsnprintf(buffer, buffer_size, fmt, args);
  /* TODO: Handle truncation */
  print_str(buffer);
}

void early::printf(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

namespace {
  void serial_print_char(const char ch)
  {
    constexpr uint16_t com1 = 0x3F8;
    while ((assembly::inb(com1 + 5) & 0x20) == 0) {
    }
    assembly::outb(com1, ch);
  }

  void print_char(const char ch)
  {
    serial_print_char(ch);
    if (out_char) {
      out_char(ch);
    }
  }

  void print_str(const char* str)
  {
    while (*str) {
      print_char(*str++);
    }
  }
}
