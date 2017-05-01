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
	Serial.printf("sendMessage: WRITE MESSAGE %x \n", writeMessageID);
    int start=0,end=16; //Fill CAN Message data
    for(int i = 0; i< datalen; i++){
        CANmsg.storeUnsignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
	}
	this->write(CANmsg);
}

void batteryNode::checkForError(int data[], int datalen, uint32_t messageID){
	//Call to sendMessage to ensure that data reporting message (non-error message) is sent
	sendMessage(messageID,data,datalen);
  //BATTERY VOLTAGE ERROR CHECK
  if(messageID >= BATTERY_VOLTAGE_1 && messageID <= BATTERY_VOLTAGE_10){
    //iterate through data to check if there are any readings above/below cutoff
    for(int i = 0; i < datalen; i++){
      if(data[i] >= CUTOFF_VOLTAGE_HIGH ){

			  sendMessage(BATTERY_ERROR_VOLTAGE_HIGH,data, datalen);
	        //return CUTOFF_VOLTAGE_HIGH;
      }
      else if( data[i] <= CUTOFF_VOLTAGE_LOW){
				//TODO: Change message to contain id of cell with error, followed by error causing data
				//There should be a message sent for each cell in error state.
			  sendMessage(BATTERY_ERROR_VOLTAGE_LOW,data, datalen);
	        //return CUTOFF_VOLTAGE_LOW;
      }
    }
  }
  //BATTERY TEMPERATURE ERROR CHECK
  else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
    //iterate through data to check if there are any readings above/below cutoff
    for(int i = 0; i < datalen; i++){
      if(data[i] >= CUTOFF_TEMP_HIGH ){
					sendMessage(BATTERY_ERROR_TEMPERATURE_HIGH, data, datalen);
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
		  sendMessage(BATTERY_WARNING_SOC_LOW, data, datalen);//Send error message
	  }
  }

}

batteryNode::batteryNode(FlexCAN bus) : TeensyNode(bus){
  for(int i =0; i < CURRENT_DATA_ROWS; i++){
    for(int j = 0; j < CURRENT_DATA_COLUMNS; j++){
      this->currentData[i][j] = 0;
    }
  }
	for (int i=0;i<CELL_FILTERS_LEN;i++){

		//this->cellFiltersTemperature[i]=new TemperatureCellFilter();
		//this->cellFiltersVoltage[i]= new VoltageCellFilter();
	}
}
void batteryNode::interpretData(uint32_t messageID){
  int index, datalen = 4;
	int global_index = messageID - BATTERY_VOLTAGE_1;
  //CANMessage CANmsg;

  if(messageID == BATTERY_BC_AC_BP_AP){
  for(int i =0; i<4; i++){
	  currentData[global_index][i] = static_cast<int>(currentFilter.getX(i));
  }

		checkForError(currentData[global_index], datalen, BATTERY_BC_AC_BP_AP);
  }
  else{
    //if it is a Voltage, set the correct index for the filter
    if(messageID >=BATTERY_VOLTAGE_1 && messageID <= BATTERY_VOLTAGE_10){
      index = messageID - BATTERY_VOLTAGE_1;
      for(int i =0; i<4; i++){
      	currentData[global_index][i] = static_cast<int>(this->cellFiltersVoltage[index].getX(i));
      }
    }
      //if it is a Temperature, set the correct index for the filter
    else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
      index = messageID - BATTERY_TEMPERATURE_1;
      for(int i =0; i<4; i++){
        currentData[global_index][i] = static_cast<int>(this->cellFiltersTemperature[index].getX(i));
      }
    }
    else{
      //if we get here, we were processing data with an id that does't belong to the batteryNode
      return;
    }


   	checkForError(currentData[global_index], datalen, messageID);


    }

}

void batteryNode::kalmanStep(int data[], int id, int arrLen){
  int index;
	int global_index = id - BATTERY_VOLTAGE_1;
  double dataAsDoubles[arrLen];
  if(id == BATTERY_BC_AC_BP_AP){
    for(int i = 0; i< 4; i++){
			dataAsDoubles[i] = static_cast<double>(data[i]);
			if(currentData[global_index][i] == 0)
				this->currentFilter.setX(i, dataAsDoubles[i]);
    }
    this->currentFilter.step(dataAsDoubles);
  }
  else{
    if(id >=BATTERY_VOLTAGE_1 && id <= BATTERY_VOLTAGE_10){
      index = id - BATTERY_VOLTAGE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i = 0; i< 4; i++){
        dataAsDoubles[i] = static_cast<double>(data[i]);
				if(currentData[global_index][i] == 0)
					this->cellFiltersVoltage[index].setX(i, dataAsDoubles[i]);
      }
      this->cellFiltersVoltage[index].step(dataAsDoubles);

    }

    if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1+10;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i =0; i< 4; i++){
				Serial.printf("\n index: %d = %d", index,currentData[index][i] );
        dataAsDoubles[i] = (data[i]);
				if(currentData[global_index][i] == 0)
					this->cellFiltersTemperature[index-10].setX(i, dataAsDoubles[i]);
				Serial.printf("Data after cast %d \n", int(dataAsDoubles[i]));
      }
			Serial.printf("Address of cellfilter %x\n", &(this->cellFiltersTemperature[index-10]));
      this->cellFiltersTemperature[index-10].step(dataAsDoubles);
			Serial.printf("After Kalman Step: data[0]: %d data[1]: %d data[2]: %d data[3]: %d", (int)this->cellFiltersTemperature[index-10].getX(0),(int)this->cellFiltersTemperature[index-10].getX(1),(int)this->cellFiltersTemperature[index-10].getX(2),(int)this->cellFiltersTemperature[index-10].getX(3));
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

	double minCharge= CUTOFF_VOLTAGE_HIGH;

	//Go through kalman filters related to cell voltage and sum together all
	for (int i=0;i<CELL_FILTERS_LEN;i++){

		for (int j=0;j<4;j++){

			if (cellFiltersVoltage[i].getX(j)!=0 && cellFiltersVoltage[i].getX(j)< minCharge){

					minCharge= cellFiltersVoltage[i].getX(j);
			}
		}
	}

	this->stateOfCharge= (int)(100*(minCharge/(CUTOFF_VOLTAGE_HIGH))); //Charges divided by total amount possible

	int data[]= {this->stateOfCharge};

	checkForError(data, 1, BATTERY_SOC);

	/*

	Calculate ESR


	*/

}
