#include "BatteryFilter.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
	
	BatteryFilter ekf;
	
	ofstream before("test2.csv");
	
	srand(time(NULL));
	
	for (int i=0;i<150;i+=5){
		
		double a[3];
		
		if (i%2==0){
			a[0] = i + rand()%200-50;
		}else{
			a[1] = i+ rand()%200-50;
		}
		
		before<<a[0]<<",";
			
		ekf.step(a);
	
		before<<ekf.getX(0)<<","<<ekf.getX(1)<<endl;

	}
	
	before.close();
	
}

