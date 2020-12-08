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


#include <Arduino.h>
#include "types.h"
#include "Flags.h"  
#include "Fonts.h"
#include "fonts/ubuntu-6-full.h"
#include "Graph.h"
#include "hotstuff.h"
#include "Display.h"
#include "Messages.h"
#include <MCUFRIEND_kbv.h>
#include "Sevensegments.h"

extern MCUFRIEND_kbv screen;
extern Display display;
extern Flags flags;
extern Fonts fonts;
extern Messages messages;
extern globalVariables globals;

/**
 * @brief Formats a float into a text buffer
 * 
 * @param value the value to convert
 * @param digits the number of digits after the FP.
 * @param b A buffer for the result
 */
void formatQuickFloat(float value, uint8_t digits, char* b)
{
    value *=  pow(10, digits);  // shift the decimal point by n digits
    char buffer[15];  // should be large enough for most cases
    sprintf(buffer, "%d", static_cast<int> (value));

    int intsize = strlen(buffer); // find how many digits are present.
    int pos {0};

    for (; pos < intsize - digits; pos++)
    {
        *b++ = buffer[pos];
    }
    if (digits) 
    {
      *b++ = '.';
    }

    for (int i =0; i < digits; i++)
    {
        *b++ = buffer[pos + i];
    }
    *b = 0; // just make damn sure this is null terminated

}

void Graph::draw(const quadrilateral_t* quad, const colours_t ink, const colours_t outline)
{
  screen.fillTriangle(quad->cords[0].X, quad->cords[0].Y,
                      quad->cords[1].X, quad->cords[1].Y,
                      quad->cords[2].X, quad->cords[2].Y,
                      ink);

  screen.fillTriangle(quad->cords[0].X, quad->cords[0].Y,
                      quad->cords[3].X, quad->cords[3].Y,
                      quad->cords[2].X, quad->cords[2].Y,
                      ink);

  screen.drawLine(quad->cords[0].X, quad->cords[0].Y, quad->cords[1].X, quad->cords[1].Y, outline);
  screen.drawLine(quad->cords[1].X, quad->cords[1].Y, quad->cords[2].X, quad->cords[2].Y, outline);
  screen.drawLine(quad->cords[2].X, quad->cords[2].Y, quad->cords[3].X, quad->cords[3].Y, outline);
  screen.drawLine(quad->cords[3].X, quad->cords[3].Y, quad->cords[0].X, quad->cords[0].Y, outline);
}

void Graph::rotate(quadrilateral_t* quad, const int16_t rotation)
{
  double sinTheta = sin(DEG_TO_RAD * rotation);
  double cosTheta = cos(DEG_TO_RAD * rotation);
  for (auto i{0}; i < 4; ++i)
  {
    int16_t tX;
    tX = ( (double) quad->cords[i].X * cosTheta - (double)quad->cords[i].Y * sinTheta);
    quad->cords[i].Y  = ( (double) quad->cords[i].X * sinTheta + (double)quad->cords[i].Y * cosTheta);
    quad->cords[i].X = tX;
  }
}

void Graph::translate(triangle_t* triangle, const coordinates_t cords)
{
  for (auto i {0}; i < 4; ++i)
  {
    triangle->cords[i].X += cords.X;
    triangle->cords[i].Y += cords.Y;
  }
}

void Graph::translate(quadrilateral_t* polygon, const coordinates_t cords)//  uint8_t width = (textWidth(msg) / 4) * 4; // Polish off any slight font width weirdness.
{
  for (auto i {0}; i < 4; ++i)
  {
    polygon->cords[i].X += cords.X;
    polygon->cords[i].Y += cords.Y;
  }
}

void Graph::drawPointers()
{
  {
    int16_t t = 33;
    quadrilateral_t pointer;
    pointer.cords[0] = {0,0};
    pointer.cords[1] = {10,-10};
    pointer.cords[2] = {0,64};
    pointer.cords[3] = {-10,-10};

    Graph::rotate(&pointer, t);
    Graph::translate(&pointer, {160,120});
    Graph::draw(&pointer, defaultPaper, defaultPaper);
  }

  {
    int16_t t = 33;
    quadrilateral_t pointer;
    pointer.cords[0] = {0,0};
    pointer.cords[1] = {10,-10};
    pointer.cords[2] = {0,64};
    pointer.cords[3] = {-10,-10};

    Graph::rotate(&pointer, t);
    Graph::translate(&pointer, {160,120});
    Graph::draw(&pointer, BLUE, WHITE);
  }

  {
    int16_t t = -43;
    quadrilateral_t pointer;
    pointer.cords[0] = {0,0};
    pointer.cords[1] = {10,-10};
    pointer.cords[2] = {0,64};
    pointer.cords[3] = {-10,-10};

    Graph::rotate(&pointer, t);
    Graph::translate(&pointer, {160,120});
    Graph::draw(&pointer, GREEN, WHITE);
  }
}

void Graph::drawRadials()
{
 for (int t = 0; t < 190; t+=10)
  {
    quadrilateral_t tick;
    tick.cords[0] = {-80, -2};
    tick.cords[1] = {-60, -2};
    tick.cords[2] = {-60, 2};
    tick.cords[3] = {-80, 2};

    Graph::rotate(&tick, t);
    Graph::translate(&tick, {160,120});
    Graph::draw(&tick, YELLOW, YELLOW);
  }

  for (int t = 190; t<360; t+=10)
  {
    quadrilateral_t tick;
    tick.cords[0] = {-80, -2};
    tick.cords[1] = {-60, -2};
    tick.cords[2] = {-60, 2};
    tick.cords[3] = {-80, 2};

    Graph::rotate(&tick, t);
    Graph::translate(&tick, {160,120});
    Graph::draw(&tick, RED, RED);
  }
}
