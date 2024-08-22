// 
// 
// 

#include "BatterySettings.h"
#include "FilterSettings.h"
#include "StorageSettings.h"

KalmanFilter filter;

void Battery::init() {
	filter.init();
	analog_value = 0;
}

float Battery::readVoltage() {
	analog_value = analogRead(SENSOR_PIN);
	return (((analog_value * 3.3) / 4095) * 2.55); // P35 i a 12 bits pin, thus 4095;
}

int Battery::getPercentage() {
	filtered_voltage = filter.run();
	return map(filtered_voltage, 0, 8.4, 0, 100);
}