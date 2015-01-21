/*
	Object.h - header for object class

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <windows.h>
#include "GameConfig.h"
#include "Input.h"
#include "Mapboard/SgMap.h"
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
	int nAnimationSpeed; //The speed of the sprite animation

	int nLastUpdateTime; //Last time sprite was updated

	bool bActive;  //True if the current sprite should be drawn when draw is activated

	LOOPMODE nLoopMode; //how the sprite should loop
}SPRITEDATA;

typedef struct tagOBJECTMODE
{
	bool bActiveSprites[MAX_SPRITES_PER_OBJECT]; //An array wich tells which sprites are active
	RECT rcObjDim; //rectangular structure containing size of object, this represent the dimension
								//releative to the location of the object, in which the sprite exists
	int nType; //the type of object
	char szModeName[MAX_SPRITE_NAME_LENGTH];
}OBJECTMODE;


class SgObject
{
protected:
	int        m_nX;
	int        m_nY;
	int        m_nDeltaX;
	int        m_nDeltaY;
	int        m_nXSpeed;
	int        m_nYSpeed;
	int        m_nXLastMoveTime; 
	int        m_nYLastMoveTime;
	SgSprite*  m_pSprite[MAX_SPRITES_PER_OBJECT]; //pointer to sprites used by the object
	SPRITEDATA m_sSpriteData[MAX_SPRITES_PER_OBJECT]; //Data about each sprite
	int        m_nNumSprites;	//how many sprites there are
	OBJECTMODE m_sObjectMode[MAX_OBJECT_MODES];
	int        m_nCurrentMode;
	int        m_nNumModes;
	bool       m_bAlive;
	int        m_nMessage[MESSAGE_BUFFER_SIZE];//The message buffer, stores messages until they are processed
	int        m_nNumMessages;//The number of messages that need processing
	SPRITEFACE m_nFace;	

public:
	//Constructors destructors
	SgObject();
	SgObject(SgSpriteManager * pSpriteMgr, unsigned int dwTime);
	SgObject(SgSpriteManager * pSpriteMgr, unsigned int dwTime, int x, int y, int nXSpeed, int nYSpeed);
	virtual ~SgObject();

	//Initialization functions
	virtual void LoadObjectSprites(SgSpriteManager* pSpriteMgr){ };
	virtual void CreateObjectModes(int dwTime){ };

	//animation functions
	void Animate(SgTimer *timer, SgMap *map, SgInputManager* pInput, void* pObjMan); //Moves the sprite, based on how much time has passed

	bool SendMessage(int nMsg); //Sends a message to the object

	virtual bool ProcessMessages(void* lpObjMan); //should process all messages in que
	virtual void ProcessAI(	SgInputManager* pInput, void* pObjMan, SgTimer* timer, SgMap* map);
	virtual bool PreInitialMovement(SgMap *map, int *nXSpeed, int *nYSpeed);
	virtual void InitialMovement(	SgTimer *timer, int nXSpeed, int nYSpeed);
	virtual void ArchAdjust(SgTimer *timer, SgMap *map);

	void DefaultArchAdjust(SgMap *map);
	
	//collision detectin with architecture
	virtual bool CollisionWithRect(SgMap *map, RECT rect, int nWidth, int nHeight);
	unsigned __int8 ArchRelative(SgMap* map, unsigned __int32 dwRelativeFlags);
	unsigned __int8 ArchRelative(SgMap* map, int x, int y);

	int DistanceFrom(SgObject *cObject);

	virtual COLLISIONTYPE DetectCollision(	SgObject *cObject);

	//Object mode functions
	void CreateMode( bool bActiveSprites[MAX_SPRITES_PER_OBJECT], RECT rcObjDim, int nType, char szModeName[MAX_SPRITE_NAME_LENGTH]);
	void SetObjectMode(int nNewMode, int dwTime);
	void SetObjectMode(char szModeName[MAX_SPRITE_NAME_LENGTH], int dwTime);

	//Object location and speed functions
	SPRITEFACE GetFace();
	bool SetObjectFace(SPRITEFACE nNewFace);

	bool SetSpeed(int nXSpeed, int nYSpeed);  //Sets the speed of the object
	bool SetXSpeed(int nXSpeed); //Sets the left/right speed of the object
	bool SetYSpeed(int nYSpeed); //Sets the up/down speed of the object
	int GetXSpeed(); //returns x speed
	int GetYSpeed(); //returns y speed

	bool SetPosition(int x, int y); //Forces the object to a certain position
	int GetX();  //returns x coordinate of object
	int GetY();  //recturns y coordinate of object

	bool IsAlive();
	void SetAliveState(bool bAlive);

	RECT GetObjectDim();

	int GetObjectAlign();

	//sprite functions
	bool ObtainPointerToSprite(SgSprite* pSprite, int x, int y, int nAnimSpeed, LOOPMODE nLoopMode);

	bool ObtainPointerToSprite(int nIndex, SgSprite* pSprite, int x, int y, int nAnimSpeed, LOOPMODE nLoopMOde);
	
	bool SetNumSprites(int nNumSprites);

	//Drawing function
	void Draw(SgViewPort *vp); //Draws the appropriate sprites for object
};

#endif //__object_h__