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

#ifndef __DRACO_HOTSTUFF_ALARM_H
#define __DRACO_HOTSTUFF_ALARM_H

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
#include "Sevensegments.h"
#include "types.h"

extern MCUFRIEND_kbv screen;

class Alarm
{

  private:

  const int LEFTMARGIN    {20};
  const int LEFTAXISLABEL {5};

  public:

  Alarm()
  {}

  void annunciators();

  /**
   * @brief Brings everthing to a halt if the DHT sensor breaks during use
   * @param response response Returned 16 bit from the DHT libary
   * @bug not really tested properly!
   */
  void sensorFailed(UniversalDHT::Response response)
  {
    // He's dead Jim! He's DEAD!

  #ifdef SENSOR_MISSING_OR_BUSTED
    return;
  #endif
    screen.fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, defaultPaper);
    
    screen.setCursor(20, 120);
    screen.print(F("-Sensor Fail-"));
    screen.setCursor(20, 135);

    switch (response.error)
    {
      case SimpleDHTErrStartLow:

        screen.print(F("Start signal arrived early ("));
        screen.print(response.time);
        screen.print(F(" microseconds)"));
        break;

      case SimpleDHTErrStartHigh:
        screen.print(F("Start high came too soon ("));
        screen.print(response.time);
        screen.print(F(" microseconds)"));
        break;

      case SimpleDHTErrDataLow:
        screen.print(F("Start datalow came too soon ("));
        screen.print(response.time);
        screen.print(F(" microseconds)"));
        break;

      case SimpleDHTErrDataEOF:
        screen.print(F("timeout on data EOF signal ("));
        screen.print(response.time);
        screen.print(F(" microseconds)"));
        break;

      case SimpleDHTErrDataHigh:
        screen.print(F("Data high failed ("));
        screen.print(response.time);
        screen.print(F(" microseconds)"));
        break;

      case SimpleDHTErrDataChecksum:
        screen.print(F("failure validating checksum"));
        break;

      case SimpleDHTSuccess: ;  // shuts up the compiler warning
    }
    STOP // loop until re-set.
  }

};

extern Alarm alarm;

#endif