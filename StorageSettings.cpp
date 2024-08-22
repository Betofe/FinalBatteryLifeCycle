// 
// 
// 

#include "StorageSettings.h"
#include "BatterySettings.h"

Battery battery;

void Storage::init() {
	DEBUG_PORT.begin(57600);
	EEPROM.begin(EEPROM_SIZE);
	curr_cycle = EEPROM.read(ADDRESS_LOCATION);
	DEBUG_PORT.print("Current cycle: ");
	DEBUG_PORT.println(curr_cycle);
	trigger1 = false;
	trigger2 = false;
	battery.init();
	ground_trigger = false;
}

void Storage::updateEeprom() {
		curr_cycle += 1;
		EEPROM.write(ADDRESS_LOCATION, curr_cycle);
		EEPROM.commit();
		DEBUG_PORT.print("New cycle: ");
		DEBUG_PORT.println(curr_cycle);

		
}

void Storage::check() {
	voltage_percentage = battery.getPercentage();

	if (voltage_percentage >= 60 && voltage_percentage < 80) {
		trigger1 = true;
	} 

	if (voltage_percentage >= 80 && voltage_percentage <= 100) {
		trigger2 = true;
	}

	if ((trigger1 == true) && (trigger2 == true) && voltage_percentage >= 90) {
		updateEeprom();
		trigger1 = false;
		trigger2 = false;
		ESP.restart();
	}
	
}
