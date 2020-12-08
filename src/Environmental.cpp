/*
  +++++----------------------------------------------------------------------+++++

  Copyright 2020, Marc Draco with Garrie "Garrington" Steele & Daniel Melvin
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
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
  DAMAGE.

*/

#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <Wire.h>

#include <Adafruit_GFX.h>     // Core graphics library by AdaFruit
#include <MCUFRIEND_kbv.h>    // David Prentice's Hardware-specific library - your shield might vary
#include "UniversalDHT.h"   // @winlinvip's DHT11/22 library functions modified by Tim Harper

#include "Alarm.h"
#include "constants.h"
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

extern globalVariables globals;

/**
 * @brief Checks damp and dry air
 * 
 */

void Environmental::checkHumidityConditions(void)
{
  if (humidity.getCMA() > DAMP_AIR_WATERSHED)
  {
    SETBIT(globals.ISR, DAMP);
  }
      
  if (humidity.getCMA() <= DAMP_AIR_WATERSHED)
  {
    if (CHECKBIT(globals.ISR, DAMP))
    {
      SETBIT(globals.ISR, CLEARDAMP);
      CLEARBIT(globals.ISR, DAMP);
    }
  }

  if (humidity.getCMA() < DRY_AIR_WATERSHED)
  {
      SETBIT(globals.ISR, DRY);
  }

  if (humidity.getCMA() >= DRY_AIR_WATERSHED)
  {
    if (CHECKBIT(globals.gp, DRY))
    {
      SETBIT(globals.ISR, CLEARDRY);
      CLEARBIT(globals.ISR, DRY);
    }
  }
}

/**
 * @brief primarily for incubation systems 
 * @remark under development
 */

void Environmental::checkTemperatureConditions(void)
{
  // This is for incubation systems. Under 35C is chilly for eggs
  // It *could be used for normal environments, but that seems overkill.

  reading_t temp = temperature.getRawReading();
  if (temp < MIN_COMFORT_TEMP)
  {
    digitalWrite(HEATER_RELAY, HIGH);
  }
  
  if (temp > MAX_COMFORT_TEMP)
  {
    digitalWrite(HEATER_RELAY, LOW);
  }
 
  #ifdef INCUBATOR
  if (temperature.getRawReading() <= 35)   
  {
    SETBIT(globals.ISR, FROST);          // start das-blinky-flashun light
  }

  if (temperature.getCMA() > 36)
  {
    if (SETBIT(globals.ISR, FROST))
    { 
      CLEARBIT(globals.ISR, FROST);     //Stop the frost warn flashing
      SETBIT(globals.ISR, CLEARFROST);  // for clean up
    }
  }
  #endif
}

void Environmental::checkHeatIndex(readings_t readings)
{
  reading_int_t effectiveTemperature = static_cast<reading_int_t>(heatIndex(readings));

  // heat routine only works reliably(ish) for temps >26 celcius and RH >= 40%
  if ((effectiveTemperature < 26) || 
                 readings.T < 26 || 
                 readings.H < 40)
  {
    if (CHECKBIT(globals.gp, WARNDANGER))
    {
      CLEARBIT(globals.gp, WARNDANGER);
      temperature.setEffective(heatIndex(readings));    
    }
    return;
  }

  SETBIT(globals.gp, WARNDANGER);
  temperature.setEffective(heatIndex(readings));
}

float Environmental::heatIndex(const readings_t readings)
{
  constexpr float c1 {-8.78469475556};
  constexpr float c2 {1.61139411};
  constexpr float c3 {2.33854883889};
  constexpr float c4 {-0.14611605};
  constexpr float c5 {-0.012308094};
  constexpr float c6 {-0.0164248277778};
  constexpr float c7 {0.002211732};
  constexpr float c8 {0.00072546};
  constexpr float c9 {-0.000003582};
  return  (c1 + 
          (c2 *  readings.T) + 
          (c3 *  readings.H) + 
          (c4 *  readings.T  *  readings.H)  + 
          (c5 * (readings.T  *  readings.T)) + 
          (c6 * (readings.H  *  readings.H)) + 
          (c7 * (readings.T  *  readings.T)  *  readings.H)  + 
          (c8 *  readings.T  * (readings.H   *  readings.H)) + 
          (c9 * (readings.T  *  readings.T)  * (readings.H   * readings.H)));
}

reading_t Environmental::magnusDewpoint()
{
  /*
  Per WIKIPEDIA:
  Discomfort also exists when the dew point is very low (below around −5 °C or 23 °F).
  The drier air can cause skin to crack and become irritated more easily. 
  It will also dry out the airways. The US Occupational Safety and Health Administration 
  recommends indoor air be maintained at 20–24.5 °C (68–76 °F) with a 20–60% relative #
  humidity, equivalent to a dew point of 4.0 to 15.5 °C (39 to 60 °F). Lower dew points, 
  less than 10 °C (50 °F), correlate with lower ambient temperatures and cause the body 
  to require less cooling. A lower dew point can go along with a high temperature only 
  at extremely low relative humidity, allowing for relatively effective cooling. 
  */

  // Magnus dew point constants
  constexpr double a = 17.62;
  constexpr double b = 243.12;
  readings_t readings {humidity.getRawReading(), temperature.getRawReading()};
  reading_t c = (a * readings.T) / (b + readings.T);
  reading_t r = log(readings.H / 100);

  return static_cast<reading_t>(b * (r + c) / (a - (r + c)));
}
