/**
  HighScoreTable.h

  This declares a winner struct which stores thw winner's name and score. And the
  global pointer EEPROM_Winner points to the array of winner stored in EEPROM.

  CalculateScores(unsigned long GameTime) takes the time that the game uses as the
  input argument and returns the score of the winner as an unsigned long.

  UpdateEEPROM() saves the new winner infomation of the game in EEPROM.

  Read_Winner_EEPROM() reads and saves winner data stored in EEPROM to an array which
  is pointed by EEPROM_Winner, and it executed qsort() to sort the winners in EEPROM_Winner
  in ascending order according to their scores. Then it saves the sorted EEPROM_Winner
  array in EEPROM.

  Clear_EEPROM() sets the first byte stored in EEPROM, which indicates the number of winner
  struct stored in EEPROM, to 0. It clears the High Score Table.
*/

#ifndef HIGHSCORETABLE_H
#define HIGHSCORETABLE_H

#include <EEPROM.h>
#include <Arduino.h>

struct winner{
  char name[9]; //The winner's name
  unsigned long scores; //The winner's score
};

extern struct winner *EEPROM_Winner;  //Point to the array of winner stored in EEPROM

/**
  This function takes GameTime as input argument, and it returns an unsigned long
  scores.
*/
unsigned long CalculateScores(unsigned long GameTime);

/**
  This function saves winner's name and scores to a new winner struct Winner. It saves this
  Winner to the end of EEPROM when the first byte stored in EEPROM is less than 30. Since
  this function only saves 30 winners infomation with highest scores in EEPROM, so when the
  number of winners stored in EEPROM is greater than or equal to 30, it compares the winner's
  score stored in the first cell in EEPROM to the new winner's score, and store the winner with
  higher score in EEPROM's first cell for winner.
*/
void UpdateEEPROM();

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
void Read_Winner_EEPROM();

/**
  Display High Score Table on the screen, print the 15 winners' data with highest
  scores in the high score table.
*/
void DisplayHighScores();

/**
  This function sets the first byte stored in EEPROM, which indicates the number
  of winners stored in EEPROM, to 0. It clears the High Score Table.
*/
void Clear_EEPROM();

#endif
