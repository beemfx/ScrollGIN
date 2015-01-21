/* Joes2.cpp - Source for Joe's Copter 2 AI.
	Copyright (c) 2004 Blaine Myers */

#include "genfuncs.h"
#include "Joes2.h"

////////////////////////////////////
/// Joes Copter 2 Object Manager ///
////////////////////////////////////

CJoes2ObjMan::CJoes2ObjMan(DWORD dwMaxObjects):
CObjectManager(dwMaxObjects)
{
	
}

CJoes2ObjMan::CJoes2ObjMan():
CObjectManager()
{
	
}


CJoes2ObjMan::~CJoes2ObjMan()
{
	
}

int CJoes2ObjMan::Initialize(DWORD dwTransparent)
{
	if(FAILED(LoadSpritesFromFile(
		TEXT("JoesSprites.ilb"),
		dwTransparent)))
	{
		return 0;
	}
	//Should create initial object.
	CreateObject(
		(OBJECTTYPE)JOES2_GOODCOPTER,
		100,
		(12-4)*40,
		0,
		0);

	//Create a test enemy chopper.
	CreateObject(
		(OBJECTTYPE)JOES2_BADCOPTER,
		500,
		200,
		0,
		0);

	CreateObject(
		(OBJECTTYPE)JOES2_BADCOPTER,
		800,
		250,
		0,
		0);

	SetUserObject(1);

	return 1;
}

HRESULT CJoes2ObjMan::CreateObject(
	const OBJECTTYPE nType, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed)
{
	if(m_dwCount>=m_dwMaxObjects)
		return E_FAIL;

	DWORD dwTime=0;
	if(m_pTimer)
		dwTime=m_pTimer->Time();

	DWORD i=0;
	while(m_ppObject[i]!=NULL)i++;

	switch(nType)
	{
	case JOES2_GOODCOPTER:
		m_ppObject[i]=new CGoodCopterObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case JOES2_GOODMISSILE:
		m_ppObject[i]=new CGoodMissileObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case JOES2_BADMISSILE:
		m_ppObject[i]=new CBadMissileObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case JOES2_SMOKE:
		m_ppObject[i]=new CSmokeObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case JOES2_EXPLOSION:
		m_ppObject[i]=new CExplosionObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case JOES2_BADCOPTER:
		m_ppObject[i]=new CBadCopterObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	case OT_DEFAULT:
	default:
		m_ppObject[i]=new CObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
		break;
	}
	m_pObjectType[i]=nType;
	m_dwCount++;

	return S_OK;
}


HRESULT CJoes2ObjMan::DetectCollisions()
{
	DWORD i=0, j=0;
	for(i=0; i<m_dwMaxObjects; i++){
		if(m_ppObject[i]!=NULL)
		{
			for(j=i+1; j<m_dwMaxObjects; j++)
			{
				if(m_ppObject[j]==NULL)
					continue;

				DWORD dwFirstAlign=0, dwSecondAlign=0;

				dwFirstAlign=m_ppObject[i]->GetObjectAlign();
				dwSecondAlign=m_ppObject[j]->GetObjectAlign();
				//here overloaded function should do
				//appropriate action depending on type
				//of collision.

				BOOL bDestruct=FALSE;

				
				//If both objects are nondestructive, continue
				if( ((dwFirstAlign&JC_DESTRUCT)==JC_DESTRUCT) )
					bDestruct=TRUE;

				if( ((dwSecondAlign&JC_DESTRUCT)==JC_DESTRUCT) )
					bDestruct=TRUE;
				
				if(bDestruct==FALSE)
					continue;


				//If objects ahve the same alignment, continue.
				if( (dwFirstAlign&0x0000000F) == (dwSecondAlign&0x0000000F))
				{	
					continue;
				}

				//If one object is neutral, continue.
				if( ((dwFirstAlign&JC_NEUTRAL)==JC_NEUTRAL) ||
					((dwSecondAlign&JC_NEUTRAL)==JC_NEUTRAL) )
				{
					continue;
				}
				


				switch(DetectCollision(i+1, j+1))
				{
				case CT_NOCLSN:
					break;
				case CT_STDCLSN:				
					//If we've made it here the objects are enemies and should be destroyed.
					m_ppObject[i]->SendMessage(JM_HIT);
					m_ppObject[j]->SendMessage(JM_HIT);
					break;
				default:
					break;
				}
			}
		}
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////
/// Generic Joes2 Object covers things for all joes2 objects. ///
/////////////////////////////////////////////////////////////////

CJoes2Object::CJoes2Object(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CObject(
		pSpriteMgr,
		dwTime,
		x,
		y,
		nXSpeed,
		nYSpeed),
	m_dwCreateTime(dwTime),
	m_dwLastAIUpdate(dwTime)
{

}

BOOL CJoes2Object::ProcessMessages(void* lpObjMan)
{
	for(WORD i=0; i<m_nNumMessages; i++){
		switch(m_nMessage[i])
		{
		case JM_KILL:
			m_bAlive=FALSE;
			break;
		case JM_SHOOT:
		{
			int nX=m_nX, nXSpeed=30;

			if(m_nFace==SF_RIGHT)
			{
				nXSpeed=30;
				nX=m_nX+50;
			}
			else if(m_nFace==SF_LEFT)
			{
				nXSpeed=-30;
				nX=m_nX-50;
			}

			
			if( (GetObjectAlign()&JC_GOOD)==JC_GOOD)
			{
				((CObjectManager*)lpObjMan)->CreateObject(
					(OBJECTTYPE)JOES2_GOODMISSILE,
					nX,
					m_nY-5,
					nXSpeed,
					0);
			}
			else if ( (GetObjectAlign()&JC_BAD)==JC_BAD)
			{
				((CObjectManager*)lpObjMan)->CreateObject(
					(OBJECTTYPE)JOES2_BADMISSILE,
					nX,
					m_nY-5,
					nXSpeed,
					0);
			}


			break;
		}
		case JM_HIT:
		case JM_EXPLODE:
			m_bAlive=FALSE;
			((CObjectManager*)lpObjMan)->CreateObject(
				(OBJECTTYPE)JOES2_EXPLOSION, 
				m_nX, 
				m_nY, 
				m_nXSpeed/8, 
				m_nYSpeed/8);
			break;
		default:
			break;
		}
	}
	//Clear the message que
	m_nNumMessages=0;
	return TRUE;
}


////////////////////////////////////////////
/// The Joes Copter 2 Good Copter Object ///
////////////////////////////////////////////

HRESULT CGoodCopterObject::ProcessAI(
	CInputManager *pInput, 
	void* pObjMan, 
	CTimerEx *timer, 
	CMapBoard* map)
{
	#define MOVEXSPEED 10
	#define MOVEYSPEED 10

	if(pInput==NULL)
	{
		//ai
		//Need to generate an ai.
		
		//Simple AI shoot every second.
		if ((timer->Time()-m_dwLastShot)>1000)
		{
			SendMessage(JM_SHOOT);
			m_dwLastShot=timer->Time();
		}

		//Simple movement randomly moves up down and left right, changing
		//course five times a second, very similar to original ai.
		if( (timer->Time()-m_dwLastAIUpdate)>200)
		{
			m_dwLastAIUpdate=timer->Time();
			//RandomSeed(timer->Time());  //This seemed to make all AI's use the same random numbers
			
			m_nXSpeed=Random(-10, 10);
			m_nYSpeed=Random(-10, 10);


			if(m_nYSpeed < 0)
				SetObjectFace(SF_LEFT);
			else
				SetObjectFace(SF_RIGHT);
		}
	}
	else
	{
		DPAD dpad;
		//g_cInput.ProcessKeyboardInput();
		dpad=pInput->GetDPad();

		//Shoot if button is down, thsi button should be mappable.
		if( ((pInput->GetButtonState(0)) || (pInput->GetKeyState(DIK_LCONTROL))) && ((timer->Time()-m_dwLastShot)>500) )
		{
			SendMessage(JM_SHOOT);
			m_dwLastShot=timer->Time();
		}
	
		switch(dpad)
		{
		case DP_RIGHT: 
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, 0);
			break;
		case DP_LEFT:
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, 0);
			break;		
		case DP_UP: 
			SetSpeed(0, MOVEYSPEED);
			break;
		case DP_DOWN:
			SetSpeed(0, -MOVEYSPEED);
			break;
		case DP_UPRIGHT:
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, MOVEYSPEED);
			break;
		case DP_DOWNRIGHT:
			SetObjectFace(SF_RIGHT);
			SetSpeed(MOVEXSPEED, -MOVEYSPEED);
			break;
		case DP_UPLEFT:
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, MOVEYSPEED);
			break;
		case DP_DOWNLEFT:
			SetObjectFace(SF_LEFT);
			SetSpeed(-MOVEXSPEED, -MOVEYSPEED);
			break;
		default:
			SetSpeed(0, 0);
			break;
		}
	}
	return S_OK;
}


BOOL CGoodCopterObject::LoadObjectSprites(CSpriteManager* pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("COPTER1")), 0, 19, 150, LP_FORWARD);
	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("DOWNCOPTER1")), 0, 15, 0, LP_FORWARD);

	return TRUE;
}

BOOL CGoodCopterObject::CreateObjectModes(DWORD dwTime)
{
	//RECT rcDims={left, top, right, bottom};
	RECT rcDims;
	rcDims.top=36;
	rcDims.bottom=0;
	rcDims.left=-34;
	rcDims.right=34;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_GOOD|JC_NONDESTRUCT, TEXT("FLYING"));

	rcDims.top=30;
	rcDims.bottom=0;
	rcDims.left=-34;
	rcDims.right=34;

	bActiveSprites[0]=FALSE;
	bActiveSprites[1]=TRUE;

	CreateMode(bActiveSprites, rcDims, JC_NEUTRAL|JC_NONDESTRUCT, TEXT("DEAD"));

	SetObjectMode(1, dwTime);
	return TRUE;
}


CGoodCopterObject::CGoodCopterObject(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime,
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CJoes2Object(pSpriteMgr, dwTime, x, y, nXSpeed, nYSpeed),
	m_dwLastShot(0)
{
	//Only create if there is a sprite manager,
	//otherwise it will get in the way of objects that are based
	//on this class (CBadCopterObject for instance).
	if(pSpriteMgr)
	{
		LoadObjectSprites(pSpriteMgr);
		CreateObjectModes(dwTime);
	}
}

////////////////////////////////
/// Joes 2 Bad Copter Object ///
////////////////////////////////

CBadCopterObject::CBadCopterObject(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CGoodCopterObject(
		NULL,
		dwTime,
		x,
		y,
		nXSpeed,
		nYSpeed)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
	SetObjectFace(SF_LEFT);
}

BOOL CBadCopterObject::CreateObjectModes(DWORD dwTime)
{
	RECT rcDims;
	rcDims.top=36;
	rcDims.bottom=0;
	rcDims.left=-34;
	rcDims.right=34;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_BAD|JC_NONDESTRUCT, TEXT("FLYING"));

	rcDims.top=30;
	rcDims.bottom=0;
	rcDims.left=-34;
	rcDims.right=34;

	bActiveSprites[0]=FALSE;
	bActiveSprites[1]=TRUE;

	CreateMode(bActiveSprites, rcDims, JC_NEUTRAL|JC_NONDESTRUCT, TEXT("DEAD"));

	SetObjectMode(1, dwTime);
	return TRUE;
}

BOOL CBadCopterObject::LoadObjectSprites(CSpriteManager* pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("COPTER2")), 0, 19, 150, LP_FORWARD);
	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("DOWNCOPTER2")), 0, 15, 0, LP_FORWARD);
	return TRUE;
}

///////////////////////////////
/// The Good Missile Object ///
///////////////////////////////

HRESULT CGoodMissileObject::ProcessAI(
	CInputManager *pInput, 
	void* pObjMan, 
	CTimerEx *timer, 
	CMapBoard* map)
{
	if(pInput==NULL)
	{
		//The missile exists for 8/10 second.
		if( (timer->Time()-m_dwCreateTime) > 800)
			SendMessage(JM_KILL);

		if( (timer->Time()-m_dwLastSmokeTime)>100)
		{
			int nX=0;

			if(m_nFace==SF_RIGHT)
			{
				nX=m_nX-25;
			}
			else if(m_nFace==SF_LEFT)
			{
				nX=m_nX+25;
			}
			m_dwLastSmokeTime=timer->Time();

			((CObjectManager*)pObjMan)->CreateObject(
				(OBJECTTYPE)JOES2_SMOKE,
				nX,
				m_nY-5,
				0,
				2);
		}

		//If the missile is no longer moving...
		if(ArchRelative(map, AR_ABOVE|AR_BELOW|AR_LEFT|AR_RIGHT))
		{
			SendMessage(JM_EXPLODE);
			/*
			((CObjectManager*)pObjMan)->CreateObject(
				JOES2_EXPOSION,
				m_nX,
				m_nY,
				0,
				0);
			*/
		}

	}
	else
	{

	}
	return S_OK;
}

CGoodMissileObject::CGoodMissileObject(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CJoes2Object(
	pSpriteMgr,
	dwTime,
	x,
	y,
	nXSpeed,
	nYSpeed),
	m_dwLastSmokeTime(dwTime),
	m_nLastX(x),
	m_nLastY(y)
{
	if(pSpriteMgr)
	{
		LoadObjectSprites(pSpriteMgr);
		CreateObjectModes(dwTime);
	}
	if(nXSpeed < 0)
		m_nFace=SF_LEFT;
}
BOOL CGoodMissileObject::LoadObjectSprites(CSpriteManager *pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("MISSILE2")), 0, 8, 150, LP_FORWARD);
	return TRUE;
}
BOOL CGoodMissileObject::CreateObjectModes(DWORD dwTime)
{
	RECT rcDims;
	rcDims.top=16;
	rcDims.bottom=0;
	rcDims.left=-20;
	rcDims.right=20;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_GOOD|JC_DESTRUCT, TEXT("MISSILE"));
	
	SetObjectMode(1, dwTime);

	return TRUE;
}

//////////////////////////////
/// The bad missile object ///
//////////////////////////////

CBadMissileObject::CBadMissileObject(
	CSpriteManager* pSpriteMgr, 
	DWORD dwTime,
	int x,
	int y,
	int nXSpeed,
	int nYSpeed):
	CGoodMissileObject(
		NULL,
		dwTime,
		x,
		y,
		nXSpeed,
		nYSpeed)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}
BOOL CBadMissileObject::LoadObjectSprites(CSpriteManager *pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("MISSILE3")), 0, 8, 150, LP_FORWARD);
	return TRUE;
}
BOOL CBadMissileObject::CreateObjectModes(DWORD dwTime)
{
	RECT rcDims;
	rcDims.top=16;
	rcDims.bottom=0;
	rcDims.left=-20;
	rcDims.right=20;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_BAD|JC_DESTRUCT, TEXT("MISSILE"));
	
	SetObjectMode(1, dwTime);

	return TRUE;
}



////////////////////////
/// The smoke object ///
////////////////////////

CSmokeObject::CSmokeObject(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CJoes2Object(
		pSpriteMgr,
		dwTime,
		x,
		y,
		nXSpeed,
		nYSpeed)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}

BOOL CSmokeObject::LoadObjectSprites(CSpriteManager* pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("SMOKE")), 0, 0, 150, LP_FORWARD);
	return TRUE;
}

BOOL CSmokeObject::CreateObjectModes(DWORD dwTime)
{
	RECT rcDims;
	rcDims.top=0;
	rcDims.bottom=0;
	rcDims.left=0;
	rcDims.right=0;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_NEUTRAL|JC_NONDESTRUCT, TEXT("SMOKE"));
	
	SetObjectMode(1, dwTime);
	return TRUE;
}

HRESULT CSmokeObject::ProcessAI(
	CInputManager* pInput, 
	void* pObjMan, 
	CTimerEx* timer, 
	CMapBoard* map)
{
	if(pInput==NULL)
	{
		//The missile exists for 8/10 second.
		if( (timer->Time()-m_dwCreateTime) > 200)
			SendMessage(JM_KILL);
	}
	else
	{

	}
	return S_OK;
}

////////////////////////////
/// The explosion object ///
////////////////////////////

CExplosionObject::CExplosionObject(
	CSpriteManager * pSpriteMgr, 
	DWORD dwTime, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed):
	CJoes2Object(
		pSpriteMgr,
		dwTime,
		x,
		y,
		nXSpeed,
		nYSpeed)
{
	LoadObjectSprites(pSpriteMgr);
	CreateObjectModes(dwTime);
}

HRESULT CExplosionObject::ProcessAI(
	CInputManager *pInput,
	void* pObjMan,
	CTimerEx* timer,
	CMapBoard* map)
{
	
	if( (timer->Time()-m_dwCreateTime) > 300)
		SendMessage(JM_KILL);
	return S_OK;
}

BOOL CExplosionObject::LoadObjectSprites(
	CSpriteManager* pSpriteMgr)
{
	if(pSpriteMgr==NULL)
		return FALSE;

	ObtainPointerToSprite(pSpriteMgr->LetPointer(TEXT("EXPLOSION")), 0, 0, 25, LP_ONCEFORWARD);
	return TRUE;
}

BOOL CExplosionObject::CreateObjectModes(DWORD dwTime)
{
	RECT rcDims;
	rcDims.top=0;
	rcDims.bottom=0;
	rcDims.left=0;
	rcDims.right=0;
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT];
	memset(&bActiveSprites, 0, sizeof(bActiveSprites));
	bActiveSprites[0]=TRUE;
	CreateMode(bActiveSprites, rcDims, JC_NEUTRAL|JC_NONDESTRUCT, TEXT("EXPLODE"));
	
	SetObjectMode(1, dwTime);
	return TRUE;
}

