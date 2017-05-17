/**
  Powerup.h

  This declares the Spawnpoint struct which stores the x, y position of the spawn powerup,
  and whether the powerup is active.

  The four powerup spawn points are stored in a global Spawnpoint array: Powerup[4].

  This declares functions related to powerup: void SpawnPowerup(int spawn);
                                              void PowerupBuffCheck();
                                              void UpdatePowerup();
*/

#ifndef POWERUP_H
#define POWERUP_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

struct Spawnpoint{
  int spawn_x;  //The x position of the spawn
  int spawn_y;  //The y position of the spawn
  bool active;  //Whether the spawn is activated
};

extern struct Spawnpoint Powerup[4];  //Array of Spawnpoint, stores 4 powerup spawn points data 

/**
  This functions draws the powerup spawn in the clockwise
*/
void SpawnPowerup(int spawn);

/**
  This function checks if the player picks up the powerup spawn point. If the player picks up
  the powerup, it increases the bomb power of the player by 1.

  This function runs in quadratic time, O(n^2).
*/
void PowerupBuffCheck();

/**
  This function draws the activated powerup spawn on the screen.

  Running time: Linear, O(n).
*/
void UpdatePowerup();

#endif
