#pragma once
#include <vector>

class Clock
{
public:
	Clock();
	//construct a clock with an explicit parent
	explicit Clock(Clock& parent);

	//Destructor must unparent itself to prevent crashes
	~Clock();
	Clock(const Clock& copy) = delete;


	//Reset all book keeping variables to zero and set last updated time to current system time.
	void Reset();

	bool IsPaused() const;
	void Pause();
	void Unpause();
	void TogglePause();

	//Step unpause, run frame, pasue
	void StepSingleFrame();

	//Set and get the value by which this clock scales delta seconds
	void SetTimeScale(double timeScale);
	double GetTimeScale() const;

	double GetDeltaSeconds() const;
	double GetTotalSeconds() const;
	double GetFrameRate() const;
	int GetFrameCount() const;
	Clock* GetChildAtIndex(int index);

public:
	//Returns a reference that by default will be the parent of all clocks
	static Clock& GetSystemClock();

	//Called at begin frame to tick the system clock which then ticks all its children and so on.
	static void TickSystemClock();

protected:
	//Calculates deltaSeconds and clamps it to max deltaTime, 
	//sets last updated time
	//Calls advance() passing deltaSeconds
	void Tick();

	//Calculates deltaTime based on pausing, timeScale etc.
	//Updates book keeping variables
	//Calls Advance() on all child clocks and passes our deltaSeconds
	//Handles pausing after frames for single frame stp
	void Advance(double deltaTimeSeconds);

	//Add a child clock, does not handle cases where this child is parented already
	void AddChild(Clock* childClock);

	//Removes a child clock. If no clock present, does nothing
	void RemoveChild(Clock* childClock);

protected:
	//Parent clock reference. Null for SystemClock.
	Clock* m_parent = nullptr;

	//All children of this clock
	std::vector<Clock*> m_children;

	//book keeping
	double	m_lastupdatedTimeInSeconds	= 0.0;
	double	m_totalSeconds				= 0.0;
	double	m_deltaSeconds				= 0.0;
	int		m_frameCount				= 0;
	double	m_timeScale					= 1.0;
	bool	m_isPaused					= false;
	bool	m_stepSingleFrame			= false;
	double	m_maxDeltaSeconds			= 0.1;
};