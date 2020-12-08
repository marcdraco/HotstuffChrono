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

#ifndef __HOTSTUFF_READING
#define __HOTSTUFF_READING

#include "Alarm.h"
#include "Display.h"
#include "Environmental.h"
#include "Fonts.h"
#include "Flags.h"
#include "Graph.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "Messages.h"
#include "Sevensegments.h"
#include "Trig.h"
#include "types.h"

extern globalVariables globals;

class Reading
{
    private:
    reading_t m_minRead    {};
    reading_t m_maxRead    {};
    reading_t m_cumulativeMovingAverage {};
    reading_t m_correction {};
    reading_t m_cmaCounter {};
    reading_t m_currRead   {};  
    reading_t m_special    {};    // Holder when overtemp occurs (so we don't break the chart!) Not used by humidity
    colours_t m_trace      {};    // graph line colour
    colours_t m_flashing   {};
   
    public:
    
    Reading() {}
    
    /**
     * @brief Set the Special object (temperature only)
     * 
     * @param value - the "effective" temp
     */

    void setEffective(reading_t value)
    {
      m_special = value;
    }

    /**
     * @brief Get the special object (temperature only)
     * 
     * @return reading_t 
     */
    reading_t getEffective()
    {
      return m_special;
    }

  /**
   * @brief Get the Trace object
   * 
   * @return colours_t 
   */
  colours_t getTrace()
  {
    return m_trace;
  }

  /**
   * @brief Set the Trace object
   * 
   * @param C 
   */
  void setTrace(const colours_t C)
  {
    m_trace = C;
  }

  /**
   * @brief 
   * 
   * @param R 
   */
  void initReads(const reading_t R)
  {
    m_cumulativeMovingAverage = R;
    m_maxRead  = R;
    m_minRead  = R;
    m_currRead = R;
  }

  /**
   * @brief Get the Min Read object
   * 
   * @return reading_t 
   */
  reading_t getMinRead()
  {
    return floor(m_minRead);
  }

  /**
   * @brief Get the Max Read object
   * 
   * @return reading_t 
   */
  reading_t getMaxRead()
  {
    return ceil(m_maxRead);
  }

  /**
   * @brief Get the Raw Reading object
   * 
   * @return reading_t 
   */
  reading_t getRawReading()
  {
    return m_currRead;
  }

  /**
   * @brief Get the Reading object adjusted for metric or imperial (temperatures)
   * 
   * @return reading_t 
   */
  reading_t getReading()
  {
    return (CHECKBIT(globals.gp, USEMETRIC) ) ? m_currRead : toFahrenheit(m_currRead);
  }

  /**
   * @brief 
   * 
   * @param reading 
   */
  void updateReading(const reading_t reading);

  /**
   * @brief Get the current RH and Temp from the DHT11/22
   * 
   */

  static readings_t takeReadings();

  
  /**
   * @brief Converts a number into a string
   * 
   * @param reading floating point reading
   * @param buffer pre-configured buffer to hold the result
   * @param flags indicates metric or imperial conversion
   */
    void bufferReading(const reading_t reading, char* buffer, const semaphore_t flags);

    /**
     * @brief get the cumulative moving average for this read
     * 
     * @return reading_t 
     */
    reading_t getCMA()
    {
      return m_cumulativeMovingAverage;
    }

  /**
   * @brief returns the CMA counter UP TO the maximum graph width (plottable points) 
   * 
   * @return uint8_t the count from 0 -> graph's plottable width
   * 
   * @remarks The CMA counter can get big, really big, you just wouldn't believe how
   * vastly mind-boggling big it can get. Listen... (apologies to Douglas Adams for 
   * butchering his script, again). The CMA counter is a (four byte) floating
   * point number so it can hand numbers into the billions but this function is
   * used to help plot the chart which is of limited width. Converting back to a byte
   * width return helps ease the burden on the program space. Ideally this number would
   * be in a "global" (single)value.. but hey ho, that's an optimization you might want
   * to do..
  */
    uint8_t getCMACount()
    {
      return (m_cmaCounter > static_cast<reading_t>(GRAPH_WIDTH)) ? GRAPH_WIDTH : static_cast<uint8_t>(m_cmaCounter);
    }
};

extern Reading temperature;
extern Reading humidity;
#endif