/*
	Timer.cpp - The extended timer class

	Copyright (c) 2003, Blaine Myers
*/

#include "timer.h"
#include <Windows.h>

SgTimer::SgTimer()
{
	m_nStartTime=0;
	m_bPaused=false;
}

SgTimer::~SgTimer()
{

}

void SgTimer::Start()
{
	m_nStartTime=timeGetTime();
}

unsigned __int32 SgTimer::Time()
{
	if(!m_bPaused)
		return timeGetTime()-m_nStartTime;
	else return m_nPausedTime;
}

void SgTimer::Pause(bool bPause)
{
	if(bPause)
	{
		//Pause
		m_nPausedTime=Time();
	}
	else
	{
		//resume
		m_nStartTime=timeGetTime()-m_nPausedTime;
	}

	m_bPaused=bPause;
}

void SgTimer::TogglePause()
{
	if(m_bPaused)
		 Pause(false);
	else
		 Pause(true);
}

bool SgTimer::IsPaused()const
{
	return m_bPaused;
}