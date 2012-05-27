/*
	view.h - header for viewport class

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __VIEW_H__
#define __VIEW_H__

#include <windows.h>

class CViewPort{
private:
	int m_nX; //Viewport x coordinate
	int m_nY; //viewport y coordinate
	int m_nDesiredX; //desired x coordinate for viewport
	int m_nDesiredY; //desirect y coordinate for viewport
	int m_nLastTimeUpdatedX; //last time the x coord was updated
	int m_nLastTimeUpdatedY; //last time the y coord was updated
	int m_nWorldWidth; //stores world width
	int m_nWorldHeight; //sotres world height
	int m_nScrollSpeedX; //stores x scroll speed
	int m_nScrollSpeedY; //stores y scroll speed

	DWORD m_dwWidth;
	DWORD m_dwHeight;
public:
	CViewPort(); //consturctor
	int GetScreenXPos();//returns view x
	int GetScreenYPos();//returns view y
	int GetWorldWidth();//returns world width
	int GetWorldHeight();//returns world height
	void force_position(int x, int y);//forces the viewport to a position
	void set_desired_position(int x, int y);//sets a desired position for viewport
	int screenX(int x); //coord relative to viewport
	int screenY(int y); //coord relative to viewport
	int normalizeX(int *x); //normalizes x value
	int normalizeY(int *y); //normalizes y value
	void update(DWORD dwTime); //updates the viewport, should be called for each game loop
	void set_world_dimensions(int width, int height); //sets the world dimensions
	void set_scroll_speed(int speedX, int speedY); //sets the scroll speeds
	void stop_scroll(); //stops viewprot from scrolling
	void SetViewportDimensions(DWORD dwWidth, DWORD dwHeight);
};


#endif //__view_h__