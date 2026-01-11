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

#include "video_driver.hpp"

#include "libk/vsnprintf.hpp"

namespace kernel {
  class Console
  {
  public:
    explicit Console(VideoDriver*);
    void set_color(color);
    void write_string(const char*);
    void set_driver(VideoDriver*);
    /* TODO */
    void printf(const char* str, ...)
    {
      char buffer[512];
      va_list args;
      va_start(args, str);
      std::vsnprintf(buffer, 512, str, args);
      write_string(buffer);
      va_end(args);
    }

  private:
    using size_type = VideoDriver::size_type;

    VideoDriver* driver_;
    size_type x_cord_ = 0;
    size_type y_cord_ = 0;
    color bg_color_ = color::black;
    color fg_color_ = color::bright_white;

    void write_char(char);
    void newline();
  };
}
