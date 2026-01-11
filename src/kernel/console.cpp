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

#include "console.hpp"

kernel::Console::Console(VideoDriver* driver) : driver_(driver)
{
  // TODO: disabled because of cleaning early output
  //  driver_->clear(bg_color_);
}

void kernel::Console::set_color(const color fg)
{
  fg_color_ = fg;
}

void kernel::Console::write_string(const char* str)
{
  while (*str) {
    write_char(*str++);
  }
}

void kernel::Console::set_driver(VideoDriver* driver)
{
  driver_ = driver;
}

void kernel::Console::write_char(const char ch)
{
  if (ch >= 32 && ch <= 126) { /* TODO: Implement isprint() */
    if (x_cord_ + 1 == driver_->width()) {
      newline();
    }
    driver_->write_char(x_cord_, y_cord_, ch, fg_color_, bg_color_);
    ++x_cord_;
    return;
  }
  switch (ch) {
  case '\n':
    newline();
    break;
  case '\r':
    x_cord_ = 0;
    break;
  case '\t':
    write_string("    ");
    break;
  }
}

void kernel::Console::newline()
{
  if (y_cord_ + 1 == driver_->height()) {
    driver_->scroll_line(bg_color_);
  } else {
    ++y_cord_;
  }
  x_cord_ = 0;
}
