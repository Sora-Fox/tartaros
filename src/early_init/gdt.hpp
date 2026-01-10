#ifndef GDT_HPP
#define GDT_HPP

#include <stdint.h>

namespace early {
  extern const uint16_t k_cs;
  extern const uint16_t k_ds;
  extern const uint16_t u_cs;
  extern const uint16_t u_ds;

  void init_gdt();
}

#endif
