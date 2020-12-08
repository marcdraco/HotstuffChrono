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

#ifndef __DRACO_HOTSTUFF_TRIG_H
#define __DRACO_HOTSTUFF_TRIG_H

#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <Wire.h>

#include <Adafruit_GFX.h>     // Core graphics library by AdaFruit
#include <MCUFRIEND_kbv.h>    // David Prentice's Hardware-specific library - your shield might vary
#include "UniversalDHT.h"     // @winlinvip's DHT11/22 library functions modified by Tim Harper

#include "Fonts.h"
#include "Flags.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "Sevensegments.h"
#include "types.h"

#ifdef CLOCKWISE
class Trig
{
  private:

  /*

  int r = random(360);

  Serial.println(Trig::getSin(r));
  Serial.println(sin((radians(r)))* 32767);

  Serial.println(Trig::getSin(r));
  Serial.println(sin((radians(r)))* 32767);

  Serial.println(Trig::getSin(r));
  Serial.println(sin((radians(r)))* 32767);

  Serial.println(Trig::getSin(r));
  Serial.println(sin((radians(r)))* 32767);

  Serial.println(Trig::getCos(r));
  Serial.println(cos((radians(r)))* 32767);

  Serial.println(Trig::getCos(r));
  Serial.println(cos((radians(r)))* 32767);

  Serial.println(Trig::getCos(r));
  Serial.println(cos((radians(r)))* 32767);

  Serial.println(Trig::getCos(r));
  Serial.println(cos((radians(r)))* 32767);

  */

  public: 
  int16_t static getSin(int16_t theta)
  {
    if (theta < 90)
    {
      return pgm_read_word(&sinTable[theta]);      
    }

    if (theta >=90 && theta < 180)
    {
      return pgm_read_word(&sinTable[90 - (theta-90)]);      
    }

    if (theta >=180 && theta < 270)
    {
      return -(pgm_read_word(&sinTable[(theta - 180)]));            
    }

    if (theta >=270 && theta < 360)
    {
      return -(pgm_read_word(&sinTable[90 - (theta - 270)]));
    }      
  }

  int16_t static getCos(int16_t theta)
  {
    if (theta < 90)
    {
      return pgm_read_word(&sinTable[90 - theta]);      
    }

    if (theta >=90 && theta < 180)
    {
      return -(pgm_read_word(&sinTable[(theta-90)]));      
    }

    if (theta >=180 && theta < 270)
    {
      return -(pgm_read_word(&sinTable[90 - (theta - 180)]));            
    }
    
    if (theta >=270 && theta < 360)
    {
      return (pgm_read_word(&sinTable[(theta - 270)]));
    }      
  }
};

#endif // #CLOCKWISE

#endif // Guard