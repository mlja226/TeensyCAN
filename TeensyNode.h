#ifndef TEENSY_NODE_H
#define TEENSY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>


class TeensyNode {
private:
  FlexCAN CANBus;

public:
  TeensyNode(uint32_t baudrate){
    this->CANBus = FlexCAN(baudrate);
  }
  int read(CAN_message_t &message){
    return this->CANBus.read(message);
  }
  int write(CAN_message_t message){
    return this->CANBus.write(message);
  }
};

#endif
