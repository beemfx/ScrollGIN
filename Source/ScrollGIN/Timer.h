/*
	Timer.h - Header for timer class

	Copyright(c) 2003, Blaine Myers
*/

#ifndef __TIMEREX_H__
#define __TIMEREX_H__

class SgTimer
{
private:
	unsigned __int32  m_Time;
	bool              m_Paused;

	unsigned __int64  m_PF;
	unsigned __int64  m_TmLast;
public:
	SgTimer();
	~SgTimer();
	void Start();
	float GetRawElapsedSec()const;
	void Update();
	unsigned __int32 Time();
	void Pause(bool bPause);
	void TogglePause();
	bool IsPaused()const;
};


#endif //__TIMEREX_H__
