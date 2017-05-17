# Arduino_Bomberman_Game
Name: Shan Lu, William Wong

CMPUT 274 A2

Final Project README

Bomberman

Table of Contents
1. Description
2. Functions of the Game
3. Matreials Needed
4. Wiring Instructions
5. Programming Instructions
6. Assumptions
7. Acknowledgements


1. Description

  Bomberman is a two-player game where they control a person who sets bombs
  which explodes after a few seconds. The objective of the game is to explode
  the other player before time is over. Walls can protect the player by
  blocking the explosion; however, they do not last forever. There are also
  powerups which when grabbed will make the player's bombs stronger. If the
  winner beats the other player fast enough they can be added to the high
  score table for them to remember.

2. Functions of the Game(Instructions)

  a) Setup
  
    After you have finished the wiring and uploaded the code onto the Arduino,
    it will wait until the other Arduino is connected before continuing on
    with the code. This might take a few seconds. It will then ask both of
    the players to enter a name, which can be typed on the computer that is
    connected to the Arduino. Players will type their name in and press enter.
    After both players have entered their names the game will start.
    
  b) The Player
  
    Depending on whether the player's Arduino is the server or the client,
    the player's cursor will be a color of red or blue. In order to move the
    cursor around, there is a joystick on the Arduino to help move. The more
    farther the joystick is moved away from the middle, the faster the
    cursor will move. There is a minimum distance that the joystick must be
    moved in order for the cursor to move, and is there to prevent
    unnecassary movements. The players can pass through each other.
    
  c) The Bombs
  
    When the player presses on the joystick, a yellow bomb will appear where
    their cursor was when they pressed the joystick. This bomb will stay
    there for 5 seconds, then it will explode. The explosion will consist of
    a horizontal and a vertical line. The explosion will stay on the screen
    for 100 ms, and if any of the players are in the explosion, the other
    player wins. If both of the players are in the explosion at the same
    time, then it will be considered a tie. There is a delay where the
    player cannot place other bomb, and players cannot pass throught the
    bombs.
    
  d) The Walls
  
    At the start of the game, the screen will display a series of walls.
    These walls are selected randomly from preset maps in the code. The
    players cannot pass through the walls. Also the bomb explosion cannot
    pass through the wall either, however the wall can only stand up to 3
    explosions before it breaks down and disapears.
    
  e) The Powerups
  
    Every 10 seconds a green powerup will spawn at the specific locations
    set. These powerups strengthen the bomb of the player that picks it up
    by a factor. It will spawn one every time unless there is already 4
    powerups on the screen, then it will not spawn other one until there
    is space.
    
  f) The Time Limit
  
    There is a timer on the top of the screen. The players cannot pass
    through the timer. It starts at 3 minutes and counts down. Once it
    reaches zero, it will end the game and both players lose.
    
  g) The Game Over Menu
  
    Once the game is over, the game will display whether the player won,
    lost, or tied against the other player, and then it will display the
    high score table. If the players want to play again, they can press
    the joystick and it will go back up to the setup.
    
  h) The High Score Table
  
    The High Score Table will display after the game is over. It will
    display at the left the player's name, and their score, which is
    measured by the time that was remaining. The highest score will be
    displayed at the top of the leaderboard.
    
  i) The High Score Erase
  
    During the game or whenever the screen is on the high score table, the
    player has the option to erase the memory of the high score table. By
    pressing the pushbutton on the Arduino, the player will have removed
    the memory, and that will be reflected the next time the high score
    table is shown.

3. Materials Needed

  Have these materials in order to ensure that the code will run properly.
    2 - Arduino Mega Microcontroller
    2 - USB cable, 3' to 6', A-B style
    2 - Single Panel Breadboard
    2 - SparkFun COM-09032 - Thumb Joystick and Breakout Board
    2 - Adafruit 1.8" 18-bit color TFT LCD display with microSD card - ST7735R
    2 - SPST - 6mm Momentary Tactile pushbutton
    1 - 560 ohm Resistor (Green Blue Brown)
    44 - 22 AWG solid jumper wire

4. Wiring Instructions

  Follow these instructions to ensure that your Arduinos will run properly.
  Please wire the Arduinos together before connecting them to the computer.

  LED Screen to Arduino(For both Arduinos):
  
    1. GND <---> Breadboard GND bus
    2. VCC <---> Breadboard Positive bus
    3. RESET <--> Pin 8
    4. D/C <---> Pin 7
    5. CARD_CS <---> Pin 5
    6. TFT_CS <---> Pin 6
    7. MOSI <---> Pin 51
    8. SCK <---> Pin 52
    9. MISO <---> Pin 50
    10. LITE <---> Breadboard Positive bus

  Joystick to Arduino(For both Arduinos):
  
    1. VCC <---> Breadboard Positive bus
    2. VERT <---> Pin A0
    3. HOR <---> Pin A1
    4. SEL <---> Pin 9
    5. GND <---> Breadboard GND bus

  Pushbutton to Arduino(For both Arduinos):
  
    1. Pin 2 <---> Pushbutton <---> Breadboard GND bus

  Arduino to Arduino:
  
    1. Arduino 1 GND <---> Arduino 2 GND
    2. Arduino 1 TX3 <---> Arduino 2 RX3
    3. Arduino 1 RX3 <---> Arduino 2 TX3

  Other Wiring:
  
    Server Arduino:
    
      1. Pin 13 <---> Resistor <---> Breadboard GND bus
    Client Arduino:
    
      1. Pin 13 <---> Arduino GND
    Both Arduinos:
    
      1. Arduino 5V <---> Breadboard Positve bus
      2. Arduino GND <---> Breadboard GND bus

5. Programming Instructions

  In order to upload the code properly on the Arduino, follow these instructions.
  If the code has already been uploaded onto the Arduino, go to Step 3.
  
  1.  Open the Terminal and change your directory so that you are in the
      folder with the code using the command cd <directory>.
      
  2.  When you are in the directory, make sure the Arduino is connected to
      the computer and each other. Then in the Terminal, type the command
      <make upload> to upload the code onto the Arduino.
      
  3.  When the code is uploaded on the Arduino, type in Terminal the command
      <serial-mon> to access the serial monitor, where you can type in your
      name and start the game.

6. Assumptions

  There is an assumption during the setup. When you are typing your name
  on the computer, we expect you to not make a mistake in typing your name.
  Pressing backspace does not fix it on the Arduino.

7. Awknowledgements

  README:
  
    The wiring procedures listed for the LCD and Joystick have been
    adapted from the Arduino Intro Labs website. Link is here if you want a
    picture reference in where to connect the various wires:
    
    ugweb.cs.ualberta.ca/~c274/web/ConcreteComputing/section/adafruit_lcd.htm
    
  Code:
  
    The following codes listed here have been adapted from our CMPUT 274
    class Dropbox, from what we learned in class.
    
    Server - Client Chat Code
      Source: Csaba Dropbox WF, 09-28, chat.cpp.
      
    Queue Linked List and Data Structure
      Source: Chris Dropbox WF, Nov18, queue, test.cpp.
      
    Double linked list
      Source: Chris Dropbox WF, Nov18, deque, deque.cpp.
      
    Qsort
      Source: Zac Dropbox WF, Mov02, qsort, qsort.cpp.
      
    Cursor and UpdateDisplay
      Source: Zac Dropbox WF, Oct14, cursor, cursor.cpp.
