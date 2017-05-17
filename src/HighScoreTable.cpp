#include <string.h>
#include "Gamesetup.h"
#include "ScreenUpdate.h"
#include "Qsort.h"
#include "HighScoreTable.h"

/**
  This function takes GameTime as input argument, and it returns an unsigned long
  scores.
*/
unsigned long CalculateScores(unsigned long GameTime){
  unsigned long totalTime = 180000; //3min = 180000ms
  unsigned long scores = totalTime - GameTime;  //Scores is calculated by how much time used
  return scores;
}

/**
  This function saves winner's name and scores to a new winner struct Winner. It saves this
  Winner to the end of EEPROM when the first byte stored in EEPROM is less than 30. Since
  this function only saves 30 winners infomation with highest scores in EEPROM, so when the
  number of winners stored in EEPROM is greater than or equal to 30, it compares the winner's
  score stored in the first cell in EEPROM to the new winner's score, and store the winner with
  higher score in EEPROM's first cell for winner.
*/
void UpdateEEPROM(){
  winner Winner;
  //Store new winner's data
  unsigned long GameTime = millis() - GameStartTime;
  Winner.scores = CalculateScores(GameTime);
  memcpy(Winner.name, Player_name[1-lose_player], 9);
  uint8_t Num_Winners;
  EEPROM.get(0, Num_Winners); //Get the number of winners stored in EEPROM
  if(Num_Winners < 30){  //Store at most 30 winners data in EEPROM
    //Store the new winner data in EEPROM
    EEPROM.put(1+(Num_Winners*sizeof(winner)), Winner);
    Num_Winners++;
    EEPROM.put(0, Num_Winners); //Update the number of winners data stored in EEPROM
    Read_Winner_EEPROM();
  }
  else if(Num_Winners >= 30){
    winner first_Winner; //Store the first winner in the EEPROM with the smallest scores
    EEPROM.put(1, first_Winner);
    if(first_Winner.scores <= Winner.scores){
      //Store winner data into the first cell in EEPROM
      EEPROM.put(1, Winner);
      Read_Winner_EEPROM();
    }
  }
}

/**
  This function saves the winners stored in EEPROM to a dynamic array EEPROM_Winner,
  and it executes qsort(EEPROM_Winner, Num_Winners) which takes the pointer to this
  array and the size of EEPROM_Winner array as input arguments and sorts the array by
  winners' scores in ascending order. Then it stores the sorted winners array in EEPROM
  and deletes the EEPROM_Winner array.

  The worst case of the running time is quadratic, O(n^2), when the EEPROM_Winner array
  is already sorted.
  The best case of the running time is linearithmic, O(nlogn).
  The average case of the running time is linearithmic, O(nlogn).
*/
void Read_Winner_EEPROM(){
  uint8_t Num_Winners;
  EEPROM.get(0, Num_Winners); //The number of winners stored in EEPROM
  winner *EEPROM_Winner = new winner[Num_Winners];
  for(int i=0; i<Num_Winners; i++){
    EEPROM.get(1+(i*sizeof(winner)), EEPROM_Winner[i]);
  }

  qsort(EEPROM_Winner, Num_Winners);  //Sort EEPROM_Winner array using quick sort

  //Store the sorted winner array in EEPROM, by scores in ascending order
  for(int i=0; i<Num_Winners; i++){
    EEPROM.put(1+(i*sizeof(winner)), EEPROM_Winner[i]);
  }
  delete[] EEPROM_Winner;
}

/**
  Display High Score Table on the screen, print the 15 winners' data with highest
  scores in the high score table.
*/
void DisplayHighScores(){
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(50,0);
  tft.setTextWrap(false);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.print("High Scores");
  tft.setCursor(20,8);
  tft.print("Name");
  tft.setCursor(60,8);
  tft.print("Scores");

  uint8_t Num_Winners;
  EEPROM.get(0, Num_Winners);
  //Print 15 winner with highest scores stored in EEPROM
  int i=0;
  while(Num_Winners-1-i>=0){  //Starts from the last winner stored in EEPROM
    winner Winner;
    EEPROM.get(1+((Num_Winners-1-i)*sizeof(winner)),Winner);
    tft.setCursor(5,16+8*i);
    tft.print(Winner.name);
    tft.setCursor(55,16+8*i);
    tft.print(Winner.scores);
    i++;
    if(i>=15){break;}
  }

  tft.setCursor(5,144);
  tft.print("Press Joystick to ");
  tft.setCursor(40, 152);
  tft.print("'Start'");
}

/**
  This function sets the first byte stored in EEPROM, which indicates the number
  of winners stored in EEPROM, to 0. It clears the High Score Table.
*/
void Clear_EEPROM(){
  EEPROM.write(0,0);  //No winner stored in EEPROM
}
