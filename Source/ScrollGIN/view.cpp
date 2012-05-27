/*
	view.cpp - The Viewport class.  Converts world coordinates
	to screen coordinates, and other similar routines.

	Copyright (c) 2002, Blaine Myers
*/
#include <ddraw.h>
#include "defines.h"
#include "timer.h"
#include "view.h"


//extern CTimerEx Timer;
/*
extern DWORD g_nDeviceHeight;
extern DWORD g_nDeviceWidth;
*/

CViewPort::CViewPort(){
	m_nX=m_nY=0;
	m_nDesiredX=m_nDesiredY=0;
	m_nLastTimeUpdatedX=0;
	m_nLastTimeUpdatedY=0;
	m_nWorldWidth=0;
	m_nWorldHeight=0;
	//give the scroll speed a decent default value
	m_nScrollSpeedX=m_nScrollSpeedY=6;
}

void CViewPort::SetViewportDimensions(DWORD dwWidth, DWORD dwHeight){
	m_dwWidth=dwWidth;
	m_dwHeight=dwHeight;
}

void CViewPort::stop_scroll(){
	set_desired_position(m_nX, m_nY);
}

int CViewPort::screenX(int x){
	int delta=x-m_nX;
	return delta+m_dwWidth/2;
}

int CViewPort::screenY(int y){
	int delta=y-m_nY;
	return delta+m_dwHeight/2;
}

void CViewPort::force_position(int x, int y){
	if((x>=(int)(0+m_dwWidth/2)) && (x<=(int)(m_nWorldWidth-m_dwWidth/2))){
		normalizeX(&x);
		m_nX=x;
	}
	if(x<(int)(0+m_dwWidth/2))
		m_nX=m_dwWidth/2;

	if(x>(int)(m_nWorldWidth-m_dwWidth/2))
		m_nX=m_nWorldWidth-m_dwWidth/2;



	if((y>=(int)(0+m_dwHeight/2)) && (y<=(int)(m_nWorldHeight-m_dwHeight/2))){
		normalizeY(&y);
		m_nY=y;
	}

	if(y<0+(int)m_dwHeight/2)
		m_nY=m_dwHeight/2;

	if(y>(int)(m_nWorldHeight-m_dwHeight/2))
		m_nY=m_nWorldHeight-m_dwHeight/2;
}

int CViewPort::normalizeX(int *x){
	return *x;
}

int CViewPort::normalizeY(int *y){
	return *y;
}

void CViewPort::set_desired_position(int x, int y){
	normalizeX(&x);
	m_nDesiredX=x;
	normalizeY(&y);
	m_nDesiredY=y;
	
}

void CViewPort::set_scroll_speed(int speedX, int speedY){
	m_nScrollSpeedX=speedX;
	m_nScrollSpeedY=speedY;
}


void CViewPort::set_world_dimensions(int width, int height){
	m_nWorldHeight=height;
	m_nWorldWidth=width;
}

int CViewPort::GetScreenXPos(){
	return m_nX;
}

int CViewPort::GetScreenYPos(){
	return m_nY;
}

int CViewPort::GetWorldHeight(){
	return m_nWorldHeight;
}

int CViewPort::GetWorldWidth(){
	return m_nWorldWidth;
}

void CViewPort::update(DWORD dwTime){

	int nScrollSpeedX=0;
	int nScrollSpeedY=0;


	//update current view position
	int deltaX; //change in x
	int deltaY; //change in y

	int tempX=m_nX;
	int tempY=m_nY;

	//deal with x
	
	if(m_nDesiredX>m_nX)
		nScrollSpeedX=m_nScrollSpeedX;
	if(m_nDesiredX<m_nX)
		nScrollSpeedX=-m_nScrollSpeedX;
	if(m_nDesiredX==m_nX)
		nScrollSpeedX=0;
	

	deltaX=(nScrollSpeedX*(dwTime-m_nLastTimeUpdatedX))/SPEEDADJUST;

	if(deltaX || nScrollSpeedX==0){ //if the view x has been changed
		//we don't need to make sure the coords fall in 
		//universe because force_set_coord does that
		tempX+=deltaX;
		m_nLastTimeUpdatedX=dwTime;
	}
	
	
	
	
	if(m_nDesiredY>m_nY)
		nScrollSpeedY = m_nScrollSpeedY;
	if(m_nDesiredY<m_nY)
		nScrollSpeedY = -m_nScrollSpeedY;
	if(m_nDesiredY==m_nY)
		nScrollSpeedY = 0;


	deltaY=(nScrollSpeedY*(dwTime-m_nLastTimeUpdatedY))/SPEEDADJUST;
	if(deltaY || nScrollSpeedY==0){ //if the view y has changed
		//we don't need to make sure coords fall in
		//universe becasue force_set_coord does that
		tempY+=deltaY;
		m_nLastTimeUpdatedY=dwTime;
	}

	//we need to reupdate the last time draw to prevent
	//the viewport from jumping around
	//m_nLastTimeUpdatedX=Timer.time();
	//m_nLastTimeUpdatedY=Timer.time();


	//update the position
	force_position(tempX, tempY);
}