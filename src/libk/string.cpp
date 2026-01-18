/*
 * this file is part of the tartaros project.
 *
 * copyright (c) 2026 sora fox [github.com/sora-fox]
 *
 * this program is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with this program. if not, see <http://www.gnu.org/licenses/>.
 */

#include "libk/string.hpp"

void* memcpy(void* dest, const void* src, size_t n)
{
  unsigned char* d = static_cast<unsigned char*>(dest);
  const unsigned char* s = static_cast<const unsigned char*>(src);
  for (size_t i = 0; i < n; ++i) {
    d[i] = s[i];
  }
  return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
  unsigned char* d = static_cast<unsigned char*>(dest);
  const unsigned char* s = static_cast<const unsigned char*>(src);

  if (d < s || d >= s + n) {
    for (size_t i = 0; i < n; ++i) {
      d[i] = s[i];
    }
  } else if (d > s) {
    for (size_t i = n; i > 0; --i) {
      d[i - 1] = s[i - 1];
    }
  }
  return dest;
}

void* memset(void* dest, int c, size_t n)
{
  unsigned char* d = static_cast<unsigned char*>(dest);
  unsigned char value = static_cast<unsigned char>(c);
  for (size_t i = 0; i < n; ++i) {
    d[i] = value;
  }
  return dest;
}

int memcmp(const void* lhs, const void* rhs, size_t n)
{
  const unsigned char* l = static_cast<const unsigned char*>(lhs);
  const unsigned char* r = static_cast<const unsigned char*>(rhs);
  for (size_t i = 0; i < n; ++i) {
    if (l[i] != r[i]) {
      return l[i] < r[i] ? -1 : 1;
    }
  }
  return 0;
}

void* memchr(const void* ptr, int c, size_t n)
{
  const unsigned char* p = static_cast<const unsigned char*>(ptr);
  unsigned char ch = static_cast<unsigned char>(c);
  for (size_t i = 0; i < n; ++i) {
    if (p[i] == ch) {
      return const_cast<void*>(static_cast<const void*>(p + i));
    }
  }
  return nullptr;
}

size_t strlen(const char* str)
{
  size_t len = 0;
  while (str[len] != '\0') {
    ++len;
  }
  return len;
}

char* strcpy(char* dest, const char* src)
{
  char* d = dest;
  while ((*d++ = *src++)) {
  }
  return dest;
}

char* strncpy(char* dest, const char* src, size_t n)
{
  char* d = dest;
  size_t i = 0;
  while (i < n && (*d++ = *src++)) {
    ++i;
  }
  while (i < n) {
    *d++ = '\0';
    ++i;
  }
  return dest;
}

char* strcat(char* dest, const char* src)
{
  char* d = dest;
  while (*d) {
    ++d;
  }
  while ((*d++ = *src++)) {
  }
  return dest;
}

char* strncat(char* dest, const char* src, size_t n)
{
  char* d = dest;
  while (*d) {
    ++d;
  }
  while (n-- && (*d++ = *src++)) {
  }
  *d = '\0';
  return dest;
}

int strcmp(const char* lhs, const char* rhs)
{
  const unsigned char* l = reinterpret_cast<const unsigned char*>(lhs);
  const unsigned char* r = reinterpret_cast<const unsigned char*>(rhs);
  while (*l && *l == *r) {
    ++l;
    ++r;
  }
  return *l - *r;
}

int strncmp(const char* lhs, const char* rhs, size_t n)
{
  const unsigned char* l = reinterpret_cast<const unsigned char*>(lhs);
  const unsigned char* r = reinterpret_cast<const unsigned char*>(rhs);
  for (size_t i = 0; i < n; ++i) {
    if (l[i] != r[i] || l[i] == '\0') {
      return l[i] - r[i];
    }
  }
  return 0;
}

char* strchr(const char* str, int c)
{
  unsigned char ch = static_cast<unsigned char>(c);
  const unsigned char* s = reinterpret_cast<const unsigned char*>(str);
  while (*s) {
    if (*s == ch) {
      return const_cast<char*>(reinterpret_cast<const char*>(s));
    }
    ++s;
  }
  return (*s == ch) ? const_cast<char*>(reinterpret_cast<const char*>(s)) : nullptr;
}

char* strrchr(const char* str, int c)
{
  unsigned char ch = static_cast<unsigned char>(c);
  const unsigned char* s = reinterpret_cast<const unsigned char*>(str);
  const unsigned char* last = nullptr;
  do {
    if (*s == ch) {
      last = s;
    }
  } while (*s++);
  return const_cast<char*>(reinterpret_cast<const char*>(last));
}

size_t strspn(const char* dest, const char* src)
{
  const char* d = dest;
  while (*d) {
    const char* s = src;
    while (*s && *s != *d) {
      ++s;
    }
    if (*s == '\0') {
      break;
    }
    ++d;
  }
  return d - dest;
}

size_t strcspn(const char* dest, const char* src)
{
  const char* d = dest;
  while (*d) {
    const char* s = src;
    while (*s && *s != *d) {
      ++s;
    }
    if (*s != '\0') {
      break;
    }
    ++d;
  }
  return d - dest;
}

char* strpbrk(const char* dest, const char* src)
{
  const char* d = dest;
  while (*d) {
    const char* s = src;
    while (*s && *s != *d) {
      ++s;
    }
    if (*s != '\0') {
      return const_cast<char*>(d);
    }
    ++d;
  }
  return nullptr;
}

char* strstr(const char* haystack, const char* needle)
{
  if (!*needle) {
    return const_cast<char*>(haystack);
  }
  const char* h = haystack;
  const char* n = needle;
  while (*h) {
    if (*h == *n) {
      const char* h2 = h + 1;
      const char* n2 = n + 1;
      while (*n2 && *h2 == *n2) {
        ++h2;
        ++n2;
      }
      if (!*n2) {
        return const_cast<char*>(h);
      }
    }
    ++h;
  }
  return nullptr;
}

char* strtok(char* str, const char* delim)
{
  static char* last = nullptr;
  if (str) {
    last = str;
  } else if (!last) {
    return nullptr;
  }

  while (*last) {
    const char* d = delim;
    while (*d && *d != *last) {
      ++d;
    }
    if (!*d) {
      break;
    }
    ++last;
  }
  if (!*last) {
    last = nullptr;
    return nullptr;
  }

  char* token = last;

  while (*last) {
    const char* d = delim;
    while (*d && *d != *last) {
      ++d;
    }
    if (*d) {
      *last++ = '\0';
      return token;
    }
    ++last;
  }

  last = nullptr;
  return token;
}
