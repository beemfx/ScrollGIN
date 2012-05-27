#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define TILEDIM 40
#define PALETTEWIDTH 3
#define PALETTEDIM (TILEDIM*PALETTEWIDTH+GetSystemMetrics(SM_CXHSCROLL))

#endif //__defines_h__