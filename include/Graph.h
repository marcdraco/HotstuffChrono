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

#ifndef __DRACO_HOTSTUFF_GRAPH_H
#define __DRACO_HOTSTUFF_GRAPH_H

#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <Wire.h>

#include <Adafruit_GFX.h>     // Core graphics library by AdaFruit
#include <MCUFRIEND_kbv.h>    // David Prentice's Hardware-specific library - your shield might vary
#include "UniversalDHT.h"     // @winlinvip's DHT11/22 library functions modified by Tim Harper

#include "Alarm.h"
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

class Graph
{
  private:
    
  #ifdef USE_GRAPH
    uint8_t m_xStep    {27}; // X - reticle
    uint8_t m_yStep    {20}; // Y reticle
    uint8_t m_circular   {};  // counts the times the graph is updated

    int16_t m_temperature[GRAPH_WIDTH] {};
    int16_t m_humidity[GRAPH_WIDTH]    {};
  #endif 

  public:

  Graph() {};

  /**
   * @brief Simple (line art) pointer
   * 
   */
  void drawPointers();

  /**
   * @brief Draw the graph lines and chart calibration markings
   * 
   */  
  void drawReticles(const uint8_t xDivs, const uint8_t yDivs);
  
  /**
   * @brief draws circular plots for the analogue version
   * 
   */
  void drawRadials();

  /**
   * @brief Displays the main graph
   * 
   * convert the temperature and humidity.reading readings into something that scales to the chart.
   */

  void drawGraph();

  /**
   * @brief Puts the scales and text around the chart
   * 
   */  
  void drawGraphScaleMarks();
  /**
   * @brief draws a trianglular polygon
   * 
   * @param polygon  pointer to a polygon structure
   * @param ink 
   * @param outline 
   */

  void draw(const triangle_t* polygon, const colours_t ink, const colours_t outline);

  /**
   * @brief draws a quadrilateral polygon
   * 
   * @param polygon  pointer to a polygon structure
   * @param ink 
   * @param outline 
   */
  void draw(const quadrilateral_t* polygon, const colours_t ink, const colours_t outline);

  /**
   * @brief translates (moves) draws a trianglular polygon
   * 
   * @param polygon  pointer to a polygon structure
   * @param ink 
   * @param outline 
   */

  void translate(triangle_t* polygon, const coordinates_t cords);
  /**
   * @brief translates (moves) a quadrilateral polygon
   * 
   * @param polygon  pointer to a polygon structure
   * @param ink 
   * @param outline 
   */

  void translate(quadrilateral_t* polygon, const coordinates_t cords);

  /**
   * @brief Rotates a triangular polygon
   * 
   * @param polygon  pointer to a polygon structure
   * @param theta angle to rotate the shape by
   */
 
  void rotate(triangle_t* polygon, const angle_t theta);

  /**
   * @brief Rotates a quadrilateral polygon
   * 
   * @param polygon pointer to a polygon structure
   * @param theta angle to rotate the shape by
   */
  
  void rotate(quadrilateral_t* polygon, const angle_t theta);

  /**
   * @brief A simple vertial bar on the chart with maxium and minimum values
   * 
   * @param x 
   * @param reading 
   * @param minimum 
   * @param maximum 
   * @param scale 
   * @param ink 
   * @param pointer 
   */
  void drawIBar(const ucoordinate_t x, const reading_t reading, int16_t minimum, int16_t maximum, const int8_t scale, const colours_t ink, const bool pointer);
  /**
   * @brief 
   * 
   * @param x 
   * @param reading 
   * @param min 
   * @param max 
   * @param scale 
   * @param ink 
   */
  void drawMinMax(const ucoordinate_t x, const reading_t reading, const int16_t min, const int16_t max, const int8_t scale, const colours_t ink);

  /**
   * @brief 
   * 
   * @param x 
   * @param reading 
   * @param scale 
   * @param size 
   * @param ink 
   */
  void drawDiamond(const ucoordinate_t x, const reading_t reading, const uint16_t scale, const uint8_t size, const colours_t ink);

  /**
   * @brief 
   * 
   * @param x 
   * @param reading 
   * @param scale 
   * @param size 
   * @param ink 
   */
  void drawTarget(const ucoordinate_t x, const reading_t reading, const uint16_t scale, const uint8_t size, const colours_t ink);
};
extern Graph graph;

#endif