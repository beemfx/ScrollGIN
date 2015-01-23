/*
	ObjMan.h - Header for object manager

	Copyright (c) 2003, Blaine Myers
*/
#ifndef __OBJMAN_H__
#define __OBJMAN_H__

#include "Mapboard/SgMap.h"
#include "Object.h"
#include "Timer.h"
#include "View.h"

#define DEFAULT_MAX_OBJECTS 256

enum OBJECTTYPE{OT_DEFAULT=0};


class SgObjectManager
{
protected:
	SgObject**      m_ppObject; //array of pointers to objects
	OBJECTTYPE*     m_pObjectType; //array of the type of object
	const int       m_dwMaxObjects; //maximum objects available
	int             m_dwCount;  //how many objects there are
	int             m_dwUserObject; //Which object the user is controlling
	SgSpriteManager m_SpriteManager;
	SgTimer*        m_pTimer;
private:
	void Cull();
	void Kill(int dwIndex);
public:
	SgObjectManager();
	SgObjectManager(int dwMaxObjects);
	SgObjectManager(int dwMaxObjects, SgTimer* pTimer);
	virtual ~SgObjectManager();

	virtual int Initialize();

	void Update( SgMap *map , SgViewPort *viewport , SgInputManager* pInput );
	void Draw( SgViewPort *viewport ); //Animate all the objects and draw them

	void ClearObjects(); //clear all objects from database

	void LoadSprites(const char* szFilename);
	void ClearSprites();

	void SetTimer(SgTimer* pTimer);
	virtual void CreateObject( const OBJECTTYPE nType, int x, int y, int nXSpeed, int nYSpeed);

	void DetectCollisions(); //detect collisions and take necessarys steps
	virtual void OnCollision( SgObject* Obj1 , SgObject* Obj2 )=0;
	COLLISIONTYPE DetectCollision( SgObject* Obj1 , SgObject* Obj2 );

	void SetUserObject(int nIndex);
	int GetUserObject();
};

#endif //__OBJMAN_H__