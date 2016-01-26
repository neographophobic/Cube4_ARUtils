/*
 *  File:		Cube4_ARUtils.h
 *  Purpose:	Header file for helper functions for Freetronics 4x4x4 RGB LED Cube
 *  Author:		Adam Reed (adam@secretcode.ninja)
 *  Licence:	BSD 3-Clause Licence
 */

// Ensure that this header file is only processed once, regardless of how
// many time it is included
#ifndef Cube4_ARUtils_h
#define Cube4_ARUtils_h

// Include for Arduino Library
#include "Arduino.h"

// Include for Cube Library
#include "Cube.h"

#define UNUSED_ANALOG_PIN 0

struct coordinate {
  byte x;
  byte y;
  byte z;
};

struct hsv {
  double h;       // angle in degrees
  double s;       // percent
  double v;       // percent
};

struct hsv rgb2hsv(rgb_t in);
rgb_t hsv2rgb(struct hsv in);
void buildLEDsArray(struct coordinate leds[]);
void shuffle(int *array, size_t n);

byte getCorner(byte X, byte Y, byte Z);
byte getCorner(struct coordinate point);

#endif
