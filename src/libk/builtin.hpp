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
