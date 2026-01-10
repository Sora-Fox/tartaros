#include "early_init/panic.hpp"
#include <stdarg.h>
#include "early_init/asm_utils.hpp"
#include "early_init/console.hpp"

namespace {
  [[noreturn]] void halt();
}

void early::detail::do_panic(const char* file, int loc, const char* fmt, ...)
{
  printf("PANIC: %s:%d\nMessage:", file, loc);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  halt();
}

namespace {
  void halt()
  {
    assembly::cli();
    while (true) {
      assembly::hlt();
    }
  }
}
