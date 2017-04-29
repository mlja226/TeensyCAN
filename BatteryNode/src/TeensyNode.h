#ifndef TEENSY_NODE_H
#define TEENSY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"

#define MAX_ERRORS 10

class TeensyNode {
private:
  FlexCAN * CANBus;

public:
  TeensyNode(FlexCAN &bus){
    this->CANBus = &bus;
  }
  /*TeensyNode(uint32_t baudrate){
    this->CANBus = FlexCAN(baudrate);
  }
*/
  int read(CANMessage &message){
    CAN_message_t msg;
    int result = this->CANBus->read(msg);
    //Serial.printf("Address in Read = %x", msg.id);

    message.translateFromFlexCAN(msg);
    return result;
  }
  int write(CANMessage message){
    CAN_message_t msg;
    Serial.printf("Message ID in write before translation %x \n",message.getMessageID());
    message.translateToFlexCAN(msg);
    Serial.printf("Message ID in write after translation %x \n",msg.id);
    int result = this->CANBus->write(msg);
    return result;
  }



};

#endif
