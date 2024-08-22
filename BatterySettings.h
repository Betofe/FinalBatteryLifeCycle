#pragma once

#include <Arduino.h>

#define SENSOR_PIN 35

class Battery {
private:
	float filtered_voltage;

public:
	int analog_value;
	void init();
	float readVoltage();
	int getPercentage();
};