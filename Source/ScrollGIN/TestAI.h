#ifndef __TESTAI_H__
#define __TESTAI_H__

#if 0

#include "object.h"
#include "objman.h"

#define CROW_OBJECT 0x00000001
#define FIGHTER_OBJECT 0x00000002

#define OM_BADCOLLISION 0x00000001

//Fighter object modes
#define FM_STANDING 1
#define FM_RUNNING 2
#define FM_RUNLOOKUP 3
#define FM_RUNLOOKDOWN 4
#define FM_LOOKUP 5
#define FM_LOOKDOWN 6
#define FM_DUCK 7

#define OA_NEUTRAL 0x00000000
#define OA_GOOD    0x00000001
#define OA_BAD     0x00000002

class CCrowObject: public CObject{
protected:
	virtual HRESULT ArchAdjust(CTimerEx *timer, CMapBoard *map);
	virtual BOOL PreInitialMovement(CMapBoard *map, int *nXSpeed, int *nYSpeed);
public:
	CCrowObject();
	CCrowObject(CSpriteManager * pSpriteMgr, DWORD dwTime);
	CCrowObject(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
	virtual BOOL ProcessMessages(void* lpObjMan);
};

class CFighterObject: public CObject{
protected:

public:
	CFighterObject();
	CFighterObject(CSpriteManager * pSpriteMgr, DWORD dwTime);
	CFighterObject(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual BOOL LoadObjectSprites(CSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);

	virtual HRESULT ProcessAI(CInputManager* pInput, void* pObjMan, CTimerEx* timer, CMapBoard* map);
};

class CTestObjman: public CObjectManager{
protected:

public:
	CTestObjman();
	CTestObjman(DWORD dwMax);
	CTestObjman(DWORD dwMax, CTimerEx * pTimer);
	virtual ~CTestObjman();
	virtual int Initialize(void* lpDD, DWORD dwTransparent);
	virtual HRESULT CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed);
	virtual HRESULT DetectCollisions();
};

#endif

#endif //__TESTAI_H__