#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "arrayNode.h"
#include "message_ids.h"
//TODO

//sendMessage: Write message as soon as data is processed
void arrayNode::sendMessage(uint32_t writeMessageID, int data[], int datalen){

	CANMessage CANmsg;
	CANmsg.setMessageID(writeMessageID);

    int start=0,end=16; //Fill CAN Message data
    for(int i = 0; i< datalen; i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
	}
	this->write(CANmsg);
}

void arrayNode::checkForError(int data[], int datalen, uint32_t messageID){
	
    switch(messageID){
		
		case MOTOR_VELOCITY_MEASUREMENT:
	
			data[0]= this->arrayEKF.getX(0);
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

arrayNode::arrayNode(){
  for(int i =0; i < DATA_ENTRIES; i++){

	  this->currentData[i]=0;

  }
}

void arrayNode::interpretData(uint32_t messageID){
    int datalen = 8;
    int data[datalen];
    //CANMessage CANmsg;

    switch(messageID){
		
		case MOTOR_VELOCITY_MEASUREMENT:
	
			data[0]= arrayEKF.getX(0);
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
void arrayNode:: updateStateCalculations(){
	
	
}

/*
 #define ARRAY_MESSAGE_RANGE_LOW 0x200
 #define ARRAY_MESSAGE_RANGE_HI 0x2FF
 #define ARRAY_SIDE_POWER_1 0x200 // ( Array Side Power 16bits x 4 )
 #define ARRAY_SIDE_POWER_2 0x201 // ...

 #define ARRAY_BATTERY_SIDE_POWER_1 0x202 // ( Battery Side Power 16bits x 4 )
 #define ARRAY_BATTERY_SIDE_POWER_2 0x203 // ...

 #define ARRAY_MPPT_STATUS 0x204 // ( Mppt Status 8bits x 8 )

 #define ARRAY_ILLUMINANCE 0x205 // ( Illuminance 16bits )( Padding 48bits )

 #define ARRAY_TEMPERATURE 0x206 // ( Temperature 8bits x 8 )
 #define ARRAY_TEMPERATURE 0x206 // ...

*/

void arrayNode::kalmanStep(int data[], int id, int arrLen){
	
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
	this->arrayEKF.step(dataAsDoubles);
	

}

