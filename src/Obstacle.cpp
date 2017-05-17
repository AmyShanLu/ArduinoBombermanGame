#include "ArduinoExtras.h"
#include "ScreenUpdate.h"
#include "Obstacle.h"

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

//Global wall_node pointers head and tail, both are set to NULL.
//Indicating that the doubly linked list is empty at the beginning
wall_node *head = NULL;
wall_node *tail = NULL;

/**
  This function lets the client saves one map data out of three map data randomly
  to wall structs saved in the double linked list wall_node. The client is going
  to send all walls' data to the sever, and the sever will draw these walls on the
  screen and save them to the linked list as well.

  Running time: Linear, O(n).
*/
void makeObstacles() {
  if (digitalRead(13) == 0) { //Client is going to create 15 walls on the screen
    //Preset 3 maps for the game
    int random_map_number = analogRead(3)%3;
    //Randomly load one map
    if(random_map_number == 0){
      //The first map data
      uint8_t databuffer[60] = {20,32,60,4,100,32,20,3,20,37,3,20,40,50,20,5,80,45,30,3,
                                20,65,3,35,35,70,20,2,65,70,15,3,90,70,20,3,115,70,3,30,
                                40,90,3,30,100,90,2,20,20,125,80,5,110,120,4,4,120,120,6,6};
      saveWall(databuffer);
    }
    else if(random_map_number == 1){
      //The second map data
      uint8_t databuffer[60] = {30,20,4,35,94,20,4,35,40,58,20,4,70,58,18,4,60,72,5,10,
                                30,72,4,20,94,72,4,20,50,70,3,40,60,90,2,10,80,70,3,40,
                                60,110,3,10,30,110,4,20,40,130,30,4,80,120,8,4,94,110,3,30};
      saveWall(databuffer);
    }
    else if(random_map_number == 2){
      //The third map data
      uint8_t databuffer[60] = {10,28,40,4,60,28,20,5,90,28,30,4,10,42,3,30,50,42,3,30,
                                80,42,3,30,105,42,4,30,10,80,40,3,80,80,40,3,10,90,3,30,
                                10,130,40,2,70,90,2,30,80,110,6,2,100,90,2,23,100,120,20,3};
      saveWall(databuffer);
    }
  }
  else {  //Sever is going to receive this 15 walls' data
    for(int i=0; i<15;i++){
      while(Serial3.available()<5){}
      wall WallMap;
      WallMap.x1 = Serial3.read();
      WallMap.y1 = Serial3.read();
      WallMap.length_x = Serial3.read();
      WallMap.length_y = Serial3.read();
      WallMap.Walllife = Serial3.read();
      insert_wall(WallMap);  //Insert this wall on the doubly linked list
      tft.fillRect(WallMap.x1, WallMap.y1, WallMap.length_x, WallMap.length_y,ST7735_WHITE);  //Draw this wall on the screen
    }
  }
}

/**
  Save buffer data into wall struct, and insert the wall_node with the wall value to the linked list

  Running time: Linear, O(n).
*/
void saveWall(uint8_t *databuffer){
  for(int i=0;i<15;i++){
    wall WallMap;
    WallMap.x1 = databuffer[i*4];
    WallMap.y1 = databuffer[i*4+1];
    WallMap.length_x = databuffer[i*4+2];
    WallMap.length_y = databuffer[i*4+3];
    WallMap.Walllife = 3;
    insert_wall(WallMap);  //Insert this wall on the doubly linked list
    //Sending this wall information to the server
    Serial3.write(WallMap.x1);
    Serial3.write(WallMap.y1);
    Serial3.write(WallMap.length_x);
    Serial3.write(WallMap.length_y);
    Serial3.write(WallMap.Walllife);
    tft.fillRect(WallMap.x1, WallMap.y1, WallMap.length_x, WallMap.length_y,ST7735_WHITE);  //Draw this wall on the screen
  }
}

/**
  Draw the wall in the wall_node pointed by temp according to its Walllife, and executes remove_wall(temp)
  if the Walllife is 0.

  Running Time: Constant, O(1).
*/
void draw_walls(wall_node *temp){
  if((temp->Wall).Walllife == 0){
    //Redraw the wall with Walllife is 0 in black
    tft.fillRect(temp->Wall.x1,temp->Wall.y1,temp->Wall.length_x,temp->Wall.length_y,ST7735_BLACK);
    //Remove the wall with Walllife is 0 from the doubly linked list
    remove_wall(temp);
  }
  else if((temp->Wall).Walllife == 1){
    //Redraw damaged wall with Walllife equal 1 in magenta
    tft.fillRect(temp->Wall.x1,temp->Wall.y1,temp->Wall.length_x,temp->Wall.length_y,ST7735_MAGENTA);
  }
  else if((temp->Wall).Walllife == 2){
    //Redraw damaged wall with Walllife equal 2 in cyan
    tft.fillRect(temp->Wall.x1,temp->Wall.y1,temp->Wall.length_x,temp->Wall.length_y,ST7735_CYAN);
  }
  else if((temp->Wall).Walllife == 3){
    //Redraw undamaged wall with Walllife equal 3 in white
    tft.fillRect(temp->Wall.x1,temp->Wall.y1,temp->Wall.length_x,temp->Wall.length_y,ST7735_WHITE);
  }
}

/**
  Go through every wall in the wall_node in the doubly linked list and draw it using draw_walls().

  Running time: Linear, O(n).
*/
void ObstacleCheck(){
  if(tail == NULL){/*Doubly linked list is empty*/}
  else{
    struct wall_node *temp = tail;  //Start checking from the tail of the linked list
    while(temp->previous != NULL){
      draw_walls(temp);
      temp = temp->previous;
    }
    temp = head;  //Draw the wall stored in the wall_node pointed by head
    draw_walls(temp);
  }
}

/**
  Insert wall to the tail of the doubly linked list

  Running time: Constant, O(1).
*/
void insert_wall(wall Wall){
  struct wall_node *newNode = new wall_node; //Allocate memory for a wall_node
  assert(newNode != NULL);  //Check if out of memory
  newNode->Wall = Wall; //Store inserted Wall info to Wall in the wall_node pointed by newNode
  newNode->next = NULL; //Set next node pointed by newNode to NULL
  if(head == NULL && tail == NULL){  //When the doubly linked list is empty
    newNode->previous = NULL; //Set newNode's previous to NULL
    head = newNode;
    tail = head; //Set both head and tail pointers to newNode pointer
  }
  else{ //When the linked list is not empty, insert newNode to the tail of the doubly linked list
    newNode->previous = tail; //Set newNode's previous points to tail
    tail->next = newNode; //Set tail's next points to newNode
    tail = newNode; //Set newNode as tail
  }
}

/**
  Remove the unwanted wall from the linked list

  Running time: O(1), Constant
*/
void remove_wall(wall_node *Unwanted_Wall_Node){
  if(Unwanted_Wall_Node == NULL || head == NULL || tail == NULL){/*Doubly linked list is empty*/}
  else if(head != tail){ //More than one node on the linked list
    if(Unwanted_Wall_Node == head){ //Removed node is head of the linked list
      struct wall_node *temp = Unwanted_Wall_Node;
      (Unwanted_Wall_Node->next)->previous = NULL;  //Set removed node's next's previous to NULL
      head = Unwanted_Wall_Node->next;  //Set head as removed node's next
      delete temp;
    }
    else if(Unwanted_Wall_Node == tail){  //Removed node is tail of the linked list
      struct wall_node *temp = Unwanted_Wall_Node;
      (Unwanted_Wall_Node->previous)->next = NULL;  //Set removed node's previous's next to NULL
      tail = Unwanted_Wall_Node->previous;  //Set tail as removed node's previous
      delete temp;
    }
    else{ //Removed node in the middle of the doubly linked list
      struct wall_node *temp = Unwanted_Wall_Node;
      (Unwanted_Wall_Node->previous)->next = Unwanted_Wall_Node->next;  //Set removed node's previous's next to removed node's next
      (Unwanted_Wall_Node->next)->previous = Unwanted_Wall_Node->previous;  //Set removed node's next's previous to removed node's previous
      delete temp;
    }
  }
  else if(head == tail && head != NULL){  //Only one node in the doubly linked list
    head = NULL;
    tail = NULL;  //Set both head and tail to NULL
  }
}
