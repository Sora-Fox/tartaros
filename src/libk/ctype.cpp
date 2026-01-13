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

#include "libk/ctype.hpp"

bool std::isalnum(int c)
{
  return isdigit(c) || isalpha(c);
}

bool std::isalpha(int c)
{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool std::isblank(int c)
{
  return c == ' ' || c == '\t';
}

bool std::iscntrl(int c)
{
  return (c >= 0 && c <= 31) || c == 127;
}

bool std::isdigit(int c)
{
  return c >= '0' && c <= '9';
}

bool std::isgraph(int c)
{
  return c >= 33 && c <= 126;
}

bool std::islower(int c)
{
  return c >= 'a' && c <= 'z';
}

bool std::isprint(int c)
{
  return c >= 32 && c <= 126;
}

bool std::ispunct(int c)
{
  bool result = false;
  result = result || (c >= 33 && c <= 47);
  result = result || (c >= 58 && c <= 64);
  result = result || (c >= 91 && c <= 96);
  result = result || (c >= 123 && c <= 126);
  return result;
}

bool std::isspace(int c)
{
  return isblank(c) || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

bool std::isupper(int c)
{
  return c >= 'A' && c <= 'Z';
}

bool std::isxdigit(int c)
{
  int lower = tolower(c);
  return isdigit(c) || (lower >= 'a' && lower <= 'f');
}

int std::tolower(int c)
{
  if (c >= 'A' && c <= 'Z') {
    return c + ('a' - 'A');
  }
  return c;
}

int toupper(int c)
{
  if (c >= 'a' && c <= 'z') {
    return c - ('a' - 'A');
  }
  return c;
}
