#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"

CANMessage message;

FlexCAN CANBus=NULL;

CAN_message_t msg;


void setup(){

	CANBus=FlexCAN(500000);
	CANBus.begin();
	pinMode(13, OUTPUT);
	digitalWrite(13,HIGH);
	msg.id=123;
}

void loop() {

	CANBus.write(msg);
}
