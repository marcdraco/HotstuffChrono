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
extern globalVariables globals;

void Messages::execute(const uint8_t M)
{
  String text = translations[M];
  fonts.print(text);
}

uint16_t Messages::textWidth(char* message)
{
  int16_t x{0};
  uint8_t i{0};

  while (message[i++])
  {
    glyphdata_t data;
    glyph_t code = fonts.findGlyphCode(message[i]);
    fonts.prepImgGlyph(code, &data);
    x += data.xAdvance;
  }
  return x;
}

void Messages::clear(const uint8_t message)
{
  display.setInk(defaultPaper);
  display.setPaper(defaultPaper);
  execute(message);
}

void Messages::showUptime(void)
{  
  /*
    This block produces the uptime data at the base
    of the screen as a quick check that the machine
    hasn't suffered a power loss or reset.
  */
  char* msg = static_cast<char *> (malloc(40));

  sprintf(msg, "Uptime: %2d Days, %02d:%02d:%02d", 
          isrTimings.timeInDays,
          isrTimings.timeInHours,
          isrTimings.timeInMinutes,
          isrTimings.timeInSeconds);
 
  display.setColours(defaultPaper, GREY);  
  fonts.setFont(&HOTSMALL);
  uint8_t width = (textWidth(msg) / 4) * 4; // Polish off any slight font width weirdness.
  uint8_t margin = (TFT_WIDTH - width) / 2;
  fonts.setBufferDimensions(TFT_WIDTH, 18);
  fonts.print(margin, TFT_HEIGHT - 16, msg);
  display.setColours(defaultInk, defaultPaper); 
  free(msg);
}

void Messages::debugger(const int X, const int Y, char* msg)
{
  fonts.setFont(&HOTSMALL);
  fonts.setBufferDimensions(TFT_WIDTH, 18);
  display.setInk(RED);
  fonts.print(X, Y, msg);
}