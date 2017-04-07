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
#define Nsta 40     // Two state values: pressure, temperature
#define Mobs 40    // Three measurements: baro pressure, baro temperature, LM35 temperature

#define LM35_PIN 0

#include "TinyEKF.h"
class Fuser : public TinyEKF {

    public:

        Fuser()
        {            
            // We approximate the process noise using a small constant
            this->setQ(0, 0, .0001);
            this->setQ(1, 1, .0001);

            // Same for measurement noise
            this->setR(0, 0, .0001);
            this->setR(1, 1, .0001);
            this->setR(2, 2, .0001);
        }

    protected:

        void model(double fx[Nsta], double F[Nsta][Nsta], double hx[Mobs], double H[Mobs][Nsta])
        {
            // Process model is f(x) = x
            fx[0] = this->x[0];
            fx[1] = this->x[1];

            // So process model Jacobian is identity matrix
            F[0][0] = 1;
            F[1][1] = 1;

            // Measurement function
            hx[0] = this->x[0]; // Barometric pressure from previous state
            hx[1] = this->x[1]; // Baro temperature from previous state
            hx[2] = this->x[1]; // LM35 temperature from previous state

            // Jacobian of measurement function
            H[0][0] = 1;        // Barometric pressure from previous state
            H[1][1] = 1 ;       // Baro temperature from previous state
            H[2][1] = 1 ;       // LM35 temperature from previous state
        }
};

Fuser ekf;
int main(){
	
}
