#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"

//Get some pointers for initializing the batteryNode
CANMessage *message;
batteryNode *thisNode;
FlexCAN *bus;

void setup(){
  //Turn on Teensy light
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  //Constuct a CANBus and a CANMessage so we can read off the bus
  //Default baud rate is 500000, but can be changed by passing an arguement to FlexCAN()
  bus = new FlexCAN();
  message = new CANMessage();

  //Now that we have what we need, lets create a batteryNode with the CAN bus we just made
  thisNode = new batteryNode(bus);
}

void loop() {

  //Reset state change to false
  bool state_change = false;
  // If RX buffer is not empty
  Serial.printf("Starting to listen.\n");

  //Keep listening until we get a message
  while(thisNode->read(*message)){
    Serial.printf("Listening...\n");

    delay(100);
  }
  Serial.printf("GOT A MESSAGE WITH ID = %x\n", message->getMessageID());

  //Now that we have a message, check to see if it's ID is a valid Battery message.
  //Otherwise, we don't care about it and should ignore it.
  if (message->getMessageID()>=BATTERY_MESSAGE_RANGE_LOW && message->getMessageID()<=BATTERY_MESSAGE_RANGE_HI){
    //Acknowledge that a change in state has occured
    state_change = true;

    //Read in each of the 4 16-bit integers we expect for a batttery message.
    int data[4];
    for (int i=0;i<4;i++){
      //Here we are interpreting the intergers as unsigned
      //If we wanted to interpret them as signed integers we would call readSignedInt() instead
      data[i]= message->readUnsignedInt(i*16,(i+1)*16);
    }
    //Now that we have our raw data, we need to filter it.
    //Summon some Kalman Filter black magic to clean up the data
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
