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

#ifndef LIBK_BUILTIN_HPP
#define LIBK_BUILTIN_HPP

#include <stddef.h>
#include <stdint.h>

extern "C" inline void* memcpy(void* dst, const void* src, size_t n)
{
  return __builtin_memcpy(dst, src, n);
}

extern "C" inline void* memset(void* s, int c, size_t n)
{
  return __builtin_memset(s, c, n);
}

extern "C" inline void* memmove(void* dst, const void* src, size_t n)
{
  return __builtin_memmove(dst, src, n);
}

extern "C" inline size_t strlen(const char* s)
{
  return __builtin_strlen(s);
}

extern "C" inline int memcmp(const void* s1, const void* s2, size_t n)
{
  return __builtin_memcmp(s1, s2, n);
}

extern "C" inline char* strchr(const char* s, int c)
{
  return __builtin_strchr(s, c);
}

extern "C" inline char* strrchr(const char* s, int c)
{
  return __builtin_strrchr(s, c);
}

extern "C" inline int strcmp(const char* s1, const char* s2)
{
  return __builtin_strcmp(s1, s2);
}

extern "C" inline int strncmp(const char* s1, const char* s2, size_t n)
{
  return __builtin_strncmp(s1, s2, n);
}

#endif
