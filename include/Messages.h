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

#ifndef __DRACO_HOTSTUFF_MESSAGES_H
#define __DRACO_HOTSTUFF_MESSAGES_H

#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <Wire.h>

#include <Adafruit_GFX.h>     // Core graphics library by AdaFruit
#include <MCUFRIEND_kbv.h>    // David Prentice's Hardware-specific library - your shield might vary
#include "UniversalDHT.h"   // @winlinvip's DHT11/22 library functions modified by Tim Harper

#include "Fonts.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "Sevensegments.h"
#include "types.h"

class Messages
{
  public:

  enum
  {
    c, f, temperatureScale, humidityScale, xScale0, xScale1, xScale2, dew, frost
  };

  String translations[frost+1];

  char* pText;

  Messages() 
  {
    pText = nullptr;
    translations[c]        = F("c");
    translations[f]        = F("f");
    translations[temperatureScale] = F("Temperature in ");
    translations[humidityScale] = F("Relative Humidity %");
    translations[xScale0]  = F("-- ");
    translations[xScale1]  = F(" minutes(s) --");
    translations[xScale2]  = F(" hour(s) --");
    translations[dew]      = F("Dew Point");
    translations[frost]    = F("Frost Pnt");
  };


  void debugger(const int X, const int Y, char* msg);

  /**
   * 
   * @brief Print a short enumerated message
   * 
   * @param M Message number
   */

  void execute(const uint8_t M);

  /**
   * @brief Print a buffer of text prepared by sprintf
   * 
   * @param pText char* to the buffer
   */

  void execute(const char* pText);

  /**
   * @brief Overwrite a predefined message in background
   * 
   * @param M enumerated message
   */

  void clear(const uint8_t M);
  
  /**
   * @brief Get a pointed to an enumerated message block
   * 
   * @param M enumerated message
   * @return const char* to message string (NOT a string object!)
   */

  const char* getString(const uint8_t M);

  /**
   * @brief Centers a block of text given character size
   * 
   * @param text String of characters
   * @return uint8_t central X offset based on the screen width
   */
  
  uint16_t textWidth(char* message);
  
  /**
   * @brief 
   * 
   * @param ink 
   * @param characterSize 
   * @param number 
   * @param flags 
   */
  void printNumber(const colours_t ink, const uint8_t characterSize, const uint8_t number, const semaphore_t flags);
  
  /**
   * @brief 
   * 
   * @param floaty 
   * @param buffer 
   * @param formatWidth 
   * @param integralWidth 
   */
  void rightAlign(const float floaty, const char* buffer, const uint8_t formatWidth, const uint16_t integralWidth);
  
  /**
  * @brief A little nod to *nix systems
  * @remark Hotstuff was developed on Linux Mint due to the much faster compiler using VSCode
  */
  void showUptime();

  /**
   * @brief the min/max temp and humidity display 
   * 
   */
  void showMinMax(void);

};

extern Messages messages;
#endif