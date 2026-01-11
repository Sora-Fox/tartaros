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

#ifndef EARLY_INIT_CONSOLE_HPP
#define EARLY_INIT_CONSOLE_HPP

#include <stdarg.h>

namespace early {
  using output_engine_t = void (*)(char);
  void init_console(output_engine_t);

  [[gnu::format(printf, 1, 2)]]
  void printf(const char*, ...);
  void vprintf(const char*, va_list);
  void puts(const char*);
}

#endif
