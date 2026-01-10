#ifndef VIDEO_DRIVER_HPP
#define VIDEO_DRIVER_HPP

#include <stddef.h>
#include <stdint.h>

namespace kernel {
  enum class color : uint8_t {
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    white,
    gray,
    light_blue,
    light_green,
    light_cyan,
    light_red,
    light_magenta,
    yellow,
    bright_white,
  };

  class VideoDriver
  {
  public:
    using size_type = size_t;

    virtual ~VideoDriver() = default;

    virtual void write_char(size_type x, size_type y, char, color fg, color bg) = 0;
    virtual void clear(color bg) = 0;
    virtual void scroll_line(color bg) = 0;

    [[nodiscard]] virtual size_type width() = 0;
    [[nodiscard]] virtual size_type height() = 0;
  };
}

#endif
