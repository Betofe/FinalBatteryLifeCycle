// MavlinkSettings.h
#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include "Libraries/fastmavlink/c_library/common/common.h"
#include "Libraries/fastmavlink/c_library/common/common_msg_entries.h"

// GLOBAL
#define SYS_ID 1
#define COMP_ID MAV_COMP_ID_AUTOPILOT1
#define STATUS_COMP_ID MAV_COMP_ID_PERIPHERAL
#define REQUEST_RATE 2
#define START_STOP_VALUE 1
#define VEHICLE_TYPE MAV_TYPE_GENERIC
#define DATA_STREAM MAV_DATA_STREAM_ALL
#define MODE_FLAG MAV_MODE_FLAG_SAFETY_ARMED
#define AUTOPILOT MAV_AUTOPILOT_INVALID
#define STATE MAV_STATE_ACTIVE


#define DEBUG
#define PC_DEBUG

#define SERIAL_PROTOCOL SERIAL_8N1
#define CUBE_SERIAL_RX 18
#define CUBE_SERIAL_TX 19
#define BAUD_RATE 57600

#define STATUS_ARRAY_SIZE 15

class MavlinkConnection {
private:
	// Required variables
	fmav_status_t status;
	uint8_t rx_buf[296];
	uint8_t tx_buf[296];
	fmav_message_t msg;

	uint32_t prev_millis;

	bool debug_flag;

	int cycle_to_display;

	char status_array[STATUS_ARRAY_SIZE];

	/*         METHODS             */

	uint16_t serialAvailable();

	void readSerial(uint8_t* c);

	uint8_t availSerialBuff(uint16_t counter);

	void writeToSerial(uint8_t* t, uint16_t len);

	void handleMessage(fmav_message_t* msg);

	uint8_t groundControlDebug();

	uint8_t requestParameters();

	void decodeMessage();

	void convertToArray();

	uint8_t sendHeartbeat();

public:

	void init();

	void run();
};