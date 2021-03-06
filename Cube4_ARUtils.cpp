/*
    File:		Cube4_ARUtils.cpp
    Purpose:	Helper functions for Freetronics 4x4x4 RGB LED Cube
    Author:		Adam Reed (adam@secretcode.ninja)
    Licence:	BSD 3-Clause Licence
*/

// Include for Arduino Library
#include "Arduino.h"

// Include for Cube Library
#include "Cube.h"

// Include the header file for these functions
#include "Cube4_ARUtils.h"

/*
    rgb2hsv Converts a RGB colour into a HSV representation

    Original code from: http://stackoverflow.com/a/6930407
*/
struct hsv rgb2hsv(rgb_t in)
{
  struct hsv         out;
  double      min, max, delta;

  min = in.color[0] < in.color[1] ? in.color[0] : in.color[1];
  min = min  < in.color[2] ? min  : in.color[2];

  max = in.color[0] > in.color[1] ? in.color[0] : in.color[1];
  max = max  > in.color[2] ? max  : in.color[2];

  out.v = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if ( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);                  // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, v is undefined
    out.s = 0.0;
    out.h = NAN;                            // its now undefined
    return out;
  }
  if ( in.color[0] >= max )                          // > is bogus, just keeps compilor happy
    out.h = ( in.color[1] - in.color[2] ) / delta;        // between yellow & magenta
  else if ( in.color[1] >= max )
    out.h = 2.0 + ( in.color[2] - in.color[0] ) / delta;  // between cyan & yellow
  else
    out.h = 4.0 + ( in.color[0] - in.color[1] ) / delta;  // between magenta & cyan

  out.h *= 60.0;                              // degrees

  if ( out.h < 0.0 )
    out.h += 360.0;

  return out;
}

/*
    hsv2rgb Converts a HSV colour representation into RGB

    Original code: from http://stackoverflow.com/a/6930407
*/
rgb_t hsv2rgb(struct hsv in)
{
  double      hh, p, q, t, ff;
  long        i;
  rgb_t         out;

  if (in.s <= 0.0) {      // < is bogus, just shuts up warnings
    out.color[0] = in.v;
    out.color[1] = in.v;
    out.color[2] = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
    case 0:
      out.color[0] = in.v;
      out.color[1] = t;
      out.color[2] = p;
      break;
    case 1:
      out.color[0] = q;
      out.color[1] = in.v;
      out.color[2] = p;
      break;
    case 2:
      out.color[0] = p;
      out.color[1] = in.v;
      out.color[2] = t;
      break;

    case 3:
      out.color[0] = p;
      out.color[1] = q;
      out.color[2] = in.v;
      break;
    case 4:
      out.color[0] = t;
      out.color[1] = p;
      out.color[2] = in.v;
      break;
    case 5:
    default:
      out.color[0] = in.v;
      out.color[1] = p;
      out.color[2] = q;
      break;
  }
  return out;
}

/*
    buildLEDsArray builds an array that holds the location of each LED.
    It starts at 0,0,0 and works it's way right (X axis), then
    snaps to the left and starts again. Once the entire bottom plane is
    done it moves up to the next one and starts again.
*/
void buildLEDsArray(struct coordinate leds[]) {
  // Start at 0,0,0
  byte X = 0;
  byte Y = 0;
  byte Z = 0;

  for (byte i = 1; i <= 64; i++) {
    // Set the LED position to the current calculated X,Y,Z coordinate
    leds[i].x = X;
    leds[i].y = Y;
    leds[i].z = Z;

    // Increment the coordinate in the X direction
    X++;

    if (X == 4) {
      // We have hit the end of the X's, so snap back to the 0 position, and
      // move to the next Y position.
      X = 0;
      Y++;
    }

    if (Y == 4) {
      // We have hit the end of the Y's, so snap back to the 0 Y position
      Y = 0;
    }

    switch (i)
    {
      // When we are at position 16, 32, and 48 it's time to move up one Z position
      case 16:
      case 32:
      case 48:
        Z++;
        break;
    }
  }
}

/*
    Arrange the N elements of ARRAY in random order.
    Only effective if N is much smaller than RAND_MAX;
    if this may not be the case, use a better random
    number generator.

    Code from: http://benpfaff.org/writings/clc/shuffle.html
*/
void shuffle(int *array, size_t n)
{
  if (n > 1) {
    size_t i;
    srand(analogRead(UNUSED_ANALOG_PIN));
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

/*
    getCorner will take the coordinates of a LED and,
    return which corner of the 4x4x4 Cube it is
*/
byte getCorner(struct coordinate point)
{
  return getCorner(point.x, point.y, point.z);
}

byte getCorner(byte X, byte Y, byte Z)
{
  if (X == 0 && Y == 0 && Z == 0) {
    return 1;
  } else if (X == 0 && Y == 3 && Z == 0) {
    return 2;
  } else if (X == 3 && Y == 3 && Z == 0) {
    return 3;
  } else if (X == 3 && Y == 0 && Z == 0) {
    return 4;
  } else if (X == 0 && Y == 0 && Z == 3) {
    return 5;
  } else if (X == 0 && Y == 3 && Z == 3) {
    return 6;
  } else if (X == 3 && Y == 3 && Z == 3) {
    return 7;
  } else if (X == 3 && Y == 0 && Z == 3) {
    return 8;
  }
}

/*
    drawLine is a wrapper around the cube's line function that works around
    an issue where the line will occasionally not display depending on the
    order of the coordinates provided.
*/
void drawLine(Cube cube, struct coordinate start, struct coordinate end, rgb_t theColour)
{
  if (start.y > end.y && start.z < end.z ||
      start.x > end.x && start.z == end.z ||
      start.x > end.x && start.y == end.y ||
      start.y > end.y && start.x == end.x
     ) {
    // Work around bug in line drawing code by swapping start and
    // end point if particular condiations are meet
    cube.line(end.x, end.y, end.z,
              start.x, start.y, start.z, theColour);
  } else {
    cube.line(start.x, start.y, start.z,
              end.x, end.y, end.z, theColour);
  }
}
