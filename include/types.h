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

#ifndef __DRACO_HOTSTUFF_TYPES_H
#define __DRACO_HOTSTUFF_TYPES_H

#include <Arduino.h>
#include <inttypes.h>
#include "constants.h"

using semaphore_t   = uint16_t;
using colours_t     = uint16_t;
using angle_t       = int16_t;
using glyph_t       = uint8_t;
using ucoordinate_t = uint16_t;
using coordinate_t  = int16_t;
using yCoordinate_t = uint8_t;  // 
using xCoordinate_t = uint8_t;  // When the display is VERY small, 8 bit is much faster.

using reading_t     = float;    // Actually, this is the same as a double
using reading_int_t = int16_t;
using isrtiming_t   = uint8_t;
using byte_t        = uint8_t;

/*
 * Nasty global variable...
 * These flags are set/checked in an interupt service so they need to avoid
 * diving in another subroutine (albetit one that could perceivably be inlined
 * by the compiler.) These macros avoids leaving to chance.
*/  
struct globalVariables
{
  uint16_t ISR = 0;  // interrupts
  uint16_t gp = 0;   // general purpose
};

using minMaxReads_t = struct  
{
  reading_t min;
  reading_t max;
};

using coordinates_t  = struct coordinates
{
  coordinate_t X; 
  coordinate_t Y;
};

using scale_t = struct scales
{
  uint8_t temperature;
  uint8_t humidity;
};


using ucoordinates_t  = struct ucoordinates
{
  ucoordinate_t X; 
  ucoordinate_t Y;
};

using dimensions_t = struct dimensions
{
  uint16_t W;
  uint16_t H;
};

using segment_t = struct segments
{
  coordinates_t coords[12];
};

using readings_t = struct readings
{
  reading_t H;
  reading_t T;
};

using limits_t = struct limits
{
  reading_t lower;
  reading_t upper;
};

using minmax_t = struct minmax
{
  reading_t min;
  reading_t max;
};

using triangle_t = struct triangles
{
  coordinates_t cords[3];
};

using quadrilateral_t = struct quadrilaterals
{
  coordinates_t cords[4];
};

using glyphdata_t = struct glyphdata    
{
  uint16_t      offset;
  dimensions_t  dimensions;
  byte_t        bits;
  byte_t        bit;
  int8_t        xo;
  int8_t        yo;
  colours_t     colour;
  uint8_t*      bitmap;
  int16_t       x;
  int16_t       y;
  glyph_t       glyph;
  coordinate_t  xAdvance;
};

using hieroglyphs_t = struct hieroglyphs
{
  int8_t total;
  glyph_t glyphics[];
};

struct 
{
  volatile isrtiming_t timeInSeconds {0};
  volatile isrtiming_t timeInMinutes {0};
  volatile isrtiming_t timeInHours   {0};
  volatile isrtiming_t timeInDays    {0};
  volatile isrtiming_t timeToGraph   {0};
  volatile isrtiming_t timeToRead    {0};
} isrTimings;

enum
{
  FONT1, // 6 x 8 
  FONT2, // 12 x 16
  FONT3, // 18 x 24
  FONT4, // 24 x 32
  FONT5  // 30 x 40
};

// The failure record array gets sensor age after a "catastrophic" failure
struct failures
{
//  bool sensorFailure = {false};
  enum 
  {
    Y, W, D, H, M
  };
  uint8_t failTime[5];
};

/*
* Font structures for newer Adafruit_GFX (1.1 and later).
* Example fonts are included in 'Fonts' directory.
* Font data stored PER GLYPH (character) and is based on Adafruit libarary
*/

typedef struct
{
    uint8_t  ascii;         ///< ASCII code for this glyph
    uint16_t bitmapOffset;  ///< Pointer into GFXfont->bitmap
    uint8_t  width;         ///< Bitmap dimensions in pixels
    uint8_t  height;        ///< Bitmap dimensions in pixels
    uint8_t  xAdvance;      ///< Distance to advance cursor (x axis)
    int8_t   xOffset;       ///< X dist from cursor pos to UL corner
    int8_t   yOffset;       ///< Y dist from cursor pos to UL corner
} gfxglyph_t;

/*
* Extended font data (based on Adafruit libarary)
*/

typedef struct FixedFontsNS
{
    uint8_t* bitmap;       ///< Glyph bitmaps, concatenated
    gfxglyph_t* glyph;     ///< Glyph array
    uint8_t glyphCount;    ///< Actual number of defined glyphs
    int8_t newline;       ///< Newline distance (y axis)
    int8_t xMin;           ///< Leftmost  
    int8_t yMin;           ///< Bottom Y
    int8_t xMax;       ///< Rightmost
    int8_t yMax;           ///< Upper Y
} gfxfont_t ;

#endif