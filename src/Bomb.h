/**
  Bomb.h

  This declares a bomb struct which stores the x position, y position, power and
  exploding time of the bomb.

  The bombs are stored in a linked list, bomb_node, which contains the bomb struct
  and a pointer points to the next bomb_node in the linked list.

  Two global bomb_node pointers are declared. These two pointers, front and back,
  point to the first and last bomb_node in the linked list respectively. And they
  are set to NULL initially, which indicates there are no item in the linked list
  at first.

  This uses the queue data structure.

  Every bomb stored in the linked list can be drawn on the lcd using draw(). This
  draw() function runs in linear time in the size of bomb_node in the linked list.
  If n bombs are placed in the screen, then draw() will require O(n) time. The best
  case is when there no bomb or one bomb in the linked list, so it's O(1). And the
  worst case is O(n).

  A new bomb_node with new bomb placed by the player can be added to the back of
  the linked list using insert(bomb inBomb). This function runs in constant time,
  it requires O(1) time.

  remove() function removes the bomb_node at the front of the linked list. This
  function runs in constant time, it requires O(1) time.
*/

#ifndef BOMB_H
#define BOMB_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

struct bomb{
  uint8_t bomb_x; //x position of bomb
  uint8_t bomb_y; //y position of bomb
  int bomb_power; //Power of the bomb
  unsigned long explodeTime;  //Exploding time of the bomb
};

//A linked list
struct bomb_node{
  bomb Bomb;  //Bomb struct stored in the bomb_node
  struct bomb_node *next; //A pointer points to the next bomb_node in the linked list
};

extern bomb_node *front;  //Points to the first bomb_node in the linked list
extern bomb_node *back;  //Points to the last bomb_node in the linked list

/**
  Draw every bomb in bomb_node in the linked list on the screen, starting at the
  bomb in the first bomb_node which is pointed by front.

  Running time: Linear, O(n).
*/
void drawbomb();

/**
  Input argument: inBomb - the bomb struct with values of the bomb placed by the player;
  Insert a new bomb_node with bomb to the back of the linked list.

  Running time: Constant, O(1).
*/
void insert(bomb inBomb);

/**
  Remove the first bomb_node which is pointed by front in the linked list.

  Running time: Constant, O(1).
*/
void remove();

#endif
