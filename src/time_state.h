#ifndef TIME_STATE_H
#define TIME_STATE_H


typedef struct TimeState {
	double lag;
	double previous;
	double current;
	double delta;
} TimeState;

double getCurrentTime(void);

#endif // TIME_STATE_H
