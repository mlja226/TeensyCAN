#ifndef TEENSY_NODE_H
#define TEENSY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"

#define MAX_ERRORS 10

class TeensyNode {
private:
  FlexCAN *CANBus;

public:
  TeensyNode(FlexCAN *bus){
    this->CANBus = bus;
  }
  /*TeensyNode(uint32_t baudrate){
    this->CANBus = FlexCAN(baudrate);
  }
*/
  int read(CANMessage * message){
    CAN_message_t msg;
    //Serial.printf("Address in Read = %p", this->CANBus);
    int result = this->CANBus->read(msg);
    message->translateFromFlexCAN(msg);
    return result;
  }
  int write(CANMessage message){
    CAN_message_t msg;
    Serial.printf("Message in write %x \n",message.getMessageID());
    message.translateToFlexCAN(msg);
    int result = this->CANBus->write(msg);
    return result;
  }



};

#endif
