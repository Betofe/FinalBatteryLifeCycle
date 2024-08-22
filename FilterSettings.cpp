// 
// 
// 

#include "FilterSettings.h"
#include "BatterySettings.h"
#include "StorageSettings.h"

Battery battery_;

void KalmanFilter::init() {
	initial_voltage = battery_.readVoltage();
	Xe = initial_voltage;
	Pc = 0.0;
	kalman_gain = 0.0;
	covar_error = 1.0;
	Xp = 0.0;
	Zp = 0.0;
}

float KalmanFilter::run() {
	new_voltage = battery_.readVoltage();
	Pc = covar_error + PROCESS_VARIANCE;
	kalman_gain = Pc / (Pc + VOLTAGE_VARIANCE);    // kalman gain
	covar_error = (1 - kalman_gain) * Pc;
	Xp = Xe;
	Zp = Xp;
	Xe = kalman_gain * (new_voltage - Zp) + Xp;
	//DEBUG_PORT.println(Xe);
	return Xe;
	
}