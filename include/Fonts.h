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

#ifndef __DRACO_HOTSTUFF_FONTS_H
#define __DRACO_HOTSTUFF_FONTS_H
#include "types.h"

class Fonts
{
  private:
    
    uint16_t  m_X {0};
    uint16_t  m_Y {0};
    uint8_t   m_rotation {0};
    char*     m_pixelBuffer {nullptr};
    uint16_t  m_bufferWidth {FONT_BUFF_WIDTH};  
    uint16_t  m_bufferHeight {FONT_BUFF_HEIGHT};
    const gfxfont_t* m_pFont {nullptr};
    
  public:

    Fonts() 
    {}

    Fonts(gfxfont_t* font) 
    {
      m_pFont = font;
    };

    ~Fonts() {}

    void init();

    void setBufferDimensions(const uint16_t W, const uint16_t H)
    {
      m_bufferWidth  = W;
      m_bufferHeight = H;
    }

    void setRotation(const uint8_t R)
    {
      m_rotation = R;
    }

    uint8_t getRotation(const uint8_t R)
    {
      return m_rotation;
    }

    void setFont(const gfxfont_t* pNewSize)
    {
      m_pFont = pNewSize;
    }

    const gfxfont_t* getFont()
    {
      return m_pFont;
    }

    
    int16_t getX()
    {
      return m_X;
    }

    int16_t getY()
    {
      return m_Y;
    }

    uint8_t getYstep()
 
   {
      const gfxfont_t* font  = m_pFont;
      return pgm_read_byte(&font->newline);
    }

    uint8_t getXstep()
    {
      const gfxfont_t* font  = m_pFont;
      return pgm_read_byte(&font->xMax);
    }

    uint8_t drawImgGlyph(const glyph_t glyph);

    void prepImgGlyph(const glyph_t glyph, glyphdata_t* data);
    
    uint8_t bufferImgGlyph(const glyph_t glyph);

    glyph_t findGlyphCode(const glyph_t glyph);
    
    dimensions_t getGlyphDimensions(const glyph_t glyph);

    void bleachGlyph(const glyph_t glyph);

    void print(const char* b);

    void print(const String &string);

    void print(const __FlashStringHelper *flashString);

    void print(char* b);

    void print(char* b1, const bool switchFloats);

    void print(const int X, const int Y, char* buffer);

    void setTextColor(const colours_t ink, const colours_t paper);  

    void printPixel(coordinate_t X, coordinate_t Y, colours_t ink);

    void bufferPixel(const uint16_t X, const uint16_t Y);

    void showBuffer(const int X, const int Y);
};

extern Fonts fonts;

#endif