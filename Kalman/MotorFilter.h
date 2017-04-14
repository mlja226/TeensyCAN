/* SensorFusion: Sensor fusion on Arduino using TinyEKF.  
 *
 * Copyright (C) 2015 Simon D. Levy
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http:#www.gnu.org/licenses/>.
 */


// These must be defined before including TinyEKF.h
#define Nsta 5  
#define Mobs 5 

#include "TinyEKF.h"

class MotorFilter : public TinyEKF {

    public:

        MotorFilter()
        {            
            // We approximate the process noise using a small constant
            for (int i=0;i<Nsta;i++){
              this->setQ(i, i, .0001);
            }

            // Same for measurement noise
            for (int i=0;i<Mobs;i++){
              this->setR(i, i, .0001);
            }
        }

    protected:

        void model(double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta])
        {
        			for (int i=0;i<Nsta;i++){
        				fx[i] = this-> x[i];
        				F[i][i]=1;
        			}
        
                    // Measurement function
        			for (int i=0;i<Mobs;i++){
        	         	hx[i] = this->x[i]; 
        				H[i][i] = 1;
        			}
        }
			
};



