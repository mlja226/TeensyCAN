#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"


CANMessage message = CANMessage();
FlexCAN bus = FlexCAN(500000);
batteryNode thisNode =  batteryNode(bus);
void setup(){
  bus.begin();
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  bool state_change = false;
  // If RX buffer is not empty
  while (thisNode.read(message)==0){
  }

  if (message.getMessageID()>=BATTERY_MESSAGE_RANGE_LOW && message.getMessageID()<=BATTERY_MESSAGE_RANGE_HI){
    // Read valid input
    state_change = true;

    // Filter new data
    int data[4];
    for (int i=0;i<4;i++){

      data[i]= message.readSignedInt(i*16,(i+1)*16);
      //Serial.printf("Data[%d] = %u",i, data[i]);
     }
      //Serial.printf("Message id = %x \n", message.getMessageID());
      //Kalman Filter black magic
      thisNode.kalmanStep(data,message.getMessageID(),4);

      // Check filtered values for errors
      thisNode.interpretData(message.getMessageID());

      // TODO Check against set break points

  }

    if( state_change ){
      // If changed compute state

      //  Compute things like voltage over time, soc, ...
      // Check computed state for errors
      thisNode.updateStateCalculations();

    }
}
