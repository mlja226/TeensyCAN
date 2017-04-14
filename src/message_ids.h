/**
 * University of Kentucky CAN network definition
 *
 * Notes...
 * > All CAN messages are assumed to have a 64bit or 8byte data packet.
 *
 * Network Summary...
 * 0x000 - 0x0FF:
 * 0x100 - 0x1FF: Small bottom shell subsystems
 * 0x200 - 0x2FF: Array
 * 0x300 - 0x3FF: Battery
 * 0x400 - 0x4FF: Motor controller
 * 0x500 - 0x5FF: Driver control box
 * 0x600 - 0x6FF: Chasis
 * 0x700 - 0xFFF:
 */

// Begin include guard
#ifndef MESSAGES_H
#define  MESSAGES_H

#include <stdint.h>
//current reading period in multiples of .7989
#define CURRENT_DELAY_MULTIPLIER 1
#define BATTERY_INTERNAL_RESISTANCE_MILLIOHMS 5
//CBS constants
#define CBS_CUTOFF_OFFSET 2500
// CBS interval = multiplier * .189787
// 120 is roughly 33 seconds
#define CBS_DELAY_MULTIPLIER 120

//assume around 200mV voltage increase to compensate for CBS
#define CBS_VOLTAGE_DROP 2000

//battery voltage cutoff points (in Volts * 10000)
#define	CUTOFF_VOLTAGE_HIGH	36490
#define	CUTOFF_VOLTAGE_LOW	26510
//#define	CUTOFF_VOLTAGE_LOW	0
//#define	CUTOFF_VOLTAGE_HIGH	50000

//battery current cutoff points (in milli-Amps)
#define	CUTOFF_CURRENT_HIGH	 80000
#define	CUTOFF_CURRENT_LOW	(-40000)

//battery temperature cutoff point (in Celcius)
#define CUTOFF_TEMP_HIGH	        45
#define CUTOFF_TEMP_HIGH_DISCHARGE	60

//array cutoff point (in Volts * 10000)
#define ARRAY_CUTON		        34500
#define ARRAY_CUTOFF	        35500
#define ARRAY_CUTON_CURRENT		 3000
#define ARRAY_CUTOFF_CURRENT	 1000
#define ARRAY_CUTOFF_TEMP	       43
/**
 * Driver Display
 * 0x100 - 0x10F
 */
#define DISPLAY_SET_VALUES 0x100 // ( SevenSeg display 1 value IEEE (32bit) float )( SevenSeg display 2 value IEEE (32bit) float )


/**
 * GPS
 * 0x110 - 0x11F
 */
 #define GPS_MESSAGE_1 0x110
 #define GPS_MESSAGE_2 0x111


/**
 * Accelerometer
 * 0x120 - 0x12F
 */



/**
 * Gyro
 * 0x130 - 0x13F
 */



/**
 * Array
 * 0x200 - 0x2FF
 */
 #define ARRAY_SIDE_POWER_1 0x200 // ( Array Side Power 16bits x 4 )
 #define ARRAY_SIDE_POWER_2 0x201 // ...

 #define ARRAY_BATTERY_SIDE_POWER_1 0x202 // ( Battery Side Power 16bits x 4 )
 #define ARRAY_BATTERY_SIDE_POWER_2 0x203 // ...

 #define ARRAY_MPPT_STATUS 0x204 // ( Mppt Status 8bits x 8 )

 #define ARRAY_ILLUMINANCE 0x205 // ( Illuminance 16bits )( Padding 48bits )

 #define ARRAY_TEMPERATURE 0x206 // ( Temperature 8bits x 8 )
 #define ARRAY_TEMPERATURE 0x206 // ...


/**
 * Battery
 * 0x300 - 0x3FF
 */
#define BATTERY_VOLTAGE_1 0x300 // ( volt 1 - 16bits )( volt 2 - 16bits )( volt 3 - 16bits )( volt 4 - 16bits )
#define BATTERY_VOLTAGE_2 0x301 // ( volt 5 - 16bits )( volt 6 - 16bits )( volt 7 - 16bits )( volt 8 - 16bits )
#define BATTERY_VOLTAGE_3 0x302 // ...
#define BATTERY_VOLTAGE_4 0x304
#define BATTERY_VOLTAGE_5 0x305
#define BATTERY_VOLTAGE_6 0x306
#define BATTERY_VOLTAGE_7 0x307
#define BATTERY_VOLTAGE_8 0x308
#define BATTERY_VOLTAGE_9 0x309
#define BATTERY_VOLTAGE_10  0x30A

#define BATTERY_TEMPERATURE_1 0x30B // ( temp 1 - 16bits )( temp 2 - 16bits )( temp 3 - 16bits )( temp 4 - 16bits )
#define BATTERY_TEMPERATURE_2 0x30C // ( temp 5 - 16bits )( temp 6 - 16bits )( temp 7 - 16bits )( temp 8 - 16bits )
#define BATTERY_TEMPERATURE_3 0x30D // ...
#define BATTERY_TEMPERATURE_4 0x30E
#define BATTERY_TEMPERATURE_5 0x30F
#define BATTERY_TEMPERATURE_6 0x310
#define BATTERY_TEMPERATURE_7 0x311
#define BATTERY_TEMPERATURE_8 0x312
#define BATTERY_TEMPERATURE_9 0x313
#define BATTERY_TEMPERATURE_10  0x314

#define BATTERY_BC_AC_BP_AP 0x315 // ( Battery Current 16bits )( Array Current 16bits )( Battery Power 16bits )( Array Power 16bits )

#define BATTERY_CBS_1 0x316 // ( CBS State 2bits x 20 )( Padding 24bits )
#define BATTERY_CBS_2 0x317 // ...

#define BATTERY_5V_12V_BUS 0x318 // ( Battery 5V Bus 16bits )( Battery 12V Bus 16bits )( Padding 32bits )

#define BATTERY_CHARGE_DISCHARGE_ENERGY 0x319 // ( Charge Energy Remaining 24bits )( Padding 8bits )( Discharge Energy Remaining 24bits )( Padding 8bits )

#define BATTERY_CMD_CURRENT_LIMITS_AND_ERRORS 0x31A // ( Command Current Limits 16bits )( Padding 16bits )( Errors 32bits )

#define BATTERY_ESR_1 0x31B // ( Module ESR 8bits x 8 )
#define BATTERY_ESR_2 0x31C // ...
#define BATTERY_ESR_3 0x31D
#define BATTERY_ESR_4 0x31E
#define BATTERY_ESR_5 0x31F


/**
 * Motor Controller
 *
 * Information pulled from tritium's datasheet. It can be found at the following link.
 * http://tritium.com.au/wp-content/uploads/2012/07/TRI50.008_Communications_Specification_v5.pdf
 *
 * 0x400 - 0x4FF
 */
#define MOTOR_IDENTIFICATION_INFORMATION 0x400

#define MOTOR_STATUS_INFORMATION 0x401

#define MOTOR_BUG_MEASUREMENT 0x402

#define MOTOR_VELOCITY_MEASUREMENT 0x403

#define MOTOR_PHASE_CURRENT_MEASUREMENT 0x404

#define MOTOR_VOLTAGE_VECTOR_MEASUREMENT 0x405

#define MOTOR_CURRENT_VECTOR_MEASUREMENT 0x406

#define MOTOR_BACKEMF_MEASUREMENT_PREDICTION 0x407

#define MOTOR_VOLTAGE_RAIL_MEASUREMENT_1 0x408

#define MOTOR_VOLTAGE_RAIL_MEASUREMENT_2 0x409

#define MOTOR_FAN_SPEED_MEASUREMENT 0x40A

#define MOTOR_SINK_AND_MOTOR_TEMPERATURE 0x40B

#define MOTOR_AIR_IN_AND_CPU_TEMPERATURE 0x40C

#define MOTOR_AIR_OUT_AND_CAP_TEMPERATURE 0x40D

#define MOTOR_ODOMETER_AND_BUS_AMP_HOURS 0x40E



/**
 * Driver Controller
 * 0x500 - 0x5FF
 */
#define MOTOR_DRIVE_COMMAND 0x501 // ( Motor Current 32bits )( Motor Velocity 32bits )

#define MOTOR_POWER_COMMAND 0x502 // ( Bus Current 32bits )( Reserved 32bits )

#define MOTOR_RESET_COMMAND 0x503 // ( Unused 64bits )



/**
 * Chasis
 * 0x600 - 0x6FF
 */



// End include guard
 #endif
