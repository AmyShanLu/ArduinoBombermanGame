/**
  Gamesetup.h

  This declares setup(), gamestart(), gameover(), PlayerInputName(), ClearLastgame() functions.
*/

#ifndef GAMESETUP_H
#define GAMESETUP_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

extern uint16_t p1_color;
extern uint16_t p2_color;
extern int8_t lose_player; //Indicate which player loses the game
extern bool endgame; //Indicate whether the game ends
extern char Player_name[2][9];
extern unsigned long GameStartTime; //Game start time

/**
  Before the game begins, one arduino acts as sever, the other acts as client.
  After both client and sever are ready to start the game. Then initializing the
  initial positions and color for two players, and start the game.
  After 'game start' shows on the screen, makeObstacles() will be executed and
  draw and insert 15 pre-set wall on the screen and in the linked list wall_node.
  This runs in linear time in the size of the wall_node which is 15.
  Also, it executed DisplayRemainingTime() function to show the remaining time of
  the game on the left corner of the screen.
  And the positions of 4 power-up are stored in an array.
*/
void setup();

/**
  Show 'Game Starts' on the screen, and let players enter their names.
*/
void gamestart();

/**
  Save two players name into Player_name array. And the player's name cannot exceed
  8 characters.
*/
void PlayerInputName();

/**
  Input argument: lose_player - Indicate which player loses the game
                                0 - This Arduino loses the game
                                1 - The other Arduino loses the game
  This function shows 'Win' or 'Lose' or 'Tie' on the screen. When one of player
  loses the game, it executed UpdateEEPROM() function which saves the winners name
  and score to EEPROM.
  Also, when the game ends, it will execute ClearLastgame() function which clears
  all remaining bombs and walls in the linked list bomb_node and wall_node, so that
  the remaining bombs and walls won't be drawn on the screen when the next round begin.
*/
void gameover(int8_t lose_player);

/**
  This function clears the remaining bombs and walls in the linked lists bomb_node
  and wall_node, and two players' name in the array. So that the next round will not
  be affected by the data from the last round.

  Running time: Linear, O(n).
*/
void ClearLastgame();

#endif
