#include "ArduinoExtras.h"
#include "ScreenUpdate.h"
#include "Gamesetup.h"
#include "Player.h"
#include "Timeremain.h"

#define PLAYER_WIDTH 4
char *gameendtime = new char[5];

//game start time
void InitialTime(){
  gameendtime[0] = '3'; //Initialize time for each round of game
  gameendtime[1] = ':';
  gameendtime[2] = '0';
  gameendtime[3] = '0';
  gameendtime[4] = NULL;
}

//Display the remaining time of the game on the upper left corner of the screen
void DisplayRemainingTime(){
  tft.setCursor(0,0);
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
  tft.setTextSize(1);
  tft.print(gameendtime);
}

//Update the time remaining, decrease the time by 1 second
void UpdateRemainingTime(){
  if(gameendtime[3] == '0'){
    gameendtime[3] = '9';
    gameendtime[2] = ((gameendtime[2] - '0') - 1) + '0';
  }
  else{
    gameendtime[3] = ((gameendtime[3] - '0') - 1) + '0';
  }

  if(gameendtime[2] < '0'){
    gameendtime[2] = '5';
    gameendtime[0] = ((gameendtime[0] - '0') - 1) + '0';
  }

  if(gameendtime[0]=='0' && gameendtime[2] == '0' && gameendtime[3] == '0'){
    gameendtime[3] = '0';
    delete[] gameendtime;
    lose_player = -1;  //No one wins
    gameover(lose_player);  //Show 'Tie!' on the screen, time is up.
  }
}

//Restrict player go through the timing area
void checktimeframe(bool isx){
  uint8_t timeframe_width = 24;
  uint8_t timeframe_height = 8;
  if(Player[0].player_x - PLAYER_WIDTH/2 < timeframe_width){
    if(Player[0].player_y - PLAYER_WIDTH/2 < timeframe_height){
      if(isx == 1){ //Player x position changed
        //Player moves to left and hit the time frame
        Player[0].player_x = timeframe_width + PLAYER_WIDTH/2;
      }
      else{ //Player x position changed
        //Player moves upward and hit the time frame
        Player[0].player_y = timeframe_height + PLAYER_WIDTH/2;
      }
    }
  }
}
