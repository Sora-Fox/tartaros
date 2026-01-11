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

#include "core_utils.hpp"
#include "console.hpp"
#include "early_init/asm_utils.hpp" // TODO: CODE ORG
#include "vga_driver.hpp"

namespace {
  void vprintf(const char* const, va_list);
}

void kernel::detail::do_panic(const char* file, int line, const char* func,
    const char* fmt, ...)
{
  printf("PANIC: %s:%d in %s()\nMessage: ", file, line, func);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  halt();
}

void kernel::printf(const char* const fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

void kernel::halt()
{
  assembly::cli();
  while (true) {
    assembly::hlt();
  }
}

void* vga_buf_addr = reinterpret_cast<void*>(0x000B'8000 + 0xC000'0000);
static kernel::VgaDriver driver(vga_buf_addr);
static kernel::Console console(&driver);

namespace {
  void vprintf(const char* const fmt, va_list args)
  {
    constexpr static size_t buf_size = 512;
    static char buffer[buf_size];
    const size_t _ = std::vsnprintf(buffer, buf_size, fmt, args);
    console.write_string(buffer);
  }

}
