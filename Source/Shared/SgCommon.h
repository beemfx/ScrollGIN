#pragma once

#if defined( __cplusplus )
#include <cstdint>
#include <fstream>
#include <vector>
#endif

#define __DXVERSION__ 11

#define countof( x ) ( sizeof(x)/sizeof(x[0]))

typedef const char*      sg_cpstr;
typedef unsigned __int8  sg_uint8;
typedef unsigned __int16 sg_uint16;
typedef unsigned __int32 sg_uint32;
#if defined( __cplusplus )
typedef wchar_t          sg_char16;
#else

#endif

struct sg_vec4
{
	float x , y , z, w;
};

struct sg_vec2
{
	float x , y;
};

struct sg_color
{
	float r , g , b , a;
};


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define SAFE_FREE(p)         { if(p) { free(p); (p)=NULL; } }

#if defined( _DEBUG )
#define assert( x ) if ( !(x) ){ __debugbreak(); }
#else
#define assert( x )
#endif
