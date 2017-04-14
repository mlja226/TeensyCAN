/* BatteryFilter.h: Header file to implement the TinyEKF class specifically for the battery in the UK solar car
 * Edited by: Jacob Miller, 4/12/2017
 * Parent class requires implemention of constructor and model function
 * Modified from SensorFusion.ino:Sensor fusion on Arduino using TinyEKF. 
 * Copyright (C) 2015 Simon D. Levy
 */


#ifndef BATTERY_FILTER_H
#define BATTERY_FILTER_H

#define Nsta 4  // Four values are being monitored by this EKF:battery current, array current, battery power, and array power
#define Mobs 4 // There is only one measurement being used per value being monitored

#include "TinyEKF.h"

class BatteryFilter : public TinyEKF {

    public:

        BatteryFilter()
        {            
            // We approximate the process noise using a small constant fro each value being checked, along the identity matrix
            for (int i=0;i<Nsta;i++){
              
			  this->setQ(i, i, .0001);
            }

            // We also approximated the measurement noise in a similar fashion
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
        void model(double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta])
        {

			for (int i=0;i<Nsta;i++){
				
				fx[i] = this-> x[i]; //function is assumed to be constant
				F[i][i]=1; // Jacobian is the identity
			}

            // Measurement function update rules
			for (int i=0;i<Mobs;i++){
		     	hx[i] = this->x[i];  //Assume function is constant and Jacobian is identity
				H[i][i] = 1;
			}
        }
			
};

#endif


