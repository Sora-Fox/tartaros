#include "libk/vsnprintf.hpp"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

namespace {
  bool is_digit(char c)
  {
    return c >= '0' && c <= '9';
  }

  size_t cstrlen(const char* s)
  {
    if (!s)
      return 0;
    const char* p = s;
    while (*p)
      ++p;
    return (size_t)(p - s);
  }

  struct OutputState
  {
    char* buf;
    size_t buf_size;
    size_t buf_pos;
    size_t total;
  };

  void out_put_char(OutputState& st, char c)
  {
    ++st.total;
    if (st.buf && st.buf_size > 0 && st.buf_pos + 1 < st.buf_size) {
      st.buf[st.buf_pos++] = c;
    }
  }

  void out_write(OutputState& st, const char* s, size_t len)
  {
    for (size_t i = 0; i < len; ++i)
      out_put_char(st, s[i]);
  }

  enum LengthMod {
    LEN_NONE,
    LEN_HH,
    LEN_H,
    LEN_L,
#if defined(ENABLE_LONG_LONG)
    LEN_LL,
#endif
    LEN_Z
  };

  LengthMod parse_length(const char*& p)
  {
    if (p[0] == 'h' && p[1] == 'h') {
      p += 2;
      return LEN_HH;
    }
    if (p[0] == 'h') {
      ++p;
      return LEN_H;
    }
#if defined(ENABLE_LONG_LONG)
    if (p[0] == 'l' && p[1] == 'l') {
      p += 2;
      return LEN_LL;
    }
#endif
    if (p[0] == 'l') {
      ++p;
      return LEN_L;
    }
    if (p[0] == 'z') {
      ++p;
      return LEN_Z;
    }
    return LEN_NONE;
  }

#if defined(ENABLE_LONG_LONG)
  long long
#else
  long
#endif
      long get_signed_arg(va_list& ap, LengthMod lm)
  {
    switch (lm) {
    case LEN_HH:
      return (signed char)va_arg(ap, int);
    case LEN_H:
      return (short)va_arg(ap, int);
    case LEN_L:
      return (long)va_arg(ap, long);
#if defined(ENABLE_LONG_LONG)
    case LEN_LL:
      return (long long)va_arg(ap, long long);
#endif
    case LEN_Z:
      return va_arg(ap, ptrdiff_t);
    default:
      return va_arg(ap, int);
    }
  }

#if defined(ENABLE_LONG_LONG)
  unsigned long long
#else
  size_t
#endif
  get_unsigned_arg(va_list& ap, LengthMod lm)

  {
    switch (lm) {
    case LEN_HH:
      return (unsigned char)va_arg(ap, unsigned int);
    case LEN_H:
      return (unsigned short)va_arg(ap, unsigned int);
    case LEN_L:
      return (unsigned long)va_arg(ap, unsigned long);
#if defined(ENABLE_LONG_LONG)
    case LEN_LL:
      return (unsigned long long)va_arg(ap, unsigned long long);
#endif
    case LEN_Z:
      return (size_t)va_arg(ap, size_t);
    default:
      return (unsigned long)va_arg(ap, unsigned int);
    }
  }

  void unsigned_to_buffer(size_t value, int base, bool upper, char* out, size_t& out_len)
  {
    const char* digits_l = "0123456789abcdef";
    const char* digits_u = "0123456789ABCDEF";
    const char* digits = upper ? digits_u : digits_l;
    char tmp[65];
    size_t tp = 0;
    if (value == 0) {
      tmp[tp++] = '0';
    } else {
      while (value && tp < sizeof(tmp)) {
        tmp[tp++] = digits[value % base];
        value /= base;
      }
    }
    out_len = 0;
    for (size_t i = 0; i < tp; ++i) {
      out[out_len++] = tmp[tp - 1 - i];
    }
  }

  void format_integer_common(OutputState& st, bool is_signed, long long svalue,
      unsigned long long uvalue, int base, bool upper, int width, int precision,
      bool left_adjust, bool show_sign, bool space_sign, bool zero_pad, bool alt_form)
  {
    char numbuf[65];
    size_t numlen = 0;
    bool negative = false;

    if (is_signed) {
      if (svalue < 0) {
        negative = true;
        unsigned long long mag = (unsigned long long)(-(svalue + 1)) + 1;
        unsigned_to_buffer(mag, base, upper, numbuf, numlen);
      } else {
        unsigned_to_buffer((unsigned long long)svalue, base, upper, numbuf, numlen);
      }
    } else {
      unsigned_to_buffer(uvalue, base, upper, numbuf, numlen);
    }

    if (precision == 0 && numlen == 1 && numbuf[0] == '0')
      numlen = 0;

    char prefix[3];
    size_t prefix_len = 0;

    if (negative)
      prefix[prefix_len++] = '-';
    else if (show_sign)
      prefix[prefix_len++] = '+';
    else if (space_sign)
      prefix[prefix_len++] = ' ';

    if (alt_form && base == 16 && numlen > 0) {
      prefix[prefix_len++] = '0';
      prefix[prefix_len++] = upper ? 'X' : 'x';
    }

    int zero_count = 0;
    if (precision >= 0) {
      zero_count = precision - (int)numlen;
    } else if (zero_pad && !left_adjust) {
      zero_count = width - (int)(prefix_len + numlen);
    }
    if (zero_count < 0)
      zero_count = 0;

    int total_len = (int)(prefix_len + zero_count + numlen);
    int space_pad = width > total_len ? width - total_len : 0;

    if (!left_adjust) {
      for (int i = 0; i < space_pad; ++i)
        out_put_char(st, ' ');
    }

    for (size_t i = 0; i < prefix_len; ++i)
      out_put_char(st, prefix[i]);
    for (int i = 0; i < zero_count; ++i)
      out_put_char(st, '0');
    for (size_t i = 0; i < numlen; ++i)
      out_put_char(st, numbuf[i]);

    if (left_adjust) {
      for (int i = 0; i < space_pad; ++i)
        out_put_char(st, ' ');
    }
  }

  void format_string(OutputState& st, const char* s, int width, int precision,
      bool left_adjust)
  {
    if (!s)
      s = "(null)";
    size_t len = cstrlen(s);
    if (precision >= 0 && (size_t)precision < len)
      len = (size_t)precision;

    int pad = width > (int)len ? width - (int)len : 0;

    if (!left_adjust) {
      for (int i = 0; i < pad; ++i)
        out_put_char(st, ' ');
    }
    out_write(st, s, len);
    if (left_adjust) {
      for (int i = 0; i < pad; ++i)
        out_put_char(st, ' ');
    }
  }

  void format_char(OutputState& st, char c, int width, bool left_adjust)
  {
    int pad = width > 1 ? width - 1 : 0;
    if (!left_adjust) {
      for (int i = 0; i < pad; ++i)
        out_put_char(st, ' ');
    }
    out_put_char(st, c);
    if (left_adjust) {
      for (int i = 0; i < pad; ++i)
        out_put_char(st, ' ');
    }
  }

}

int std::vsnprintf(char* buffer, size_t size, const char* format, va_list ap)
{
  if (!format) {
    if (buffer && size)
      buffer[0] = '\0';
    return 0;
  }

  OutputState st;
  st.buf = buffer;
  st.buf_size = size;
  st.buf_pos = 0;
  st.total = 0;

  const char* p = format;

  while (*p) {
    if (*p != '%') {
      out_put_char(st, *p++);
      continue;
    }

    ++p;

    bool left_adjust = false;
    bool show_sign = false;
    bool space_sign = false;
    bool zero_pad = false;
    bool alt_form = false;

    for (;;) {
      if (*p == '-')
        left_adjust = true;
      else if (*p == '+')
        show_sign = true;
      else if (*p == ' ')
        space_sign = true;
      else if (*p == '0')
        zero_pad = true;
      else if (*p == '#')
        alt_form = true;
      else
        break;
      ++p;
    }

    int width = 0;
    if (*p == '*') {
      width = va_arg(ap, int);
      if (width < 0) {
        left_adjust = true;
        width = -width;
      }
      ++p;
    } else {
      while (is_digit(*p)) {
        width = width * 10 + (*p - '0');
        ++p;
      }
    }

    int precision = -1;
    if (*p == '.') {
      ++p;
      precision = 0;
      if (*p == '*') {
        precision = va_arg(ap, int);
        if (precision < 0)
          precision = -1;
        ++p;
      } else {
        while (is_digit(*p)) {
          precision = precision * 10 + (*p - '0');
          ++p;
        }
      }
    }

    LengthMod lm = parse_length(p);
    char spec = *p ? *p++ : '\0';

    switch (spec) {
    case 's':
      format_string(st, va_arg(ap, const char*), width, precision, left_adjust);
      break;
    case 'c':
      format_char(st, (char)va_arg(ap, int), width, left_adjust);
      break;
    case 'd':
    case 'i':
      format_integer_common(st, true, get_signed_arg(ap, lm), 0, 10, false, width,
          precision, left_adjust, show_sign, space_sign, zero_pad && precision < 0,
          false);
      break;
    case 'u':
      format_integer_common(st, false, 0, get_unsigned_arg(ap, lm), 10, false, width,
          precision, left_adjust, false, false, zero_pad && precision < 0, false);
      break;
    case 'x':
    case 'X':
      format_integer_common(st, false, 0, get_unsigned_arg(ap, lm), 16, spec == 'X',
          width, precision, left_adjust, false, false, zero_pad && precision < 0,
          alt_form);
      break;
    case 'p': {
      uintptr_t v = (uintptr_t)va_arg(ap, void*);
      out_put_char(st, '0');
      out_put_char(st, 'x');
      char buf[2 * sizeof(uintptr_t)];
      size_t len;
      unsigned_to_buffer(v, 16, false, buf, len);
      if (!len)
        out_put_char(st, '0');
      else
        out_write(st, buf, len);
      break;
    }
    case '%':
      out_put_char(st, '%');
      break;
    default:
      out_put_char(st, '%');
      if (spec)
        out_put_char(st, spec);
      break;
    }
  }

  if (st.buf && st.buf_size) {
    size_t pos = st.buf_pos < st.buf_size ? st.buf_pos : st.buf_size - 1;
    st.buf[pos] = '\0';
  }

  return (int)st.total;
}
