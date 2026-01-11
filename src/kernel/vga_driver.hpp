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

#ifndef VGA_DRIVER_HPP
#define VGA_DRIVER_HPP

#include <stdint.h>
#include "video_driver.hpp"

namespace kernel {
  class VgaDriver final : public VideoDriver
  {
  public:
    explicit VgaDriver(void*);
    virtual ~VgaDriver() = default;

    void write_char(size_type x, size_type y, char, color fg, color bg) override;
    void clear(color bg) override;
    void scroll_line(color bg) override;

    [[nodiscard]] size_type width() override;
    [[nodiscard]] size_type height() override;

  private:
    constexpr static size_type width_ = 80;
    constexpr static size_type height_ = 25;

    volatile uint16_t* buffer_;
  };
}

#endif
