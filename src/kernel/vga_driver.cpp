#include "vga_driver.hpp"
#include "libk/algorithm.hpp"

kernel::VgaDriver::VgaDriver(void* const buffer) : buffer_(static_cast<uint16_t*>(buffer))
{
  /* TODO: Disable hardware cursor */
}

void kernel::VgaDriver::write_char(size_type x, size_type y, char ch, color fg, color bg)
{
  const auto ch_bits = static_cast<uint16_t>(ch);
  const auto fg_bits = static_cast<uint16_t>(fg) << 8;
  const auto bg_bits = static_cast<uint16_t>(bg) << 12;
  const auto x_cord = std::min(x, width_ - 1);
  const auto y_cord = std::min(y, height_ - 1);
  const auto idx = y_cord * width_ + x_cord;
  buffer_[idx] = bg_bits | fg_bits | ch_bits;
}

void kernel::VgaDriver::clear(const color bg)
{
  constexpr static auto char_bits = static_cast<uint16_t>(' ');
  const auto bg_bits = static_cast<uint16_t>(bg) << 12;
  const auto value = bg_bits | char_bits;
  std::fill(buffer_, buffer_ + width_ * height_, value);
}

void kernel::VgaDriver::scroll_line(const color bg)
{
  constexpr static auto char_bits = static_cast<uint16_t>(' ');
  const auto bg_bits = static_cast<uint16_t>(bg) << 12;
  const auto value = bg_bits | char_bits;
  const auto buffer_end = buffer_ + width_ * height_;
  const auto second_line = buffer_ + width_;
  const auto last_line = buffer_end - width_;
  std::copy(second_line, buffer_end, buffer_);
  std::fill(last_line, buffer_end, value);
}

kernel::VgaDriver::size_type kernel::VgaDriver::width()
{
  return width_;
}

kernel::VgaDriver::size_type kernel::VgaDriver::height()
{
  return height_;
}
