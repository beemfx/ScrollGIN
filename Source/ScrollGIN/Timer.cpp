/*
	Timer.cpp - The extended timer class

	Copyright (c) 2003, Blaine Myers
*/

#include "timer.h"
#include <Windows.h>

SgTimer::SgTimer()
: m_TmLast( 0 )
, m_PF( 0 )
, m_Time( 0 )
, m_Paused( false )
{
	BOOL Res = QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>(&m_PF) );
	assert( Res );
	Update();
	Update();
}

SgTimer::~SgTimer()
{

}

void SgTimer::Start()
{
	m_Time = 0;
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

	m_Time += m_Paused ? 0 : static_cast<unsigned __int32>(ElapsedSec*1000);
}

unsigned __int32 SgTimer::Time()
{
	return m_Time;
}

void SgTimer::Pause(bool bPause)
{
	m_Paused=bPause;
}

void SgTimer::TogglePause()
{
	if(m_Paused)
		 Pause(false);
	else
		 Pause(true);
}

bool SgTimer::IsPaused()const
{
	return m_Paused;
}