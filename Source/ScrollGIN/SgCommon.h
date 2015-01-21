#pragma once

#define countof( x ) ( sizeof(x)/sizeof(x[0]))

typedef unsigned __int16 sg_uint16;
typedef unsigned __int32 sg_uint32;
#if defined( __cplusplus )
typedef wchar_t          sg_char16;
#else

#endif
