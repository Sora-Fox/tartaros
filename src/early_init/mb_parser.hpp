#ifndef MB_PARSER_HPP
#define MB_PARSER_HPP

#include <stddef.h>
#include <stdint.h>

struct multiboot_info;

namespace early {

  struct mb_info
  {
    size_t total_ram_bytes = 0;
    uintptr_t heap_phys_addr = 0;
    size_t heap_size_bytes = 0;
    bool framebuffer_present = false;
  };

  [[nodiscard]] bool parse_multiboot(uint32_t, const multiboot_info*, mb_info*);
}

#endif
