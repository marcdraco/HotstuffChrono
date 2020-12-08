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

#ifdef USE_GRAPH

void Graph::drawGraph()
{
  m_temperature[m_circular] = static_cast<int16_t>(temperature.getCMA() * READ_SCALAR);  // hide the floating point in a large integer
  m_humidity[m_circular]    = static_cast<int16_t>(humidity.getCMA()    * READ_SCALAR);

  // Bump the current plot position each time the graph is updated
  m_circular++;

  if (m_circular == GRAPH_WIDTH)
  {
    for (uint8_t i {1}; i < GRAPH_WIDTH; ++i)
    {
      m_temperature[i - 1] = m_temperature[i];
      m_humidity[i - 1]    = m_humidity[i];
    }
    --m_circular;
  }

  const colours_t tInk = temperature.getTrace();
  const colours_t hInk = humidity.getTrace();

  drawReticles(6, 6);
  drawGraphScaleMarks();

  minMaxReads_t temp  {temperature.getMinRead(), temperature.getMaxRead()};
  minMaxReads_t humid {humidity.getMinRead(),    humidity.getMaxRead()};

  reading_t perPixTemperature = static_cast<float>(GRAPH_HEIGHT) / ((temp.max - temp.min > 1)   ? (temp.max - temp.min)   :  1); 
  reading_t perPixHumidity    = static_cast<float>(GRAPH_HEIGHT) / ((humid.max - humid.min > 1) ? (humid.max - humid.min) :  1);
 
  int16_t X = 1 + GRAPH_LEFT;

  for (uint8_t i {1}; i < m_circular; ++i)
  {
    float temperature = (static_cast<float>(m_temperature[i]) - temp.min *  READ_SCALAR) / READ_SCALAR * perPixTemperature;   // convert from integer back to floats
    float humidity    = (static_cast<float>(m_humidity[i])    - humid.min * READ_SCALAR) / READ_SCALAR * perPixHumidity;

    uint8_t Y = BASE - humidity;
    screen.drawPixel(X + i, Y, hInk);
    Y = BASE - temperature;
    screen.drawPixel(X + i, Y, tInk);
  }
}

void Graph::drawIBar(const ucoordinate_t x, const reading_t reading, int16_t minimum, int16_t maximum, const int8_t scale, const colours_t ink, const bool pointer)
{
  const ucoordinate_t vX = (pointer) ? x - 1 : x + 1; 
  const ucoordinate_t y  = BASE - (static_cast<int8_t>(round(reading)) * scale);
  const uint16_t max     = BASE - (static_cast<int8_t>(round(abs(maximum))) * scale);  
  const uint16_t min     = BASE - (static_cast<int8_t>(round(abs(minimum))) * scale);
  const uint16_t outset  = 5; 
  screen.drawFastVLine(vX, max, min - max, ink);

  if (ink == temperature.getTrace())
  {
    screen.drawFastHLine(vX - outset, y,   outset, ink);
    screen.drawFastHLine(vX - outset, max, outset, ink);
    screen.drawFastHLine(vX - outset, min, outset, ink);      
  }
  else
  {
    screen.drawFastHLine(vX, y,   outset, ink);
    screen.drawFastHLine(vX, max, outset, ink);
    screen.drawFastHLine(vX, min, outset, ink);      
  }
}

void Graph::drawTarget(const ucoordinate_t x, const reading_t reading, const uint16_t scale, const uint8_t size, const colours_t ink)
{
  const uint16_t y = BASE - (static_cast<int8_t>(round(reading)) * scale);
  screen.drawFastHLine(x - size, y, size * 2, ink);
  screen.drawFastVLine(x, y - size, size * 2, ink);
  screen.drawCircle(x, y, 4, ink);
}

void Graph::drawMinMax(const ucoordinate_t x, const reading_t reading, const int16_t minimum, const int16_t maximum, const int8_t scale, const colours_t ink)
{
  const uint16_t outset  = 3; 
  const ucoordinate_t vX = (ink == temperature.getTrace()) ? x - outset : x + 1;
  const ucoordinate_t Y  = BASE - (static_cast<int8_t>(round(reading)) * scale);
  const uint16_t max     = BASE - (static_cast<int8_t>(round(abs(maximum))) * scale);  
  const uint16_t min     = BASE - (static_cast<int8_t>(round(abs(minimum))) * scale);

  screen.drawFastVLine(vX, max, min - max, ink);

  screen.drawLine(vX - outset, max - outset, vX, max, ink);
  screen.drawLine(vX + outset, max - outset, vX, max, ink);
  screen.drawLine(vX - outset, min + outset, vX, min, ink);
  screen.drawLine(vX + outset, min + outset, vX, min, ink);

  if (ink == temperature.getTrace())
  {
    screen.drawFastHLine(vX - outset, Y, outset, ink);
  }
  else
  {
    screen.drawFastHLine(vX, Y, outset, ink);
  }
}

void Graph::drawDiamond(const ucoordinate_t x, const reading_t reading, const uint16_t scale, const uint8_t size, const colours_t ink)
{
  const uint16_t y = BASE - (static_cast<int8_t>(round(reading)) * scale);

  ucoordinates_t d[4] { x - size,  y, 
                        x,  y - size, 
                        x + size,  y, 
                        x,  y + size
                      };

  for (int i{0}; i < 3; ++i)
  {
    screen.drawLine(d[i].X, d[i].Y, d[i + 1].X, d[i + 1].Y, ink);
  }
  screen.drawLine(d[3].X, d[3].Y, d[0].X, d[0].Y, ink);
}

void Graph::drawReticles(const uint8_t xDivs, const uint8_t yDivs)
{
  // This is a little harsh and causes a small flicker during the redraw
  // We tried some more "surgical" methods but they proved to be too complex
  // to justify the extra Flash space (even a few % is too much when you're short) 
  screen.fillRect(GRAPH_LEFT + 1, GRAPH_Y, GRAPH_WIDTH,  BASE - GRAPH_Y, defaultPaper);

  for (uint8_t i {0}; i < 7; ++i)    // vertical divisions
  {
    screen.drawFastVLine(GRAPH_LEFT + m_xStep * i, GRAPH_Y, GRAPH_HEIGHT, reticleColour);
  }

  for (uint8_t i {0}; i < 6; ++i)   // horizontal divisions
  {
    screen.drawFastHLine(GRAPH_LEFT, GRAPH_Y + i * m_yStep, GRAPH_WIDTH, reticleColour);
  }

  for (uint8_t i {0}; i < 6; ++i)   // humidity ticks
  {
    screen.drawFastHLine(GRAPH_LEFT + GRAPH_WIDTH, GRAPH_Y + i * m_yStep, 5, defaultInk);
  }

  for (int8_t i {0}; i < 6; ++i)   // temp ticks
  {
    screen.drawFastHLine(GRAPH_LEFT - 5,           GRAPH_Y + (i * m_yStep), 5, defaultInk);
  }

  screen.drawFastHLine(GRAPH_LEFT, GRAPH_Y + GRAPH_HEIGHT,   GRAPH_WIDTH,       defaultInk);
  screen.drawFastVLine(GRAPH_LEFT,               GRAPH_Y -5, GRAPH_HEIGHT + 10, defaultInk);
  screen.drawFastVLine(GRAPH_LEFT + GRAPH_WIDTH, GRAPH_Y -5, GRAPH_HEIGHT + 10, defaultInk);
}

void Graph::drawGraphScaleMarks(void)
{
    // Pop the screen sideways on for some font printing   
    fonts.setRotation(3);

    // Puts the "Temperature in" message up.
    screen.setCursor(AXIS_Y_POSITION, fonts.getYstep());
    messages.execute(Messages::temperatureScale);
    (CHECKBIT(globals.gp, USEMETRIC)) ? messages.execute(Messages::c) : messages.execute(Messages::f);

    // Puts the "Relative Humidity %" message up.
    screen.setCursor(AXIS_Y_POSITION, TFT_WIDTH - fonts.getYstep());
    messages.execute(Messages::humidityScale);

    // Return rotation to normal for the rest of the printing
    fonts.setRotation(0);

    // Overwrite the Dew/Frost Point messages.
    screen.fillRect(5, 95, 60,10, defaultPaper);  

    int8_t dp  = static_cast<int>((environment.magnusDewpoint()));

    screen.setCursor(5, 103);
    if (dp > 0)
    {
      messages.execute(Messages::dew);
    }
    else
    {
      messages.execute(Messages::frost);
    }

    // Calculate the abd display the APPROXIMATE chart width in hours
    constexpr int   SECONDS_PER_DAY = 60 * 60;
    constexpr float r1 = (CHART_UPDATE_FREQUENCY * READ_UPDATE_TIME);
    constexpr float r2 = r1 * GRAPH_WIDTH;
    constexpr float hours = r2 / SECONDS_PER_DAY;

    screen.setCursor(XSCALE_X, XSCALE_Y);
    messages.execute(Messages::xScale0);

    if (hours >= 1.0)
    { 
      char b[10];
      formatQuickFloat(hours, 1, b);
      fonts.print(b);
      messages.execute(Messages::xScale2);
    }
    else
    {
      char b[10];
      float time = hours * 60;
      formatQuickFloat(time, 1, b);
      fonts.print(b);
      messages.execute(Messages::xScale1);
    }

    
    //Much of this is optimised out, but is left for clearer code.
    
    minmax_t T;
    minmax_t H;
    readings_t range;
    readings_t step;

    T.max      = temperature.getMaxRead();
    T.min      = temperature.getMinRead();

    if (!CHECKBIT(globals.gp, USEMETRIC))
    {
      T.max    = toFahrenheit(T.max);
      T.min    = toFahrenheit(T.min);
    }

    H.max      = humidity.getMaxRead();
    H.min      = humidity.getMinRead();

    range.T    = (T.max - T.min  > 1) ? T.max - T.min  : 1; 
    range.H    = (H.max - H.min  > 1) ? H.max - H.min  : 1; 

    step.T     = range.T / 5;
    step.H     = range.H / 5;

    // Develop the temperature and humidity annotations
    for (uint8_t i {0}; i < 6; i++) 
    {
      int yShift = (fonts.getYstep() / 2) - 2;
      char b[10];
      char buff[10];

      reading_t value  = (i * step.T) + T.min;

      int X = ((TFT_WIDTH - GRAPH_WIDTH) >> 1) - (fonts.getXstep() * 3);
      screen.setCursor(X, BASE - (i * 20) + yShift);
      temperature.bufferReading(value, b, METRIC);
      sprintf(buff, "%s", b);
      screen.fillRect(X-10, BASE - (i * 20) + yShift - 10, 35, 12, defaultPaper);   // remove the old values (messy but...)
      fonts.print(buff);
      
      value = ((i * step.H) + H.min);
      X = ((TFT_WIDTH - GRAPH_WIDTH) >> 1) + GRAPH_WIDTH + fonts.getXstep();
      screen.setCursor(X, BASE - (i * 20) + yShift);
      humidity.bufferReading(value, b, METRIC);
      sprintf(buff, "%s", b);
      screen.fillRect(X, BASE - (i * 20) + yShift - 10, 25, 12, defaultPaper);      // limited Flash space means we have to compromise
      fonts.print(buff);
    }
}

#endif