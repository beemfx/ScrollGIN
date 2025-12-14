/*
	Object.cpp - The object class

	Copyright (c) 2002, Blaine Myers
*/

#include <math.h>
#include <stdio.h>
#include "object.h"
#include "ObjMan.h"
#include "view.h"


SgObject::SgObject()
{
	m_nNumMessages=0;
	m_nX=m_nY=0;
	m_nXSpeed=m_nYSpeed=0;
	m_nFace=SF_RIGHT;
	m_nNumModes=0;
	m_nCurrentMode=0;
	m_nXLastMoveTime=m_nYLastMoveTime=0;
	m_nDeltaX=m_nDeltaY=0;

	//Set the default object mode
	for(int i=0; i<MAX_OBJECT_MODES; i++)
	{
		m_sObjectMode[0].bActiveSprites[i]=TRUE;
	}

	m_sObjectMode[0].rcObjDim.top=-50;
	m_sObjectMode[0].rcObjDim.bottom=50;
	m_sObjectMode[0].rcObjDim.left=-50;
	m_sObjectMode[0].rcObjDim.right=50;
	//end of setting default object mode
	
	for(int i=0; i<MAX_SPRITES_PER_OBJECT; i++){
		m_pSprite[i]=NULL;
	}

	ZeroMemory(&m_sSpriteData, sizeof(SPRITEDATA)*MAX_SPRITES_PER_OBJECT);

	m_bAlive=TRUE;
	m_nNumSprites=0;
}

SgObject::SgObject(SgSpriteManager * pSpriteMgr, unsigned int dwTime)
{
	m_nNumMessages=0;
	m_nX=m_nY=0;
	m_nXSpeed=m_nYSpeed=0;
	m_nFace=SF_RIGHT;
	m_nNumModes=0;
	m_nCurrentMode=0;
	m_nXLastMoveTime=m_nYLastMoveTime=dwTime;
	m_nDeltaX=m_nDeltaY=0;
	int i=0;
	//Set the default object mode
	for(i=0; i<MAX_OBJECT_MODES; i++)
			m_sObjectMode[0].bActiveSprites[i]=TRUE;

	m_sObjectMode[0].rcObjDim.top=-50;
	m_sObjectMode[0].rcObjDim.bottom=50;
	m_sObjectMode[0].rcObjDim.left=-50;
	m_sObjectMode[0].rcObjDim.right=50;
	//end of setting default object mode
	
	for(i=0; i<MAX_SPRITES_PER_OBJECT; i++){
		m_pSprite[i]=NULL;
	}

	ZeroMemory(&m_sSpriteData, sizeof(SPRITEDATA)*MAX_SPRITES_PER_OBJECT);

	m_bAlive=TRUE;
	m_nNumSprites=0;
}

SgObject::SgObject(SgSpriteManager * pSpriteMgr, unsigned int dwTime, int x, int y, int nXSpeed, int nYSpeed)
{
	m_nNumMessages=0;
	m_nX=m_nY=0;
	m_nXSpeed=m_nYSpeed=0;
	m_nFace=SF_RIGHT;
	m_nNumModes=0;
	m_nCurrentMode=0;
	m_nXLastMoveTime=m_nYLastMoveTime=dwTime;
	m_nDeltaX=m_nDeltaY=0;

	int i=0;
	//Set the default object mode
	for(i=0; i<MAX_OBJECT_MODES; i++)
			m_sObjectMode[0].bActiveSprites[i]=TRUE;

	m_sObjectMode[0].rcObjDim.top=-50;
	m_sObjectMode[0].rcObjDim.bottom=50;
	m_sObjectMode[0].rcObjDim.left=-50;
	m_sObjectMode[0].rcObjDim.right=50;
	//end of setting default object mode
	
	for(i=0; i<MAX_SPRITES_PER_OBJECT; i++){
		m_pSprite[i]=NULL;
	}

	ZeroMemory(&m_sSpriteData, sizeof(SPRITEDATA)*MAX_SPRITES_PER_OBJECT);

	m_nNumSprites=0;

	m_bAlive=TRUE;

	SetPosition(x, y);
	SetSpeed(nXSpeed, nYSpeed);
}

SgObject::~SgObject(){
	
}

bool SgObject::SendMessage(int nMsg)
{
	if(m_nNumMessages >= MESSAGE_BUFFER_SIZE)return FALSE;
	//Filter out the message if it's already been sent
	for(WORD i=0; i<m_nNumMessages; i++)
		if(nMsg==m_nMessage[i])return false;
	//Put message in que
	m_nMessage[m_nNumMessages]=nMsg;
	m_nNumMessages++;

	return true;
}

bool SgObject::ProcessMessages(void* lpObjMan){
	//All messages should be processed

	//Clear the message que
	m_nNumMessages=0;
	return false;
}

bool SgObject::IsAlive(){
	return m_bAlive;
}

void SgObject::SetAliveState(bool bAlive){
	m_bAlive=bAlive;
}

void SgObject::ProcessAI(SgInputManager* pInput, void* pObjMan, SgTimer* timer, SgMap* map)
{
	//SgObjectManager* pObjectMan=(SgObjectManager*)pObjMan;
	
	if(pInput==NULL){
	//If the input manager is null it means we use AI

	}else{
	//if input manager exists we use input as intelligence
	
	}
}

RECT SgObject::GetObjectDim(){
	return m_sObjectMode[m_nCurrentMode].rcObjDim;
}

int SgObject::DistanceFrom(SgObject *cObject){
	if(this==cObject)return 0;
	//get the x and y distance
	int x = abs(m_nX - cObject->GetX());
	int y = abs(m_nY - cObject->GetY());

	//pythagoram's theorom will get us the distance
	return (int)sqrt((double)x*x+(double)y*y);
}

COLLISIONTYPE SgObject::DetectCollision(SgObject *cObject){
	//if we passed the object to itself we return
	if(this==cObject)return CT_NOCLSN;

	RECT rcObject, rcTemp, rcCurrentObject;
	rcObject=cObject->GetObjectDim();
	rcCurrentObject=GetObjectDim();

	//now that we have the rects we need to adjust them to the objects' positoins
	OffsetRect(&rcCurrentObject, m_nX, m_nY);

	OffsetRect(&rcObject, cObject->GetX(), cObject->GetY());

	/*
	#ifdef DEBUGRECTS
	DrawRect(rcCurrentObject, g_lpBackBuffer);
	DrawRect(rcObject, g_lpBackBuffer);
	#endif DEBUGRECTS
	*/

	if(IntersectRect(&rcTemp, &rcObject, &rcCurrentObject))
	{
		//we know that the objects collide so now we should return an
		//apppropriate value depending on if the objects are fatal to each other or not
		//the follwoing code is only an example of what the final might look like
		return CT_STDCLSN;
	}
	return CT_NOCLSN;
}

void SgObject::SetObjectMode(char szModeName[MAX_SPRITE_NAME_LENGTH], int dwTime){
	for(int i=1; i<=m_nNumModes; i++){
		if(strcmp(szModeName, m_sObjectMode[i].szModeName)==0)
			SetObjectMode(i, dwTime);
	}
}

void SgObject::SetObjectMode(int nNewMode, int dwTime){
	//if the mode hasn't changed we do nothing
	if(m_nCurrentMode==nNewMode)return;

	//if the mode is out of range we set it to the default mode
	if(nNewMode>m_nNumModes || nNewMode<1){
		nNewMode=0;
	}
	m_nCurrentMode=nNewMode;

	//Now that we have the current mode set we set the appropriate sprites
	bool bNewStatus;
	for(int i=0; i<m_nNumSprites; i++){
		bNewStatus=m_sObjectMode[m_nCurrentMode].bActiveSprites[i];
		if(m_sSpriteData[i].bActive!=bNewStatus){
			m_sSpriteData[i].nCurrentFrame=1;
			m_sSpriteData[i].bActive=bNewStatus;
			m_sSpriteData[i].nLastUpdateTime=dwTime;
		}
	}
}

int SgObject::GetObjectAlign(){
	return m_sObjectMode[m_nCurrentMode].nType;
}

void SgObject::CreateMode(bool bActiveSprites[MAX_SPRITES_PER_OBJECT], RECT rcObjDim, int nType, char szModeName[MAX_SPRITE_NAME_LENGTH])
{
	for(int i=0; i<m_nNumSprites; i++)
		m_sObjectMode[m_nNumModes+1].bActiveSprites[i]=bActiveSprites[i];
	//we revers top and bottom for the rects
	rcObjDim.top=-(rcObjDim.top);
	rcObjDim.bottom=-(rcObjDim.bottom);

	//make sure the rect was valid if it might not be we switch a few vars around
	LONG nTemp;
	if(rcObjDim.bottom < rcObjDim.top){
		nTemp=rcObjDim.bottom;
		rcObjDim.bottom=rcObjDim.top;
		rcObjDim.top=nTemp;
	}

	if(rcObjDim.left > rcObjDim.right){
		nTemp=rcObjDim.left;
		rcObjDim.left=rcObjDim.right;
		rcObjDim.left=nTemp;
	}
	m_sObjectMode[m_nNumModes+1].nType=nType;

	m_sObjectMode[m_nNumModes+1].rcObjDim=rcObjDim;
	strcpy_s( m_sObjectMode[m_nNumModes+1].szModeName , countof(m_sObjectMode[m_nNumModes+1].szModeName) , szModeName );
	
	m_nNumModes++;
}

SPRITEFACE SgObject::GetFace()
{
	return m_nFace;
}

void SgObject::Draw(SgViewPort *vp){
	for(int i=0; i<m_nNumSprites; i++){
		if(m_pSprite[i]!=NULL){
			if(m_sSpriteData[i].bActive==TRUE){
				m_pSprite[i]->Draw(
							m_sSpriteData[i].nCurrentFrame,
							m_nFace, 
							vp->screenX(m_nX+m_sSpriteData[i].nX), 
							vp->screenY(m_nY-m_sSpriteData[i].nY),
							m_sSpriteData[i].nLoopMode);
			}
		}
	}

	#ifdef DEBUGRECTS
	RECT rcObject;
	rcObject=GetObjectDim();

	//now that we have the rects we need to adjust them to the objects' positoins

	OffsetRect(&rcObject, m_nX, m_nY);
	DrawRect(rcObject, (LPDIRECTDRAWSURFACE7)lpBuffer, vp);
	#endif //DEBUGRECTS
}

bool SgObject::SetSpeed(int nXSpeed, int nYSpeed){
	SetXSpeed(nXSpeed); SetYSpeed(nYSpeed);
	return TRUE;
}

bool SgObject::SetXSpeed(int nXSpeed){
	m_nXSpeed=nXSpeed;
	return TRUE;
}

bool SgObject::SetYSpeed(int nYSpeed){
	m_nYSpeed=nYSpeed;
	return TRUE;
}

int SgObject::GetX(){
	return m_nX;
}

int SgObject::GetY(){
	return m_nY;
}

void SgObject::InitialMovement(SgTimer *timer, int nXSpeed, int nYSpeed)
{
	int xdelta=0, ydelta=0;
	int time=timer->Time();

	const int MOVESPEEDADJUST=SPEEDADJUST;

	//we need to animate the x
	int tfactor=time-m_nXLastMoveTime;

	xdelta=(nXSpeed*tfactor)/MOVESPEEDADJUST;
	if(xdelta|| (nXSpeed==0))
		m_nXLastMoveTime=time;
	
	//we need to animate the y
	tfactor=time-m_nYLastMoveTime;
	ydelta=(nYSpeed*tfactor)/MOVESPEEDADJUST;

	ydelta=-ydelta;

	if(ydelta|| (nYSpeed==0))
		m_nYLastMoveTime=time;

	//Set the position
	SetPosition(m_nX+=xdelta, m_nY+=ydelta);


	m_nDeltaX=xdelta;
	m_nDeltaY=ydelta;

	//animate the sprites
	for(int i=0; i<m_nNumSprites; i++)
	{
		if(m_sSpriteData[i].bActive==TRUE)
		{
			if(static_cast<int>((timer->Time()-m_sSpriteData[i].nLastUpdateTime))>m_sSpriteData[i].nAnimationSpeed)
			{
				m_sSpriteData[i].nLastUpdateTime=timer->Time();// just setting to this to the current time isn't the best way because the animation may be off by a few millisecons
				m_sSpriteData[i].nCurrentFrame++;
				if(m_sSpriteData[i].nCurrentFrame>m_sSpriteData[i].nNumFrames){
					if((m_sSpriteData[i].nLoopMode!=LP_ONCEFORWARD) &&(m_sSpriteData[i].nLoopMode!=LP_ONCEBACKWARD))
						m_sSpriteData[i].nCurrentFrame=1;
					else m_sSpriteData[i].nCurrentFrame=m_sSpriteData[i].nNumFrames;
				}
			}
		}
	}
}

bool SgObject::CollisionWithRect(SgMap *map, RECT rect, int nWidth, int nHeight)
{
	int i=0, j=0;
	int dwTileDim=map->GetTileDim();
	bool result=FALSE;

	//detect if there is a collision with the specifiect rectangle
	for(i=rect.left; i <= rect.right; i+=dwTileDim){
		for(j=rect.top; j<=rect.bottom; j+=dwTileDim){
			if(ArchRelative(map, i, j)){
				result=TRUE;
			}
		}
	}
	//the following method did not work out
	/*
	for(i=0; i < ((int)(nWidth/40)); i++){
		for(j=0; j < ((int)(nHeight/40)); j++){
			sprintf(string, "i: %i j: %i\n%i, %i, %i, %i", i, j, rect.left, rect.right, nWidth, nHeight);
			MessageBox(0, string, 0, 0);
			if(ArchRelative(map, rect.left + (i*dwTileDim), rect.top + (j*dwTileDim))){
				//sprintf(string, "x: %i y: %i", rect.left + (i*dwTileDim), rect.top + (j*dwTileDim));
				//MessageBox(0, string, 0, 0);
				result=TRUE;
			}
		}
	}
	*/
	//now test the other two corners
	if(ArchRelative(map, rect.left, rect.bottom))result=TRUE;
	if(ArchRelative(map, rect.right, rect.bottom))result=TRUE;
	if(ArchRelative(map, rect.right, rect.top))result=TRUE;

	return result;
}

/*
	The base version of DefaultArchAdjust will prevent an object from
	entering any position that is not blank.  It is designed to have
	"slippery" surfaces, that is an object will not stop when it hits
	architecture but it will slide along (if it was moving at an angle).

	This is probably the largest, most complicated function I have ever
	written.  I can only imagine what it would be like to create a function
	like this for a 3D game.
*/
void SgObject::DefaultArchAdjust(SgMap *map)
{
	//the default move, stops no matter what type of architecture, should
	//be used as a template for all movements

	//we don't need to do anything if there wasn't any movement
	if( (m_nDeltaX==0) && (m_nDeltaY==0 )) return;
	//we need the tile width because we can use it to speed up the process
	int dwTileWidth=map->GetTileDim();

	int nHeight, nWidth;//width and height of the boject
	nWidth=m_sObjectMode[m_nCurrentMode].rcObjDim.right-m_sObjectMode[m_nCurrentMode].rcObjDim.left;
	nHeight=m_sObjectMode[m_nCurrentMode].rcObjDim.bottom-m_sObjectMode[m_nCurrentMode].rcObjDim.top;


	if(m_nDeltaY==0){
		//only moved in x direction
		int nSavedX=m_nX;

		int nMoveAmount=0;

		if(m_nDeltaX < 0)
			nMoveAmount=-1;
		if(m_nDeltaX > 0)
			nMoveAmount=1;

		if(m_nDeltaX*nMoveAmount < nWidth){

			if(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
				//we need to attempt moving backwards till there is no collision, unless the movement
				//was greater than the object width because we'll have to make sure we didn't move
				//through anything

				m_nX-=nMoveAmount;

				int MaxCollisionChecks = nWidth + nHeight;
				for (int i = 0; i < MaxCollisionChecks; i++)
				{
					if (CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight))
					{
						m_nX -= nMoveAmount;
					}
				}
			}
		}else { //if the amount of movement was greater than the width
			
			m_nX-=m_nDeltaX;
			while(!CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
				m_nX+=nMoveAmount;
				if(m_nX==nSavedX){
					m_nX+=nMoveAmount; 
					break;
				}
			}	
			m_nX-=nMoveAmount;
		
		}
	
	}else if(m_nDeltaX==0){
		//we do this if only the y value has changed
		int nSavedY=m_nY;

		int nMoveAmount=0;

		if(m_nDeltaY < 0)
			nMoveAmount=-1;
		if(m_nDeltaY > 0)
			nMoveAmount=1;

		if(m_nDeltaY*nMoveAmount < nHeight){

			if(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
				//we need to attempt moving backwards till there is no collision, unless the movement
				//was greater than the object width because we'll have to make sure we didn't move
				//through anything

				m_nY-=nMoveAmount;

				while(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
					m_nY-=nMoveAmount;
				}		
			}
		}else { //if the amount of movement was greater than the width
			
			m_nY-=m_nDeltaY;
			while(!CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
				m_nY+=nMoveAmount;
				if(m_nY==nSavedY){
					m_nY+=nMoveAmount; 
					break;
				}
			}	
			m_nY-=nMoveAmount;
		
		}


	}else{
		//we do this if both values have changed
		int nSavedX=m_nX, nSavedY=m_nY;
		int nMoveAmountX=0, nMoveAmountY=0;

		if(m_nDeltaX < 0)
			nMoveAmountX=-1;
		if(m_nDeltaX > 0)
			nMoveAmountX=1;

		if(m_nDeltaY < 0)
			nMoveAmountY=-1;
		if(m_nDeltaY > 0)
			nMoveAmountY=1;

		if( (m_nDeltaX*nMoveAmountX < nWidth) && (m_nDeltaY*nMoveAmountY < nHeight) ){
			//movement was less than width or height
			if(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
				//there was a collision lets move
				
				//the following loop (with all its complication) may only be completely effective
				//if the absolute value m_nDeltaX and m_nDeltaY values are equal
				//However, I have done extensive tests and it does seem to work when the two are.
				//at a ratio less than 1:2 relative to each other
				//strangely I have no idea how I came up with this loop.  After a lot
				//of thought I just sort of camp up with the generalized theory
				//at high framerates speed tends to be cut in half
				do{
					m_nX-=nMoveAmountX;

					if(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
						
						m_nX+=nMoveAmountX;
						m_nY-=nMoveAmountY;

						if(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
							m_nX-=nMoveAmountX;
							if(m_nY==nSavedY)break;
							continue;
						}

					}else break;

					if(m_nX==nSavedX)
						break;

				}while(CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight));
		
			}
		}else{
			//this method works fairly effectively
			
			//first move the object to it's original position
			m_nX-=m_nDeltaX;
			m_nY-=m_nDeltaY;
			if( abs(m_nDeltaX) == abs(m_nDeltaY) ){
				//if the change in y is the same as the change is x this is quite a bit simplier
				
				//move one pixel at a time untill collision
				while(!CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
					if( (m_nX == nSavedX)  || (m_nY == nSavedY) )
						break;
					m_nX+=nMoveAmountX;
					m_nY+=nMoveAmountY;

				}
				m_nX-=nMoveAmountX;
				m_nY-=nMoveAmountY;
			}else{
				//movement amounts were not the same

				//we find the relative movement to each other and go on the smaller one
				int nRatio=0;

				if(abs(m_nDeltaY) > abs(m_nDeltaX) ){
					nRatio = ( abs(m_nDeltaY) / abs(m_nDeltaX) );

					while(!CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
						if( (m_nX == nSavedX)  || (m_nY == nSavedY) )
							break;

						m_nX+=nMoveAmountX;
						m_nY+=nMoveAmountY*nRatio;

					}
					m_nX-=nMoveAmountX;
					m_nY-=nMoveAmountY*nRatio;

				}else{
					nRatio = ( abs(m_nDeltaX) / abs(m_nDeltaY) );

					while(!CollisionWithRect(map, m_sObjectMode[m_nCurrentMode].rcObjDim, nWidth, nHeight)){
						if( (m_nX == nSavedX)  || (m_nY == nSavedY) )
							break;

						m_nX+=nMoveAmountX*nRatio;
						m_nY+=nMoveAmountY;

					}
					m_nX-=nMoveAmountX*nRatio;
					m_nY-=nMoveAmountY;
				}
			}

		}
	}	
}

void SgObject::ArchAdjust(SgTimer *timer, SgMap *map)
{
	//at this point we should call a custom function that is designed for
	//the specific game.  If there is no outside function we pull off the defult
	//move
	DefaultArchAdjust(map);
}

int SgObject::GetXSpeed(){
	return m_nXSpeed;
}

int SgObject::GetYSpeed(){
	return m_nYSpeed;
}

void SgObject::Animate(SgTimer *timer, SgMap *map, SgInputManager* pInput, void* pObjMan)
{
	int nTempXSpeed=m_nXSpeed;
	int nTempYSpeed=m_nYSpeed;

	int nX=m_nX;
	int nY=m_nY;


	ProcessAI(pInput, pObjMan, timer, map);
	ProcessMessages(pObjMan);
	PreInitialMovement(map, &nTempXSpeed, &nTempYSpeed);
	InitialMovement(timer, nTempXSpeed, nTempYSpeed);
	ArchAdjust(timer, map);

	//make sure we don't move in teh opposite direction were
	//supposed to.  Should remove this is problems are experienced.
	//with the object not moving in the opposite direction when it
	//should (but that should be controlled by the artificial
	//intelligence and not the architecture adjustments.)
	if((nTempXSpeed > 0) && (m_nX<nX))m_nX=nX;
	if((nTempXSpeed < 0) && (m_nX>nX))m_nX=nX;

	if((nTempYSpeed > 0) && (m_nY>nY))m_nY=nY;
	if((nTempYSpeed < 0) && (m_nY<nY))m_nY=nY;

	//Now make sure we are still within the bounds of the map.
	RECT rcObject=GetObjectDim();

	if((m_nX+rcObject.left) < 0)
		m_nX=rcObject.right;

	if((m_nY+rcObject.top) < 0)
		m_nY=0-rcObject.top;

	if((m_nX+rcObject.right) > (signed int)(map->GetMapWidth()*map->GetTileDim()))
		m_nX=map->GetMapWidth()*map->GetTileDim()-rcObject.right;

	if((m_nY+rcObject.bottom) > (signed int)(map->GetMapHeight()*map->GetTileDim()))
		m_nY=map->GetMapHeight()*map->GetTileDim()-rcObject.bottom;
}

bool SgObject::SetNumSprites(int nNumSprites){
	m_nNumSprites=nNumSprites;
	return TRUE;
}

bool SgObject::SetObjectFace(SPRITEFACE nNewFace){
	//if the face is the same we don't need to change anything
	if(nNewFace==m_nFace)return FALSE;
	
	//Change the face
	m_nFace=nNewFace;
	//when we change the face we need to reset all the sprites to their first frame
	for(int i=0; i<m_nNumSprites; i++){
		m_sSpriteData[i].nCurrentFrame=1;
	}
	return TRUE;
}
bool SgObject::SetPosition(int x, int y){
	m_nX=x;
	m_nY=y;
	return TRUE;
}


bool SgObject::ObtainPointerToSprite(SgSprite* pSprite, int x, int y, int nAnimSpeed, LOOPMODE nLoopMode)
{
	if(m_nNumSprites>MAX_SPRITES_PER_OBJECT)return false;
	bool Res = ObtainPointerToSprite(m_nNumSprites+1, pSprite, x, y, nAnimSpeed, nLoopMode);

	if( Res )
		m_nNumSprites++;

	return Res;
}

bool SgObject::ObtainPointerToSprite(int nIndex, SgSprite* pSprite, int x, int y, int nAnimSpeed, LOOPMODE nLoopMode)
{
	if(nIndex<1 || nIndex > MAX_SPRITES_PER_OBJECT)return false;
	if(pSprite==NULL)return false;
	//Set the pointer
	m_pSprite[nIndex-1]=pSprite;

	//Set the data for the sprite
	m_sSpriteData[nIndex-1].nX=x;
	m_sSpriteData[nIndex-1].nY=y;
	m_sSpriteData[nIndex-1].nAnimationSpeed=nAnimSpeed;
	m_sSpriteData[nIndex-1].nCurrentFrame=1;
	m_sSpriteData[nIndex-1].nNumFrames=pSprite->GetNumFrames(nLoopMode);
	m_sSpriteData[nIndex-1].bActive=TRUE;
	m_sSpriteData[nIndex-1].nLoopMode=nLoopMode;

	return true;
}

BYTE SgObject::ArchRelative(SgMap *map, int x, int y){
	//we first need to adjust the coordinates relative to the object
	x+=m_nX;
	y+=m_nY;

	BYTE nArch=0x00;

	//Return achitecture if out of range.
	if( (x<0) || (y<0) )
		return 0x11;

	if( (x > (int)(map->GetMapWidth()*map->GetTileDim())) ||
		(y > (int)(map->GetMapHeight()*map->GetTileDim())) )
		return 0x11;

	//now find out which type of Arch is at the selected location
	return map->GetArch((int)x/map->GetTileDim()+1, (int)y/map->GetTileDim()+1);
}

unsigned __int8 SgObject::ArchRelative(SgMap* map, unsigned __int32 dwRelativeFlags)
{
	bool bArchIsRelative=FALSE;
	RECT rcCurrent=m_sObjectMode[m_nCurrentMode].rcObjDim;
	int dwTileDim=map->GetTileDim();
	int i=0;

	if((dwRelativeFlags&AR_ABOVE)==AR_ABOVE)
	{
		//Check to see if arch is above
		for(i=rcCurrent.left; i<=rcCurrent.right; i+=dwTileDim)
		{
			if(ArchRelative(map, i, rcCurrent.top-1))
				bArchIsRelative=TRUE;
		}
		/*
		if(ArchRelative(map, rcCurrent.left, rcCurrent.top-1))
			bArchIsRelative=TRUE;
		*/
		if(ArchRelative(map, rcCurrent.right, rcCurrent.top-1))
			bArchIsRelative=TRUE;

	}

	if((dwRelativeFlags&AR_BELOW)==AR_BELOW){
		//Check to see if arch is below
		for(i=rcCurrent.left; i<=rcCurrent.right; i+=dwTileDim){
			if(ArchRelative(map, i, rcCurrent.bottom+1))
				bArchIsRelative=TRUE;
		}
		/*
		if(ArchRelative(map, rcCurrent.left, rcCurrent.bottom+1))
			bArchIsRelative=TRUE;
		*/
		if(ArchRelative(map, rcCurrent.right, rcCurrent.bottom+1))
			bArchIsRelative=TRUE;
	}

	if((dwRelativeFlags&AR_LEFT)==AR_LEFT){
		//Check to see if arch is below
		for(i=rcCurrent.top; i<=rcCurrent.bottom; i+=dwTileDim){
			if(ArchRelative(map, rcCurrent.left-1, i))
				bArchIsRelative=TRUE;
		}
		/*
		if(ArchRelative(map, rcCurrent.left-1, rcCurrent.top))
			bArchIsRelative=TRUE;
		*/
		if(ArchRelative(map, rcCurrent.left-1, rcCurrent.bottom))
			bArchIsRelative=TRUE;
	}

	if((dwRelativeFlags&AR_RIGHT)==AR_RIGHT){
		//Check to see if arch is below
		for(i=rcCurrent.top; i<=rcCurrent.bottom; i+=dwTileDim){
			if(ArchRelative(map, rcCurrent.right+1, i))
				bArchIsRelative=TRUE;
		}
		/*
		if(ArchRelative(map, rcCurrent.right+1, rcCurrent.top))
			bArchIsRelative=TRUE;
		*/
		if(ArchRelative(map, rcCurrent.right+1, rcCurrent.bottom))
			bArchIsRelative=TRUE;
	}
	
	
	

	return bArchIsRelative;

}

bool SgObject::PreInitialMovement(
	SgMap *map, 
	int *nXSpeed, 
	int *nYSpeed)
{
	if((*nXSpeed==0) && (*nYSpeed==0))return TRUE;


	if(ArchRelative(map, AR_ABOVE)){
		if(*nYSpeed>0)*nYSpeed=0;
	}
	if(ArchRelative(map, AR_BELOW)){
		if(*nYSpeed<0)*nYSpeed=0;
	}
	
	if(ArchRelative(map, AR_LEFT)){
		if(*nXSpeed<0)*nXSpeed=0;
	}
	
	if(ArchRelative(map, AR_RIGHT)){
		if(*nXSpeed>0)*nXSpeed=0;
	}	
	
	return TRUE;
}

