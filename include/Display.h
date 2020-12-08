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
#ifndef __DRACO_HOTSTUFF_DISPLAY_H
#define __DRACO_HOTSTUFF_DISPLAY_H


#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <Wire.h>

#include <Adafruit_GFX.h>     // Core graphics library by AdaFruit
#include <MCUFRIEND_kbv.h>    // David Prentice's Hardware-specific library - your shield might vary
#include "UniversalDHT.h"   // @winlinvip's DHT11/22 library functions modified by Tim Harper

#include "Fonts.h"
#include "Flags.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "Reading.h"
#include "Sevensegments.h"
#include "types.h"

extern Reading temperature;
extern Reading humidity;

class Display
{
  colours_t m_ink {defaultInk};
  colours_t m_paper {defaultPaper};
  colours_t m_flash {defaultInk};
  uint8_t m_rotation {};

  public:
  const uint16_t width  {TFT_WIDTH};
  const uint16_t height {TFT_HEIGHT};

  /**
   * @brief Set the Colours object
   * 
   * @param ink 
   * @param paper 
   */
  
  void setColours(const colours_t ink, const colours_t paper)
  {
    m_ink   = ink;
    m_paper = paper;
  }

  /**
   * @brief Get the Ink object
   * 
   * @return colours_t 
   */

  colours_t getInk()
  {
    return m_ink;
  }

  /**
   * @brief Set the Ink object
   * 
   * @param C 
   */
  void setInk(const colours_t C)
  {
    m_ink = C;
  }

  /**
   * @brief Set the Flash Ink object
   * 
   * @param C 
   */
  void setFlashInk(const colours_t C)
  {
    m_flash = C;
  }

  /**
   * @brief Get the Paper object
   * 
   * @return colours_t 
   */
  colours_t getPaper()
  {
    return m_paper;
  }

  /**
   * @brief Set the Paper object
   * 
   * @param C 
   */
  void setPaper(const colours_t C)
  {
    m_paper = C;
  }

  enum  
  {
    rotatePortraitNorth, 
    rotateLandscapeNorth,
    rotatePortraitSouth, 
    rotateLandscapeSouth
  };

  /**
   * @brief 
   * 
   * @param X 
   * @param Y 
   * @param H 
   * @param W 
   * @param buffer 
   */

  /**
   * @brief 
   * 
   * @param X 
   * @param Y 
   * @param H 
   * @param W 
   * @param buffer 
   */
  void displaySmallBitmap(ucoordinate_t X, ucoordinate_t Y, uint8_t H, uint8_t W, uint8_t* buffer)
  {
    // Tweaked by MD from the orginal by Limor "Lady Ada" Fried. 
    // 8-bits are used in preference to 16 where possible because these are faster on AtMegas  
    screen.startWrite();

    int16_t byteWidth = (W + 7) >> 3;
    uint8_t byte {0};

    for (int8_t j {0}; j < H; j++, Y++) 
    {
      for (int8_t i {0}; i < W ; i++) 
      {
        if (i & 7)
        {
          byte <<= 1;
        }
        else
        {
          byte = pgm_read_byte(&buffer[j * byteWidth + (i >> 3)]);
        }
        if (! (byte & 0x80))
        {
          screen.writePixel(X + i, Y, m_flash);
        }
      }
    }
    screen.endWrite();
  }


};

extern Display display;
#endif
