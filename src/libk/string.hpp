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

#ifndef LIBK_STRING_HPP
#define LIBK_STRING_HPP

#include <stddef.h>

extern "C" {
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int c, size_t n);
int memcmp(const void* lhs, const void* rhs, size_t n);
void* memchr(const void* ptr, int c, size_t n);

size_t strlen(const char* str);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

int strcmp(const char* lhs, const char* rhs);
int strncmp(const char* lhs, const char* rhs, size_t n);

char* strchr(const char* str, int c);
char* strrchr(const char* str, int c);

size_t strspn(const char* dest, const char* src);
size_t strcspn(const char* dest, const char* src);

char* strpbrk(const char* dest, const char* src);
char* strstr(const char* haystack, const char* needle);

char* strtok(char* str, const char* delim);
}

namespace std {
  using ::memchr;
  using ::memcmp;
  using ::memcpy;
  using ::memmove;
  using ::memset;
  using ::strcat;
  using ::strchr;
  using ::strcmp;
  using ::strcpy;
  using ::strcspn;
  using ::strlen;
  using ::strncat;
  using ::strncmp;
  using ::strncpy;
  using ::strpbrk;
  using ::strrchr;
  using ::strspn;
  using ::strstr;
  using ::strtok;
}

#endif
