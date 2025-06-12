#pragma once
#include "Engine/Core/EngineCommon.hpp"

class Timer
{
public:
	//Create a clock with a period and the clock being used. If clock is null use system clock
	explicit Timer(double period, const Clock* clock = nullptr);

	//Set start time to clock's current total
	void Start();

	//Set stop time back to negative one
	void Stop();

	//Returns zero if stopped otherwise returns time elapsed time between current time and start time
	double GetElapsedTime() const;

	//Return the elapsed percentage of period. Can be greater than 1.
	double GetElapsedFraction() const;

	//Returns true if start time less than zero
	bool IsStopped() const;

	//Returns true if our elapsed time is greater than our period and we are not stopped
	bool HasPeriodElapsed() const;

	//If period elapsed and not stopped,
	//decrements period by adding period to start time and returns true. 
	//Called in a loop until it returns false to process "laps"
	bool DecrementPeriodIfElapsed();

	//Clock used to get current time
	const Clock* m_clock = nullptr;

	//Clock time the timer started at. Can be inncremented by period amounts or -1 if stopped
	double m_startTime = -1.0f;

	double m_period = 0.0f;
};