/**
  Player.h

  This declares the player struct which stores the x position, y position, bomb power,
  whether player's position is changed and whether the player placed a bomb.

  This declares two global arrays with size 2, Player[] saves two player's current
  position and prev_Player[] saves players' previous position.

  placeBomb(player *Player) is executed when the player pressed the joystick. It will
  draw bomb at player's position on the screen, and calls insert(inBomb) to add this
  bomb to the queue.

  checkisbomb() function restricts the player go through the bombs. It runs in linear
  time in the size of bombs on the queue.

  CheckHitWall() function restricts the player go through the walls. It runs in linear
  time in the size of walls on the double linked list.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <math.h>

struct player{
  uint8_t player_x; //x position of player
  uint8_t player_y; //y position of player
  uint8_t bomb_buff; //the player's bomb power
  bool placechanged;  //whether the player's position was changed
  bool isbomb;  //whether the player placed a bomb
  bool isable;  //whether the player can place a bomb
};

extern struct player Player[2];  //Global variable, an array for 2 players
extern struct player prev_Player[2];  //Global variable, 2 players' previous positions

/**
  When player presses the joystick, place one bomb at player's position and insert it on the queue

  Running time: Constant, O(1).
*/
void placeBomb(player *Player);

/**
  Input argument: bool isx - Indicate the x position or the y position of the player is changed.

  This function goes through every bomb in the bomb_node in the queue, check if the
  player hits the bomb. If the player hits the bomb, it changes player's position so
  that it can restrict player go through the bomb.

  Running time: Linear, O(n).
*/
void checkisbomb(bool isx);

/**
  Input argument: bool isx - Indicate the x position or the y position of the player is changed.

  This function goes through every wall in the wall in the linked list, check if the
  player hits the wall. If the player hits the wall, it changes player's position so
  that it can restrict player go through the wall.

  Running time: Linear, O(n).
*/
void CheckHitWall(bool isx);


#endif
