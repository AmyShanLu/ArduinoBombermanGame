/**
  Communication.h

  Acknowledgement: This uses wait_on_serial3( uint8_t nbytes, long timeout) function
  from the first assignment of CMPUT 274 on eclass.
  (https://eclass.srv.ualberta.ca/mod/page/view.php?id=2052214)

  This declares client() and sever() function, these two functions make sure both
  Arduinos start the game at the same time.
*/

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

/*Waits for a certain number of bytes on Serial3 or timeout*/
bool wait_on_serial3( uint8_t nbytes, long timeout);

/**
  The Arduino configured as the client sends Connection Request 'C' to the sever.
  If it receives acknowledgement 'A' from the sever, the client sends acknowledgement
  of acknowledgement 'A' to sever, and returns true which indicates the client is
  ready to start the game. Otherwise, timeout and the client re-sends Connection
  Request 'C' to the sever.
*/
bool client();

/**
  The Arduino configured as the sever sends acknowledgement 'A' to the client when
  receives 'C'. While consuming all connection request 'C' from the client, when the
  sever receives acknowledgement of acknowledgement 'A' from the client, the function
  returns true which indicates the sever is ready to start the game.
*/
bool sever();

#endif
