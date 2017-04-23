#ifndef BATTERY_NODE_H
#define BATTERY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "BatteryCellFilter.h"
#define CURRENT_DATA_ROWS 20
#define CURRENT_DATA_COLUMNS 4
#define CELL_FILTERS_LEN 20

class batteryNode : public TeensyNode {
private:
BatteryCellFilter cellFilters[CELL_FILTERS_LEN]; //First 10 are the temperatures for cells 1-40, second 10 are voltages

//TODO Separate cellFilters for voltage vs temperature

BatteryFilter currentFilter;
int currentData[CURRENT_DATA_ROWS][CURRENT_DATA_COLUMNS];

int stateOfCharge, batteryESR;



public:
  batteryNode();
  void interpretData( uint32_t messageID);

  void kalmanStep(int data[], int id, int arrLen);
  void  checkForError(int data[], int datalen, uint32_t messageID);
  void sendMessage(uint32_t writeMessageID, int data[], int datalen);
  
  void updateStateCalculations();


};
#endif
