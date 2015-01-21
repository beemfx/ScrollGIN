/*
	Timer.h - Header for timer class

	Copyright(c) 2003, Blaine Myers
*/

#ifndef __TIMEREX_H__
#define __TIMEREX_H__

class SgTimer
{
private:
	unsigned __int32  m_nStartTime;
	unsigned __int32  m_nPausedTime;
	bool              m_bPaused;
public:
	SgTimer();
	~SgTimer();
	void Start();
	unsigned __int32 Time();
	void Pause(bool bPause);
	void TogglePause();
	bool IsPaused()const;
};


#endif //__TIMEREX_H__
