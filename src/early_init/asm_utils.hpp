#ifndef ASM_UTILS_HPP
#define ASM_UTILS_HPP

#include <stdint.h>

namespace assembly {
  inline void hlt();
  inline void sti();
  inline void cli();

  inline uint32_t get_cr2();
  inline uint32_t get_cr3();

  inline uint8_t inb(const uint16_t);
  inline void outb(const uint16_t, const uint8_t);
}

void assembly::hlt()
{
  asm volatile("hlt");
}

void assembly::cli()
{
  asm volatile("cli");
}

void assembly::sti()
{
  asm volatile("sti");
}

uint32_t assembly::get_cr2()
{
  uint32_t result = 0;
  asm volatile("movl %%cr2, %0" : "+r"(result));
  return result;
}

uint32_t assembly::get_cr3()
{
  uint32_t result = 0;
  asm volatile("movl %%cr3, %0" : "+r"(result));
  return result;
}

uint8_t assembly::inb(const uint16_t port)
{
  uint8_t value;
  asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
  return value;
}

void assembly::outb(const uint16_t port, const uint8_t value)
{
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif
