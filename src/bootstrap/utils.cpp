#include "bootstrap/utils.hpp"

namespace {
  [[gnu::section(".bootstrap.text")]] void outb(uint16_t port, uint8_t val)
  {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
  }

  [[gnu::section(".bootstrap.text")]] uint8_t inb(uint16_t port)
  {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
  }
}

void bootstrap::panic(const char* s)
{
  uint16_t com1 = 0x3F8;
  outb(com1 + 1, 0x00);
  outb(com1 + 3, 0x03);
  volatile uint16_t* vga = reinterpret_cast<uint16_t*>(0xB8000);
  for (uint16_t i = 0; i != 80 && *s; ++i, ++s) {
    vga[i] = (0x0C << 8) | *s;
    while ((inb(com1 + 5) & 0x20) == 0) {
    }
    outb(com1, *s);
  }
  while (true) {
    asm volatile("hlt");
  }
}
