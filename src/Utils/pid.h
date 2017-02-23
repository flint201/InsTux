#pragma once
class PID
{
public:
	PID(double p, double i, double d);
	void setParam(double p, double i, double d);
	void clear();
	float step(float x);

	float prev_err;
	float integral;

	float kp;
	float ki;
	float kd;

	float last_step;
};

