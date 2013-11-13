/*
 * pid.h
 */

#ifndef PID_H_
#define PID_H_

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

void pidInitState(PidState* state, float Kp, float Ki, float Kd, int integral_threshold);

int pidGetTarget(PidState* state);

void pidSetTarget(PidState* state, int target);

void pidIncrementTarget(PidState* state, int amount);

int pid(PidState* state, int pos, int dt);

#endif /* PID_H_ */
