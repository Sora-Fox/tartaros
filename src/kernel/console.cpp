#include "console.hpp"

kernel::Console::Console(VideoDriver* driver) : driver_(driver)
{
  // TODO: disabled because of cleaning early output
  //  driver_->clear(bg_color_);
}

void kernel::Console::set_color(const color fg)
{
  fg_color_ = fg;
}

void kernel::Console::write_string(const char* str)
{
  while (*str) {
    write_char(*str++);
  }
}

void kernel::Console::set_driver(VideoDriver* driver)
{
  driver_ = driver;
}

void kernel::Console::write_char(const char ch)
{
  if (ch >= 32 && ch <= 126) { /* TODO: Implement isprint() */
    if (x_cord_ + 1 == driver_->width()) {
      newline();
    }
    driver_->write_char(x_cord_, y_cord_, ch, fg_color_, bg_color_);
    ++x_cord_;
    return;
  }
  switch (ch) {
  case '\n':
    newline();
    break;
  case '\r':
    x_cord_ = 0;
    break;
  case '\t':
    write_string("    ");
    break;
  }
}

void kernel::Console::newline()
{
  if (y_cord_ + 1 == driver_->height()) {
    driver_->scroll_line(bg_color_);
  } else {
    ++y_cord_;
  }
  x_cord_ = 0;
}
