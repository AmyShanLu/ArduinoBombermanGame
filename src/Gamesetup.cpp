#include "ScreenUpdate.h"
#include "Communication.h"
#include "Player.h"
#include "Obstacle.h"
#include "Timeremain.h"
#include "HighScoreTable.h"
#include "Powerup.h"
#include "Bomb.h"
#include "Gamesetup.h"

#define TFT_WIDTH 124
#define TFT_HEIGHT 160
#define buttonPin 2
#define JOY_SEL 9
#define JOY_VERT_ANALOG 0

uint16_t p1_color;
uint16_t p2_color;
int8_t lose_player; //Indicate which player loses the game
bool endgame = 0; //Indicate whether the game ends
char Player_name[2][9];
unsigned long GameStartTime;

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
void setup(){
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735R chip, black tab
  tft.fillScreen(ST7735_BLACK);

  pinMode(JOY_SEL, INPUT);
  digitalWrite(JOY_SEL, HIGH);
  pinMode(13, INPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);

  int checkclient = digitalRead(13); //Read digital port 13 value
  if(checkclient == LOW){  //Act as client
    bool startgame = client(); //when client is ready to start game
    //Initialize two players' positions and colors for the client
    Player[0].player_x = 64;
    Player[0].player_y = 20;
    prev_Player[0].player_x = 64;
    prev_Player[0].player_y = 20;
    Player[0].isable = 1;
    Player[1].player_x = 64;
    Player[1].player_y = 140;
    prev_Player[1].player_x = 64;
    prev_Player[1].player_y = 140;
    p1_color = ST7735_RED;
    p2_color = ST7735_BLUE;

    gamestart();  //Start game
  }
  else if(checkclient == HIGH){  //Act as sever
    bool startgame = sever(); //when sever is ready to start game
    //Initialize two players' positions and colors for the sever
    Player[0].player_x = 64;
    Player[0].player_y = 140;
    prev_Player[0].player_x = 64;
    prev_Player[0].player_y = 140;
    Player[0].isable = 1;
    Player[1].player_x = 64;
    Player[1].player_y = 20;
    prev_Player[1].player_x = 64;
    prev_Player[1].player_y = 20;
    p1_color = ST7735_BLUE;
    p2_color = ST7735_RED;

    gamestart();  //Start game
  }

  makeObstacles();  //Draw pre-set walls on the screen and insert them in the linked list

  GameStartTime = millis(); //Game start time
  InitialTime();  //Initialize time to 3:00
  DisplayRemainingTime();  //Display the remaining time on the screen

  Player[0].bomb_buff = 1;  //Initially, both player's bomb power is 1
  Player[1].bomb_buff = 1;

  //Set the positions for 4 power-up
  Powerup[0].spawn_x = TFT_WIDTH/4;
  Powerup[0].spawn_y = TFT_HEIGHT/4 + TFT_HEIGHT/8;
  Powerup[0].active = false;
  Powerup[1].spawn_x = 3*TFT_WIDTH/4;
  Powerup[1].spawn_y = TFT_HEIGHT/4 + TFT_HEIGHT/8;
  Powerup[1].active = false;
  Powerup[2].spawn_x = TFT_WIDTH/4;
  Powerup[2].spawn_y = TFT_HEIGHT/4 + 3*TFT_HEIGHT/8;
  Powerup[2].active = false;
  Powerup[3].spawn_x = 3*TFT_WIDTH/4;
  Powerup[3].spawn_y = TFT_HEIGHT/4 + 3*TFT_HEIGHT/8;
  Powerup[3].active = false;
}

/**
  Show 'Game Starts' on the screen, and let players enter their names.
*/
void gamestart(){
  tft.setCursor(0, 70); // where the characters will be displayed
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println("Game Starts");
  delay(1000);
  tft.fillScreen(ST7735_BLACK);
  PlayerInputName();  //Let player enters name
}

/**
  Save two players name into Player_name array. And the player's name cannot exceed
  8 characters.
*/
void PlayerInputName(){
  tft.setCursor(0,20);
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("Your name: ");
  Serial.print("Your name: ");
  tft.setCursor(40, 28);
  uint8_t this_name_length = 0; //The length of the name
  uint8_t other_name_length = 0;
  bool this_end = 0;  //Indicate whether the name is saved
  bool other_end = 0;

  while(true){
    if(Serial.available()>0 && this_end ==0){
      char c = Serial.read();
      if((int) c == 13){  //Receive a carriage return
        Serial3.write(c); //Sending to the other arduino
        Player_name[0][++this_name_length] = 0;
        this_end = 1; //This player's name saved
      }
      else{
        Player_name[0][this_name_length] = c;
        Serial3.write(c); //Sending to the other arduino
        tft.print(c); //Print on lcd
        Serial.print(c);  //Print on serial monitor
        if(++this_name_length>=8){  //Name length maximum 8 characters
          Player_name[0][this_name_length] = 0;
          this_end = 1; //This player's name saved
        }
      }
    }

    if(Serial3.available()>0 && other_end == 0){
      char c = Serial3.read();
      if((int) c == 13){  //Receive a carriage return
        Player_name[1][++other_name_length] = 0;
        other_end = 1;  //Other player's name saved
      }
      else{
        Player_name[1][other_name_length] = c;  //Save other player's name
        if(++other_name_length>=8){ //Name length maximum 8 characters
          Player_name[1][other_name_length] = 0;
          other_end = 1;  //Other player's name saved
        }
      }
    }

    //Break when both players' name are saved
    if(this_end == 1 && other_end == 1){break;}
  }

  Serial.println("");
  Serial.print("Hi, ");
  Serial.print(Player_name[0]);
  Serial.print(". ");
  Serial.print("You are playing against ");
  Serial.print(Player_name[1]);
  Serial.println(".");
  Serial.println("Game Starts!");
  tft.fillScreen(ST7735_BLACK);
}

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
void gameover(int8_t lose_player){
  tft.setCursor(0, 70); // where the characters will be displayed
  tft.setTextWrap(false);
  tft.setTextSize(2);
  if(lose_player == 0){ //This arduino loses
    tft.setTextColor(ST7735_GREEN);
    tft.println("You Lose.");
    UpdateEEPROM(); //Save winner data in EEPROM
  }
  else if(lose_player == 1){  //This arduino wins
    tft.setTextColor(ST7735_RED);
    tft.println("You Win!");
    UpdateEEPROM(); //Save winner data in EEPROM
  }
  else if(lose_player == -1){  //No one wins
    tft.setTextColor(ST7735_GREEN);
    tft.println("Tie!");
  }
  delay(500);
  ClearLastgame();  //Clear data from last game
  endgame = 1;  //Game ended
}

/**
  This function clears the remaining bombs and walls in the linked lists bomb_node
  and wall_node, and two players' name in the array. So that the next round will not
  be affected by the data from the last round.

  Running time: Linear, O(n).
*/
void ClearLastgame(){
  //Delete remaining bomb on the queue
  while(front != NULL){
    remove();
  }
  //Delete remaining walls in the linked list
  while(head != NULL){
    remove_wall(head);
  }
  //Clear player's name
  for(int i=0;i<9;i++){
    Player_name[0][i] = 0;
    Player_name[1][i] = 0;
  }
}
