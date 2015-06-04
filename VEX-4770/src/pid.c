/*
 * pid.c
 */

#include <math.h>
#include "main.h"

#define ERROR_LOG_LEN 1

typedef struct {
	float Kp;
	float Ki;
	float Kd;

	int lastError;

	float integral;

	int target;

	int previousErrors[ERROR_LOG_LEN];

	int integralThreshold;
} PidState;

void pidAppendError(PidState* state, int error) {
	int i = 0;
	for (; i < ERROR_LOG_LEN - 1; ++i) {
		state->previousErrors[i] = state->previousErrors[i+1];
	}

	state->previousErrors[i] = error;
}

int pidSumErrors(PidState* state) {
	int sum = 0;

	for (int i = 0; i < ERROR_LOG_LEN; ++i) {
		sum += state->previousErrors[i];
	}

	return sum;
}

void pidInitState(PidState* state, float Kp, float Ki, float Kd, int integralThreshold) {
	state->Kp = Kp;
	state->Ki = Ki;
	state->Kd = Kd;

	state->lastError = 0;
	state->integral = 0;
	state->target = 0;

	state->integralThreshold = integralThreshold;

	for (int i = 0; i < ERROR_LOG_LEN; ++i) {
		state->previousErrors[i] = 0;
	}
}

int pidGetTarget(PidState* state) {
	return state->target;
}

void pidSetTarget(PidState* state, int target) {
	if (state->target != target) {
		state->target = target;
		state->integral = 0;
	}
}

void pidIncrementTarget(PidState* state, int amount) {
	pidSetTarget(state, state->target + amount);
}

int pid(PidState* state, int pos, int dt) {
	int error = state->target - pos;

	int previousErrorsAvg = pidSumErrors(state) / ERROR_LOG_LEN;
	pidAppendError(state, error);

	state->integral += (float) (error * dt) / 1000;

	if (signbit(state->lastError) != signbit(error)) {
		state->integral = 0;
	}

	if (fabs(state->integral) > state->integralThreshold) {
		state->integral = 0;
	}

	float derivative = (float) (error - previousErrorsAvg) / dt;
	//printf("Error: %d    Integral: %d    Derivative: %d\r\n", error, (int) state->integral, (int)derivative);

	int output = (int) (state->Kp * error + state->Ki * state->integral + state->Kd * derivative);

	//printf("Output: %d\r\n", output);

	if (output > 127) output = 127;
	if (output < -127) output = -127;

	state->lastError = error;

	return output;
}


