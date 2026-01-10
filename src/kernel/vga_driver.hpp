#ifndef VGA_DRIVER_HPP
#define VGA_DRIVER_HPP

#include <stdint.h>
#include "video_driver.hpp"

namespace kernel {
  class VgaDriver final : public VideoDriver
  {
  public:
    explicit VgaDriver(void*);
    virtual ~VgaDriver() = default;

    void write_char(size_type x, size_type y, char, color fg, color bg) override;
    void clear(color bg) override;
    void scroll_line(color bg) override;

    [[nodiscard]] size_type width() override;
    [[nodiscard]] size_type height() override;

  private:
    constexpr static size_type width_ = 80;
    constexpr static size_type height_ = 25;

    volatile uint16_t* buffer_;
  };
}

#endif
