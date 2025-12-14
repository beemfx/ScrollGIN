/* ScrollGIN.h - Header for ScrollGIN game class.
	Copyright (c) 2004 Blaine Myers. */

#ifndef __SCROLLGIN_H__
#define __SCROLLGIN_H__

#include "ObjMan.h"
#include "TileManager.h"
#include "SpriteManager.h"
#include "View.h"
#include "Input.h"
#include "Timer.h"
#include "Background.h"

class SgScrollGINGame
{
protected:
	SgTileManager    m_TileManager;
	SgTimer          m_Timer;
	SgViewPort       m_Viewport;
	SgInputManager   m_Input;
	SgMap        m_Mapboard;
	SgBackground     m_Background;
	SgObjectManager* m_lpObjectManager;
	bool             m_bInitialized;
	int              m_dwWidth;
	int              m_dwHeight;
public:
	SgScrollGINGame();
	~SgScrollGINGame();

	void Init(int dwWidth, int dwHeight, SgObjectManager* lpObjMan, std::uintptr_t hwnd);
	void Deinit();

	void Pause(bool bPause);
	void TogglePause();
	bool IsPaused()const;

	void Update();
	void Render();

	bool IsKeyPressed(int nKey);
	void LoadMap(const char* szFilename);

private:
	void DrawMapBoard();
};

#endif //__SCROLLGIN_H__