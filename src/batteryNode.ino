#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"

//sendMessage: Write message as soon as data is processed
void batteryNode::sendMessage(uint32_t writeMessageID, int data[], int datalen){

	CANMessage CANmsg;
	CANmsg.setMessageID(writeMessageID);

    int start=0,end=16;
    for(int i = 0; i< datalen; i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
	}
	this->write(CANmsg);
}

//TODO
void batteryNode::checkForError(int data[], int datalen, uint32_t messageID){

  //BATTERY VOLTAGE ERROR CHECK
  if(messageID >= BATTERY_VOLTAGE_1 && messageID <= BATTERY_VOLTAGE_10){
    //iterate through data to check if there are any readings above/below cutoff
    for(int i = 0; i < datalen; i++){
      if(data[i] >= CUTOFF_VOLTAGE_HIGH ){

		  sendMessage(CUTOFF_VOLTAGE_HIGH,data, datalen);
        //return CUTOFF_VOLTAGE_HIGH;
      }
      else if( data[i] <= CUTOFF_VOLTAGE_LOW){
		  sendMessage(CUTOFF_VOLTAGE_LOW,data, datalen);
        //return CUTOFF_VOLTAGE_LOW;

      }
    }
  }
  //BATTERY TEMPERATURE ERROR CHECK
  else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
    //iterate through data to check if there are any readings above/below cutoff
    for(int i = 0; i < datalen; i++){
      if(data[i] >= CUTOFF_TEMP_HIGH ){
		sendMessage(CUTOFF_TEMP_HIGH,data, datalen);
        //return CUTOFF_TEMP_HIGH;
      }
    }
  }
  //BATTERY_BC_AC_BP_AP ERROR CHECK
  //TODO: DETERMINE VALID RANGE FOR THESE VALUES
  else if(messageID == BATTERY_BC_AC_BP_AP){

  }
  //BATTERY CBS ERROR CHECK
  else if(messageID == BATTERY_CBS_1 || messageID == BATTERY_CBS_1){

  }
  //BATTERY ESR ERROR CHECK
  else if(messageID >= BATTERY_ESR_1 && messageID <= BATTERY_ESR_5){

  }

}



batteryNode::batteryNode() : TeensyNode(){
  for(int i =0; i < CURRENT_DATA_ROWS; i++){
    for(int j = 0; j < CURRENT_DATA_COLUMNS; j++){
      this->currentData[i][j] = 0;

    }
  }
}
void batteryNode::interpretData(uint32_t messageID){
  int index, datalen = 8;
  int data[datalen];
  //CANMessage CANmsg;

  if(messageID == BATTERY_BC_AC_BP_AP){
    for(int i =0; i<4; i++){
      data[i] = static_cast<int>(currentFilter.getX(i));
    }

	checkForError(data, datalen, BATTERY_BC_AC_BP_AP);
    //CANmsg.setMessageID(errormsg);
  }
  else{
    //if it is a Voltage, set the correct index for the filter
    if(messageID >=BATTERY_VOLTAGE_1 && messageID <= BATTERY_VOLTAGE_10){
      index = messageID - BATTERY_VOLTAGE_1;
    }
      //if it is a Temperature, set the correct index for the filter
    else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
      index = messageID - BATTERY_TEMPERATURE_1;
    }
    else{
      //if we get here, we were processing data with an id that does't belong to the batteryNode
      return;
    }
    for(int i =0; i<4; i++){
      data[i] = static_cast<int>(this->cellFilters[index].getX(i));
    }

   	checkForError(data, datalen, messageID);


    }

}

void batteryNode::kalmanStep(int data[], int id, int arrLen){
  int index;
  double dataAsDoubles[arrLen];

  if(id == BATTERY_BC_AC_BP_AP){
    for(int i = 0; i< 4; i++){
      dataAsDoubles[i] = static_cast<double>(data[i]);
    }
    this->currentFilter.step(dataAsDoubles);
  }
  else{
    if(id >=BATTERY_VOLTAGE_1 && id <= BATTERY_VOLTAGE_10){
      index = id - BATTERY_VOLTAGE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i = 0; i< 4; i++){
        currentData[index][i] = data[i];
        dataAsDoubles[i] = static_cast<double>(data[i]);
      }

    }

    else if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1+10;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i =0; i< 4; i++){
        currentData[index][i] = data[i];
        dataAsDoubles[i] = static_cast<double>(data[i]);
      }
    }
    else{
      //TODO: Decide what to do for other IDs
      index = 0;
    }

    this->cellFilters[index].step(dataAsDoubles);
  }

}

CANMessage message;
batteryNode thisNode;
void setup(){



}

void loop() {
  bool state_change = false;
  // If RX buffer is not empty
  while (thisNode.read(message)==0){
  }

  if (message.getMessageID()>=BATTERY_MESSAGE_WINDOW_LOW && message.getMessageID()<=BATTERY_MESSAGE_WINDOW_HIGH){
	    // Read valid input
	    state_change = true;

	    // Filter new data
	    // TODO Kalman Filter black magic
		

	    // Check filtered values for errors
	    // TODO Chack against set break points
	  }

  if( state_change ){
    // If changed compute state
    // TODO Compute things like voltage over time, soc, ...

    // Check computed state for errors
    // TODO Check against set break points
  }

}
