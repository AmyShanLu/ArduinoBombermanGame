#include "Communication.h"

/** Waits for a certain number of bytes on Serial3 or timeout
* @param nbytes: the number of bytes we want
* @param timeout: timeout period (ms); specifying a negative number
*                turns off timeouts (the function waits indefinitely
*                if timeouts are turned off).
* @return True if the required number of bytes have arrived.
*/
bool wait_on_serial3( uint8_t nbytes, long timeout ) {
  unsigned long deadline = millis() + timeout;//wraparound not a problem
  while (Serial3.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1); // be nice, no busy loop
  }
  return Serial3.available()>=nbytes;
}

/**
  The Arduino configured as the client sends Connection Request 'C' to the sever.
  If it receives acknowledgement 'A' from the sever, the client sends acknowledgement
  of acknowledgement 'A' to sever, and returns true which indicates the client is
  ready to start the game. Otherwise, timeout and the client re-sends Connection
  Request 'C' to the sever.
*/
bool client(){
  enum State {Start, WaitingForAck, DataExchange};
  State curr_state = Start;
  while(true){
    if(curr_state == Start){
      Serial3.write('C'); //Send Connection request 'C' to the server
      curr_state = WaitingForAck;
    }
    else if(curr_state == WaitingForAck){
      if(wait_on_serial3((uint8_t) 1, (long) 1000) == 0){
        curr_state = Start; //Timeout and return to Start state
      }
      if(Serial3.read() == 'A'){  //Receive Acknowledgement 'A'
        Serial3.write('A'); //Send Acknowledgement of Acknowledgement 'A'
        curr_state = DataExchange;
      } //If does not receive A, stay in WaitingForAck state
    }
    else if(curr_state == DataExchange) {
      return 1;
    }
  }
}

/**
  The Arduino configured as the sever sends acknowledgement 'A' to the client when
  receives 'C'. While consuming all connection request 'C' from the client, when the
  sever receives acknowledgement of acknowledgement 'A' from the client, the function
  returns true which indicates the sever is ready to start the game.
*/
bool sever() {
  enum State {listen,waitforack,dataexchange};
  State curr_state = listen;
  char CR = 'C',ACK = 'A';
  uint32_t ckey;
  while(true){
    if(curr_state == listen && Serial3.read() == CR){
      curr_state = waitforack;  //Receive Connection request 'C', go to waitforack
      Serial3.write(ACK); //send Acknowledgement 'A' to the client
    }
    else if(curr_state == waitforack && wait_on_serial3(1, 1000) == true){ //Have 1 bytes on Serial3 and not timeout
      char receive = Serial3.read();
      if (receive == CR){ //Receive 'C', stay in waitforack
        curr_state = waitforack;
      }
      else if(receive == ACK){ //Receive Acknowledgement of Acknowledgement 'A'
        curr_state = dataexchange;  //go to dataexchange and return 1
        return 1;
      }
    }
    else{
      curr_state = listen;  //Go back to listen state when timeout
    }
  }
}
