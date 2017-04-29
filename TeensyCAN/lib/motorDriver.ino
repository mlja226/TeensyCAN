#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "motorNode.h"
#include <vector>
#include "message_ids.h"
CANMessage *message;
motorNode *thisNode;
void setup(){
  //Turn on Teensy light
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //Constuct a CANBus and a CANMessage so we can read off the bus
  //Default baud rate is 500000, but can be changed by passing an arguement to FlexCAN()
  bus = new FlexCAN();
  message = new CANMessage();
  //Now that we have what we need, lets create a batteryNode with the CAN bus we just made
  thisNode = new motorNode(bus);

}

void loop() {
  //Reset state change to false
  bool state_change = false;
  // If RX buffer is not empty
  while (thisNode->read(message)==0){
  }

  if (message->getMessageID()>=MOTOR_MESSAGE_RANGE_LOW && message->getMessageID()<=MOTOR_MESSAGE_RANGE_HI){
    
	    state_change = true;

	    // Filter new data
		int data[4];
		for (int i=0;i<4;i++){

			data[i]= message.readSignedInt(i*16,(i+1)*16);
		}

	    //Kalman Filter black magic
		thisNode->kalmanStep(data,message.getMessageID(),4);

	    // Check filtered values for errors
		thisNode->interpretData(message.getMessageID());

	    // TODO Chack against set break points

	  }

  if( state_change ){
    // If changed compute state

	//  Compute things like voltage over time, soc, ...
    // Check computed state for errors
	  thisNode.updateStateCalculations();

  }

}
