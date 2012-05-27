/*
	ObjMan.cpp - The CObjectManager class

	Copyright (c) 2003, Blaine Myers
*/

#include "Defines.h"
#include "error.h"
#include "ObjMan.h"
#include "genfuncs.h"

CObjectManager::CObjectManager():
m_dwMaxObjects(DEFAULT_MAX_OBJECTS)
{
	SetError(TEXT("Created objects using default maximum"));
	m_dwCount=0;
	m_dwUserObject=0;
	m_ppObject=new CObject*[m_dwMaxObjects];
	m_pObjectType=new OBJECTTYPE[m_dwMaxObjects];
	for(DWORD i=0; i<m_dwMaxObjects; i++){
		m_ppObject[i]=NULL;
	}
}

CObjectManager::CObjectManager(DWORD dwMaxObjects):
m_dwMaxObjects(dwMaxObjects)
{
	SetError(TEXT("Created objects using user maximum"));
	m_dwCount=0;
	m_dwUserObject=0;
	m_ppObject=new CObject*[m_dwMaxObjects];
	m_pObjectType=new OBJECTTYPE[m_dwMaxObjects];
	for(DWORD i=0; i<m_dwMaxObjects; i++){
		m_ppObject[i]=NULL;
	}
}

CObjectManager::CObjectManager(
	DWORD dwMaxObjects, 
	CTimerEx * pTimer):
m_dwMaxObjects(dwMaxObjects)
{
	SetError(TEXT("Created objects using user and obtained pointers"));
	m_dwCount=0;
	m_dwUserObject=0;
	m_ppObject=new CObject*[m_dwMaxObjects];
	m_pObjectType=new OBJECTTYPE[m_dwMaxObjects];
	for(DWORD i=0; i<m_dwMaxObjects; i++){
		m_ppObject[i]=NULL;
	}

	ObtainTimer(pTimer);
}

CObjectManager::~CObjectManager(){
	SetError(TEXT("Successfully closed object manager!"));
	for(DWORD i=0; i<m_dwMaxObjects; i++){
		SAFE_DELETE(m_ppObject[i]);
	}
	SAFE_DELETE_ARRAY(m_ppObject);
}

HRESULT CObjectManager::CreateSpriteFromData(
	LPTSTR szFilename,
	LPVOID lpDirectDraw,
	DWORD dwTransparent,
	int nNumImages,
	LPTSTR szSpriteName,
	LPVOID lpCreationData)
{
	return m_SpriteManager.CreateSpriteFromData(
		lpDirectDraw,
		dwTransparent,
		nNumImages,
		szSpriteName,
		szFilename,
		lpCreationData);
}

void CObjectManager::Release()
{
	m_SpriteManager.Release();
}
void CObjectManager::Restore()
{
	m_SpriteManager.Restore();
}
void CObjectManager::ReloadImages()
{
	m_SpriteManager.ReloadImages();
}

HRESULT CObjectManager::LoadSpritesFromFile(
	LPTSTR szFilename, 
	LPVOID lpDirectDraw, 
	DWORD dwTransparent)
{
	return m_SpriteManager.CreateSpritesFromFile(
		lpDirectDraw, 
		dwTransparent, 
		szFilename);
}
int CObjectManager::ClearSprites()
{
	m_SpriteManager.ClearDataBase();
	return 1;
}

void CObjectManager::ClearObjects(){
	m_dwCount=0;

	for(DWORD i=0; i<m_dwMaxObjects; i++){
		SAFE_DELETE(m_ppObject[i]);
	}
}

void CObjectManager::Kill(DWORD dwIndex){
	if((m_dwCount<1))return;
	if(m_ppObject[dwIndex-1]){
		m_dwCount--;
		SAFE_DELETE(m_ppObject[dwIndex-1]);
	}
}

void CObjectManager::Cull(){
	//This function should check each object
	//and kill it if it's life has expired,
	//or technically no longer exists
	CObject* pObject;
	for(DWORD i=0; i<m_dwMaxObjects; i++)
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



BOOL CObjectManager::ObtainTimer(CTimerEx * pTimer)
{
	m_pTimer=pTimer;

	if(pTimer==NULL)return FALSE;
	else return TRUE;
}



HRESULT CObjectManager::CreateObject(
	const OBJECTTYPE nType, 
	int x, 
	int y, 
	int nXSpeed, 
	int nYSpeed)
{
	if(m_dwCount<m_dwMaxObjects){
		//Find first free slot
		DWORD dwTime=0;
		if(m_pTimer)
			dwTime=m_pTimer->Time();
		DWORD i=0;
		while(m_ppObject[i]!=NULL)i++;
		switch(nType)
		{
		case OT_DEFAULT:
			m_ppObject[i]=new CObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		default:
			m_ppObject[i]=new CObject(&m_SpriteManager, dwTime, x, y, nXSpeed, nYSpeed);
			break;
		}
		m_pObjectType[i]=nType;
		m_dwCount++;
		return S_OK;
	}else return E_FAIL;
}

void CObjectManager::Replace(DWORD dwIndex){
	if(dwIndex<1)return;
	CObject *pObject=m_ppObject[dwIndex-1];
	if(pObject==NULL)return;

	BOOL bKill=TRUE;
	OBJECTTYPE nNewType=OT_DEFAULT;

	//When this function is overloaded it will
	//determine which object should replace the
	//current object type.
	switch(m_pObjectType[dwIndex-1])
	{
	case OT_DEFAULT:
		break;
	default:
		break;
	}

	if(bKill)Kill(dwIndex);
	else{
		int nX=0, nY=0, nYSpeed=0, nXSpeed=0;
		nX=m_ppObject[dwIndex-1]->GetX();
		nY=m_ppObject[dwIndex-1]->GetY();
		nXSpeed=m_ppObject[dwIndex-1]->GetXSpeed();
		nYSpeed=m_ppObject[dwIndex-1]->GetYSpeed();
		SAFE_DELETE(m_ppObject[dwIndex-1]);
		CreateObject(nNewType, nX, nY, nXSpeed, nYSpeed);
	}
}

COLLISIONTYPE CObjectManager::DetectCollision(DWORD dwIndex1, DWORD dwIndex2){
	if((dwIndex1<1) || 
		(dwIndex2<1) || 
		(dwIndex1>m_dwMaxObjects) || 
		(dwIndex2>m_dwMaxObjects) )return CT_NOCLSN;

	if(m_ppObject[dwIndex1-1]==NULL)return CT_NOCLSN;
	if(m_ppObject[dwIndex2-1]==NULL)return CT_NOCLSN;


	return m_ppObject[dwIndex1-1]->DetectCollision(m_ppObject[dwIndex2-1]);
}

HRESULT CObjectManager::DetectCollisions(){
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
				default:
					break;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CObjectManager::Animate(
	LPVOID lpSurface, 
	CMapBoard *map,
	CViewPort *viewport,
	CInputManager* pInput)
{
	DWORD i=0;
	//if the game is paused we just draw the objects (and don't animate them)
	if(m_pTimer->IsPaused()){
		for(i=0; i<m_dwMaxObjects; i++){
			if(m_ppObject[i]!=NULL)
				m_ppObject[i]->Draw(viewport, lpSurface);
		}
		return S_OK;
	}
	//Create a new random seed for randomness.
	RandomSeed(timeGetTime());
	//animate each object individually (if user object we pass the input along)
	for(i=0; i<m_dwMaxObjects; i++){
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
		viewport->force_position(
			m_ppObject[m_dwUserObject-1]->GetX(), 
			m_ppObject[m_dwUserObject-1]->GetY());
		
	}
	else
	{
		viewport->stop_scroll();
	}

	//detect collisions (which will do all necessary steps if there is a collision)
	DetectCollisions();
	
	Cull(); //cull old objects

	//Draw each of the objects
	for(i=0; i<m_dwMaxObjects; i++){
		if(m_ppObject[i]!=NULL)
			m_ppObject[i]->Draw(viewport, lpSurface);
	}

	return S_OK;
}

void CObjectManager::SetUserObject(DWORD nIndex){
	if((nIndex<1) || (nIndex>m_dwMaxObjects))return;
	else m_dwUserObject=nIndex;
}

DWORD CObjectManager::GetUserObject(){
	return m_dwUserObject;
}

int CObjectManager::Initialize(LPVOID lpDD, DWORD dwTransparent)
{
	return 1;
}