#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "motorNode.h"
#include <vector>
#include "message_ids.h"


CANMessage message= CANMessage();
FlexCAN bus= FlexCAN(500000);
motorNode thisNode = motorNode(bus);

#define MESSAGE_PIECES 2
#define MESSAGE_SIZE 32

void setup(){

}

void loop() {

  //Reset state change to false
  bool state_change = false;

  // If RX buffer is not empty
  while (thisNode.read(message)==0){
  }

  //is this a motor specific change?
  if (message.getMessageID()>=MOTOR_MESSAGE_RANGE_LOW && message.getMessageID()<=MOTOR_MESSAGE_RANGE_HI){

	    state_change = true; //State has changed

	    // Filter new data
		int data[MESSAGE_PIECES];

		for (int i=0;i<MESSAGE_PIECES;i++){

			data[i]= message.readSignedInt(i*MESSAGE_SIZE,(i+1)*MESSAGE_SIZE);
		}

	    //Kalman Filter black magic
		thisNode.kalmanStep(data,message.getMessageID(),MESSAGE_PIECES);

	    // Check filtered values for errors
		thisNode.interpretData(message.getMessageID());

	  }

  if( state_change ){
    // If changed compute state

	//  Compute things like voltage over time, soc, ...
    // Check computed state for errors
	  thisNode.updateStateCalculations();

  }

}
