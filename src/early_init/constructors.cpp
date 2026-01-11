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

#include "constructors.hpp"

using init_func = void (*)();

extern "C" init_func _init_array_begin[];
extern "C" init_func _init_array_end[];
extern "C" init_func _fini_array_begin[];
extern "C" init_func _fini_array_end[];

void early::call_constructors()
{
  for (auto i = _init_array_begin; i != _init_array_end; ++i) {
    (*i)();
  }
}

void early::call_destructors()
{
  if (_fini_array_end <= +_fini_array_begin) {
    return;
  }
  for (auto i = _fini_array_end - 1; i != _fini_array_begin; --i) {
    (*i)();
  }
  (*_fini_array_begin)();
}
