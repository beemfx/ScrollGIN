#if 0

#include <stdio.h>
#include "Error.h"
#include "TestAI.h"


void TestCreate(void* lpDD, DWORD dwTrans, SgObjectManager* lpObjMan)
{
	//This function is merely a test to demonstrate how to create a sprite from data
	char szImageName[] = TEXT("freedom sprites2.bmp");
	char szImage2Name[] = TEXT("sprites.bmp");
	struct SPRITECREATESTRUCT
	{
		int nFX;
		int nFY;
		int nFWidth;
		int nFHeight;
		int nWidth;
		int nHeight;
	};
	SPRITECREATESTRUCT scs[6];
	scs[0].nFX=11;scs[0].nFY=155;scs[0].nFWidth=85;scs[0].nFHeight=37;scs[0].nWidth=85;scs[0].nHeight=37;
	scs[1].nFX=101;scs[1].nFY=155;scs[1].nFWidth=85;scs[1].nFHeight=37;scs[1].nWidth=85;scs[1].nHeight=37;
	scs[2].nFX=192;scs[2].nFY=155;scs[2].nFWidth=85;scs[2].nFHeight=37;scs[2].nWidth=85;scs[2].nHeight=37;
	scs[3].nFX=281;scs[3].nFY=155;scs[3].nFWidth=85;scs[3].nFHeight=37;scs[3].nWidth=85;scs[3].nHeight=37;
	scs[4].nFX=372;scs[4].nFY=155;scs[4].nFWidth=85;scs[4].nFHeight=37;scs[4].nWidth=85;scs[4].nHeight=37;
	scs[5].nFX=462;scs[5].nFY=155;scs[5].nFWidth=85;scs[5].nFHeight=37;scs[5].nWidth=85;scs[5].nHeight=37;	
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 6, TEXT("legsrunning"), (void*)scs);
	scs[0].nWidth=85;scs[0].nHeight=40;scs[0].nFX=11;scs[0].nFY=43;scs[0].nFWidth=85;scs[0].nFHeight=40;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("upperbody"), (void*)scs);
	scs[0].nWidth=85;scs[0].nHeight=37;scs[0].nFX=11;scs[0].nFY=112;scs[0].nFWidth=85;scs[0].nFHeight=37;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("legsstanding"), (void*)scs);
	scs[0].nFX=102;scs[0].nFY=37;scs[0].nFWidth=85;scs[0].nFHeight=46;scs[0].nWidth=85;scs[0].nHeight=46;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("ubangleup"), (void*)scs);
	scs[0].nFX=200;scs[0].nFY=37;scs[0].nFWidth=85;scs[0].nFHeight=56;scs[0].nWidth=85;scs[0].nHeight=56;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("ubangledown"), (void*)scs);
	scs[0].nFX=294;scs[0].nFY=36;scs[0].nFWidth=85;scs[0].nFHeight=56;scs[0].nWidth=85;scs[0].nHeight=56;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("ubup"), (void*)scs);
	scs[0].nFX=384;scs[0].nFY=36;scs[0].nFWidth=85;scs[0].nFHeight=72;scs[0].nWidth=85;scs[0].nHeight=72;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("ubdown"), (void*)scs);
	scs[0].nFX=109;scs[0].nFY=106;scs[0].nFWidth=91;scs[0].nFHeight=31;scs[0].nWidth=91;scs[0].nHeight=31;
	lpObjMan->CreateSpriteFromData(szImageName, lpDD, dwTrans, 1, TEXT("ducking"), (void*)scs);

}

BOOL TestCreateObject(void* lpDD, SgObjectManager* lpObjMan)
{
	//This function creates an object for testing purposes
	TestCreate(lpDD, dwTrans, lpObjMan);
	lpObjMan->LoadSpritesFromFile(TEXT("NedSprites.ilb"));
	/*
	ObjectManager.ObtainSpriteManager(&g_cSpriteManager);
	ObjectManager.ObtainTimer(&Timer);
	*/

	lpObjMan->CreateObject((OBJECTTYPE)CROW_OBJECT, 100, 180, 5, 1);
	lpObjMan->CreateObject((OBJECTTYPE)CROW_OBJECT, 20, 100, 6, 0);
	lpObjMan->CreateObject((OBJECTTYPE)FIGHTER_OBJECT, 40, 319, 0, 0);
	lpObjMan->CreateObject((OBJECTTYPE)CROW_OBJECT, 10, 50, 4, -2);
	lpObjMan->CreateObject((OBJECTTYPE)CROW_OBJECT, 155, 75, 7, 0);

	lpObjMan->SetUserObject(3);

	return TRUE;
}

//be sure to call overloaded SgObjectManager when
//calling user defined object manager consturctor...
CTestObjman::CTestObjman(DWORD dwMaxObjects):
SgObjectManager(dwMaxObjects)
{
	SetError(TEXT("Test object manager created with user max"));
}

CTestObjman::CTestObjman(){
	SetError(TEXT("Test object manager with default max created"));
}

CTestObjman::CTestObjman(DWORD dwMax, SgTimer * pTimer):
SgObjectManager(dwMax, pTimer)
{
	SetError(TEXT("Test object manager complete created"));
}

CTestObjman::~CTestObjman(){
	SetError(TEXT("Text successfully close test objmgr"));
}

int CTestObjman::Initialize()
{
	TestCreateObject(lpDD, this);
	return 1;
}

HRESULT CTestObjman::CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed){
	if(m_dwCount<m_dwMaxObjects){
		//Find first free slot
		DWORD dwTime=0;
		if(m_pTimer)
			dwTime=m_pTimer->Time();

		DWORD i=0;
		while(m_ppObject[i]!=NULL)i++;
		switch(nType)
		{
		case CROW_OBJECT:
			m_ppObject[i]=new CCrowObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		case FIGHTER_OBJECT:
			m_ppObject[i]=new CFighterObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		case OT_DEFAULT:
			m_ppObject[i]=new SgObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		default:
			m_ppObject[i]=new SgObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		}
		m_pObjectType[i]=nType;
		m_dwCount++;
		return S_OK;
	}else return E_FAIL;

	return S_OK;

}

/*
	The collision detection for this ai example
	simply makes the second object tested in a
	standard collision have a negative velocity.

	In an actual game, this would probably have
	the object go to it's next stage suck as
	destruction or an exploding object.
*/
HRESULT CTestObjman::DetectCollisions(){
	DWORD i=0, j=0;
	for(i=0; i<m_dwMaxObjects; i++){
		if(m_ppObject[i]!=NULL){
			for(j=i+1; j<m_dwMaxObjects; j++){
				//here overloaded function should do
				//appropriate action depending on type
				//of collision.

				switch(DetectCollision(i+1, j+1))
				{
				case CT_NOCLSN:
					break;
				case CT_STDCLSN:
					m_ppObject[i]->SendMessage(OM_BADCOLLISION);
					m_ppObject[j]->SendMessage(OM_BADCOLLISION);
					/*
					//m_ppObject[j]->SetAliveState(FALSE);
					m_ppObject[j]->SetSpeed(0, -5);
					*/
					break;
				default:
					break;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CFighterObject::ProcessAI(SgInputManager *pInput, void* pObjMan, SgTimer *timer, CMapBoard* map){
	if(pInput==NULL){
		//ai
	}else{
		DPAD dpad;
		//g_cInput.ProcessKeyboardInput();
		dpad=pInput->GetDPad();

		#define MOVEXSPEED 10
		#define MOVEYSPEED 10
	
		switch(dpad){
		case DP_RIGHT: 
			SetObjectMode(FM_RUNNING, timer->Time());
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, 0);
			break;
		case DP_LEFT:
			SetObjectMode(FM_RUNNING, timer->Time());
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, 0);
			break;		
		case DP_UP: 
			SetObjectMode(FM_LOOKUP, timer->Time());
			SetSpeed(0, 0);//MOVEYSPEED);
			break;
		case DP_DOWN:
			if((pInput->GetButtonState(1)) && (m_nCurrentMode!=7))
				SetObjectMode(FM_LOOKDOWN, timer->Time());
			else SetObjectMode(FM_DUCK, timer->Time());
			SetSpeed(0, 0);//-MOVEYSPEED);
			break;
		case DP_UPRIGHT:
			SetObjectMode(FM_RUNLOOKUP, timer->Time());
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, MOVEYSPEED);
			break;
		case DP_DOWNRIGHT:
			SetObjectMode(FM_RUNLOOKDOWN, timer->Time());
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, -MOVEYSPEED);
			break;
		case DP_UPLEFT:
			SetObjectMode(FM_RUNLOOKUP, timer->Time());
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, MOVEYSPEED);
			break;
		case DP_DOWNLEFT:
			SetObjectMode(FM_RUNLOOKDOWN, timer->Time());
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, -MOVEYSPEED);
			break;
		default:
			SetObjectMode(FM_STANDING, timer->Time());
			SetSpeed(0, 0);
			break;
		}
	}
	return S_OK;
}

BOOL CFighterObject::LoadObjectSprites(SgSpriteManager *pSprite){
	//crow objects has one sprite
	if(pSprite==NULL)return FALSE;
	ObtainPointerToSprite(1, pSprite->LetPointer(TEXT("legsrunning")), 0, 18, 150, LP_FORWARD);
	ObtainPointerToSprite(2, pSprite->LetPointer(TEXT("upperbody")), 0, 53, 100, LP_FORWARD);
	ObtainPointerToSprite(3, pSprite->LetPointer(TEXT("legsstanding")), 0, 18, 100, LP_FORWARD);
	ObtainPointerToSprite(4, pSprite->LetPointer(TEXT("ubangleup")), 0, 56, 100, LP_FORWARD);
	ObtainPointerToSprite(5, pSprite->LetPointer(TEXT("ubangledown")), 0, 45, 100, LP_FORWARD);
	ObtainPointerToSprite(6, pSprite->LetPointer(TEXT("ubup")), 0, 61, 100, LP_FORWARD);
	ObtainPointerToSprite(7, pSprite->LetPointer(TEXT("ubdown")), 0, 37, 100, LP_FORWARD);
	ObtainPointerToSprite(8, pSprite->LetPointer(TEXT("ducking")), 0, 15, 100, LP_FORWARD);
	SetNumSprites(8);


	return TRUE;
}

BOOL CFighterObject::CreateObjectModes(DWORD dwTime){
	
	RECT rcDims;
	rcDims.top=70;
	rcDims.bottom=0;
	rcDims.left=-20;
	rcDims.right=20;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	bActiveSprites[0]=FALSE;
	bActiveSprites[1]=TRUE;
	bActiveSprites[2]=TRUE;
	//This first mode represents a standing character, standing legs and upper body
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("standing"));
	
	
	bActiveSprites[0]=TRUE;
	bActiveSprites[2]=FALSE;
	//this second mode represents a runnng character, animated legs and upper body
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("running"));
	bActiveSprites[0]=TRUE;
	bActiveSprites[1]=FALSE;
	bActiveSprites[2]=FALSE;
	bActiveSprites[3]=TRUE;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("runlookup"));
	bActiveSprites[3]=FALSE;
	bActiveSprites[4]=TRUE;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("runlookdown"));
	bActiveSprites[0]=FALSE;
	bActiveSprites[2]=TRUE;
	bActiveSprites[4]=FALSE;
	bActiveSprites[5]=TRUE;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("lookup"));
	bActiveSprites[5]=FALSE;
	bActiveSprites[6]=TRUE;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("lookdown"));
	bActiveSprites[2]=FALSE;
	bActiveSprites[6]=FALSE;
	bActiveSprites[7]=TRUE;
	rcDims.top=25;
	rcDims.bottom=0;
	rcDims.left=-45;
	rcDims.right=45;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("ducking"));
	SetObjectMode(1, dwTime);

	return TRUE;
}

CFighterObject::CFighterObject():
	SgObject()
{
	CreateObjectModes(0);
}

CFighterObject::CFighterObject(SgSpriteManager * pSpriteMgr, DWORD dwTime):
SgObject(pSpriteMgr, dwTime)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}

CFighterObject::CFighterObject(
	SgSpriteManager * pSpriteMgr, 
	DWORD dwTime,
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	SgObject(pSpriteMgr, dwTime, x, y, nXSpeed, nYSpeed)

{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}


BOOL CCrowObject::ProcessMessages(void* lpObjMan)
{
	for(WORD i=0; i<m_nNumMessages; i++){
		switch(m_nMessage[i])
		{
		case OM_BADCOLLISION:
			SetSpeed(0, -5);
			break;
		default:
			return FALSE;
		}
	}
	//Clear the message que
	m_nNumMessages=0;
	return TRUE;
}

HRESULT CCrowObject::ArchAdjust(SgTimer *timer, CMapBoard *map){
	return S_OK;
}

BOOL CCrowObject::PreInitialMovement(CMapBoard *map, int *nXSpeed, int *nYSpeed){
	return TRUE;
}

BOOL CCrowObject::LoadObjectSprites(SgSpriteManager *pSprite){
	if(pSprite==NULL)return FALSE;
	//crow objects has one sprite
	ObtainPointerToSprite(1,
		pSprite->LetPointer(TEXT("CROW")), 
		0, 
		0, 
		150, 
		LP_FORWARDBACKWARD);

	SetNumSprites(1);

	return TRUE;
}

BOOL CCrowObject::CreateObjectModes(DWORD dwTime){
	RECT rcDims;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	//crow object has one mode see the following
	rcDims.top=15;
	rcDims.bottom=-15;
	rcDims.left=-29;
	rcDims.right=29;
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, OA_BAD, TEXT("FLYING"));
	SetObjectMode(1, dwTime);
	SetObjectFace(SF_LEFT);

	return TRUE;
}


CCrowObject::CCrowObject():
	SgObject()
{
	CreateObjectModes(0);
}

CCrowObject::CCrowObject(SgSpriteManager * pSpriteMgr, DWORD dwTime):
SgObject(pSpriteMgr, dwTime)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}

CCrowObject::CCrowObject(
	SgSpriteManager * pSpriteMgr, 
	DWORD dwTime,
	int x, 
	int y, 
	int nXSpeed,
	int nYSpeed):
	SgObject(pSpriteMgr, dwTime, x, y, nXSpeed, nYSpeed)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);

}

#endif