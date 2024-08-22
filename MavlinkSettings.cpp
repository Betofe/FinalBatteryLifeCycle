// 
// 
// 

#include "MavlinkSettings.h"
#include "StorageSettings.h"

/*          DO NOT FORGET TO SET SERIAL2 PROTOCOL TO MAVLINK 2 AND BAUDRATE TO 57600	   */

HardwareSerial CUBE_SERIAL(1);
Storage storage_;

void MavlinkConnection::init() {
	CUBE_SERIAL.begin(BAUD_RATE, SERIAL_PROTOCOL, CUBE_SERIAL_RX, CUBE_SERIAL_TX);
	fmav_init();
	debug_flag = false;
	prev_millis = 0;
	cycle_to_display = EEPROM.read(0);
	convertToArray();
}

uint16_t MavlinkConnection::serialAvailable() {
	uint16_t available = CUBE_SERIAL.available();
	return (available > 0) ? available : 0;
}

void MavlinkConnection::readSerial(uint8_t* c) {
	*c = CUBE_SERIAL.read();
}

uint8_t MavlinkConnection::availSerialBuff(uint16_t counter) {
	return (CUBE_SERIAL.availableForWrite() >= counter) ? 1 : 0;
}

void MavlinkConnection::writeToSerial(uint8_t* buf, uint16_t len) {
	for (uint16_t i = 0; i < len; i++) {
		CUBE_SERIAL.write(buf[i]);
	}
}

//uint8_t MavlinkConnection::checkMessage(fmav_message_t* msg) {
//	// Message either has no target_sysid or is broadcast, accept
//	if (msg->target_sysid == 0) return 1;
//
//	// Message has a target_sysid but it is not ours, reject
//	if (msg->target_sysid != SYS_ID) return 0;
//
//	// Message either has no target_compid or is broadcast, accept
//	if (msg->target_compid == 0) return 1;
//
//	// Message has a target_compid and it is ours, accept
//	if (msg->target_compid == SYS_ID) return 1;
//
//	// Message has a target_compid but it is not ours, so reject
//	return 0;
//}

uint8_t MavlinkConnection::requestParameters() {
	uint16_t counter = fmav_msg_request_data_stream_pack(
		&msg,
		SYS_ID,
		COMP_ID,
		VEHICLE_TYPE,
		NULL,
		DATA_STREAM,
		REQUEST_RATE,
		START_STOP_VALUE,
		&status
	);

	if (availSerialBuff(counter)) {
		fmav_msg_to_frame_buf(tx_buf, &msg);
		writeToSerial(tx_buf, counter);
		return 1;
	}
	return 0;
}

void MavlinkConnection::handleMessage(fmav_message_t* msg) {
	switch (msg->msgid) {

	case FASTMAVLINK_MSG_ID_HEARTBEAT: {
		fmav_heartbeat_t data;
		fmav_msg_heartbeat_decode(&data, msg);
		if (data.autopilot == MAV_AUTOPILOT_INVALID && data.type == MAV_TYPE_GCS) {
			debug_flag = true;
		}
	}break;

	default:
		break;
	}
}

uint8_t MavlinkConnection::groundControlDebug() {
	fmav_statustext_t data;
	data.severity = MAV_SEVERITY_INFO;
	memset(&data.text, 0, FASTMAVLINK_MSG_STATUSTEXT_FIELD_TEXT_LEN);
	memcpy(&data.text, status_array, sizeof(status_array));
	data.id = 0;
	data.chunk_seq = 0;
	uint16_t count = fmav_msg_statustext_encode_to_frame_buf(
		tx_buf,
		SYS_ID,
		STATUS_COMP_ID,
		&data,
		&status);
	if (availSerialBuff(count)) {
		writeToSerial(tx_buf, count);
		return 1;
	}
	return 0;
}

void MavlinkConnection::decodeMessage() {
	uint16_t available = serialAvailable();
	for (uint16_t i = 0; i < available; i++) {
		uint8_t c;
		readSerial(&c);

		uint8_t res = fmav_parse_to_msg(&msg, &status, c);

		if (res == FASTMAVLINK_PARSE_RESULT_OK) {

			if (res == FASTMAVLINK_PARSE_RESULT_OK) {
				if (fmav_msg_is_for_me(SYS_ID, COMP_ID, &msg)) {
					handleMessage(&msg);
				}
			}
		}
	}
}


uint8_t MavlinkConnection::sendHeartbeat(void)
{
	// Here we don't use the payload structure and xxx_encode() function, but rather pack the data
	// directly into the tx working buffer using the xxx_pack_to_frame_buf() function. Just to show also this.

	uint16_t count = fmav_msg_heartbeat_pack_to_frame_buf(
		tx_buf,
		SYS_ID,
		COMP_ID,
		VEHICLE_TYPE,
		AUTOPILOT,
		MODE_FLAG,
		0,
		STATE,
		&status);

	if (availSerialBuff(count)) {
		writeToSerial(tx_buf, count);
		return 1;
	}

	return 0;
}

void MavlinkConnection::run() {
	//requestParameters();
	decodeMessage();
	if (debug_flag) {
		if (groundControlDebug()) {
			debug_flag = false;
		}
	}

	uint32_t curr_millis = millis();
	if ((curr_millis - prev_millis) > 1000) {
		if (sendHeartbeat()) {
			prev_millis += 1000;
		}
	}
}

void MavlinkConnection::convertToArray() {
	String cycle_status = "Lifecycle: ";
	cycle_status += cycle_to_display;
	cycle_status.toCharArray(status_array, STATUS_ARRAY_SIZE);
}
