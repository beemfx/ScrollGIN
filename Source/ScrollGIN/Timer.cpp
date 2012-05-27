/*
	Timer.cpp - The extended timer class

	Copyright (c) 2003, Blaine Myers
*/

#include "timer.h"


CTimerEx::CTimerEx(){
	m_nStartTime=0;
	m_bPaused=FALSE;
}

CTimerEx::~CTimerEx(){

}

void CTimerEx::Start(){
	m_nStartTime=timeGetTime();
}

DWORD CTimerEx::Time(){
	if(!m_bPaused)
		return timeGetTime()-m_nStartTime;
	else return m_nPausedTime;
}

BOOL CTimerEx::Elapsed(DWORD * pStart, DWORD nInterval){
	DWORD nCurrentTime=Time();
	if(nCurrentTime >= (*pStart+nInterval)){
		*pStart=nCurrentTime; return TRUE;
	}
	return FALSE;
}


BOOL CTimerEx::Pause(BOOL bPause){
	if(bPause){
		//Pause
		m_nPausedTime=Time();

	}else{
		//resume
		m_nStartTime=timeGetTime()-m_nPausedTime;
	}

	m_bPaused=bPause;

	return m_bPaused;
}

BOOL CTimerEx::TogglePause(){
	if(m_bPaused)
		return Pause(FALSE);
	else
		return Pause(TRUE);
}

BOOL CTimerEx::IsPaused(){
	return m_bPaused;
}