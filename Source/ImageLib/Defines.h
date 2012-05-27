#ifndef __DEFINES_H__
#define __DEFINES_H__

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_FREE(p)         { if(p) { free(p); (p)=NULL; } }

#endif //__DEFINES_H__