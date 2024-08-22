#include "MavlinkSettings.h"
#include "FilterSettings.h"
#include "BatterySettings.h"
#include "StorageSettings.h"

Storage storage;
MavlinkConnection mavlink;

void setup() {
	storage.init();
	mavlink.init();
}

// the loop function runs over and over again until power down or reset
void loop() {
	storage.check();
	mavlink.run();
}
