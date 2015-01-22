/*
	Timer.cpp - The extended timer class

	Copyright (c) 2003, Blaine Myers
*/

#include "timer.h"
#include <Windows.h>

SgTimer::SgTimer()
: m_TmLast( 0 )
, m_PF( 0 )
{
	m_nStartTime=0;
	m_bPaused=false;
	BOOL Res = QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>(&m_PF) );
	assert( Res );
}

SgTimer::~SgTimer()
{

}

void SgTimer::Start()
{
	m_nStartTime=timeGetTime();
}

float SgTimer::GetRawElapsedSec()const
{
	unsigned __int64 Tm;
	QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&Tm) );
	assert( Tm >= m_TmLast );
	unsigned __int64 Elapsed = Tm - m_TmLast;
	float ElapsedSec = static_cast<float>(static_cast<double>(Elapsed)/m_PF);
	return ElapsedSec;
}

void SgTimer::Update()
{
	unsigned __int64 Tm;
	QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>(&Tm) );
	assert( Tm >= m_TmLast );
	unsigned __int64 Elapsed = Tm - m_TmLast;
	float ElapsedSec = static_cast<float>(static_cast<double>(Elapsed)/m_PF);
	m_TmLast = Tm;
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