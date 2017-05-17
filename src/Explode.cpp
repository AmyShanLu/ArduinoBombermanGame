#include "Bomb.h"
#include "Obstacle.h"
#include "ScreenUpdate.h"
#include "Player.h"
#include "Gamesetup.h"
#include "Powerup.h"
#include "Explode.h"

#define PLAYER_WIDTH 4
#define BOMB_RADIUS 2
#define TFT_WIDTH 128
#define TFT_HEIGHT 160
#define Bomb_color ST7735_YELLOW

uint8_t hitcount = 0;

/**
  This function explodes the bomb whose exploding time is less than the current
  time. It shows the exploding area of the bomb by 100 ms in the screen.

  Running time: Linear, O(n).
*/
void explodeBomb(){
  //Each bomb can explode a horizontal and a vertical rectangle according to its power.
  uint8_t *vertical_x0, temp_vertical_x0; //x0 position of the vertical exploded rectangle
  uint8_t *vertical_y0, temp_vertical_y0; //y0 position of the vertical exploded rectangle
  uint8_t *horizontal_x0, temp_horizontal_x0; //x0 position of the horizontal exploded rectangle
  uint8_t *horizontal_y0, temp_horizontal_y0; //y0 position of the horizontal exploded rectangle
  uint8_t *vertical_width, temp_vertical_width; //width of the vertical exploded rectangle
  uint8_t *vertical_height, temp_vertical_height; //height pf the vertical exploded rectangle
  uint8_t *horizontal_width, temp_horizontal_width;  //width of the horizontal exploded rectangle
  uint8_t *horizontal_height, temp_horizontal_height; //height of the horizontal exploded rectangle

  //The x0 and y0 position of the exploding rectangle cannot exceed the screen
  temp_vertical_x0 = constrain(front->Bomb.bomb_x - BOMB_RADIUS, 0, TFT_WIDTH-(2*BOMB_RADIUS));
  vertical_x0 = &temp_vertical_x0;
  //y0 position for the vertical exploded rectangle
  temp_vertical_y0 = constrain(front->Bomb.bomb_y - ((1+2*(front->Bomb.bomb_power))*BOMB_RADIUS), 0, TFT_HEIGHT - (4*BOMB_RADIUS));
  vertical_y0 = &temp_vertical_y0;
  //x0 position for the horizontal exploded rectangle
  temp_horizontal_x0 = constrain(front->Bomb.bomb_x - ((1+2*(front->Bomb.bomb_power))*BOMB_RADIUS), 0, TFT_WIDTH - (4*BOMB_RADIUS));
  horizontal_x0 = &temp_horizontal_x0;
  //y0 position for the horizontal exploded rectangle
  temp_horizontal_y0 = constrain(front->Bomb.bomb_y - BOMB_RADIUS, 0, TFT_HEIGHT - (2*BOMB_RADIUS));
  horizontal_y0 = &temp_horizontal_y0;
  temp_vertical_width = 2*BOMB_RADIUS; //width for the vertial exploded rectangle
  vertical_width = &temp_vertical_width;
  temp_horizontal_height = 2*BOMB_RADIUS;  //height for the horizontal exploded rectangle
  horizontal_height = &temp_horizontal_height;

  //Calculate the exploded area for vertical rectangle
  if(*vertical_y0 == 0){
    temp_vertical_height = (1+2*front->Bomb.bomb_power)*BOMB_RADIUS + front->Bomb.bomb_y;
    vertical_height = &temp_vertical_height;
  }
  else{
    if((*vertical_y0 + 2*(1+2*front->Bomb.bomb_power)*BOMB_RADIUS) > TFT_HEIGHT){
      temp_vertical_height = TFT_HEIGHT - *vertical_y0;
      vertical_height = &temp_vertical_height;
    }
    else{
      temp_vertical_height = 2*(1+2*front->Bomb.bomb_power)*BOMB_RADIUS;
      vertical_height = &temp_vertical_height;
    }
  }

  //Calculate the exploded area for horizontal rectangle
  if(*horizontal_x0 == 0){
    temp_horizontal_width = (1+2*front->Bomb.bomb_power)*BOMB_RADIUS + front->Bomb.bomb_x;
    horizontal_width = &temp_horizontal_width;
  }
  else{
    if((*horizontal_x0 + 2*(1+2*front->Bomb.bomb_power)*BOMB_RADIUS) > TFT_WIDTH){
      temp_horizontal_width = TFT_WIDTH - *horizontal_x0;
      horizontal_width = &temp_horizontal_width;
    }
    else{
      temp_horizontal_width = 2*(1+2*front->Bomb.bomb_power)*BOMB_RADIUS;
      horizontal_width = &temp_horizontal_width;
    }
  }

  //Go through all walls, check if there're walls in the exploding area
  CheckExplodeWall(vertical_x0, vertical_y0, vertical_width, vertical_height, 0); //Check the vertical exploding area
  CheckExplodeWall(horizontal_x0, horizontal_y0, horizontal_width, horizontal_height, 1); //Check the horizontal exploding area

  //Go through all walls, check if the walls' edge overlaps the exploding rectangle
  UpdateWalllife(vertical_x0, vertical_y0, vertical_width, vertical_height, 0); //Check the vertical rectangle
  UpdateWalllife(horizontal_x0, horizontal_y0, horizontal_width, horizontal_height, 1); //Check the horizontal rectangle

  unsigned long explodedtime = millis();  //The time that bomb exploded
  unsigned long dispeartime = explodedtime + 100; //The time that the bomb explosion effect would dispear

  //Draw the vertical and horizontal exploding rectangle
  tft.fillRect(*vertical_x0, *vertical_y0, *vertical_width, *vertical_height, Bomb_color);
  tft.fillRect(*horizontal_x0, *horizontal_y0, *horizontal_width, *horizontal_height, Bomb_color);

  int *ishit; //Whether the player is hitted by the bomb
  (*ishit) = 0; //Initialize to 0.
  checkexplodedarea(*vertical_x0, *vertical_y0, *vertical_width, *vertical_height, ishit);  //Check the vertical exploding area
  checkexplodedarea(*horizontal_x0, *horizontal_y0, *horizontal_width, *horizontal_height, ishit);  //Check the horizontal exploding area
  int a = (*ishit);
  if(a == 1){ //Game over if (*ishit) is 1
    gameover(lose_player);
  }

  while(millis()<dispeartime){/*Keep the explosion effect (100ms)*/}
  //Redraw the exploded area with the background color
  tft.fillRect(*vertical_x0, *vertical_y0, *vertical_width, *vertical_height, ST7735_BLACK);
  tft.fillRect(*horizontal_x0, *horizontal_y0, *horizontal_width, *horizontal_height, ST7735_BLACK);

  //Since the bomb pointed by the front pointer in the linked list has the smallest explodeTime
  //this bomb has to explode first
  remove(); //Remove the front bomb from the queue
  drawbomb(); //Draw the remaining bombs, so that other bombs in the exploding area won't dispear

  //Go through all walls, change its color or remove it from the screen according to its Walllife
  ObstacleCheck();

  //Draw activated power up on the screen, so that the power up in the exploding area won't dispear
  UpdatePowerup();
}

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
void CheckExplodeWall(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx){
  if(tail == NULL){/*Empty linked list*/}
  else{
    struct wall_node *temp = tail;  //Start checking from the tail of the linked list
    //Go through every wall_node in the linked list
    while(temp->previous != NULL || temp == head){
      //The wall is in the exploding area of the bomb
      if(temp->Wall.x1 + temp->Wall.length_x >= *x0 && temp->Wall.x1 <= *x0 + *width){
        if (temp->Wall.y1 + temp->Wall.length_y >= *y0 && temp->Wall.y1 <= *y0 + *height) {
          if (isx == 0) {
            //check for vertical rectangle, recalculate height or y position of the exploded area
            if (front->Bomb.bomb_y < temp->Wall.y1) {
              //if bomb is above Wall
              *height = temp->Wall.y1 - *y0;
            }
            else if (front->Bomb.bomb_y > (temp->Wall.y1 + temp->Wall.length_y+BOMB_RADIUS)) {
              //if bomb is below Wall
              *height = *height - (temp->Wall.y1 + temp->Wall.length_y - *y0);
              *y0 = temp->Wall.y1 + temp->Wall.length_y;
            }
          }
          else if (isx == 1) {
            //check for horizontal rectangle, recalculate width or x position of the exploded area
            if (front->Bomb.bomb_x<temp->Wall.x1) {
              //if bomb is on the left hand side of the Wall
              *width = temp->Wall.x1 - *x0;
            }
            else if (front->Bomb.bomb_x>=(temp->Wall.x1+temp->Wall.length_x+BOMB_RADIUS)) {
              //if bomb is on the right hand side of the Wall
              *width = *width - (temp->Wall.x1 + temp->Wall.length_x - *x0);
              *x0 = temp->Wall.x1 + temp->Wall.length_x;
            }
          }
        }
      }
      if(temp!=head){
        temp = temp->previous;
      }
      else{break;}
    }
  }
}

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
void UpdateWalllife(uint8_t *x0, uint8_t *y0, uint8_t *width, uint8_t *height, bool isx){
  if(tail == NULL){/*Empty linked list*/}
  else{
    struct wall_node *temp = tail;  //Start checking from the tail of the linked list
    //Go through every wall_node in the linked list
    while(temp->previous != NULL || temp == head){
      if(temp->Wall.x1 + temp->Wall.length_x >= *x0 && temp->Wall.x1 <= *x0 + *width){
        if (temp->Wall.y1 + temp->Wall.length_y >= *y0 && temp->Wall.y1 <= *y0 + *height) {
          if(isx==0){ //Check vertical exploded rectangle
            if(temp->Wall.y1 + temp->Wall.length_y == *y0){
              temp->Wall.Walllife--;
            }
            else if(temp->Wall.y1 == (*y0) + (*height)){
              temp->Wall.Walllife--;
            }
          }
          else if(isx == 1){  //Check horizontal exploded rectangle
            if(temp->Wall.x1 + temp->Wall.length_x == *x0){
              temp->Wall.Walllife--;
            }
            else if(temp->Wall.x1 == (*x0) + (*width)){
              temp->Wall.Walllife--;
            }
          }
        }
      }
      if(temp!=head){
        temp = temp->previous;
      }
      else{break;}
    }
  }
}

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
void checkexplodedarea(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, int *ishit){
  hitcount = 0;
  //Check the players position
  for(int i=0; i<2; i++){
    if(Player[i].player_x>=x0-(PLAYER_WIDTH/2) && Player[i].player_x<= x0+width+(PLAYER_WIDTH/2)){
      if(Player[i].player_y>=y0-(PLAYER_WIDTH/2) && Player[i].player_y<= y0+height+(PLAYER_WIDTH/2)){
        //Player was hit by the bomb
        lose_player = i;  //The player loses the game
        (*ishit) = 1;
        hitcount++; //One player was hitted by the bomb
      }
    }
  }
  if(hitcount == 2){  //Both players were hitted by the bomb, Tie
    lose_player = -1;
    gameover(lose_player);
  }
}
