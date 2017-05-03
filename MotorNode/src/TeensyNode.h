#ifndef TEENSY_NODE_H
#define TEENSY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"

#define MAX_ERRORS 10
/*TeensyNode: This class is meant to act as a parent class for current Teensy nodes
* and a template for future node development. This class handles the basics of CANBus operations.
*/
class TeensyNode {
private:
  //The bus we will be reading/writing from/to
  FlexCAN * CANBus;
public:
  //TeensyNode Constructor: Takes a FlexCAN CANBus that is initialized and .start()-ed.
  //Start/stopping the bus should be handled outside this class.
  TeensyNode(FlexCAN &bus){
    this->CANBus = &bus;
  }
  //Read from the CANBus
  //A filter may be added in child classes to only read certain messages
  int read(CANMessage &message){
    CAN_message_t msg;
    int result = this->CANBus->read(msg);
    //Translate to FlexCAN for reading
    message.translateFromFlexCAN(msg);
    // Return the outcome of the read ( 1= Success. 0= Failure)
    return result;
  }
  //Write to CANBus
  int write(CANMessage message){
    CAN_message_t msg;
    //Translate to FlexCAN for writing
    message.translateToFlexCAN(msg);
    int result = this->CANBus->write(msg);
    // Return the outcome of the write ( 1= Success. 0= Failure)
    return result;
  }



};

#endif
