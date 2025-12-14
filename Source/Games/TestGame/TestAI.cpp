#include "TestAI.h"
#include "ScrollGIN.h"

//be sure to call overloaded SgObjectManager when
//calling user defined object manager consturctor...
CTestObjman::CTestObjman(DWORD dwMaxObjects):
SgObjectManager(dwMaxObjects)
{
}

void CTestObjman::Initialize(SgScrollGINGame* InGame)
{
	SgObjectManager::Initialize(InGame);

	LoadSprites(TEXT("NedSprites.ilb"));

	InGame->LoadMap("test.map");

	CreateObject((OBJECTTYPE)CROW_OBJECT, 100, 180, 5, 1);
	CreateObject((OBJECTTYPE)CROW_OBJECT, 20, 100, 6, 0);
	CreateObject((OBJECTTYPE)FIGHTER_OBJECT, 40, 319, 0, 0);
	CreateObject((OBJECTTYPE)CROW_OBJECT, 10, 50, 4, -2);
	CreateObject((OBJECTTYPE)CROW_OBJECT, 155, 75, 7, 0);

	SetUserObject(3);
}

void CTestObjman::CreateObject(const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed){
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
		return;
	}
}

void CTestObjman::OnCollision(SgObject* Obj1, SgObject* Obj2)
{
	Obj1->SendMessage(OM_BADCOLLISION);
	Obj2->SendMessage(OM_BADCOLLISION);
}

void CFighterObject::ProcessAI(SgInputManager *pInput, void* pObjMan, SgTimer *timer, SgMap* map){
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
}

void CFighterObject::LoadObjectSprites(SgSpriteManager *pSprite){
	//crow objects has one sprite
	if(pSprite==NULL)return;
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("legsrunning")), 0, 18, 150, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("upperbody")), 0, 53, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("legsstanding")), 0, 18, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("ubangleup")), 0, 56, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("ubangledown")), 0, 45, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("ubup")), 0, 61, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("ubdown")), 0, 37, 100, LP_FORWARD);
	ObtainPointerToSprite(pSprite->GetSprite(TEXT("ducking")), 0, 15, 100, LP_FORWARD);
}

void CFighterObject::CreateObjectModes(int dwTime){
	
	RECT rcDims;
	rcDims.top=70;
	rcDims.bottom=0;
	rcDims.left=-20;
	rcDims.right=20;
	bool bActiveSprites[MAX_SPRITES_PER_OBJECT];
	bActiveSprites[0]=false;
	bActiveSprites[1]=true;
	bActiveSprites[2]=true;
	//This first mode represents a standing character, standing legs and upper body
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("standing"));
	
	
	bActiveSprites[0]=true;
	bActiveSprites[2]=false;
	//this second mode represents a runnng character, animated legs and upper body
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("running"));
	bActiveSprites[0]=true;
	bActiveSprites[1]=false;
	bActiveSprites[2]=false;
	bActiveSprites[3]=true;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("runlookup"));
	bActiveSprites[3]=false;
	bActiveSprites[4]=true;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("runlookdown"));
	bActiveSprites[0]=false;
	bActiveSprites[2]=true;
	bActiveSprites[4]=false;
	bActiveSprites[5]=true;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("lookup"));
	bActiveSprites[5]=false;
	bActiveSprites[6]=true;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("lookdown"));
	bActiveSprites[2]=false;
	bActiveSprites[6]=false;
	bActiveSprites[7]=true;
	rcDims.top=25;
	rcDims.bottom=0;
	rcDims.left=-45;
	rcDims.right=45;
	CreateMode(bActiveSprites, rcDims, OA_GOOD, TEXT("ducking"));
	SetObjectMode(1, dwTime);
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


bool CCrowObject::ProcessMessages(void* lpObjMan)
{
	for(WORD i=0; i<m_nNumMessages; i++){
		switch(m_nMessage[i])
		{
		case OM_BADCOLLISION:
			SetSpeed(0, -5);
			break;
		default:
			return false;
		}
	}
	//Clear the message que
	m_nNumMessages=0;
	return true;
}

void CCrowObject::LoadObjectSprites(SgSpriteManager *pSprite){
	if(pSprite==NULL)return;
	//crow objects has one sprite
	ObtainPointerToSprite(
		pSprite->GetSprite(TEXT("CROW")), 
		0, 
		0, 
		150, 
		LP_FORWARDBACKWARD);
}

void CCrowObject::CreateObjectModes(int dwTime){
	RECT rcDims;
	bool bActiveSprites[MAX_SPRITES_PER_OBJECT];
	//crow object has one mode see the following
	rcDims.top=15;
	rcDims.bottom=-15;
	rcDims.left=-29;
	rcDims.right=29;
	bActiveSprites[0]=true;
	CreateMode(bActiveSprites, rcDims, OA_BAD, TEXT("FLYING"));
	SetObjectMode(1, dwTime);
	SetObjectFace(SF_LEFT);
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
