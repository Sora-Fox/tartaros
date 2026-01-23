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

#include "early_init/pic.hpp"
#include "asm_utils.hpp"

namespace {
  constexpr uint16_t master_cmd = 0x0020;
  constexpr uint16_t master_data = master_cmd + 1;
  constexpr uint16_t slave_cmd = 0x00A0;
  constexpr uint16_t slave_data = slave_cmd + 1;

  void mask_all();
}

void early::init_pic()
{
  assembly::outb(master_cmd, 0x11);
  assembly::outb(slave_cmd, 0x11);

  assembly::outb(master_data, 0x20);
  assembly::outb(slave_data, 0x28);
  assembly::outb(master_data, 0x04);
  assembly::outb(slave_data, 0x02);
  assembly::outb(master_data, 0x01);
  assembly::outb(slave_data, 0x01);

  mask_all();
}

void early::pic_send_eoi(const uint8_t irq)
{
  constexpr static uint8_t eoi = 0x20;
  if (irq >= 8) {
    assembly::outb(slave_cmd, eoi);
  }
  assembly::outb(master_cmd, eoi);
}

void early::mask_irq(const uint8_t irq)
{
  const auto pic_data = irq < 8 ? master_data : slave_data;
  const auto mask = assembly::inb(pic_data) | (1 << (irq % 8));
  assembly::outb(pic_data, mask);
}

void early::unmask_irq(const uint8_t irq)
{
  const auto pic_data = irq < 8 ? master_data : slave_data;
  const auto mask = assembly::inb(pic_data) & ~(1 << (irq % 8));
  assembly::outb(pic_data, mask);
}

namespace {
  void mask_all()
  {
    assembly::outb(master_data, 0xFF);
    assembly::outb(slave_data, 0xFF);
  };
}
