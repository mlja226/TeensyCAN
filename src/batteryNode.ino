#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include <vector>
#include "message_ids.h"
//TODO
int batteryNode::checkForError(vector<int> data, int messageID){}
batteryNode::batteryNode(){
  for(int i =0; i < CURRENT_DATA_ROWS; i++){
    for(int j = 0; j < CURRENT_DATA_COLUMNS; j++){
      this->currentData[i][j] = 0;

    }
  }
}
void batteryNode::interpretData(int messageID){
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

void batteryNode::kalmanStep(int data[], int id, int arrLen){
  int index;

  if(id == BATTERY_BC_AC_BP_AP){
    this->currentFilter.step(data);
  }
  else{
    if(id >=BATTERY_VOLTAGE_1 && id <= BATTERY_VOLTAGE_10){
      index = id - BATTERY_VOLTAGE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i = 0; i< 4; i++){
        currentData[index][i] = data[i];
      }

    }

    else if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i =0; i< 4; i++){
        currentData[index][i+4] = data[i];
      }
    }
    this->cellFilter[index].step(currentData[index]);
    this->cellFilter[index]
  }

}
