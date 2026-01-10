#include "bootstrap/enter_virtual.hpp"
#include "common/multiboot.hpp"

#if !defined(__i386__)
#  error "This needs i386-elf compiler"
#endif

constexpr uint32_t align = 1 << 0;
constexpr uint32_t meminfo = 1 << 1;
constexpr uint32_t framebuffer = 1 << 2;
constexpr uint32_t flags = align | meminfo | framebuffer;
constexpr uint32_t checksum = -flags - MULTIBOOT_HEADER_MAGIC;

[[using gnu: aligned(MULTIBOOT_HEADER_ALIGN), used,
    section(".multiboot")]] const static multiboot_header mbh{
  .magic = MULTIBOOT_HEADER_MAGIC,
  .flags = flags,
  .checksum = checksum,

  .header_addr = 0,
  .load_addr = 0,
  .load_end_addr = 0,
  .bss_end_addr = 0,
  .entry_addr = 0,

  .mode_type = MULTIBOOT_FRAMEBUFFER_TYPE_RGB,
  .width = 1024,
  .height = 768,
  .depth = 32,
};

extern "C" [[using gnu: naked, section(".bootstrap.text"), noreturn]] void _start(void)
{
  asm volatile("movl $_stack_top, %%esp\n"
               "subl $_kernel_virtual_base, %%esp\n"
               "pushl %%ebx\n"
               "pushl %%eax\n"
               "call %P0\n"
      :
      : "i"(bootstrap::enter_virtaul)
      :);
}
