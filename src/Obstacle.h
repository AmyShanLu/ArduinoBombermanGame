/**
  Obstacle.h

  This declares wall struct which stores the x position, y position, length of wall
  in x and y axis and the number of times that the wall can be hitted by the bomb.

  This uses the double linked list data structure for wall_node, each wall_node stores
  one wall struct, a previous pointer pointing to the previous wall_node in the double
  linked list, and a next pointer pointing to the next wall_node in the linked list.

  makeObstacles() function executes saveWall() function which saves wall data in array
  to the wall struct and draws 15 pre-set walls on the screen, also it executes insert_wall()
  to insert these walls into the linked list. The client is going to send all wall structs to
  the sever, and the sever will draw the same walls on the screen and save these walls into the
  linked list. This function runs in linear time in the size of walls in the linked list, so it
  requires O(n) where n is 15.

  draw_walls(wall_node *temp) draws the wall saved in wall_node pointed by temp on the screen.

  ObstacleCheck() function goes through and executes draw_walls() on every wall_node in the linked
  list. This function runs in linear time in the size of wall_node linked list.

  insert_wall(wall Wall) function inserts a new wall_node with Wall value to the tail of the linked
  list, it's running time is constant, O(1).

  remove_wall(wall_node *Unwanted_Wall_Node) function removes the wall_node with Wall whose Walllife
  is 0 from the linked list. This function runs in constant time.
*/

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

struct wall{
  uint8_t x1; //x position for the upper left corner of the wall
  uint8_t y1; //y position for the upper left corner of the wall
  uint8_t length_x; //length of wall in x axis
  uint8_t length_y; //length of wall in y axis
  uint8_t Walllife; //The number of times that the wall can be hit by bomb
};

//The doubly linked list for wall struct
struct wall_node{
  wall Wall;  //Wall data stored in the wall_node
  struct wall_node *next; //Pointer points to the next wall_node in the linked list
  struct wall_node *previous; //Pointer points to the previous wall_node in the linked list
};

extern wall_node *head; //Global wall_node pointer, points to the first wall_node in the linked list
extern wall_node *tail; //Global wall_node pointer, points to the last wall_node in the linked list

/**
  This function lets the client saves one map data out of three map data randomly
  to wall structs saved in the double linked list wall_node. The client is going
  to send all walls' data to the sever, and the sever will draw these walls on the
  screen and save them to the linked list as well.

  Running time: Linear, O(n).
*/
void makeObstacles();

/**
  Save buffer data into wall struct, and insert the wall_node with the wall value to the linked list

  Running time: Linear, O(n).
*/
void saveWall(uint8_t *databuffer);

/**
  Draw the wall in the wall_node pointed by temp according to its Walllife, and executes remove_wall(temp)
  if the Walllife is 0.

  Running Time: Constant, O(1).
*/
void draw_walls(wall_node *temp);

/**
  Go through every wall in the wall_node in the doubly linked list and draw it using draw_walls().

  Running time: Linear, O(n).
*/
void ObstacleCheck();

/**
  Insert wall to the tail of the doubly linked list

  Running time: Constant, O(1).
*/
void insert_wall(wall Wall);

/**
  Remove the unwanted wall from the linked list

  Running time: O(1), Constant
*/
void remove_wall(wall_node *Unwanted_Wall_Node);

#endif
