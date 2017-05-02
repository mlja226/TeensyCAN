import re, time, struct, subprocess, sys, os, random, math
from random import randrange
import codecs
from struct import *
import binascii
'''
/**
 * Array
 * 0x200 - 0x2FF
 */
'''
ARRAY_SIDE_POWER_1 = "200" # ( Array Side Power 16bits x 4 )
ARRAY_SIDE_POWER_2 = "201" # ...

ARRAY_BATTERY_SIDE_POWER_1 = "202" # ( Battery Side Power 16bits x 4 )
ARRAY_BATTERY_SIDE_POWER_2 = "203" # ...

ARRAY_MPPT_STATUS = "204" # ( Mppt Status 8bits x 8 )

ARRAY_ILLUMINANCE = "205" #( Illuminance 16bits )( Padding 48bits )

ARRAY_TEMPERATURE ="206" #( Temperature 8bits x 8 )

#Battery ID's
BATTERY_VOLTAGE_1 = "300" #( volt 1 - 16bits )( volt 2 - 16bits )( volt 3 - 16bits )( volt 4 - 16bits )
BATTERY_VOLTAGE_2 = "301" #( volt 5 - 16bits )( volt 6 - 16bits )( volt 7 - 16bits )( volt 8 - 16bits )
BATTERY_VOLTAGE_3 = "302" # ...
BATTERY_VOLTAGE_4 = "303"
BATTERY_VOLTAGE_5 = "304"
BATTERY_VOLTAGE_6 = "305"
BATTERY_VOLTAGE_7 = "306"
BATTERY_VOLTAGE_8 = "307"
BATTERY_VOLTAGE_9 = "308"
BATTERY_VOLTAGE_10 ="309"

CUTOFF_VOLTAGE_HIGH = 36490
CUTOFF_VOLTAGE_LOW = 26510

BATTERY_TEMPERATURE_1 = "30A" # ( temp 1 - 16bits )( temp 2 - 16bits )( temp 3 - 16bits )( temp 4 - 16bits )
BATTERY_TEMPERATURE_2 = "30B" # ( temp 5 - 16bits )( temp 6 - 16bits )( temp 7 - 16bits )( temp 8 - 16bits )
BATTERY_TEMPERATURE_3 = "30C"#...
BATTERY_TEMPERATURE_4 = "30D"
BATTERY_TEMPERATURE_5 = "30E"
BATTERY_TEMPERATURE_6 = "30F"
BATTERY_TEMPERATURE_7 = "310"
BATTERY_TEMPERATURE_8 = "311"
BATTERY_TEMPERATURE_9 = "312"
BATTERY_TEMPERATURE_10 ="313"

CUTOFF_TEMP_HIGH = 45


BATTERY_BC_AC_BP_AP = "315" # ( Battery Current 16bits )( Array Current 16bits )( Battery Power 16bits )( Array Power 16bits )

BATTERY_CBS_1 = "316" #( CBS State 2bits x 20 )( Padding 24bits )
BATTERY_CBS_2 = "317" # ...

BATTERY_5V_12V_BUS = "318" # ( Battery 5V Bus 16bits )( Battery 12V Bus 16bits )( Padding 32bits )

BATTERY_CHARGE_DISCHARGE_ENERGY = "319" # ( Charge Energy Remaining 24bits )( Padding 8bits )( Discharge Energy Remaining 24bits )( Padding 8bits )

BATTERY_CMD_CURRENT_LIMITS_AND_ERRORS = "31A" # ( Command Current Limits 16bits )( Padding 16bits )( Errors 32bits )

BATTERY_ESR_1 = "31B" # ( Module ESR 8bits x 8 )
BATTERY_ESR_2 = "31C" # ...
BATTERY_ESR_3 = "31D"
BATTERY_ESR_4 = "31E"
BATTERY_ESR_5 = "31F"
#Motor Drive ID's
MOTOR_DRIVE_COMMAND = "501" #( Motor Current 32bits )( Motor Velocity 32bits )
MOTOR_POWER_COMMAND = "502" # ( Bus Current 32bits )( Reserved 32bits )
MOTOR_RESET_COMMAND = "503" # ( Unused 64bits )

def generate_voltage_message(voltageID, currentVoltage):
	randomVoltageConst = random.randrange(0, 100)
	voltageMessage1 = 0x00000000
	voltageMessage2 = 0x00000000
	#Get voltages within constant of each other
	voltage1 = (random.randrange(currentVoltage-randomVoltageConst-1, currentVoltage+ randomVoltageConst))
	voltage2 = (random.randrange(currentVoltage-randomVoltageConst-1, currentVoltage+ randomVoltageConst))
	voltage3 = (random.randrange(currentVoltage-randomVoltageConst-1, currentVoltage+ randomVoltageConst))
	voltage4 = (random.randrange(currentVoltage-randomVoltageConst-1, currentVoltage+ randomVoltageConst))
	voltageMessage1 = voltageMessage1 | voltage1<<16
	voltageMessage1 = voltageMessage1 | voltage2
	voltageMessage2 = voltageMessage2 | voltage3<<16
	voltageMessage2 = voltageMessage2 | voltage4
	message = "T00000"+voltageID+"8"+struct.pack('<I',voltageMessage1).encode('hex')+struct.pack('<I',voltageMessage2).encode('hex')+ " "+ str(voltage1)+ " "+ str(voltage2) + " "+ str(voltage3) + " "+ str(voltage4) +"\r"
	return str(message)

def generate_temperature_message(tempID, currentTemp):
	tempMessage1 = 0x00000000
	tempMessage2 = 0x00000000
	temp1 = random.randrange(currentTemp-1, currentTemp+1)
	temp2 = random.randrange(currentTemp-1, currentTemp+1)
	temp3 = random.randrange(currentTemp-1, currentTemp+1)
	temp4 = random.randrange(currentTemp-1, currentTemp+1)
	tempMessage1 = tempMessage1 | temp1<<16
	tempMessage1 = tempMessage1 | temp2
	tempMessage2 = tempMessage2 | temp3<<16
	tempMessage2 = tempMessage2 | temp4
	message = "T00000"+tempID +"8"+struct.pack('<I',tempMessage1).encode('hex')+struct.pack('<I',tempMessage2).encode('hex') + " "+str(temp1)+ " "+ str(temp2) + " "+ str(temp3) + " "+ str(temp4)+"\r"
	return message
def make_array_file(array_file):
        for i in range(0,10):
                array_file.write("T00000")

def make_battery_file( battery_file ):
	voltageIDs = [BATTERY_VOLTAGE_1,BATTERY_VOLTAGE_2, BATTERY_VOLTAGE_3, BATTERY_VOLTAGE_4, BATTERY_VOLTAGE_5, BATTERY_VOLTAGE_6, BATTERY_VOLTAGE_7, BATTERY_VOLTAGE_8, BATTERY_VOLTAGE_9, BATTERY_VOLTAGE_10]
	tempIDs = [BATTERY_TEMPERATURE_1,BATTERY_TEMPERATURE_2,BATTERY_TEMPERATURE_3,BATTERY_TEMPERATURE_4,BATTERY_TEMPERATURE_5,BATTERY_TEMPERATURE_6,BATTERY_TEMPERATURE_7,BATTERY_TEMPERATURE_8,BATTERY_TEMPERATURE_9,BATTERY_TEMPERATURE_10]

	#Assuming voltage begins slightly below max cutoff
	currVoltage = 30000.0
	#Voltage will decrease linearly
	#Start battery temperature at roughly 70 deg F

	currTemp = 21.0
	tempIncrease = 0.001
	#Temperature should increase linearly

	#Create messages for each Voltage and temperature
	for i in range(0, 10):
		currTemp += tempIncrease
		for x in range(0,len(voltageIDs)):
			currVoltage = currVoltage - 1
			voltageMessage = generate_voltage_message(voltageIDs[x], int(currVoltage))
			tempMessage = generate_temperature_message(tempIDs[x], int(currTemp))
			battery_file.write(voltageMessage)
			battery_file.write(tempMessage)
			#TODO: Determine what readings for ESR should be.
	battery_file.flush()

battery_file = open("battery_data.txt","w")
make_battery_file(battery_file)
