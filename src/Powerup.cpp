#include "Player.h"
#include "Powerup.h"
#include "Gamesetup.h"
#include "ScreenUpdate.h"

#define TFT_WIDTH 124
#define TFT_HEIGHT 160

struct Spawnpoint Powerup[4];

/**
  This functions activates and draws the powerup spawn in the clockwise

  Running time: Linear, O(n).
*/
void SpawnPowerup(int spawn) {
  int i = 0;
  while (i < 4) {
    if (Powerup[(spawn+i)%4].active == false) {
      Powerup[(spawn+i)%4].active = true;
      tft.fillCircle(Powerup[(spawn+i)%4].spawn_x, Powerup[(spawn+i)%4].spawn_y, 2, ST7735_GREEN);
      break;
    }
    i++;
  }
}

/**
  This function checks if the player picks up the powerup spawn point. If the player picks up
  the powerup, it increases the bomb power of the player by 1.

  This function runs in quadratic time, O(n^2).
*/
void PowerupBuffCheck() {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      if (Player[i].player_y >= Powerup[j].spawn_y - 4 && Player[i].player_y <= Powerup[j].spawn_y + 4 & Powerup[j].active == true) {
        if (Player[i].player_x >= Powerup[j].spawn_x - 4 && Player[i].player_x <= Powerup[j].spawn_x + 4 & Powerup[j].active == true) {
          //Player picks up the powerup
          tft.fillCircle(Powerup[j].spawn_x, Powerup[j].spawn_y, 2, ST7735_BLACK);
          Powerup[j].active = false;
          Player[i].bomb_buff ++;
        }
      }
    }
  }
}

/**
  This function draws the activated powerup spawn on the screen.

  Running time: Linear, O(n).
*/
void UpdatePowerup() {
  for (int i = 0; i < 4; i++) {
    if (Powerup[i].active == true) {
      tft.fillCircle(Powerup[i].spawn_x, Powerup[i].spawn_y, 2, ST7735_GREEN);
    }
  }
}
