#ifndef EARLY_INIT_CONSOLE_HPP
#define EARLY_INIT_CONSOLE_HPP

#include <stdarg.h>

namespace early {
  using output_engine_t = void (*)(char);
  void init_console(output_engine_t);

  [[gnu::format(printf, 1, 2)]]
  void printf(const char*, ...);
  void vprintf(const char*, va_list);
  void puts(const char*);
}

#endif
