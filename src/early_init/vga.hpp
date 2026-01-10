#ifndef EARLY_INIT_VGA_HPP
#define EARLY_INIT_VGA_HPP

#include <stdint.h>

namespace early {
  void init_vga(void*);
  void vga_out_char(char);
}

#endif
