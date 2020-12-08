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
#include "hotstuff.h"
#include "Display.h"
#include "Messages.h"
#include <MCUFRIEND_kbv.h>
#include "Fonts.h"
#include "fonts/ubuntu-6-full.h"
#include "Graph.h"
#include "Reading.h"
#include "UniversalDHT.h"

extern MCUFRIEND_kbv screen;
extern Display display;
extern Fonts fonts;
extern Messages messages;
extern Reading temperature;
extern Reading humidity;

readings_t Reading::takeReadings(void)
{
  /*
    Get the current temp and humidity from the sensor.
    Check that the DHTxx didn't develop a fault or break
    during since the last read. If it did, we're going
    to stop work and report a service fault giving the
    failure time so engineers can figure out how reliable
    these devices are in their environment.
  */
  readings_t R;
  UniversalDHT::Response reading = dht22.read(&R.H, &R.T);
  if (reading.error)
  {
    alarm.sensorFailed(reading);
  }

  temperature.updateReading(R.T);
  humidity.updateReading(R.H);
  environment.checkHumidityConditions();
  
  #ifndef INCUBATOR
  environment.checkTemperatureConditions();
  #endif

  environment.checkHeatIndex(R);  
  return R;
}

void Reading::updateReading(const reading_t reading)
{
  m_currRead = reading + m_correction;
  m_minRead  = (m_currRead < m_minRead) ? m_currRead : m_minRead;
  m_maxRead  = (m_currRead > m_maxRead) ? m_currRead : m_maxRead;
  m_cumulativeMovingAverage = (m_cumulativeMovingAverage + (m_currRead - m_cumulativeMovingAverage) / ++m_cmaCounter) + m_correction;
}

void Reading::bufferReading(const reading_t reading, char* buffer, const semaphore_t flags)
{
  using mixed_t = struct 
  {
    int8_t floatPart {0};
    int8_t intPart   {0};
  };
  
  reading_t r = reading;
  if ( flags & IMPERIAL)
  {
    r = toFahrenheit(reading);
  }

  float integer {0};
  float fract   {0};

  mixed_t read;
  fract = modff(r, &integer);
  read.intPart   = static_cast<int>(integer);
  read.floatPart = static_cast<int>(abs(fract * 10));

  if (read.intPart < 100 && read.intPart > - 9)
  {
    sprintf(buffer, "%d.%1d", read.intPart, read.floatPart);
  }
  else
  {
    sprintf(buffer, "%d", read.intPart);
  }
  
  if (flags & HUMIDITY)
  {
    sprintf(buffer, "%d%c", read.intPart, '%');
  }
}
