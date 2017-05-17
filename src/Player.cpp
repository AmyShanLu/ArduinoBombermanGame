#include "Bomb.h"
#include "Obstacle.h"
#include "ScreenUpdate.h"
#include "Player.h"

#define PLAYER_WIDTH 4
#define BOMB_RADIUS 2
#define Bomb_color ST7735_YELLOW

struct player Player[2];
struct player prev_Player[2];

/**
  When player presses the joystick, place one bomb at player's position and insert it on the queue

  Running time: Constant, O(1).
*/
void placeBomb(player *Player){
  bomb inBomb;  //Create a bomb struct
  inBomb.bomb_x = Player->player_x;  //the x position of bomb
  inBomb.bomb_y = Player->player_y;  //the y position of bomb
  inBomb.explodeTime = millis() + 5000; //explodeTime of bomb, bomb explodes after 5s
  inBomb.bomb_power = Player->bomb_buff;
  tft.fillCircle(inBomb.bomb_x, inBomb.bomb_y, BOMB_RADIUS, Bomb_color);
  insert(inBomb); //Insert the new placed bomb into the queue
  Player->isbomb = 0;  //Change isbomb to 0 after bomb is placed
}

/**
  Input argument: bool isx - Indicate the x position or the y position of the player is changed.

  This function goes through every bomb in the bomb_node in the queue, check if the
  player hits the bomb. If the player hits the bomb, it changes player's position so
  that it can restrict player go through the bomb.

  Running time: Linear, O(n).
*/
void checkisbomb(bool isx){
  if(front != NULL){
    struct bomb_node *temp = front; //Start checking at the bomb in the bomb_node pointed by front
    while(temp->next != NULL || temp == back){  //Go throung all bombs in the queue
      if(abs(temp->Bomb.bomb_x - Player[0].player_x)<=(BOMB_RADIUS+PLAYER_WIDTH/2)){
        if(abs(temp->Bomb.bomb_y - Player[0].player_y)<=(BOMB_RADIUS+PLAYER_WIDTH/2)){
          //The player go through the bomb
          uint8_t player_position;
          uint8_t prev_player_position;
          uint8_t bomb_position;
          if(isx == 0){ //Player's y position changed
            player_position = Player[0].player_y;
            prev_player_position = prev_Player[0].player_y;
            bomb_position = temp->Bomb.bomb_y;
          }
          else if(isx == 1){  //Player's x position changed
            player_position = Player[0].player_x;
            prev_player_position = prev_Player[0].player_x;
            bomb_position = temp->Bomb.bomb_x;
          }

          //Compare player's current position to its previous position, calculate player's new current position
          if(player_position > prev_player_position){
            if(prev_player_position == bomb_position){
              //Player is on the bomb, let player go to right
              player_position = bomb_position + BOMB_RADIUS + PLAYER_WIDTH/2;
            }
            else{
              //Player moves downward (when isx == 1) or moves right (when isx == 0) and hits the bomb
              player_position = bomb_position - BOMB_RADIUS - PLAYER_WIDTH/2;
            }
          }
          else if(player_position < prev_player_position){
            if(prev_player_position == bomb_position){
              //Player is on the bomb, let cursor go to left
              player_position = bomb_position - BOMB_RADIUS - PLAYER_WIDTH/2;
            }
            else{
              //Player moves upward (when isx == 1) or moves left (when isx == 0) and hits the bomb
              player_position = bomb_position + BOMB_RADIUS + PLAYER_WIDTH/2;
            }
          }

          if(isx == 0){ //Restore player's y position
            Player[0].player_y = player_position;
          }
          else if(isx == 1){  //Restore player's x position
            Player[0].player_x = player_position;
          }
        }
      }

      if(temp != back){
        temp = temp->next;  //Check the next bomb_node in the queue
      }
      else{break;}
    }
  }
}

/**
  Input argument: bool isx - Indicate the x position or the y position of the player is changed.

  This function goes through every wall in the wall in the linked list, check if the
  player hits the wall. If the player hits the wall, it changes player's position so
  that it can restrict player go through the wall.

  Running time: Linear, O(n).
*/
void CheckHitWall(bool isx) {
  if(tail == NULL){/*link list is empty, no walls on the screen*/}
  else{
    struct wall_node *temp = tail;  //Start checking from the tail of the linked list
    while(temp->previous != NULL || temp == head){
      if (Player[0].player_y >= temp->Wall.y1 - PLAYER_WIDTH/2 && Player[0].player_y <= temp->Wall.y1 + temp->Wall.length_y + PLAYER_WIDTH/2) {
        if (Player[0].player_x >= temp->Wall.x1 - PLAYER_WIDTH/2 && Player[0].player_x <= temp->Wall.x1 + temp->Wall.length_x + PLAYER_WIDTH/2) {
          //The player goes through the wall
          uint8_t wallposition;
          uint8_t walllength;
          uint8_t playerposition;
          uint8_t prev_playerposition;
          if (isx == 1) { //Player's x position change
            playerposition = Player[0].player_x;
            prev_playerposition = prev_Player[0].player_x;
            wallposition = temp->Wall.x1;
            walllength = temp->Wall.length_x;
          }
          else {  //Player's y position change
            playerposition = Player[0].player_y;
            prev_playerposition = prev_Player[0].player_y;
            wallposition = temp->Wall.y1;
            walllength = temp->Wall.length_y;
          }

          //Compare player's current position to its previous position, calculate player's new current position
          if (playerposition > prev_playerposition) {
            playerposition = wallposition - PLAYER_WIDTH/2 - 1; //Restrict the player go through the wall
          }
          else if (playerposition < prev_playerposition) {
            playerposition = wallposition + walllength + PLAYER_WIDTH/2 + 1;  //Restrict the player go through the wall
          }

          if (isx == 1) { //Restore player's x position
            Player[0].player_x = playerposition;
          }
          else if (isx == 0) {  //Restore player's y position
            Player[0].player_y = playerposition;
          }
        }
      }

      if(temp!=head){
        temp = temp->previous;  //Check the previous wall_node in the double linked list
      }
      else{break;}
    }
  }
}
