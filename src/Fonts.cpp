/*
  Copyright 2020, Marc Draco & Daniel Melvin

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Alarm.h"
#include "Display.h"
#include "Environmental.h"
#include "Fonts.h"
#include "Flags.h"
#include "Graph.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "Messages.h"
#include "Reading.h"
#include "Sevensegments.h"
#include "Trig.h"
#include "types.h"


extern MCUFRIEND_kbv screen;
extern Display display;
extern Fonts fonts;

/**
 * @brief 
 * 
 * @param flashString 
 */

void Fonts::print(const __FlashStringHelper *flashString)
{
  PGM_P p = reinterpret_cast<PGM_P>(flashString);
  while (glyph_t glyph = pgm_read_byte(p++))
  {
    drawImgGlyph(glyph);
  }
}

/**
 * @brief Prints a C string 
 * 
 * @param b pointer to a null-terminated buffer
 */
void Fonts::print(char* b)
{
  int i{0};
  while (b[i])
  {
    drawImgGlyph(b[i]);
    ++i;
  }
}
/**
 * @brief 
 * 
 * @param X 
 * @param Y 
 * @param buffer 
 */
void Fonts::print(const int X, const int Y, char* buffer)
{
    int size = (m_bufferWidth * m_bufferHeight) >> 3;
    m_pixelBuffer = new char[size] {};

    if (! m_pixelBuffer)
    {
      screen.fillRect(X, Y, m_bufferWidth, m_bufferHeight, defaultInk);
      STOP
    }
    else
    { 
      screen.setCursor(X, Y);
      m_X = 0;
      m_Y = getYstep();

      int i{0};

      while (buffer[i])
      {
        glyphdata_t glyph;
        prepImgGlyph(findGlyphCode(buffer[i]), &glyph);
        bufferImgGlyph(buffer[i]);
        ++i;
      } 
      showBuffer(X, Y); 
      delete [] m_pixelBuffer;
    }
}

/**
 * @brief prints a C++ string type
 * 
 * @param string 
 */

void Fonts::print(const String &string)
{
  for (uint8_t i{0}; i < string.length(); i++)
  {
      drawImgGlyph(string.charAt(i));
  }
}

/**
 * @brief 
 * 
 * @param glyph 
 * @return glyph_t 
 */

glyph_t Fonts::findGlyphCode(const glyph_t glyph)
{
  // This searches the Flash memory for matching glyph
  // All this pulava is to reduce memory consumption
  // from a bunch of glyphs we'll never use. Bitmaps use 
  // a LOT of space we simply don't have any to waste.

  const gfxfont_t *gfxFont = m_pFont;
  uint8_t glyphCount  = static_cast<uint8_t>(pgm_read_byte(&gfxFont->glyphCount));
  
  int i {0};
  for (; i != glyphCount; ++i)
  {
    gfxglyph_t* theGlyph  = pgm_read_glyph_ptr(gfxFont, i);
    if (glyph == pgm_read_byte(&theGlyph->ascii))
    {
       return i;
    }
  }
  return 0;
}

/**
 * @brief Reads the "flash" data for the glyph
 * 
 * @param g 
 * @param data 
 */
void Fonts::prepImgGlyph(const glyph_t g, glyphdata_t* data)
{
    const gfxfont_t*  font  = m_pFont;
    gfxglyph_t* glyph  = pgm_read_glyph_ptr(font, g);

    data->bitmap       = pgm_read_bitmap_ptr(font);
    data->offset       = pgm_read_word(&glyph->bitmapOffset);
    data->dimensions.H = pgm_read_byte(&glyph->height);
    data->dimensions.W = pgm_read_byte(&glyph->width);
    data->xo           = pgm_read_byte(&glyph->xOffset);
    data->yo           = pgm_read_byte(&glyph->yOffset);
    data->xAdvance     = pgm_read_byte(&glyph->xAdvance);
    data->bits         = 0;
    data->bit          = 0;
    data->x            = fonts.getX();
    data->y            = fonts.getY();
    data->glyph        = g;
    data->colour       = display.getInk();
}

/**
 * @brief 
 * 
 * @param glyph 
 * @return uint8_t 
 */
uint8_t Fonts::bufferImgGlyph(const glyph_t glyph)
{
  screen.startWrite();
  glyphdata_t thisGlyph;
  prepImgGlyph(findGlyphCode(glyph), &thisGlyph);

  for (uint8_t i {0}; i < thisGlyph.dimensions.H; ++i) 
  {
      uint16_t X = m_X + thisGlyph.xo;
      uint16_t Y = m_Y + thisGlyph.yo + i;

      for (uint16_t j {0}; j < thisGlyph.dimensions.W; ++j) 
      {
          if (! (thisGlyph.bit & 0x07)) 
          {
            thisGlyph.bits = pgm_read_byte(&thisGlyph.bitmap[thisGlyph.offset++]);
          }
          
          if (thisGlyph.bits & 0x80) 
          {
            bufferPixel(X, Y);
          }
          ++X;
          ++thisGlyph.bit;
          thisGlyph.bits = thisGlyph.bits << 1;
      }
  }
  m_X = m_X + thisGlyph.xAdvance;
 
  screen.setCursor(m_X, m_Y);
  screen.endWrite();
  return thisGlyph.xAdvance;
}

uint8_t Fonts::drawImgGlyph(const glyph_t glyph)
{
  m_X = screen.getCursorX();
  m_Y = screen.getCursorY();
  colours_t ink = display.getInk();
  colours_t paper = display.getPaper();
  
  screen.startWrite();
  glyphdata_t thisGlyph;
  prepImgGlyph(findGlyphCode(glyph), &thisGlyph);

  for (uint8_t i {0}; i < thisGlyph.dimensions.H; ++i) 
  {
      uint16_t X = m_X + thisGlyph.xo;
      uint16_t Y = m_Y + thisGlyph.yo + i;

      for (uint16_t j {0}; j < thisGlyph.dimensions.W; ++j) 
      {
          if (! (thisGlyph.bit & 0x07)) 
          {
            thisGlyph.bits = pgm_read_byte(&thisGlyph.bitmap[thisGlyph.offset++]);
          }
          
          if (thisGlyph.bits & 0x80) 
          {
            printPixel(X, Y, ink);
          }
          else
          {
            printPixel(X, Y, paper);
          }
          ++X;
          ++thisGlyph.bit;
          thisGlyph.bits = thisGlyph.bits << 1;
      }
  }
  m_X = m_X + thisGlyph.xAdvance;
  screen.setCursor(m_X, m_Y);
  screen.endWrite();
  return thisGlyph.xAdvance;
}

void Fonts::bufferPixel(const uint16_t X, const uint16_t Y)
{
  if (X >= m_bufferWidth || Y >= m_bufferHeight) 
  {
    return;
  }
  int16_t byteAddress = ((Y * m_bufferWidth) + X) >> 3;  // fast divide by 8
  int16_t bit = 128 >> (X % 8);
  m_pixelBuffer[byteAddress] |= bit;
}

void Fonts::showBuffer(const int X, const int Y)
{
  colours_t ink = display.getInk();
  colours_t paper = display.getPaper();

  for (uint16_t y{0}; y < m_bufferHeight; ++y)
  {
    for (uint16_t x{0}; x < m_bufferWidth; ++x)
    {
      int16_t byteAddress  = ((y * m_bufferWidth) + x) >> 3;  // fast divide by 8
      uint8_t pixelAddress = x % 8;
      uint8_t bit          = 128 >> pixelAddress;

      if (m_pixelBuffer[byteAddress] & bit)
      {
        screen.drawPixel(x + X, y + Y, ink);
      }
      else
      {
        screen.drawPixel(x + X, y + Y, paper);
      }
    }
  }
}

dimensions_t Fonts::getGlyphDimensions(const glyph_t glyph)
{
    glyphdata_t G;
    glyph_t code = findGlyphCode(glyph);
    prepImgGlyph(code, &G);
    return {G.dimensions.W, G.dimensions.W};
}

void Fonts::printPixel(coordinate_t X, coordinate_t Y, colours_t ink)
{
  coordinate_t t;
  switch (m_rotation) 
  {
    case 1:
      t = X;
      X = TFT_WIDTH - 1 - Y;
      Y = t;
      break;
    case 2:
      X = TFT_WIDTH - 1 - X;
      Y = TFT_HEIGHT - 1 - Y;
      break;
    case 3:
      t = X;
      X = Y;
      Y = TFT_HEIGHT - 1 - t;
      break;
  }
  screen.writePixel(X, Y, ink);
}
