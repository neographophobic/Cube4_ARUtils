/*
    File:		Cube4_ARUtils.h
    Purpose:	Header file for helper functions for Freetronics 4x4x4 RGB LED Cube
    Author:		Adam Reed (adam@secretcode.ninja)
    Licence:	BSD 3-Clause Licence
*/

// Ensure that this header file is only processed once, regardless of how
// many time it is included
#ifndef Cube4_ARUtils_h
#define Cube4_ARUtils_h

// Include for Arduino Library
#include "Arduino.h"

// Include for Cube Library
#include "Cube.h"

/*==========================================================================
    APPLICATION SETTINGS (USER EDITABLE)

    UNUSED_ANALOG_PIN   An unused analog pin that is used as a seed for the
                        random number generator to ensure that the random
                        pattern is different each time the cube is started

    TOTAL_LEDS			The total number + 1 of LEDs on the cube.
  ========================================================================== */
const byte UNUSED_ANALOG_PIN = 0;
const byte TOTAL_LEDS = 65;

/*==========================================================================
    Data Structures
  ========================================================================== */
// coordinate is used to track the X, Y, and Z location of a LED
struct coordinate {
  byte x;
  byte y;
  byte z;
};

// hsv is used to track a colour in the hsv representation
struct hsv {
  double h;       // angle in degrees (Hue)
  double s;       // percent (Saturation)
  double v;       // percent (Value - brightness)
};

/*==========================================================================
    Functions
  ========================================================================== */
// rgb2hsv Converts a RGB colour into a HSV representation
struct hsv rgb2hsv(rgb_t in);

// hsv2rgb Converts a HSV colour representation into RGB
rgb_t hsv2rgb(struct hsv in);

// buildLEDsArray builds an array of all possible LED coordinates
void buildLEDsArray(struct coordinate leds[]);

// shuffle randomly shuffles an array of integers
void shuffle(int *array, size_t n);

// getCorner will convert a LED coordinate into a number which represents
// the corner of the cube where that LED is located
byte getCorner(struct coordinate point);
byte getCorner(byte X, byte Y, byte Z);

// drawLine draws a line on the cube working around an issue in the cube's
// line function
void drawLine(Cube cube, struct coordinate start, struct coordinate end, rgb_t theColour);

#endif
