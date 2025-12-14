#ifndef __TESTAI_H__
#define __TESTAI_H__

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
public:
	CCrowObject();
	CCrowObject(SgSpriteManager * pSpriteMgr, DWORD dwTime);
	CCrowObject(SgSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual void LoadObjectSprites(SgSpriteManager *pSpriteMgr) override;
	virtual void CreateObjectModes(int dwTime) override;
	virtual bool ProcessMessages(void* lpObjMan) override;
};

class CFighterObject: public SgObject{
public:
	CFighterObject();
	CFighterObject(SgSpriteManager * pSpriteMgr, DWORD dwTime);
	CFighterObject(SgSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);

	virtual void LoadObjectSprites(SgSpriteManager *pSpriteMgr) override;
	virtual void CreateObjectModes(int dwTime) override;

	virtual void ProcessAI(SgInputManager* pInput, void* pObjMan, SgTimer* timer, SgMap* map);
};

class CTestObjman: public SgObjectManager{
public:
	CTestObjman(DWORD dwMax);

	virtual void Initialize(SgScrollGINGame* InGame) override;
	virtual void CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed) override;
	virtual void OnCollision(SgObject* Obj1, SgObject* Obj2) override;
};

#endif //__TESTAI_H__