/*
  Copyright 2020, Marc Draco & Daniel Melvin

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

  3. Commercial use must include a per-unit donation of not less than 5% of the unit
  margin to the "Astro Nerd" cancer fund. Non commerical users may use this software
  without paying, but all donations to Rose's treatment are welcomed. Neither of the
  authors are connected to Rose "DF", but it's hoped that this software, named in her
  honour will help raise awareness and some money to help take the rough edges of her 
  life. Cancer sucks people. It could hit any of us, at any time! Follow Rose on
  Twitter @_Astro_Nerd_

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

#include <Arduino.h>
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
extern Sevensegments segments;

/**
 * @brief Dim a colour by a percentage 
 * 
 * @param C The colour value as 5-6-5 encoded.
 * @param brightness 100% (max) to 0% (min) Steps are about 7% (so 15 levels)
 * @return colours_t the new colour value
 * @remarks Due to low number of bits per colour, this is ONLY approximate!
 */

colours_t Sevensegments::dimmer(colours_t C, uint8_t brightness)
{
  // decompose from 5 - 6 - 5 RGB to separate RGB channels
  // R and B channels are 0 -- 31 and G is 0 -- 63 
  // The first three lines normalise them to 8 bit values
  if (brightness == 100)
  {
    return C; // no dimming at all
  }
  uint16_t r = ((C & 0xF800) >> 8);
  uint16_t g = ((C & 0x07E0) >> 3);
  uint16_t b = ((C & 0x001F) << 3);

  float rLevel = r/31.0 * brightness; 
  float gLevel = g/63.0 * brightness; 
  float bLevel = b/31.0 * brightness; 

  return RGB(static_cast<int8_t>(rLevel), static_cast<int8_t>(gLevel), static_cast<int8_t>(bLevel));
}

void Sevensegments::drawGlyph(const coordinate_t X, const coordinate_t Y, const uint8_t glyph, uint8_t wide, uint8_t high, const uint8_t rows, const uint8_t bias)
{
  wide = ((wide >> 1) << 1);
  high = ((high >> 1) << 1);
  
  setXlen(wide);
  setYlen(high);
  setRows(rows);
  setBias(bias);
  uint8_t S = translateChar(glyph);
  if (S == B11111110)
  {
    slash(X, Y, wide, high*2, 2);
    return;
  }

  uint8_t biasA = bias << 1;
  
  drawHSegment(X + rows + bias,  Y,                              (S & B10000000) ? 1 : 0);  //seg A
  drawHSegment(X + bias + rows,  Y + (high << 1) + biasA + bias, (S & B00010000) ? 1 : 0);  //seg D
  drawHSegment(X + rows + bias,  Y + high + bias + (bias >> 1),  (S & B00000010) ? 1 : 0);  //seg G (Bias >> 1 + bias is a fast mult by 1.5)

  drawVSegment(X + wide + biasA, Y + rows +        bias,         (S & B01000000) ? 1 : 0);  //seg B
  drawVSegment(X + wide + biasA, Y + high + rows + biasA,        (S & B00100000) ? 1 : 0);  //seg C
  drawVSegment(X,                Y + high + rows + biasA,        (S & B00001000) ? 1 : 0);  //seg E
  drawVSegment(X,                Y + rows +        bias,         (S & B00000100) ? 1 : 0);  //seg F
}

void Sevensegments::drawPercent(coordinate_t X, coordinate_t Y, const uint8_t size, const uint8_t rows, const uint8_t bias)
{
  setXlen(size);
  setYlen(size);
  setRows(rows);
  setBias(bias);

  uint8_t biasA = m_bias << 1;

  drawHSegment(X + rows + bias,  Y,                              1);  //seg A
  drawHSegment(X + rows + bias,  Y + size + bias + (bias >> 1),  1);  //seg G (Bias >> 1 + bias is a fast mult by 1.5)
  drawVSegment(X + size + biasA, Y + rows +        bias,         1);  //seg B
  drawVSegment(X,                Y + rows +        bias,         1);  //seg F

  X += m_Xlength << 1;
  Y += m_Xlength << 1;

  drawHSegment(X + rows + bias,  Y,                              1);  //seg A
  drawHSegment(X + rows + bias,  Y + size + bias + (bias >> 1),  1);  //seg G (Bias >> 1 + bias is a fast mult by 1.5)
  drawVSegment(X + size + biasA, Y + rows +        bias,         1);  //seg B
  drawVSegment(X,                Y + rows +        bias,         1);  //seg F
  slash(X, Y - m_Xlength, m_Xlength, (m_Xlength << 1), 4);
}

int Sevensegments::segmentedString(coordinate_t X, coordinate_t Y, char * b, uint8_t size, uint8_t rows, uint8_t bias, uint8_t step)
{
  uint8_t i {0};
  while (*b)
  {
    segments.drawGlyph(X + (step * i++), Y, *b++, size, size, rows, bias);
  }
  return X + step * (i -1);
}

 void Sevensegments::slash(const coordinate_t X, const coordinate_t Y, const uint8_t wide, const uint8_t high, const uint8_t rows)
{
  // insert dope Saul Hudson gag here...  
  for (int i = 0; i < rows; ++i)
  {
    screen.drawLine(X + i, Y, X + i - wide, Y + high, m_lit);
  }
}

void Sevensegments::drawGlyph16(const coordinate_t X, const coordinate_t Y, const uint8_t glyph, uint8_t size, const uint8_t rows, uint8_t bias)
{
  uint16_t S = translateChar16(glyph);

  uint8_t shorts = size >> 1;

  int16_t X0 = X  + bias   + rows;
  int16_t X1 = X0 + shorts - rows;
  int16_t X2 = X1 + bias   + rows;
  int16_t X3 = X2 + shorts - rows;

  int16_t Y0 = Y  + bias   + rows + 1;
  int16_t Y2 = Y  + bias   + size + rows + 2;
  int16_t Y1 = Y2 - rows   - 1;
  int16_t Y3 = Y  + bias   * 2    + size * 2 + 2;

  setXlen(shorts);
  setYlen(size);
  setRows(rows);
  setBias(bias);
  
  drawHSegment(X0, Y,  (S & SEG_A1) ? 1 : 0);  //seg A1
  drawHSegment(X2, Y,  (S & SEG_A2) ? 1 : 0);  //seg A2

  drawHSegment(X0, Y1, (S & SEG_G1) ? 1 : 0);  //seg G1 
  drawHSegment(X2, Y1, (S & SEG_G2) ? 1 : 0);  //seg G2 

  drawVSegment(X,  Y0, (S & SEG_F) ? 1 : 0);  //seg F 
  drawVSegment(X,  Y2, (S & SEG_E) ? 1 : 0);  //seg E 

  drawHSegment(X0, Y3, (S & SEG_D1) ? 1 : 0);  //seg D1 
  drawHSegment(X2, Y3, (S & SEG_D2) ? 1 : 0);  //seg D2 

  drawVSegment(X1, Y0, (S & SEG_I) ? 1 : 0);  //seg I
  drawVSegment(X1, Y2, (S & SEG_L) ? 1 : 0);  //seg L

  drawVSegment(X3, Y0, (S & SEG_B) ? 1 : 0);  //seg B
  drawVSegment(X3, Y2, (S & SEG_C) ? 1 : 0);  //seg C

  drawLRSegment(X0 + rows + 1,
                Y  + (rows << 1) + 1 + bias, 
                X  + shorts, 
                Y  + size - (rows >> 1) - rows,
                (rows << 1), (S & SEG_H) ? 1 : 0);

  drawRLSegment(X1 + shorts,
                Y  + (rows << 1) + 1 + bias, 
                X2 + 1 + rows,
                Y  + size - (rows >> 1) - rows,
                (rows << 1), (S & SEG_J) ? 1 : 0);

  drawRLSegment(X  + shorts, 
                Y1 + (rows << 1) + 1 + bias, 
                X0 + rows + 1, 
                Y1 + size - (rows >> 1) - rows,
                (rows << 1), (S & SEG_K) ? 1 : 0);

  drawLRSegment(X2 + 1 + rows, 
                Y1 + (rows << 1) + 1 + bias, 
                X1 + shorts - 1, 
                Y1 + size - (rows >> 1) - rows - + 1 - bias,
                (rows << 1), (S & SEG_M) ? 1 : 0);
}

void Sevensegments::drawDP(const coordinate_t X, const coordinate_t Y, const uint8_t radius, const uint8_t onFlag)
{
  screen.fillCircle(X, Y, radius, (onFlag) ? m_lit : m_unlit);
}

void Sevensegments::fastShortLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t ink) 
{
  int8_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) 
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) 
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int8_t dx    = x1     - x0;
  int8_t dy    = abs(y1 - y0);
  int8_t error = dx >> 1;
  int8_t ystep = (y0 < y1) ? 1 : -1;

  for (; x0 <= x1; x0++) 
  {
    if (steep) 
    {
      screen.drawPixel(y0, x0, ink);
    } 
    else 
    {
      screen.drawPixel(x0, y0, ink);
    }

    error -= dy;
    if (error < 0) 
    {
      y0    += ystep;
      error += dx;
    }
  }
}

 void Sevensegments::drawHSegment(const coordinate_t X, const coordinate_t Y, const uint8_t onFlag)
{
  coordinate_t y = Y + m_rows;
  for (uint8_t i = m_rows; i != 0; --i)                       // counting DOWN to 0 is faster for time critical loops
  {
    screen.drawFastHLine(X + i, y + i, m_Xlength - (i << 1), (onFlag) ? m_lit : m_unlit);
    screen.drawFastHLine(X + i, y - i, m_Xlength - (i << 1), (onFlag) ? m_lit : m_unlit);
  }
    screen.drawFastHLine(X, y, m_Xlength, (onFlag) ? m_lit : m_unlit);
}

 void Sevensegments::drawVSegment(const coordinate_t X, const coordinate_t Y, const  uint8_t onFlag)
{
 coordinate_t x = X + m_rows; 
 for (uint8_t i = m_rows; i != 0; --i)
  {
    screen.drawFastVLine(x + i, Y + i, m_Ylength - (i << 1), (onFlag) ? m_lit : m_unlit);
    screen.drawFastVLine(x - i, Y + i, m_Ylength - (i << 1), (onFlag) ? m_lit : m_unlit);
  }
    screen.drawFastVLine(x, Y, m_Ylength, (onFlag) ? m_lit : m_unlit);
}

 void Sevensegments::drawRLSegment(coordinate_t X, coordinate_t Y, coordinate_t X1, coordinate_t Y1, const uint8_t rows, uint8_t onFlag)
{
  Y1 -= rows;
  for (uint8_t i = (rows << 1); i > 0; --i)
  {
    fastShortLine(X, Y + i, X1, Y1 + i, (onFlag) ? m_lit : m_unlit);
  }
}

 void Sevensegments::drawLRSegment(coordinate_t X, coordinate_t Y, coordinate_t X1, coordinate_t Y1, const uint8_t rows, uint8_t onFlag)
{
  Y1 -= rows;  
  for (uint8_t i = (rows << 1); i > 0 ; --i)
  {
    fastShortLine(X, Y + i, X1, Y1 + i, (onFlag) ? m_lit : m_unlit);
  }
}

