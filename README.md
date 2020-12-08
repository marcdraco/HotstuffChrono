NOTE: This is not a medical device and is no substitute for proper planning and working conditions!
 
Aiming to become the greatest little graphing thermohygrometer available for an Arduino Uno. This is v1.0

Applications include:

* Infant/nursery temperature monitor
* Outbuilding temperature monitor
* Greenhouse monitor
* External atmospheric check
* Home/office HVAC check and monitoring
* Incubator monitor

-- Features 

* Completely free for non-commercial use.
* Comes with a super-speed 7 segment simulated font "Rose Digital" and the 16 segment with full alpha, "Astro Nerd" (see licensing for limitations on this part of the software, it’s for a good cause, we promise).
* Flicker free updates (1) 
* Auto ranging graph covering the whole range of the DHT11 and DHT22 sensors
* Uses a DHT11 (at a pinch, we haven't tried) or DHT22 to get temp and relative humidity.
* Displays relative humidity and temperature in Fahrenheit or Celsius
* Displays dew (condensation) AND frost (condensing ice) points in the current units
* Flashing graphic damp and dry air warnings.
* User customisable proportional display fonts (option)
* Main display range -9 —> 99 F or -9 —> 80C (Range warning if exceeded)
* Humidity from 0% -> 99% RH.
* Records max and minimum temperature and humidity since reset
* Includes Steadman's calculations to for uncomfortable or dangerous working conditions
* Requires a minimum of parts Uno, 3.5" TFT shield
* Optionally switchable F/C
* Programmable heater control (for incubators, etc.)
* Simple to build
* Highly modular code
* <lie mode>Clean, fully commented & documented code</lie mode>
* Did we say it was free?

(1) Buffering limitations on the UNO mean the chart flashes briefly during updates.

In the following documentation, the compilation constants can be found in “constants.h”.

Q: Can it be used as a household heating control?

A: Yes and no. Yes, because it's really no different to any other thermostat (if we ignore humidity for a moment) and no because we can't, in good conscience encourage people to mess around with mains electricity, even if you're using opto-isolated, solid state relays.

Q: Why are there only indicators for dry and damp air? Why do they take so long to go on/off once the “watershed” is passed?

A: We originally intended to have an indicator icons for heating/cooling too but there simply wasn’t the flash space on the target Arduino model (UNO/AtMega328p). Our icons are still in the code but are remarked out so they are not compiled.
We kept damp/dry because these are more difficult to notice when we go about our daily lives but are equally important to human health and well-being. While we “feel” hot or cold (and can adjust for that) we can’t feel damp or dry air as easily. Consistently damp air will give rise to mold, a living organism that produces invisible spores that travel through the air and can lodge in our airways producing some nefarious effects like asthma. Further, mold can be a serious problem in our homes because despite being unsightly, it can eat away at wood behind the scenes, often with expensive consequences for home owners.

At the other end of the scale is dry air. Now you might think dry air would be fine, but in fact, it’s also hazardous for us (less so for where we live). While not obvious, our airways and lungs are kept slightly damp by the mucosa. Mucous is apt to dry out in dry air in just the same way as clothes dry when hung outside and when that happens it gets difficult to breathe. Dry air is also unpleasant for people with dry skin conditions and might make contribute to dry and tired eyes.

If you’re watching the “real time” (default 5 second) reading updates, you’ll notice that the lamps don’t come on immediately and it may take a few minutes before they do (or clear after the condition passes). The clue is in the chart - or more specifically the cumulative averaged figure that we use to prevent sudden blips (like someone breathing on the sensor!) from setting off a visible warning. 

Compile constants:
DAMP_AIR_WATERSHED
DRY_AIR_WATERSHED

Q: How does the graph work?

A: The graph attempts to cover a fairly narrow range by calculating the maximum and minimum temperature/humidity. Each value is treated separately. Readings are taken (by default) every five seconds and stored as a statistic called a “cumulative moving average”. The chart’s resolution is programmable from 5 seconds per plotted point. New plotted points only appear on the chart update but this allows for a horizontal resolution for about 15 minutes to at least 24 hours.

Compile Constant:
CHART_UPDATE_FREQUENCY: 

Q: Why does the graph appear as a straight line even though there are obvious fluctuations in the readings?

A: This is by design and is a function of the cumulative moving average (CMA). Small fluctuations in temperature/humidity happen throughout the day but they are unlikely to have any great effect on people or property as they are canceled out. The CMA gives a broader, more accurate picture over a time-scale of around 24 hours, although it will run for much longer if allowed to.

Q: What are the Steadman Heat Index functions and what do they do?

A: Glad you asked, in a nutshell, think “feels like” or “wind chill” (colder) temperature for how our bodies react in high degrees of humidity! Robert Steadman came up with this idea of effective “heat” around 1979 and developed the functions we’ve adapted here. Heat stroke isn’t something a lot of us are familiar with, but it’s akin to sun stroke and, like sun stroke can result in serious sickness and even death.

We regulate our internal (core) body temperature around 98.6 f / 37 c and no matter how our environment changes our body does its best to maintain that (a process known as homeostasis). We accomplish this in a number of ways, but our primary mode of keeping cool is to excrete water (and a small amount of other compounds such as sodium salt) on our skin where it evaporates taking away a small amount of energy as heat. (A smaller amount comes through our lungs but this is negligible compared to how our skin keeps us cool.) You’d also notice as you get hot (as your core temperature rises) your skin becomes red as the capillaries open and fill with blood. This is more noticeable in drier air because our sweat evaporates instantly. Bodies are amazing.

We all know we need water to live, but we also need salt. Not a lot - many Westerners get far too much anyway - but salt is essential to live as sodium ions are used by our cells for electrical signaling. The most obvious symptom of hyponatremia (low concentration of sodium ions in blood) is muscle cramps and if you recall that your heart is a muscle and your lungs are driven by them, you can see why this is a serious issue.

So far, so good, but this evaporative cooling is directly affected by the amount of water that’s already in the air around you. Air at any given temperature (and pressure, but we’ll assume that’s not going affect the accuracy here) can only “hold” a given amount of water as gas. As air approaches reaches this saturation amount, it becomes increasingly difficult for water to enter its gaseous state and we develop visible beads of sweat that stand on the skin and soak into our clothes.

What makes this even worse for people working in humid climates is the fact that air holds more gaseous water as the temperature increases. This is particularly noticeable in rain forests of course (many of us will be familiar with the sight of soldiers sweating profusely as they hack their way through tropical jungle. The same isn’t usually true of deserts where, although the temperature may be high, the humidity is low. We don’t need to “break” a visible sweat in such conditions, to still lose huge amounts of water leading to dehydration and, potentially, death.

We generate heat by a process called metabolism - by reacting glucose with oxygen - according to how hard we’re exerting ourselves and most of that comes from our muscles. Even when we’re completely still, we’re generating a small amount of heat (even our brain generates about 20 watts!) The lowest measure when we’re absolutely still is called the Basal Metabolic rate - this is sometimes compared to our resting metabolism (RMR) - but sitting watching TV or studying for a term paper pushes our metabolic rate up as our brain is using resources. As fascinating as this is, it’s beyond the scope of this paper.

Ok, so we all know that when we’re working out we start to sweat (or our skin gets a “healthy” glow as described above). It’s clear that the harder we exert ourselves the hotter we get and the harder our body has to work to keep our temperature within limits. Standing sweat is an indicator that the air around you is too humid to keep you cool. If you continue to work, your core body temperature will slowly rise and this is when heat stroke becomes a serious and extant problem. On hot, humid days, manual workers can find rapidly find themselves feeling unwell with symptoms including headache, confusion, vomiting, aggression; eventually leading to syncope, unconsciousness and death.

And herein enters the Steadman’s heat index. Steadman’s formulas estimate the effective temperature we’re working in based on the actual air temperature and the relative humidity. This function only kicks in when the measured temperature reaches about 81 f / 27 c because the functions are not sufficiently accurate below that and even with 100% RH, it’s thought we can keep a safe temperature.

TL;DR

As the limit is passed the display changes so the “instant” temperature starts flashing and switches to the effective (feels like) temperature. At the same time the humidity display is switched off and displays “oH”  (overheat). Very high effective temperatures exceed two digits in Fahrenheit, so the third digit appears in the “tenths” column, so (40 c) 105f appears as 10.5f but flashes in alternative colours. This is not a bug. 

There are four levels of Heat Index according to the danger to human health (of people who are active in this climate). It’s important to note these numbers are the effective temperature, not the actual temperature.

Blue:     27 - 31 c / 81 - 88 f - CAUTION
Yellow: 32 - 40 c / 90 - 105 f  - EXTREME CAUTION
Purple:  41 - 53 c / 106 - 127 f - DANGER
Red:       > 53 c / > 130 f - EXTREME DANGER
https://en.wikipedia.org/wiki/Basal_metabolic_rate
https://en.wikipedia.org/wiki/Heat_index

Compile Constants:
TEMP_CAUTION
TEMP_WARNING
TEMP_RISK
TEMP_DANGER
STEADMAN

Q: Why are the main numbers on the display flashing?

A: The readings flash when the unit is trying to attract your attention, typically because it has detected an unsafe or even dangerous thermal environment for humans.  The function is enabled by default but can be compiled out.

Q: What if the temperature or humidity goes out of range?

A: During the design phase, we opted for a dual digit humidity and “two point five” digits for temperature. This covers most applications in the home, office, and outbuildings while affording a relatively large display font so it can be read from several feet away. If these ranges are exceeded the display switches to solid red. The graph will still do its best to follow the underlying conditions.
 
Q: Can it be adapted for used for use with an incubator?

A: Absolutely - with some reservations if you want to use it as a control (rather than a monitor). First of all, the heater should be low voltage (12 - 24v is ideal) and fully isolated from any mains electricity. Secondly, the accuracy of DHT22s may not be as precise as needed. We are including some features specifically for incubators with a display more oriented to that task - these are a compilation option which will be added in due course.

In the current version (1.0 as of this writing) pin 13 goes high (lighting the on-board LED) when the temperature falls below the temperature (in C) set at “MIN_COMFORT_LEVEL” and switches off when it goes above “MAX_COMFORT_LEVEL”. This could, in theory, drive a power MOSFET transistor or solid-state relay to activate a heater. This function has not been tested in a practical application so that is left to users for experimentation.

Incubator design is more difficult than people imagine although chicken eggs (which are Marc's area of expertise) seem to tolerate a fairly wide range without too much of an issue. A wide range means about +/-1 degree C either side of ideal where cooler is better since warmer temperatures tend to kill female embryos. Automatic humidity control isn't really practical for land-based birds in simple incubators. Better to add a small tray of water as required and monitor manually. Ducks and other waterfowl require more humidity (the parents incubated the eggs after returning from feeding which often means going into water) so a "spritzer" may be possible.

Although incubator design is beyond the scope of this README, it's also worth noting that the sensor should be placed as close to the eggs as possible - ideally about half-way up the side of the eggs. Air tends to stratify in an incubator (and under a parent bird, I expect, but to a lesser degree) so the addition of fans as seen in some non-commercial designs is often counterproductive.

Provided there is some airflow through the unit, the embryos should hatch without serious issue - they don't need a huge amount of oxygen during the incubation as all the gaseous exchange is through the egg's shell! They need far more immediately they pierce the shell but as this stage the heating requirements change from incubation to brooding as the chicks are free to move around and are consuming oxygen at a far greater rate. Removal of the unit's lid to check on them after "pipping" should be enough to replenish the oxygen before they are placed in the brooder. Hotstuff can, of course, be used to keep an eye on your brooder temperatures too.

Compile Constants:

MIN_COMFORT_LEVEL
MAX_COMFORT_LEVEL

# HotStuffChrono
# HotStuffChrono
# HotStuffChrono
