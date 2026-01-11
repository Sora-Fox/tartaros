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

#ifndef VIDEO_DRIVER_HPP
#define VIDEO_DRIVER_HPP

#include <stddef.h>
#include <stdint.h>

namespace kernel {
  enum class color : uint8_t {
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    white,
    gray,
    light_blue,
    light_green,
    light_cyan,
    light_red,
    light_magenta,
    yellow,
    bright_white,
  };

  class VideoDriver
  {
  public:
    using size_type = size_t;

    virtual ~VideoDriver() = default;

    virtual void write_char(size_type x, size_type y, char, color fg, color bg) = 0;
    virtual void clear(color bg) = 0;
    virtual void scroll_line(color bg) = 0;

    [[nodiscard]] virtual size_type width() = 0;
    [[nodiscard]] virtual size_type height() = 0;
  };
}

#endif
