/*
	Object.h - header for object class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <windows.h>
#include "defines.h"
#include "Input.h"
#include "../MapBoard/mapboard.h"
#include "SpriteManager.h"
#include "timer.h"
#include "view.h"

#define AR_ABOVE 0x01000000l
#define AR_BELOW 0x02000000l
#define AR_LEFT  0x04000000l
#define AR_RIGHT 0x08000000l

#define MESSAGE_BUFFER_SIZE 10

typedef enum tagCOLLISIONTYPE
{
	CT_NOCLSN=0, 
	CT_STDCLSN, 
	CT_DANGERCLSN, 
	CT_SAFECLSN
}COLLISIONTYPE;

/*
typedef enum tagOBJECTALIGNMENT
{
	OA_GOOD=0, 
	OA_BAD, 
	OA_GOODBULLET, 
	OA_BADBULLET, 
	OA_NEUTRAL, 
	OA_NEUTRALBULLET
}OBJECTALIGNMENT;
*/

typedef struct tagSPRITEDATA
{
	int nX; //The x location of the sprite relative to the local of the object
	int nY; //The y location of the sprite relative to the local of the object

	int nNumFrames; //How many frames the object has
	int nCurrentFrame; //The current frame of animation
	DWORD nAnimationSpeed; //The speed of the sprite animation

	DWORD nLastUpdateTime; //Last time sprite was updated

	BOOL bActive;  //True if the current sprite should be drawn when draw is activated

	LOOPMODE nLoopMode; //how the sprite should loop
}SPRITEDATA;

typedef struct tagOBJECTMODE
{
	BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT]; //An array wich tells which sprites are active
	RECT rcObjDim; //rectangular structure containing size of object, this represent the dimension
								//releative to the location of the object, in which the sprite exists
	DWORD nType; //the type of object
	TCHAR szModeName[MAX_SPRITE_NAME_LENGTH];
}OBJECTMODE;


class CObject
{
protected:
	int m_nX, m_nY;	//The objects location
	int m_nDeltaX, m_nDeltaY; //change in location of object since last object
	int m_nXSpeed, m_nYSpeed;	//The object's speed

	int m_nXLastMoveTime, m_nYLastMoveTime; //Last time object moved

	CSprite* m_pSprite[MAX_SPRITES_PER_OBJECT]; //pointer to sprites used by the object
	SPRITEDATA m_sSpriteData[MAX_SPRITES_PER_OBJECT]; //Data about each sprite
	WORD m_nNumSprites;	//how many sprites there are

	OBJECTMODE m_sObjectMode[MAX_OBJECT_MODES];
	int m_nCurrentMode;
	int m_nNumModes;
	BOOL m_bAlive;
	LONG m_nMessage[MESSAGE_BUFFER_SIZE];//The message buffer, stores messages until they are processed
	WORD m_nNumMessages;//The number of messages that need processing

	SPRITEFACE m_nFace;	

public:
	//Constructors destructors
	CObject();
	CObject(CSpriteManager * pSpriteMgr, DWORD dwTime);
	CObject(CSpriteManager * pSpriteMgr, DWORD dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual ~CObject();

	//Initialization functions
	virtual BOOL LoadObjectSprites(CSpriteManager* pSpriteMgr);
	virtual BOOL CreateObjectModes(DWORD dwTime);

	//animation functions
	HRESULT Animate(
		CTimerEx *timer, 
		CMapBoard *map, 
		CInputManager* pInput, 
		LPVOID pObjMan); //Moves the sprite, based on how much time has passed

	BOOL SendMessage(LONG nMsg); //Sends a message to the object

	virtual BOOL ProcessMessages(LPVOID lpObjMan); //should process all messages in que

	virtual HRESULT ProcessAI(
		CInputManager* pInput, 
		LPVOID pObjMan, 
		CTimerEx* timer,
		CMapBoard* map);

	virtual BOOL PreInitialMovement(
		CMapBoard *map, 
		int *nXSpeed, 
		int *nYSpeed);

	virtual HRESULT InitialMovement(
		CTimerEx *timer, 
		int nXSpeed, 
		int nYSpeed);

	virtual HRESULT ArchAdjust(
		CTimerEx *timer, 
		CMapBoard *map);

	HRESULT DefaultArchAdjust(CMapBoard *map);
	
	//collision detectin with architecture
	virtual BOOL CollisionWithRect(
		CMapBoard *map, 
		RECT rect, 
		DWORD nWidth, 
		DWORD nHeight);

	BOOL ArchRelative(
		CMapBoard* map, 
		DWORD dwRelativeFlags);

	BYTE ArchRelative(
		CMapBoard* map, 
		int x, 
		int y);

	int DistanceFrom(CObject *cObject);

	virtual COLLISIONTYPE DetectCollision(
		CObject *cObject);

	//Object mode functions
	BOOL CreateMode(
		BOOL bActiveSprites[MAX_SPRITES_PER_OBJECT], 
		RECT rcObjDim, 
		DWORD nType, 
		TCHAR szModeName[MAX_SPRITE_NAME_LENGTH]);

	BOOL SetObjectMode(
		int nNewMode, 
		DWORD dwTime);

	BOOL SetObjectMode(
		TCHAR szModeName[MAX_SPRITE_NAME_LENGTH], 
		DWORD dwTime);

	//Object location and speed functions
	SPRITEFACE GetFace();
	BOOL SetObjectFace(SPRITEFACE nNewFace);

	BOOL SetSpeed(int nXSpeed, int nYSpeed);  //Sets the speed of the object
	BOOL SetXSpeed(int nXSpeed); //Sets the left/right speed of the object
	BOOL SetYSpeed(int nYSpeed); //Sets the up/down speed of the object
	int GetXSpeed(); //returns x speed
	int GetYSpeed(); //returns y speed

	BOOL SetPosition(int x, int y); //Forces the object to a certain position
	int GetX();  //returns x coordinate of object
	int GetY();  //recturns y coordinate of object

	void GetDelta(
		int *DeltaX, 
		int *DeltaY);

	BOOL IsAlive();
	void SetAliveState(BOOL bAlive);

	RECT GetObjectDim();

	DWORD GetObjectAlign();

	//sprite functions
	HRESULT ObtainPointerToSprite(
		CSprite* pSprite, 
		int x, 
		int y, 
		int nAnimSpeed, 
		LOOPMODE nLoopMode);  //puts pointer to sprite in next available position

	HRESULT ObtainPointerToSprite(
		int nIndex, 
		CSprite* pSprite, 
		int x, 
		int y, 
		int nAnimSpeed, 
		LOOPMODE nLoopMOde); //Obtains a pointer to a sprite
	
	BOOL SetNumSprites(int nNumSprites);

	//Drawing function
	void Draw(
		CViewPort *vp, 
		LPVOID lpBuffer); //Draws the appropriate sprites for object
};

#endif //__object_h__