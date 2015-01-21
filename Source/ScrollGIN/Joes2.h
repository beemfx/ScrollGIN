/* Joes2.h - Header for Joes Copter 2 AI.
	Copyright (c) 2004 Blaine Myers */

#ifndef __JOES2_H__
#define __JOES2_H__

#define JOES2_GOODCOPTER  0x00000001
#define JOES2_GOODMISSILE 0x00000002
#define JOES2_SMOKE       0x00000003
#define JOES2_EXPLOSION   0x00000004
#define JOES2_BADCOPTER   0x00000005
#define JOES2_BADMISSILE  0x00000006

//Joes 2 Messages
#define JM_KILL    0x08000001
#define JM_EXPLODE 0x08000002
#define JM_SHOOT   0x08000003
#define JM_HIT     0x08000004

//Joes 2 Collision types
#define JC_GOOD        0x00000001
#define JC_BAD         0x00000002
#define JC_NEUTRAL     0x00000004

#define JC_NONDESTRUCT 0x00000010
#define JC_DESTRUCT    0x00000020

#include "object.h"
#include "objman.h"

class CJoes2Object: public CObject
{
protected:
	DWORD m_dwCreateTime;
	DWORD m_dwLastAIUpdate;
public:
	CJoes2Object(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual BOOL ProcessMessages(void* lpObjMan);
};

class CGoodCopterObject: public CJoes2Object
{
protected:
	DWORD m_dwLastShot;

	CObject* m_lpEnemy; //Pointer to target chosen as enemy.
	BOOL ChooseEnemy(CObject* lpEnemy); //Chooses which target it would like as enemy.

	virtual HRESULT ProcessAI(CInputManager *pInput, void* pObjMan, CTimerEx *timer, CMapBoard* map);
public:
	CGoodCopterObject(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	
	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CBadCopterObject: public CGoodCopterObject
{
protected:

public:
	CBadCopterObject(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	
	virtual BOOL LoadObjectSprites(CSpriteManager* pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CGoodMissileObject: public CJoes2Object
{
protected:
	DWORD m_dwLastSmokeTime;
	int m_nLastX;
	int m_nLastY;
	virtual HRESULT ProcessAI(CInputManager *pInput, void* pObjMan, CTimerEx *timer, CMapBoard* map);
public:
	CGoodMissileObject(
		CSpriteManager * pSpriteMgr, 
		DWORD dwTime, 
		int x, 
		int y, 
		int nXSpeed, 
		int nYSpeed);

	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CBadMissileObject: public CGoodMissileObject
{
protected:

public:
	CBadMissileObject(
		CSpriteManager * pSpriteMgr, 
		DWORD dwTime, 
		int x, 
		int y, 
		int nXSpeed, 
		int nYSpeed);

	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CSmokeObject: public CJoes2Object
{
protected:

	virtual HRESULT ProcessAI(
		CInputManager *pInput, 
		void* pObjMan, 
		CTimerEx *timer, 
		CMapBoard* map);

public:
	CSmokeObject(
		CSpriteManager * pSpriteMgr, 
		DWORD dwTime, 
		int x, 
		int y, 
		int nXSpeed, 
		int nYSpeed);

	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CExplosionObject: public CJoes2Object
{
protected:

	virtual HRESULT ProcessAI(
		CInputManager *pInput, 
		void* pObjMan, 
		CTimerEx *timer, 
		CMapBoard* map);
public:
	CExplosionObject(
		CSpriteManager * pSpriteMgr, 
		DWORD dwTime, 
		int x, 
		int y, 
		int nXSpeed, 
		int nYSpeed);

	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
};

class CJoes2ObjMan: public CObjectManager
{
protected:

public:
	CJoes2ObjMan();
	CJoes2ObjMan(DWORD dwMax);

	virtual ~CJoes2ObjMan();
	virtual int Initialize(DWORD dwTransparent);
	virtual HRESULT CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed);
	virtual HRESULT DetectCollisions();
};



#endif // __JOES2_h__