/*
 * This file is part of the Tartaros project.
 *
 * Copyright (C) 2026 Sora Fox [github.com/Sora-Fox]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "early_init/framebuffer.hpp"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include "libk/ctype.hpp"

#define PSF1_FONT_MAGIC 0x0436
#define PSF_FONT_MAGIC 0x864ab572

extern char _binary_font_psf_start;
extern char _binary_font_psf_end;
char* fb;
int scanline;

typedef struct
{
  uint16_t magic;        // Magic bytes for identification.
  uint8_t fontMode;      // PSF font mode.
  uint8_t characterSize; // PSF character size.
} PSF1_Header;

/* PSF2 */
typedef struct
{
  uint32_t magic;         /* magic bytes to identify PSF */
  uint32_t version;       /* zero */
  uint32_t headersize;    /* offset of bitmaps in file, 32 */
  uint32_t flags;         /* 0 if there's no unicode table */
  uint32_t numglyph;      /* number of glyphs */
  uint32_t bytesperglyph; /* size of each glyph */
  uint32_t height;        /* height in pixels */
  uint32_t width;         /* width in pixels */
} PSF_font;

uint16_t* unicode;

void psf_init()
{
  static char buf[USHRT_MAX * 2];
  uint16_t glyph = 0; /* cast the address to PSF header struct */
  PSF_font* font = (PSF_font*)&_binary_font_psf_start; /* is there a unicode table? */
  if (font->flags == 0) {
    unicode = nullptr;
    return;
  }

  /* get the offset of the table */
  char* s = (char*)((unsigned char*)&_binary_font_psf_start + font->headersize +
                    font->numglyph * font->bytesperglyph);
  /* allocate memory for translation table */
  // unicode = static_cast<uint16_t*>(calloc(USHRT_MAX, 2));
  unicode = reinterpret_cast<uint16_t*>(buf);
  while ((unsigned char*)s < (unsigned char*)&_binary_font_psf_end) {
    uint16_t uc = (uint16_t)(((unsigned char*)s)[0]);
    if (uc == 0xFF) {
      glyph++;
      s++;
      continue;
    } else if (uc & 128) {
      /* UTF-8 to unicode */
      if ((uc & 32) == 0) {
        uc = ((s[0] & 0x1F) << 6) + (s[1] & 0x3F);
        s++;
      } else if ((uc & 16) == 0) {
        uc = ((((s[0] & 0xF) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F);
        s += 2;
      } else if ((uc & 8) == 0) {
        uc = ((((((s[0] & 0x7) << 6) + (s[1] & 0x3F)) << 6) + (s[2] & 0x3F)) << 6) +
             (s[3] & 0x3F);
        s += 3;
      } else
        uc = 0;
    }
    /* save translation */
    unicode[uc] = glyph;
    s++;
  }
}

#define PIXEL uint32_t /* pixel pointer */

void putchar(
    /* note that this is int, not char as it's a unicode character */
    unsigned short int c,
    /* cursor position on screen, in characters not in pixels */
    int cx, int cy,
    /* foreground and background colors, say 0xFFFFFF and 0x000000 */
    uint32_t fg, uint32_t bg)
{
  /* cast the address to PSF header struct */
  PSF_font* font = (PSF_font*)&_binary_font_psf_start;
  /* unicode translation */
  if (unicode != NULL) {
    c = unicode[c];
  }
  /* get the glyph for the character. If there's no
     glyph for a given character, we'll display the first glyph. */
  unsigned char* glyph = (unsigned char*)&_binary_font_psf_start + font->headersize +
                         (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;
  /* calculate the upper left corner on screen where we want to display.
     we only do this once, and adjust the offset later. This is faster. */
  int offs = (cy * font->height * scanline) + (cx * (font->width + 1) * sizeof(PIXEL));

  /* Calculate the number of bytes for a line in a glyph. If the
  glyph width isn't byte aligned, then it is rounded up to be byte aligned. */
  uint32_t bytesPerGlyphLine = (font->width + 7) / 8;
  /* finally display pixels according to the bitmap */
  uint32_t x, y, line;
  for (y = 0; y < font->height; y++) {
    /* save the starting position of the line */
    line = offs;
    /* Calulate where the first byte for this line of the glyph is. */
    unsigned char* currentByte = glyph + (bytesPerGlyphLine * y);
    /* Start with a mask at this byte's MSB */
    uint8_t mask = 1 << 7;
    /* display a row */
    for (x = 0; x < font->width; x++) {
      *((PIXEL*)(fb + line)) = (*currentByte & mask) ? fg : bg;
      mask >>= 1;
      if (mask == 0) {
        /* We have read this byte of the glyph.
        Reset mask and move to next byte */
        mask = 1 << 7;
        currentByte += 1;
      }
      /* adjust to the next pixel in framebuffer */
      line += sizeof(PIXEL);
    }
    /* adjust to the next line in framebuffer */
    offs += scanline;
  }
}

namespace {
  int width = 0;
  int height = 0;
}

void early::init_framebuffer(void* fb_ptr, int pitch, int w, int h)
{
  psf_init();
  fb = static_cast<char*>(fb_ptr);
  scanline = pitch;
  width = w;
  height = h;
}

void early::framebuffer_out_char(char ch)
{
  auto white = 0xFFFFFF;
  auto black = 0x000000;
  static int x = 0;
  static int y = 0;
  if (ch == '\n') {
    if (y + 1 == height) {
      x = 0;
      y = 0;
    } else {
      ++y;
      x = 0;
    }
  }
  if (!std::isprint(ch)) {
    return;
  }
  putchar(ch, x, y, white, black);
  if (x + 1 == width) {
    if (y + 1 == height) {
      x = 0;
      y = 0;
    } else {
      ++y;
      x = 0;
    }
  } else {
    ++x;
  }
}
