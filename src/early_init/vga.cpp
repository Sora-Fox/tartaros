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

#include "early_init/vga.hpp"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include "early_init/asm_utils.hpp"
#include "libk/algorithm.hpp"

namespace {
  constexpr size_t width = 80;
  constexpr size_t height = 25;

  volatile uint16_t* buffer = nullptr;
  constexpr uint16_t color = 0x0F;

  size_t cursor_x = 0;
  size_t cursor_y = 0;

  void clear_screen();
  void scroll_line();
  void newline();
  void update_hardware_cursor();

}

void early::init_vga(void* buf)
{
  buffer = static_cast<uint16_t*>(buf);
  clear_screen();
  update_hardware_cursor();
}

void early::vga_out_char(char ch)
{
  switch (ch) {
  case '\n':
    update_hardware_cursor();
    newline();
    return;
  case '\r':
    update_hardware_cursor();
    cursor_x = 0;
    return;
  }
  if (ch < 32 || ch > 126) { /* TODO: Implement isprint() */
    return;
  }
  if (cursor_x + 1 == width) {
    newline();
  }
  const auto idx = cursor_y * width + cursor_x;
  buffer[idx] = (color << 8) | static_cast<uint16_t>(ch);
  ++cursor_x;
  update_hardware_cursor();
}

namespace {
  void clear_screen()
  {
    const auto value = (color << 8) | static_cast<uint16_t>(' ');
    std::fill(buffer, buffer + width * height, value);
  }

  void scroll_line()
  {
    const auto value = (color << 8) | static_cast<uint16_t>(' ');
    const auto buffer_end = buffer + width * height;
    const auto second_line = buffer + width;
    const auto last_line = buffer_end - width;
    std::copy(second_line, buffer_end, buffer);
    std::fill(last_line, buffer_end, value);
  }

  void newline()
  {
    if (cursor_y + 1 == height) {
      scroll_line();
    } else {
      ++cursor_y;
    }
    cursor_x = 0;
  }

  void update_hardware_cursor()
  {
    const auto idx = cursor_y * width + cursor_x;
    assembly::outb(0x3D4, 0x0F);
    assembly::outb(0x3D5, idx & 0xFF);
    assembly::outb(0x3D4, 0x0E);
    assembly::outb(0x3D5, idx >> 8);
  }

}
