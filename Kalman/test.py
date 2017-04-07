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
#define Nsta 2     // Two state values: pressure, temperature
#define Mobs 3     // Three measurements: baro pressure, baro temperature, LM35 temperature

#define LM35_PIN 0

#include "TinyEKF.h"
#include "Wire.h"

class Battery : public TinyEKF {

    public:

        Battery()
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
			
			for (int i=0;i<Nsta;i++){
				fx[i] = this-> x[i];
				F[i]=1;
			}

            // Measurement function
			for (int i=0;i<Mobs;i++){
	            hx[i] = this->x[i]; 
				H[i] = 1;
			}
			
};

Battery ekf;

void setup() {

    Serial.begin(9600);

    // Set up to read from LM35
    analogReference(INTERNAL);
	
}

void loop() {

    // Read pressure, temperature from BMP180
    double baroTemperature, baroPressure;
	
    // Read temperature from LM35
    float lm35Temperature = analogRead(LM35_PIN) / 9.31;

    // Send these measurements to the EKF
    double z[3] = {baroPressure, baroTemperature, lm35Temperature};
    ekf.step(z);

    // Report measured and predicted/fused values
    Serial.print(z[0]);
    Serial.print(" ");
    Serial.print(z[1]);
    Serial.print(" ");
    Serial.print(z[2]);
    Serial.print(" ");
    Serial.print(ekf.getX(0));
    Serial.print(" ");
    Serial.println(ekf.getX(1));
	
}