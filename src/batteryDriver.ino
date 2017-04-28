#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"

CANMessage *message;
CAN_message_t msg;
batteryNode *thisNode;

TeensyNode * teensy = NULL;
void setup(){
  teensy = new TeensyNode(500000);
  teensy->CANBus.begin();
  pinMode(13, OUTPUT);
  Serial.begin(19200);
}

void loop() {
  bool state_change = false;
  // If RX buffer is not empty
  digitalWrite(13, HIGH);
  //Serial.println("Starting to listen");
  int ret = 0;
  while (ret == 0){
    //Serial.println("Listening...");
    //Serial.println(teensy->CANBus.read(msg));
    //Serial.println(msg.id);
    delay(100);
  }
  Serial.println("GOT A MESSAGE!!: ");
  Serial.println(message->getMessageID());

if (message->getMessageID()>=BATTERY_MESSAGE_RANGE_LOW && message->getMessageID()<=BATTERY_MESSAGE_RANGE_HI){
	    // Read valid input
	    state_change = true;

	    // Filter new data
		int data[4];
		for (int i=0;i<4;i++){

			data[i]= message->readUnsignedInt(i*16,(i+1)*16);
		}

	    //Kalman Filter black magic
		thisNode->kalmanStep(data,message->getMessageID(),4);

	    // Check filtered values for errors
		thisNode->interpretData(message->getMessageID());

	    // TODO Chack against set break points

	  }

  if( state_change ){
    // If changed compute state

	//  Compute things like voltage over time, soc, ...
    // Check computed state for errors
	  thisNode->updateStateCalculations();

    // TODO Check against set break points
  }
}
