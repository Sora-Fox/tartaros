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

#ifndef CORE_UTILS_HPP
#define CORE_UTILS_HPP

#include <stdarg.h>

#define panic(fmt, ...) \
  kernel::detail::do_panic(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)

namespace kernel {
  namespace detail {
    [[noreturn]] void do_panic(const char*, int, const char*, const char*, ...);
  }

  void printf(const char*, ...);

  [[noreturn]] void halt();
}

#endif
