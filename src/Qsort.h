/**
  Qsort.h

  This declares the functions for the quick sort algorithm.

  The worst running time case for the Quicksort is quadratic, O(n^2). When the
  Quicksort is performed on a sorted array.
  The best case and the average case for the Quicksort is linearithmic, O(nlogn).
*/

#ifndef QSORT_H
#define QSORT_H

#include <Arduino.h>
#include "Gamesetup.h"
#include "HighScoreTable.h"

//Select the middle of the array as pivot
int pick_pivot(winner *EEPROM_Winner, int Num_Winners);

// Swap two Winner data of winner struct
void swap_rest(winner* ptr_winner1, winner* ptr_winner2);

/**
  Rearrange the array, in place, so that all items left of the pivot have value
  at most the pivot and all items to the right of the pivot have greater than the pivot.

  Running time: Linear, O(n).s
*/
int partition(winner* EEPROM_Winner, int Num_Winners, int pivot_idx);

/**
  Quick sort
  Sorting winners stored in EEPROM by their scores in ascending order

  Running time: worst case: Quadratic, O(n^2)
                best case: Linearithmic, O(nlogn)
                average case: Linearithmic, O(nlogn)
*/
void qsort(winner* EEPROM_Winner, int Num_Winners);

#endif
