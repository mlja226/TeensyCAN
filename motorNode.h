#ifndef MOTOR_NODE_H
#define MOTOR_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "MotorFilter.h"

#define CURRENT_DATA_ENTRIES 5

class motorNode : protected TeensyNode {
private:

	MotorFilter motorEKF;
	int currentData[CURRENT_DATA_ENTRIES];
	
public:
  motorNode();
  void interpretData( uint32 messageID);

  void kalmanStep(int data[], uint32 id, int arrLen);


};
