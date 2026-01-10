#include "early_init/vga.hpp"
#include <stddef.h>
#include <stdint.h>
#include "libk/algorithm.hpp"

namespace {
  constexpr size_t width = 80;
  constexpr size_t height = 25;

  volatile uint16_t* buffer = nullptr;
  constexpr uint16_t color = 0x0F;

  size_t cursor_x = 0;
  size_t cursor_y = 0;

  void clear_screen();
  void scroll_line();
  void newline();

}

void early::init_vga(void* buf)
{
  buffer = static_cast<uint16_t*>(buf);
  clear_screen();
}

void early::vga_out_char(char ch)
{
  switch (ch) {
  case '\n':
    newline();
    return;
  case '\r':
    cursor_x = 0;
    return;
  }
  if (ch < 32 || ch > 126) { /* TODO: Implement isprint() */
    return;
  }
  if (cursor_x + 1 == width) {
    newline();
  }
  const auto idx = cursor_y * width + cursor_x;
  buffer[idx] = (color << 8) | static_cast<uint16_t>(ch);
  ++cursor_x;
}

namespace {
  void clear_screen()
  {
    const auto value = (color << 8) | static_cast<uint16_t>(' ');
    std::fill(buffer, buffer + width * height, value);
  }

  void scroll_line()
  {
    const auto value = (color << 8) | static_cast<uint16_t>(' ');
    const auto buffer_end = buffer + width * height;
    const auto second_line = buffer + width;
    const auto last_line = buffer_end - width;
    std::copy(second_line, buffer_end, buffer);
    std::fill(last_line, buffer_end, value);
  }

  void newline()
  {
    if (cursor_y + 1 == height) {
      scroll_line();
    } else {
      ++cursor_y;
    }
    cursor_x = 0;
  }
}
