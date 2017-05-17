/**
  Name: William Wong
        Shan Lu
  Section: A2
*/

#include "Gamesetup.h"
#include "ScreenUpdate.h"
#include "Player.h"
#include "Bomb.h"
#include "Explode.h"
#include "Powerup.h"
#include "Timeremain.h"
#include "HighScoreTable.h"

#define buttonPin 2
#define JOY_SEL 9
#define MILLIS_PER_FRAME 50

int main(){
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  while(true){
    setup();
    int startTime = millis();
    unsigned long UpdateTime = millis();
    unsigned long PwrUpSpwnTme = millis();
    endgame = 0;
    int spawn = 1;
    while(true){
      if(digitalRead(buttonPin) == LOW){  //Button is pressed, clear EEPROM
        Clear_EEPROM();
      }

      if (millis() - PwrUpSpwnTme >= 10000) { //Draw the next spawn Powerup points in 10s
        SpawnPowerup(spawn);
        spawn = (spawn + 1)%4;
        PwrUpSpwnTme = millis();
      }

      if(endgame == 1){
        //Print the high score table
        DisplayHighScores();
        while(digitalRead(JOY_SEL) == true){
          if(digitalRead(buttonPin) == LOW){  //Button is pressed, clear EEPROM
            Clear_EEPROM();
          }
        }  //Not press joystick
        break;
      }

      scanJoystick(); //Scan joystick

      if(Serial3.available()>=3){ //If receive the other arduino's player_x and y and isbomb
        //Update screen for the other player
        Player[1].player_x = Serial3.read();
        Player[1].player_y = Serial3.read();
        Player[1].isbomb = Serial3.read();
        if(Player[1].isbomb == 1){
          placeBomb(Player+1);
        }
      }

      PowerupBuffCheck(); //Checks if the player picks up the powerup

      updateDisplay();  //Update the screen and two player's position

      drawbomb(); //Draw the remaining bombs on the screens

      //Check whether the bomb in the bomb_node pointed by front is about to explode
      if(front != NULL){  //When there are bomb on the queue
        if( millis() >= front->Bomb.explodeTime){
          explodeBomb();  //If millis > front bomb's explodeTime, explode bomb
        }
      }

      //Decrease 1s on the timing board
      if(UpdateTime+1000<=millis()){
        UpdateRemainingTime();  //Decrease the previous time by 1 second
        DisplayRemainingTime();
        UpdateTime = millis();  //The new time
      }

      //create a variable delay, so that each loop iteration takes
      //MILLIS_PER_FRAME milliseconds
      int curTime = millis();
      if((curTime-startTime) < MILLIS_PER_FRAME){
        delay(MILLIS_PER_FRAME - (curTime-startTime));
      }
      startTime = millis();
    }
  }

  Serial3.end();
  Serial.end();
  return 0;
}
