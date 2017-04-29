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
	//Serial.printf("WRITE MESSAGE %x", writeMessageID);
    int start=0,end=16; //Fill CAN Message data
    for(int i = 0; i< datalen; i++){
        CANmsg.storeUnsignedInt(int64_t(data[i]),start, end);
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
  //BATTERY State of Charge Check
  else if (messageID==BATTERY_SOC){

	  if (data[0]<CUTOFF_SOC_LOW){ //low state of charge
		  sendMessage(CUTOFF_SOC_LOW, data, datalen);//Send error message
	  }
  }

}

batteryNode::batteryNode(FlexCAN *bus) : TeensyNode(bus){
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
      for(int i =0; i<4; i++){
        data[i] = static_cast<int>(this->cellFiltersVoltage[index].getX(i));
      }
    }
      //if it is a Temperature, set the correct index for the filter
    else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
      index = messageID - BATTERY_TEMPERATURE_1;
      for(int i =0; i<4; i++){
        data[i] = static_cast<int>(this->cellFiltersTemperature[index].getX(i));
      }
    }
    else{
      //if we get here, we were processing data with an id that does't belong to the batteryNode
      return;
    }


   	checkForError(data, datalen, messageID);


    }

}

void batteryNode::kalmanStep(int data[], int id, int arrLen){
  int index;
  double dataAsDoubles[arrLen];
	Serial.printf("KALMANAN STEP: ID= %x \n", id );
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
      this->cellFiltersVoltage[index].step(dataAsDoubles);

    }

    else if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1+10;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i =0; i< 4; i++){
        currentData[index][i] = data[i];
        dataAsDoubles[i] = static_cast<double>(data[i]);
      }
      this->cellFiltersTemperature[index-10].step(dataAsDoubles);

    }
    else{
      //TODO: Decide what to do for other IDs
      index = 0;
    }

  }

}

void batteryNode::updateStateCalculations(){


	/*

	Calculate State Of Charge

	*/

	int sumOfCharge=0;

	//Go through kalman filters related to cell voltage and sum together all
	for (int i=0;i<CELL_FILTERS_LEN;i++){

		for (int j=0;j<4;j++){

			sumOfCharge+=cellFiltersVoltage[i].getX(j);

		}
	}
	this->stateOfCharge= 100*(sumOfCharge/(40*CUTOFF_VOLTAGE_HIGH)); //Charges divided by total amount possible

	int data[]= {this->stateOfCharge};

	checkForError(data, 1, BATTERY_SOC);
	/*

	Calculate ESR

	*/


}
