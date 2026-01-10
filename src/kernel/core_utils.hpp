#ifndef CORE_UTILS_HPP
#define CORE_UTILS_HPP

#include <stdarg.h>

#define panic(fmt, ...) \
  kernel::detail::do_panic(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)

namespace kernel {
  namespace detail {
    [[noreturn]] void do_panic(const char*, int, const char*, const char*, ...);
  }

  void printf(const char*, ...);

  [[noreturn]] void halt();
}

#endif
