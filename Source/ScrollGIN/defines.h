/*
	ScrollGIN Defines.h - General Definitions for game engine

	Copyright (c) 2002, Blaine Myers
*/
#ifndef __DEFINES_H__
#define __DEFINES_H__

//Constants
#define SPEEDADJUST (50);

//Macros
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define MAX_TILESURFACES 255
#define MAX_SPRITES 255
#define MAX_IMAGES_PER_SPRITE 10
#define MAX_SPRITE_IMAGES (MAX_SPRITES*MAX_IMAGES_PER_SPRITE)

#define MAX_SPRITES_PER_OBJECT 10

#define MAX_OBJECT_MODES 25

#define MAX_SPRITE_NAME_LENGTH 15

#define MAX_BACKGROUNDS 5

#endif // Defines.h