#pragma once

#define VOLTAGE_VARIANCE 1.12184278324081E-05
#define PROCESS_VARIANCE 1E-8

class KalmanFilter {
private:
	float Pc;
	float kalman_gain;
	float covar_error;
	float Xp; // Next state predictor
	float Zp; // Next state predictor
	float initial_voltage;
	float new_voltage;

public:
	float Xe; // Kalman Estimation
	void init();

	float run();
};