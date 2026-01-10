#ifndef LIBK_VSNPRINTF_HPP
#define LIBK_VSNPRINTF_HPP

#include <stdarg.h>
#include <stddef.h>

namespace std {
  int vsnprintf(char*, size_t, const char*, va_list);
}

#endif
