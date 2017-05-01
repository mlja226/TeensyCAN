#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "arrayNode.h"
#include "message_ids.h"

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

		case ARRAY_SIDE_POWER_1:

			break;
		case ARRAY_SIDE_POWER_2:
			break;

		case ARRAY_BATTERY_SIDE_POWER_1:
			break;

		case ARRAY_BATTERY_SIDE_POWER_2:
			break;

		case ARRAY_MPPT_STATUS:
			break;

		case ARRAY_ILLUMINANCE:
			break;

		case ARRAY_TEMPERATURE:
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


    switch(messageID){

		case ARRAY_SIDE_POWER_1:

			data[0]= arrayEKF.getX(0);
			checkForError(data,datalen,messageID);
			break;
		case ARRAY_SIDE_POWER_2:
			break;

		case ARRAY_BATTERY_SIDE_POWER_1:
			break;

		case ARRAY_BATTERY_SIDE_POWER_2:
			break;

		case ARRAY_MPPT_STATUS:
			break;

		case ARRAY_ILLUMINANCE:
			break;

		case ARRAY_TEMPERATURE:
			break;

		default:
			break;



    }
}
void arrayNode:: updateStateCalculations(){


}


void arrayNode::kalmanStep(int data[], int id, int arrLen){

	double dataAsDoubles[DATA_ENTRIES];
	switch (id){

		case ARRAY_SIDE_POWER_1:

			currentData[0]= data[0];

			break;

		case ARRAY_SIDE_POWER_2:
			break;

		case ARRAY_BATTERY_SIDE_POWER_1:
			break;

		case ARRAY_BATTERY_SIDE_POWER_2:
			break;

		case ARRAY_MPPT_STATUS:
			break;

		case ARRAY_ILLUMINANCE:
			break;

		case ARRAY_TEMPERATURE:
			break;

		default:
			break;

	}
	for (int i=0;i<DATA_ENTRIES;i++){

		dataAsDoubles[i]=static_cast<double>(currentData[i]);
	}
	this->arrayEKF.step(dataAsDoubles);


}
