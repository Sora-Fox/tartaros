#ifndef EARLY_INIT_PANIC_HPP
#define EARLY_INIT_PANIC_HPP

namespace early::detail {
  [[noreturn, gnu::format(printf, 3, 4)]]
  void do_panic(const char*, int, const char*, ...);
}

#define panic(...) early::detail::do_panic(__FILE__, __LINE__, __VA_ARGS__)

#endif
