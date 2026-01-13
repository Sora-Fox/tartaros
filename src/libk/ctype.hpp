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

#ifndef LIBK_CTYPE_HPP
#define LIBK_CTYPE_HPP

namespace std {
  bool isalnum(int);
  bool isalpha(int);
  bool isblank(int);
  bool iscntrl(int);
  bool isdigit(int);
  bool isgraph(int);
  bool islower(int);
  bool isprint(int);
  bool ispunct(int);
  bool isspace(int);
  bool isupper(int);
  bool isxdigit(int);
  int tolower(int);
  int toupper(int);
}

#endif
