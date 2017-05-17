/**
  Timeremain.h

  This declares functions that related to the timing of the game:
    void InitialTime();
    void DisplayRemainingTime();
    void UpdateRemainingTime();
    void checktimeframe(bool isx);
*/

#ifndef TIMEREMAIN_H
#define TIMEREMAIN_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

//Time limit for each round of game is 3 minutes
extern char *gameendtime;  //values in the array indicates time x:xx

//game start time
void InitialTime();

//Display the remaining time of the game on the upper left corner of the screen
void DisplayRemainingTime();

//Update the time remaining, decrease the time by 1 second
void UpdateRemainingTime();

//Restrict player go through the timing area
void checktimeframe(bool isx);

#endif
