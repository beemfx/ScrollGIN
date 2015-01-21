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

class CCrowObject: public SgObject{
protected:
	virtual HRESULT ArchAdjust(SgTimer *timer, SgMap *map);
	virtual BOOL PreInitialMovement(SgMap *map, int *nXSpeed, int *nYSpeed);
public:
	CCrowObject();
	CCrowObject(SgSpriteManager * pSpriteMgr, DWORD dwTime);
	CCrowObject(SgSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual BOOL LoadObjectSprites(SgSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);
	virtual BOOL ProcessMessages(void* lpObjMan);
};

class CFighterObject: public SgObject{
protected:

public:
	CFighterObject();
	CFighterObject(SgSpriteManager * pSpriteMgr, DWORD dwTime);
	CFighterObject(SgSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual BOOL LoadObjectSprites(SgSpriteManager *pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);

	virtual HRESULT ProcessAI(SgInputManager* pInput, void* pObjMan, SgTimer* timer, SgMap* map);
};

class CTestObjman: public SgObjectManager{
protected:

public:
	CTestObjman();
	CTestObjman(DWORD dwMax);
	CTestObjman(DWORD dwMax, SgTimer * pTimer);
	virtual ~CTestObjman();
	virtual int Initialize();
	virtual HRESULT CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed);
	virtual HRESULT DetectCollisions();
};

#endif

#endif //__TESTAI_H__