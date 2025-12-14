/*
	ObjMan.cpp - The SgObjectManager class

	Copyright (c) 2003, Blaine Myers
*/

#include "ObjMan.h"
#include "GameConfig.h"
#include "genfuncs.h"

SgObjectManager::SgObjectManager(int dwMaxObjects)
: m_dwMaxObjects(dwMaxObjects)
{
	m_dwCount=0;
	m_dwUserObject=0;
	m_ppObject=new SgObject*[m_dwMaxObjects];
	m_pObjectType=new OBJECTTYPE[m_dwMaxObjects];
	for(int i=0; i<m_dwMaxObjects; i++){
		m_ppObject[i]=NULL;
	}
}

SgObjectManager::~SgObjectManager()
{
	for(int i=0; i<m_dwMaxObjects; i++){
		SAFE_DELETE(m_ppObject[i]);
	}
	SAFE_DELETE_ARRAY(m_ppObject);
}

void SgObjectManager::ClearSprites()
{
	m_SpriteManager.Clear();
}

void SgObjectManager::LoadSprites(const char* szFilename)
{
	 m_SpriteManager.LoadLib(szFilename);
}

void SgObjectManager::ClearObjects()
{
	m_dwCount=0;

	for(int i=0; i<m_dwMaxObjects; i++)
	{
		SAFE_DELETE(m_ppObject[i]);
	}
}

void SgObjectManager::Kill(int dwIndex)
{
	if((m_dwCount<1))return;
	if(m_ppObject[dwIndex-1]){
		m_dwCount--;
		SAFE_DELETE(m_ppObject[dwIndex-1]);
	}
}

void SgObjectManager::Cull()
{
	//This function should check each object
	//and kill it if it's life has expired,
	//or technically no longer exists
	SgObject* pObject;
	for(int i=0; i<m_dwMaxObjects; i++)
	{
		pObject=m_ppObject[i];
		if(pObject != NULL){
			if(!pObject->IsAlive())
			{
				Kill(i+1);
				if( (i+1)==m_dwUserObject)
					m_dwUserObject=0;
			}
		}
	}
}


void SgObjectManager::SetTimer(SgTimer* pTimer)
{
	m_pTimer=pTimer;
}


void SgObjectManager::CreateObject(	const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed)
{
	if(m_dwCount<m_dwMaxObjects)
	{
		//Find first free slot
		int dwTime=0;
		if(m_pTimer)
			dwTime=m_pTimer->Time();
		int i=0;
		while(m_ppObject[i]!=NULL)i++;
		switch(nType)
		{
		case OT_DEFAULT:
			m_ppObject[i]=new SgObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		default:
			m_ppObject[i]=new SgObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		}
		m_pObjectType[i]=nType;
		m_dwCount++;
	}
}

COLLISIONTYPE SgObjectManager::DetectCollision( SgObject* Obj1 , SgObject* Obj2 )
{
	assert( Obj1 && Obj2 );

	return Obj1->DetectCollision(Obj2);
}

void SgObjectManager::DetectCollisions()
{
	for(int i=0; i<m_dwMaxObjects; i++)
	{
		if(m_ppObject[i]!=NULL)
		{
			for(int j=i+1; j<m_dwMaxObjects; j++)
			{
				if( NULL == m_ppObject[j] )continue;
				//here overloaded function should do
				//appropriate action depending on type
				//of collision.
				switch(DetectCollision(m_ppObject[i], m_ppObject[j]))
				{
				case CT_STDCLSN:
					OnCollision( m_ppObject[i] , m_ppObject[j] );
					break;
				default:
					break;
				}
			}
		}
	}
}

void SgObjectManager::Update( SgMap *map , SgViewPort *viewport , SgInputManager* pInput )
{
	//if the game is paused we just draw the objects (and don't animate them)
	if(m_pTimer->IsPaused())
	{
		return;
	}
	//Create a new random seed for randomness.
	RandomSeed(timeGetTime());
	//animate each object individually (if user object we pass the input along)
	for(int i=0; i<m_dwMaxObjects; i++){
		//if object exists
		if(m_ppObject[i]!=NULL){
			if(i!=(m_dwUserObject-1)){
				m_ppObject[i]->Animate(m_pTimer, map, NULL, this);
			}else{
				m_ppObject[i]->Animate(m_pTimer, map, pInput, this);
			}
		}
	}

	//set the viewport to the user objects
	viewport->update(m_pTimer->Time());

	if((m_dwUserObject>0) && (m_ppObject[m_dwUserObject-1]!=NULL) )
	{
		viewport->force_position(m_ppObject[m_dwUserObject-1]->GetX(), m_ppObject[m_dwUserObject-1]->GetY());
		
	}
	else
	{
		viewport->stop_scroll();
	}

	//detect collisions (which will do all necessary steps if there is a collision)
	DetectCollisions();
	
	Cull(); //cull old objects
}

void SgObjectManager::Draw( SgViewPort *viewport )
{
	//Draw each of the objects
	for(int i=0; i<m_dwMaxObjects; i++)
	{
		if(m_ppObject[i])m_ppObject[i]->Draw(viewport);
	}
}

void SgObjectManager::SetUserObject(int nIndex)
{
	if((nIndex<1) || (nIndex>m_dwMaxObjects))return;
	else m_dwUserObject=nIndex;
}

int SgObjectManager::GetUserObject()
{
	return m_dwUserObject;
}

void SgObjectManager::Initialize(SgScrollGINGame* InGame)
{
	
}