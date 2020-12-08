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

#ifndef __DRACO_HOTSTUFF_EVIRONMENTAL
#define __DRACO_HOTSTUFF_EVIRONMENTAL

class Environmental
{

  public:
  /**
   * @brief Test the humidity is within watershed (and trigger alarms)
   * 
   */
  void checkHumidityConditions();

  /**
   * @brief Test the temperature is within watershed (and trigger alarms)
   * 
   */
  void checkTemperatureConditions();

  /**
   * @brief Set warning conditions if Steadman's temp is exceeded
   * 
   * @param T Temperature in C
   * @param H Relative humidity in %
   */
  void checkHeatIndex(const readings_t readings);

  /**
   * @brief Display warnings about effective working temperature
   * 
   * @param T Steadman's Effective temperature in C
   * 
   * @remarks Displays warnings in the lower half of the screen to stop or reduce 
   * activity for health and safety. 
   */

  void unsafeTempWarnings(const float T);
  
   /**
   * @brief Steadman's 1994 approximation for heat in given humidity 
   * 
   * @param T Temperature in degrees C
   * @param R Relative humidity in %
   * @return float 
   * 
   * @remarks  Approximate the "Heat index" per Steadman 1994. While the dry bulb temperature
   *  may be well within safe limits for humans, the addition of humidity reading
   * prevents our bodies from effectively evaporatively cooling with the result
   * that our internal temperature rises - and the different of just a few degrees
   * C can cause fatigue, aggression and eventually heat stroke and possibly death.
   * These figures assume the person is doing mildly stressful work such as housework
   * Workers in more extreme physical labour will suffer faster and vice versa.
   * This is becoming a much more widespread problem as our climate changes.
   * 
   * Wikipedia https://en.wikipedia.org/wiki/Heat_index
   * 26–32 °C CAUTION: fatigue is possible with prolonged exposure and activity.
   *          Continuing activity could result in heat cramps.
   * 32–41 °C Extreme caution: heat cramps and heat exhaustion are possible.
   *          Continuing activity could result in heat stroke.
   * 41–54 °C Danger: heat cramps and heat exhaustion are likely.
   *          Heat stroke is probable with continued activity.
   * > 54 °C  Extreme danger: heat stroke is imminent.
   */

  float heatIndex(const readings_t R);
  /**
 * @brief Magnus' Dew Point (condensation temperature) calculation 
 * 
 * @param T Temperature in degrees C
 * @param R Relative humidity in %
 * @return float 
 * @remarks 
 * https://en.wikipedia.org/wiki/Dew_point
 * 
 */
  reading_t magnusDewpoint();

  void setColour(const reading_t value, const limits_t limits);
};

extern Environmental environment;
#endif
