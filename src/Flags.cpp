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

#include "Fonts.h"
#include "hotstuff_fonts.h"
#include "hotstuff.h"
#include "types.h"

class Flags 
{
  private:
    semaphore_t semaphore;

  public:

  Flags()
  {
    semaphore = 0;
  }
  void clear(const semaphore_t flag)
  {
    semaphore &= (flag ^ 0xFFFF);
  }

  void set(const semaphore_t flag)
  {
    semaphore |= flag;
  }

  bool isSet(const semaphore_t flag)
  {
    return (semaphore & flag);
  }

  bool isClear(const semaphore_t flag)
  {
    return !(semaphore & flag);
  }

  void flip(const semaphore_t flag)
  {
    semaphore ^= flag;
  }
};
