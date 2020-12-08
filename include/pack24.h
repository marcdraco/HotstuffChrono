#ifndef __MARCDRACO_PACK24__ARDUINO
#define __MARCDRACO_PACK24__ARDUINO

/***********************************************************************************
Copyright (c) 2020 Marc Draco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#include "Arduino.h"
#include "types.h"

using pack24_t = struct 
{
    uint8_t   U8;
    uint16_t L16;
};

using unpack24_t = struct 
{
    uint16_t X;
    uint16_t Y;
};

/**
 * @brief 
 * 
 * @param X First 16 bit number range +/- 4095 
 * @param Y Second 16 bit number  +/- 4095
 * @param U8 unsigned char packed value
 * @param L16 unsigned word packed value
 * @brief pack 32 bits into 24 bits 
 */

void pack24(uint16_t X, uint16_t Y, uint8_t* U8, uint16_t* L16)
{
    *U8  = ((X & 0x0F00) >> 4) | ((Y & 0x0F00) >> 8);
    *L16 = ((X & 0x00FF) << 8) |  (Y & 0x00FF);
}

/**
 * @brief Pack an X, Y 32 bit pair into 24 bits
 * 
 * @param X Unsigned int 0 - 4095
 * @param Y Unsigned int 0 - 4095
 * 
 * @return pack24_t 
 */
pack24_t pack24(uint16_t X, uint16_t Y)
{
    pack24_t r;

    r.U8  = ((X & 0x0F00) >> 4) | ((Y & 0x0F00) >> 8);
    r.L16 = ((X & 0x00FF) << 8) |  (Y & 0x00FF);
    return r;
}

/**
 * @brief Unpack a previously packed 16 bit pair
 * 
 * @param U8 Packed char from pack routine 
 * @param L16 Packed word from pack routine
 * @param X Address of var to return "X"
 * @param Y Address of var to return "Y"
 */
void unpack24(uint8_t U8, uint16_t L16, uint16_t* X, uint16_t* Y)
{
    *X  = ((U8 & 0x0F) << 8) |  (L16 & 0x00FF);
    *Y  = ((U8 & 0xF0) << 4) | ((L16 & 0xFF00) >> 8) ;
}

/**
 * @brief Unpack a previously packed 16 bit pair
 * 
 * @param U8 Packed char from pack routine 
 * @param L16 Packed word from pack routine
 * 
 * @return unpacked24_t 
 */
 
unpack24_t unpack24(uint8_t U8, uint16_t L16)
{
    unpack24_t r; 
    r.X  = ((U8 & 0x0F) << 8) |  (L16 & 0x00FF);
    r.Y  = ((U8 & 0xF0) << 4) | ((L16 & 0xFF00) >> 8) ;
    return r;
}

/**
 * @brief Pack 32 bits from two signed variables into 24 bits
 * 
 * @param X First variable - +/- 2047
 * @param Y Second variable - +/- 2047
 * @param U8 Address to store 8 bit variable
 * @param L16 Address to store 16 bit variable
 */
void packSigned24(int16_t X, int16_t Y, uint8_t* U8, uint16_t* L16)
{
    uint8_t sX = ( (X & 0x8000) >> 8);
    uint8_t sY = ( (Y & 0x8000) >> 8);

    if (sX)
    {
        X = (X ^ 0xFFFF) + 1;
    }

    if (sY)
    {
        Y = (Y ^ 0xFFFF) + 1;
    }

    *U8  =  ((X & 0x0700) >> 4) | 
            ((Y & 0x0700) >> 8) | 
              sX | 
              sY >> 4;

    *L16 =  ((X & 0x00FF) << 8) |  
             (Y & 0x00FF);

}

/**
 * @brief Unpack a previously packed signed 16 bit pair
 * 
 * @param U8 Packed char from pack routine 
 * @param L16 Packed word from pack routine
 * @param X Address of var to return "X"
 * @param Y Address of var to return "Y"
 */
void unpackSigned24(uint8_t U8, uint16_t L16, int16_t* X, int16_t* Y)
{
    uint8_t sX = (U8 & 0x80) >> 4; // Sign bit for X
    uint8_t sY = (U8 & 0x08);      // Sign bit for Y

    *X  = (L16 >> 8) | ((U8 & 0x70) << 4);
    *Y  = (L16 & 0xFF) | ((U8 & 0x07) << 8);

    if (sX)
    {
        *X = (*X ^ 0xFFFF) + 1;
    }

    if (sY)
    {
        *Y = (*Y ^ 0xFFFF) + 1;
    }
}

#endif
