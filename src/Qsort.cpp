#include "Qsort.h"

//Select the middle of the array as pivot
int pick_pivot(winner *EEPROM_Winner, int Num_Winners) {
  return Num_Winners/2;
}

// Swap two Winner data of winner struct
void swap_rest(winner *ptr_winner1, winner *ptr_winner2) {
  winner tmp = *ptr_winner1;
  *ptr_winner1 = *ptr_winner2;
  *ptr_winner2 = tmp;
}

/**
  Rearrange the array, in place, so that all items left of the pivot have value
  less than the pivot and all items to the right of the pivot have value greater
  than or equal to the pivot.

  Running time: Linear, O(n).
*/
int partition(winner* EEPROM_Winner, int Num_Winners, int pivot_idx) {
  //Swap pivot winner with the winner at the end of array
  swap_rest(&EEPROM_Winner[pivot_idx], &EEPROM_Winner[Num_Winners-1]);

  int low = 0;
  int high = Num_Winners -2;
  while(low<high){
    if((EEPROM_Winner[low].scores>=EEPROM_Winner[Num_Winners-1].scores) && (EEPROM_Winner[high].scores<EEPROM_Winner[Num_Winners-1].scores)){
      //Swap the item at low and high, so that all items left of the pivot have value less than the
      //pivot value, and all items to the right of the pivot have greater than or equal to the pivot value
      swap_rest(&EEPROM_Winner[low], &EEPROM_Winner[high]);
    }
    if(EEPROM_Winner[low].scores<EEPROM_Winner[Num_Winners-1].scores){
      //Increment low when the value is less than the pivot value
      low++;
    }
    if(EEPROM_Winner[high].scores>=EEPROM_Winner[Num_Winners-1].scores){
      //Decrement high when the value is greater than or equal to the pivot value
      high--;
    }
  }

  if(high<=low){  //When high is equal or less than low
    if(EEPROM_Winner[high].scores > EEPROM_Winner[Num_Winners-1].scores){
      swap_rest(&EEPROM_Winner[high], &EEPROM_Winner[Num_Winners-1]);
      return high;
    }
    else{
      swap_rest(&EEPROM_Winner[high+1], &EEPROM_Winner[Num_Winners-1]);
      return high+1;
    }
  }
}

/**
  Quick sort
  Sorting winners stored in EEPROM by their scores in ascending order

  Running time: worst case: Quadratic, O(n^2)
                best case: Linearithmic, O(nlogn)
                average case: Linearithmic, O(nlogn)
*/
void qsort(winner* EEPROM_Winner, int Num_Winners) {
  if (Num_Winners <= 1) return; // sorted already
  // choose the pivot
  int pivot_idx = pick_pivot(EEPROM_Winner, Num_Winners);
  // partition around the pivot and get the new pivot position
  pivot_idx = partition(EEPROM_Winner, Num_Winners, pivot_idx);
  // recurse on the halves before and after the pivot
  qsort(EEPROM_Winner, pivot_idx);
  qsort(EEPROM_Winner+pivot_idx + 1, Num_Winners - pivot_idx - 1);
}
