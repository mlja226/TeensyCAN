#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "motorNode.h"
#include "message_ids.h"


//sendMessage: Write message as soon as data is processed
void motorNode::sendMessage(uint32_t writeMessageID, int data[], int datalen){

	CANMessage CANmsg;
	CANmsg.setMessageID(writeMessageID);

    int start=0,end=MESSAGE_SIZE; //Fill CAN Message data
    for(int i = 0; i< datalen; i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += MESSAGE_SIZE;
        end += MESSAGE_SIZE;
	}
	this->write(CANmsg);
}

void motorNode::checkForError(int data[], int datalen, uint32_t messageID){

	//Call to sendMessage to ensure that data reporting message (non-error message) is sent
	sendMessage(messageID,data,datalen);

    switch(messageID){

		case MOTOR_VELOCITY_MEASUREMENT:

			data[0]= motorEKF.getX(0);
			checkForError(data,datalen,messageID);
			break;

		case MOTOR_PHASE_CURRENT_MEASUREMENT:
			break;

		case MOTOR_BACKEMF_MEASUREMENT_PREDICTION:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_1:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_2:
			break;

		case MOTOR_FAN_SPEED_MEASUREMENT:
			break;

		case MOTOR_SINK_AND_MOTOR_TEMPERATURE:
			break;

		case MOTOR_AIR_IN_AND_CPU_TEMPERATURE:
			break;

		case MOTOR_AIR_OUT_AND_CAP_TEMPERATURE:
			break;

		case MOTOR_ODOMETER_AND_BUS_AMP_HOURS:
			break;
		default:
			break;
    }


}

motorNode::motorNode(FlexCAN bus) : TeensyNode(bus) {
  for(int i =0; i < DATA_ENTRIES; i++){

	  this->currentData[i]=0;

  }
}

void motorNode::interpretData(uint32_t messageID){
    int datalen = MESSAGE_PIECES;
    int data[datalen];
    //CANMessage CANmsg;

    switch(messageID){

		case MOTOR_VELOCITY_MEASUREMENT:

			data[0]= motorEKF.getX(0);
			checkForError(data,datalen,messageID);
			break;

		case MOTOR_PHASE_CURRENT_MEASUREMENT:
			break;

		case MOTOR_BACKEMF_MEASUREMENT_PREDICTION:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_1:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_2:
			break;

		case MOTOR_FAN_SPEED_MEASUREMENT:
			break;

		case MOTOR_SINK_AND_MOTOR_TEMPERATURE:
			break;

		case MOTOR_AIR_IN_AND_CPU_TEMPERATURE:
			break;

		case MOTOR_AIR_OUT_AND_CAP_TEMPERATURE:
			break;

		case MOTOR_ODOMETER_AND_BUS_AMP_HOURS:
			break;
		default:
			break;
    }
}
void motorNode:: updateStateCalculations(){


}

/*
#define MOTOR_VELOCITY_MEASUREMENT 0x403

#define MOTOR_PHASE_CURRENT_MEASUREMENT 0x404

#define MOTOR_VOLTAGE_VECTOR_MEASUREMENT 0x405

#define MOTOR_CURRENT_VECTOR_MEASUREMENT 0x406

#define MOTOR_BACKEMF_MEASUREMENT_PREDICTION 0x407

#define MOTOR_VOLTAGE_RAIL_MEASUREMENT_1 0x408

#define MOTOR_VOLTAGE_RAIL_MEASUREMENT_2 0x409

#define MOTOR_FAN_SPEED_MEASUREMENT 0x40A

#define MOTOR_SINK_AND_MOTOR_TEMPERATURE 0x40B

#define MOTOR_AIR_IN_AND_CPU_TEMPERATURE 0x40C

#define MOTOR_AIR_OUT_AND_CAP_TEMPERATURE 0x40D

#define MOTOR_ODOMETER_AND_BUS_AMP_HOURS 0x40E

*/

void motorNode::kalmanStep(int data[], int id, int arrLen){

	double dataAsDoubles[DATA_ENTRIES];
	switch (id){

		case MOTOR_VELOCITY_MEASUREMENT:

			currentData[0]= data[0];

			break;

		case MOTOR_PHASE_CURRENT_MEASUREMENT:
			break;

		case MOTOR_BACKEMF_MEASUREMENT_PREDICTION:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_1:
			break;

		case MOTOR_VOLTAGE_RAIL_MEASUREMENT_2:
			break;

		case MOTOR_FAN_SPEED_MEASUREMENT:
			break;

		case MOTOR_SINK_AND_MOTOR_TEMPERATURE:
			break;

		case MOTOR_AIR_IN_AND_CPU_TEMPERATURE:
			break;

		case MOTOR_AIR_OUT_AND_CAP_TEMPERATURE:
			break;

		case MOTOR_ODOMETER_AND_BUS_AMP_HOURS:
			break;
		default:
			break;

	}
	for (int i=0;i<DATA_ENTRIES;i++){

		dataAsDoubles[i]=static_cast<double>(currentData[i]);
	}
	this->motorEKF.step(dataAsDoubles);


}
