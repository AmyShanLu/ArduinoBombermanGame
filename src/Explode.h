/**
  Explode.h

  This declares explodeBomb(), CheckExplodeWall(uint8_t *x0, uint8_t *y0, uint8_t *width,
  uint8_t *height, bool isx), UpdateWalllife(uint8_t *x0, uint8_t *y0, uint8_t *width,
  uint8_t *height, bool isx) and checkexplodedarea(uint8_t x0, uint8_t y0, uint8_t width,
  uint8_t height, int *ishit) functions.

  explodeBomb() shows the exploding area of the bomb whose exploding time is less than the
  current time.

  CheckExplodeWall(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx) goes
  through all walls in wall_node in the double linked list, it checks whether there're walls in
  the exploding area of the bomb. If there're walls in the exploding area of the bomb, it changes
  the position of the exploding rectangle.

  UpdateWalllife(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx) goes
  through all walls in wall_node in the double linked list, it decreases the Walllife of the wall
  whose edge overlaps the edge of the exploding rectangle of the bomb.

  checkexplodedarea(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, int *ishit) checks
  if any of two players are hitted by the bomb. It updates the lose_player value when the player
  is in the exploding area of the bomb.

  All of these functions run in linear time, so if n is the number of items in the linked
  list, these functions require O(n) time.
*/

#ifndef EXPLODE_H
#define EXPLODE_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

extern uint8_t hitcount;

/**
  This function explodes the bomb whose exploding time is less than the current
  time. It shows the exploding area of the bomb by 100 ms in the screen.

  Running time: Linear, O(n).
*/
void explodeBomb();

/**
  This function takes input arguments:
  *x0 - pointer to the x position of the upper left corner of the exploding rectangle;
  *y0 - pointer to the y postion of the upper left corner of the exploding rectangle;
  *width - pointer to the width of the exploding rectangle;
  *height - pointer to the height of the exploding rectangle;
  isx - whether the exploding rectangle is horizontal or vertical.
        1 represents the horizontal exploding rectangle;
        0 represents the vertical exploding rectangle;

  This function goes through every wall in wall_node in the double linked list. It checks
  if there're any walls in the exploding area of the bomb. If bomb's exploding area go
  through the wall, it resets the contents pointed by x0, y0, width or height which are
  the exploding area of the bomb.

  Running time: Linear, O(n).
*/
void CheckExplodeWall(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx);

/**
  This function takes input arguments:
  *x0 - pointer to the x position of the upper left corner of the exploding rectangle;
  *y0 - pointer to the y postion of the upper left corner of the exploding rectangle;
  *width - pointer to the width of the exploding rectangle;
  *height - pointer to the height of the exploding rectangle;
  isx - whether the exploding rectangle is horizontal or vertical.
        1 represents the horizontal exploding rectangle;
        0 represents the vertical exploding rectangle;

  This function goes through every wall in wall_node in the double linked list, it decreases
  the Walllife of the wall whose edge overlaps the edge of the exploding rectangle of the bomb.

  Running time: Linear, O(n).
*/
void UpdateWalllife(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx);

/**
  This function takes input arguments:
  *x0 - pointer to the x position of the upper left corner of the exploding rectangle;
  *y0 - pointer to the y postion of the upper left corner of the exploding rectangle;
  *width - pointer to the width of the exploding rectangle;
  *height - pointer to the height of the exploding rectangle;
  *ishit - whether the player is hitted by the bomb.
           1 represents the player is hitted by the bomb;
           0 represents the player is not hitted by the bomb;

  This function checks whether the players are in the exploding area of the bomb. It updates
  the lose_player value to indicate which player loses the game.

  Running time: Linear, O(n) where n is the size of player which is 2.
*/
void checkexplodedarea(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, int *ishit);

#endif
