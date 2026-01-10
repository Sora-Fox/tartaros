#include "video_driver.hpp"

#include "libk/vsnprintf.hpp"

namespace kernel {
  class Console
  {
  public:
    explicit Console(VideoDriver*);
    void set_color(color);
    void write_string(const char*);
    void set_driver(VideoDriver*);
    /* TODO */
    void printf(const char* str, ...)
    {
      char buffer[512];
      va_list args;
      va_start(args, str);
      std::vsnprintf(buffer, 512, str, args);
      write_string(buffer);
      va_end(args);
    }

  private:
    using size_type = VideoDriver::size_type;

    VideoDriver* driver_;
    size_type x_cord_ = 0;
    size_type y_cord_ = 0;
    color bg_color_ = color::black;
    color fg_color_ = color::bright_white;

    void write_char(char);
    void newline();
  };
}
