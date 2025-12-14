/*
	view.h - header for viewport class

	Copyright (c) 2002, Blaine Myers
*/

#ifndef __VIEW_H__
#define __VIEW_H__

class SgViewPort{
private:
	int m_nX = 0; //Viewport x coordinate
	int m_nY = 0; //viewport y coordinate
	int m_nDesiredX = 0; //desired x coordinate for viewport
	int m_nDesiredY = 0; //desirect y coordinate for viewport
	int m_nLastTimeUpdatedX = 0; //last time the x coord was updated
	int m_nLastTimeUpdatedY = 0; //last time the y coord was updated
	int m_nWorldWidth = 0; //stores world width
	int m_nWorldHeight = 0; //sotres world height
	int m_nScrollSpeedX = 0; //stores x scroll speed
	int m_nScrollSpeedY = 0; //stores y scroll speed

	std::uint32_t m_dwWidth = 0;
	std::uint32_t m_dwHeight = 0;
public:
	SgViewPort(); //consturctor
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
	void update(std::uint32_t dwTime); //updates the viewport, should be called for each game loop
	void set_world_dimensions(int width, int height); //sets the world dimensions
	void set_scroll_speed(int speedX, int speedY); //sets the scroll speeds
	void stop_scroll(); //stops viewprot from scrolling
	void SetViewportDimensions(std::uint32_t dwWidth, std::uint32_t dwHeight);
};


#endif //__view_h__