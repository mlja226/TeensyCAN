#include "BatteryFilter.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
	
	BatteryFilter ekf;
	
	ofstream before("test.csv");
	
	srand(time(NULL));
	for (int i=0;i<150;i+=5){
		
		double a= i + rand()%100-50;
		
		before<<a<<",";
			
		ekf.step(&a);
		before<<ekf.getX(0)<<endl;

	}
	
	before.close();
	
}

