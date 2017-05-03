/* VoltageFilter.h: Header file to implement the TinyEKF class specifically for a set of 4 cells in the UK solar car battery, particularly pertaining to voltage
 * Edited by: Jacob Miller, 4/12/2017
 * Parent class requires implemention of constructor and model function
 * Modified from SensorFusion.ino:Sensor fusion on Arduino using TinyEKF.
 * Copyright (C) 2015 Simon D. Levy
 */

#ifndef VOLTAGE_CELL_FILTER_H
#define VOLTAGE_CELL_FILTER_H
#define Nsta 4  // four state values- voltages for cells 1-4
#define Mobs 4 // four measurements, one for each cell

#include "TinyEKF.h"

class VoltageCellFilter : public TinyEKF {

public:

  VoltageCellFilter(){
    // We approximate the process noise using a small constant for each value being checked, along the identity matrix
    for (int i=0;i<Nsta;i++){
      this->setQ(i, i, .0001);
    }

    // Same for measurement noise
    for (int i=0;i<Mobs;i++){
      this->setR(i, i, .0001);
    }

  }

protected:
/*
Model method needs to be implemented
Purpose: give an idea of of how we expect our data to behave as well as our measurements, give the update rules for our values
Parameters: fx is our updated values after our function is applied and F is the corresponding jacobian
hx is the associated model for the measurement values and H is the measurement's Jacobian
*/

  void model(double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta]){
    for (int i=0;i<Nsta;i++){
      fx[i] = this-> x[i];//TODO Make a proper function that is not simply constant
      F[i][i]=1;
    }

    // Measurement function
    for (int i=0;i<Mobs;i++){
      hx[i] = this->x[i];
      H[i][i] = 1;
    }

  }

};
#endif
