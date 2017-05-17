#include "ArduinoExtras.h"
#include "ScreenUpdate.h"
#include "Bomb.h"

#define BOMB_RADIUS 2
#define Bomb_color ST7735_YELLOW

//Global bomb_node pointers, front and back.
//Set to NULL indicating that the linked list is empty at first
bomb_node *front = NULL;
bomb_node *back = NULL;

/**
  Draw every bomb in bomb_node in the linked list on the screen, starting at the
  bomb in the first bomb_node which is pointed by front.

  Running time: Linear, O(n).
*/
void drawbomb(){
  if(front == NULL){/*The linked list is empty*/}
  else{
    //Starting at the bomb in the bomb_node pointed by front
    struct bomb_node *temp = front;
    //Draw every bomb in the bomb_node in the linked list
    while(temp->next != NULL || temp == back){
      tft.fillCircle(temp->Bomb.bomb_x, temp->Bomb.bomb_y, BOMB_RADIUS, Bomb_color);
      if(temp!=back){
        temp = temp->next;  //Set temp to the bomb_node pointed by temp's next
      }
      else{break;}  //Break when bomb in bomb_node pointed by back is drawn
    }
  }
}

/**
  Input argument: inBomb - the bomb struct with values of the bomb placed by the player;
  Insert a new bomb_node with bomb to the back of the linked list.

  Running time: Constant, O(1).
*/
void insert(bomb inBomb){
  struct bomb_node *newNode = new bomb_node; //Allocate memory for a new bomb_node
  assert(newNode != NULL);  //Check if out of memory
  newNode->Bomb = inBomb; //Store inserted bomb info in bomb_node pointed by newNode
  newNode->next = NULL; //Set next bomb_node pointed by newNode points to NULL
  if(front == NULL){  //When the queue is empty
    front = newNode;
    back = front; //Set both front and back pointers to newNode pointer
  }
  else{ //When the queue is not empty
    back->next = newNode; //Set the back's next pointer points to newNode
    back = newNode; //Assign newNode to back
  }
}

/**
  Remove the first bomb_node which is pointed by front in the linked list.

  Running time: Constant, O(1).
*/
void remove(){
  if(front == NULL){/*The linked list is empty*/}
  else{
    struct bomb_node *temp = front;
    front = front->next;  //Assign the front's next pointer to front
    delete temp; //Delete the bomb_node pointed by temp
  }
}
