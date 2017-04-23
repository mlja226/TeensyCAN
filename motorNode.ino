#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "motorNode.h"
#include <vector>
#include "message_ids.h"
//TODO

int motorNode::checkForError(vector<int> data, uint32 messageID){}


motorNode::motorNode(){
  for(int i =0; i < CURRENT_DATA_ENTRIES; i++){
	  
	  this->currentData[i]=0;
	  
  }
}

void motorNode::interpretData(uint32 messageID){
  int index errormsg;
  vector<int> data;
  if(messageID == BATTERY_BC_AC_BP_AP){
    for(int i =0; i<4; i++){
      data.push_back() = currentFilter.getX(i);
    }
    errormsg = checkForError(, BATTERY_BC_AC_BP_AP);
    CANMessage() CANmsg;
    if(errormsg){
      CANmsg->messageID = errormsg;
    }
    else{
      CANmsg->messageID = messageID;
    }
    //Pack the data
    int start=0,end=16;
    for(int i = 0; i< data.size(); i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
  }
  else{
    if(id >=BATTERY_VOLTAGE_1 && id <= BATTERY_VOLTAGE_10){
      index = id - BATTERY_VOLTAGE_1;
      for(int i =0; i<4; i++){
        data.push_back(this->cellFilters[index].getX(i));
      }

    }
    else if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1;
      for(int i =0; i<4; i++){
        data.push_back(this->cellFilters[index].getX(i));
      }
    }
    errormsg = checkForError(data, messageID);
    CANMessage() CANmsg;
    if(errormsg){
      CANmsg->messageID = errormsg;
    }
    else{
      CANmsg->messageID = messageID;
    }
    //Pack the data
    int start=0,end=16;
    for(int i = 0; i< data.size(); i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
      }
    }
  }
  this->CANBus.write(CANmsg);

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

void motorNode::kalmanStep(int data[], uint32 id, int arrLen){
	
	
	switch (id){
		
		case MOTOR_VELOCITY_MEASUREMENT:
			
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
			
			
		
		
		
		
	}
	
}
