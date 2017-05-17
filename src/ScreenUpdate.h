/**
  ScreenUpdate.h

  This declares: void scanJoystick() - Scan joystick;
                 void updateDisplay() - Update two players' position on the screen;
*/

#ifndef SCREENUPDATE_H
#define SCREENUPDATE_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <math.h>

extern Adafruit_ST7735 tft;
extern unsigned long timeBombplaced;

//Scan joystick
void scanJoystick();

//Update two players' position on the screen
void updateDisplay();

#endif
