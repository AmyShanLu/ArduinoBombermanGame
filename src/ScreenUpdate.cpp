#include "Player.h"
#include "Gamesetup.h"
#include "Timeremain.h"
#include "ScreenUpdate.h"

// standard U of A library settings, assuming Atmel Mega SPI pins
#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

#define JOY_SEL 9
#define JOY_VERT_ANALOG 0
#define JOY_HORIZ_ANALOG 1
#define JOY_DEADZONE 64
#define JOY_CENTRE 512
#define JOY_STEPS_PER_PIXEL 64

#define PLAYER_WIDTH 4
#define TFT_WIDTH 128
#define TFT_HEIGHT 160

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
unsigned long timeBombplaced = 0;

//Scan joystick
void scanJoystick() {
  int v = analogRead(JOY_VERT_ANALOG);
  int h = analogRead(JOY_HORIZ_ANALOG);
  bool invselect = digitalRead(JOY_SEL);

  if (abs(v - JOY_CENTRE) > JOY_DEADZONE) {
    //outside of deadzone
    int delta = (v - JOY_CENTRE) / JOY_STEPS_PER_PIXEL;
    Player[0].player_y = constrain(Player[0].player_y + delta, PLAYER_WIDTH/2, TFT_HEIGHT - PLAYER_WIDTH/2);
    //The y position of the player
    checktimeframe(0);  //Restrict the player go through the time frame
    checkisbomb(0);  //Restrict the player go through the bombs
    CheckHitWall(0);  //Restrict the player go through the walls

    Player[0].placechanged = 1;
  }
  if (abs(h - JOY_CENTRE) > JOY_DEADZONE) {
    //outside of deadzone
    int delta = (h - JOY_CENTRE) / JOY_STEPS_PER_PIXEL;
    Player[0].player_x = constrain(Player[0].player_x + delta, PLAYER_WIDTH/2, TFT_WIDTH - PLAYER_WIDTH/2);
    //The x position of the player
    checktimeframe(1);  //Restrict the player go through the time frame
    checkisbomb(1); //Restrict the player go through the bombs
    CheckHitWall(1);  //Restrict the player go through the walls

    Player[0].placechanged = 1;
  }

  //When the joystick is pressed, set isbomb to 1, indicating that the player placed a bomb
  if (invselect == 0 && Player[0].isable == 1) {
    Player[0].isbomb = 1;
    timeBombplaced = millis();
    Player[0].isable = 0;
    Player[0].placechanged = 1;
  }
  //delay a little, so the player cannot place bomb too soon.
  if(millis() > timeBombplaced + 150){  //Able to place another bomb
    Player[0].isable = 1;
  }

  //Send player's data to the other arduino when player's position changed
  if (Player[0].placechanged == 1) {
    Serial3.write(Player[0].player_x);
    Serial3.write(Player[0].player_y);
    Serial3.write(Player[0].isbomb);
  }

  if(Player[0].isbomb == 1){
    placeBomb(Player+0);  //Place a bomb when isbomb is 1
  }
}

//Update two players' position on the screen
void updateDisplay() {
  tft.fillRect(prev_Player[0].player_x - PLAYER_WIDTH/2, prev_Player[0].player_y - PLAYER_WIDTH/2, PLAYER_WIDTH, PLAYER_WIDTH, ST7735_BLACK);
  tft.fillRect(Player[0].player_x - PLAYER_WIDTH/2, Player[0].player_y - PLAYER_WIDTH/2, PLAYER_WIDTH, PLAYER_WIDTH, p1_color);

  tft.fillRect(prev_Player[1].player_x - PLAYER_WIDTH/2, prev_Player[1].player_y - PLAYER_WIDTH/2, PLAYER_WIDTH, PLAYER_WIDTH, ST7735_BLACK);
  tft.fillRect(Player[1].player_x - PLAYER_WIDTH/2, Player[1].player_y - PLAYER_WIDTH/2, PLAYER_WIDTH, PLAYER_WIDTH, p2_color);
  Player[0].placechanged = 0;
  Player[1].placechanged = 0;
  prev_Player[1].player_x = Player[1].player_x;
  prev_Player[1].player_y = Player[1].player_y;
  prev_Player[0].player_x = Player[0].player_x;
  prev_Player[0].player_y = Player[0].player_y;
}
