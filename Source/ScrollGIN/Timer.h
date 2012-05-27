/*
	Timer.h - Header for timer class

	Copyright(c) 2003, Blaine Myers
*/

#ifndef __TIMEREX_H__
#define __TIMEREX_H__

#include <windows.h> 


class CTimerEx{
protected:
	DWORD m_nStartTime;

	DWORD m_nPausedTime;
	BOOL m_bPaused;
public:
	CTimerEx();
	~CTimerEx();
	void Start();
	DWORD Time();
	BOOL Elapsed(DWORD * pStart, DWORD nInterval);
	BOOL Pause(BOOL bPause);
	BOOL TogglePause();
	BOOL IsPaused();
};


#endif //__TIMEREX_H__
