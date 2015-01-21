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


class SgObjectManager{
protected:
	SgObject **m_ppObject; //array of pointers to objects
	OBJECTTYPE *m_pObjectType; //array of the type of object
	
	const DWORD m_dwMaxObjects; //maximum objects available
	DWORD m_dwCount;  //how many objects there are

	DWORD m_dwUserObject; //Which object the user is controlling

	SgSpriteManager m_SpriteManager;
	SgTimer * m_pTimer;

	void Cull();
	void Kill(DWORD dwIndex);
	void Replace(DWORD dwIndex);
public:
	SgObjectManager();
	SgObjectManager(DWORD dwMaxObjects);
	SgObjectManager(DWORD dwMaxObjects, SgTimer * pTimer);
	virtual ~SgObjectManager();

	virtual int Initialize();

	HRESULT Animate(
		CMapBoard *map,
		SgViewPort *viewport,
		SgInputManager* pInput); //Animate all the objects and draw them

	void ClearObjects(); //clear all objects from database

	void LoadSprites(LPTSTR szFilename);
	void ClearSprites();

	BOOL ObtainTimer(SgTimer* pTimer);

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