/*
	ObjMan.h - Header for object manager

	Copyright (c) 2003, Blaine Myers
*/
#ifndef __OBJMAN_H__
#define __OBJMAN_H__

#include <windows.h>
#include "../MapBoard/Mapboard.h"
#include "Object.h"
#include "Timer.h"
#include "View.h"

#define DEFAULT_MAX_OBJECTS 256

typedef enum tagOBJECTTYPE{OT_DEFAULT=0}OBJECTTYPE;


class CObjectManager{
protected:
	CObject **m_ppObject; //array of pointers to objects
	OBJECTTYPE *m_pObjectType; //array of the type of object
	
	const DWORD m_dwMaxObjects; //maximum objects available
	DWORD m_dwCount;  //how many objects there are

	DWORD m_dwUserObject; //Which object the user is controlling

	CSpriteManager m_SpriteManager;
	CTimerEx * m_pTimer;

	void Cull();
	void Kill(DWORD dwIndex);
	void Replace(DWORD dwIndex);
public:
	CObjectManager();
	CObjectManager(DWORD dwMaxObjects);
	CObjectManager(DWORD dwMaxObjects, CTimerEx * pTimer);
	virtual ~CObjectManager();

	virtual int Initialize(DWORD dwTransparent);

	HRESULT Animate(
		CMapBoard *map,
		CViewPort *viewport,
		CInputManager* pInput); //Animate all the objects and draw them

	void ClearObjects(); //clear all objects from database

	HRESULT LoadSpritesFromFile(
		LPTSTR szFilename, 
		DWORD dwTransparent);
	
	int ClearSprites();

	void Release();

	BOOL ObtainTimer(
		CTimerEx * pTimer);

	virtual HRESULT CreateObject(
		const OBJECTTYPE nType, 
		int x, 
		int y, 
		int nXSpeed, 
		int nYSpeed); //create object of specified type

	virtual HRESULT DetectCollisions(); //detect collisions and take necessarys steps
	COLLISIONTYPE DetectCollision(
		DWORD nIndex1, 
		DWORD dwIndex2);

	void SetUserObject(DWORD nIndex);
	DWORD GetUserObject();

};

#endif //__OBJMAN_H__