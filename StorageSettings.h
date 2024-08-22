#pragma once

#include <EEPROM.h>

#define DEBUG_PORT Serial
#define ADDRESS_LOCATION 0
#define EEPROM_SIZE 1

class Storage {
private:

	bool trigger1;
	bool trigger2;

	int voltage_percentage;

	void updateEeprom();

public:

	int curr_cycle;

	bool ground_trigger;

	void init();
	void check();
};