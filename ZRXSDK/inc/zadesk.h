

#ifndef   _ZSOFT_H
#define   _ZSOFT_H

#include "zAdAChar.h" 

#if __LP64__
#define ZSoft_Boolean_is_bool 1
#endif

#pragma pack (push, 8)

#if defined(_MSC_VER)
#define _ZSOFT_WINDOWS_ 1
#else
#include <stdint.h>
#if defined(__APPLE__) && defined(__MACH__)
#define _ZSOFT_MAC_
#else
#define _ZSOFT_LINUX_
#endif

#define __w64

#if __LP64__
	#define _ZC64
#endif //__LP64__

#endif //_MSC_VER

struct ZSoft
{

    typedef char       Int8;
    typedef short      Int16;

    typedef unsigned char  UInt8;
    typedef unsigned short UInt16;

#ifdef __LP64__
	typedef unsigned int   UInt32;
	typedef int            Int32;
#else
	typedef unsigned long  UInt32;
	typedef long           Int32;
#endif //__LP64__

#ifndef _ZSOFT_WINDOWS_
	typedef int64_t				Int64;
	typedef uint64_t			UInt64;

#else
	typedef __int64				Int64;
	typedef unsigned __int64	UInt64;
#endif //_ZSOFT_WINDOWS_


    typedef unsigned char  uchar;
    typedef unsigned short ushort;
    typedef unsigned int   uint;

#ifdef __LP64__
	typedef UInt32         ulong;
#else
	typedef unsigned long  ulong;
#endif //__LP64__

#if !defined(_WIN64) && !defined (_ZC64)

    typedef __w64 long          LongPtr;
    typedef __w64 unsigned long ULongPtr;
    
    typedef __w64 int           IntPtr;
    typedef __w64 unsigned int  UIntPtr;
#else //_WIN64 || _ZC64
   
#ifndef _ZSOFT_WINDOWS_
	typedef int64_t             LongPtr;
	typedef uint64_t            ULongPtr;
	typedef int64_t             IntPtr;
	typedef uint64_t            UIntPtr;
#else
	typedef __int64             LongPtr;
	typedef unsigned __int64    ULongPtr;
	typedef __int64             IntPtr;
	typedef unsigned __int64    UIntPtr;
#endif // _ZSOFT_WINDOWS_

#endif //_WIN64 || _ZC64

    typedef LongPtr IntDbId;
    typedef IntPtr GsMarker;

#ifndef ZSoft_Boolean_is_bool
    typedef int        Boolean;
    enum           { kFalse = 0, kTrue = 1 };
#else
    typedef bool       Boolean;
    static const bool kFalse = false;
    static const bool kTrue = true;
#endif
};

#ifndef NULL
#define NULL 0
#endif

#pragma pack (pop)
typedef          char         LEGACY_CHAR;
typedef          char*        LEGACY_STRING;
typedef const    char*        LEGACY_STRING_CONST;
typedef       unsigned short  WIDE_CHAR;          
typedef       unsigned short* WIDE_STRING;        
typedef const unsigned short* WIDE_STRING_CONST;  

typedef          char*   MEMORY_POINTER;
typedef const    char*   MEMORY_POINTER_CONST;

typedef unsigned char    ZC_BYTE;

#if defined(_MSC_VER)
#define ZSOFT_NO_VTABLE __declspec(novtable)
#define ZSOFT_STDCALL   __stdcall
#define ZSOFT_DEPRECATED __declspec(deprecated)
#define ZSOFT_DLLEXPORT	__declspec(dllexport)
#define ZSOFT_DLLIMPORT __declspec(dllimport)
#define ZSOFT_FORCE_INLINE __forceinline
#else //_MSC_VER
#define ZSOFT_NO_VTABLE 
#define ZSOFT_STDCALL   __attribute__((stdcall))
#define ZSOFT_DEPRECATED 
#define ZSOFT_DLLEXPORT	__attribute__((visibility("default")))
#define ZSOFT_DLLIMPORT __attribute__((visibility("default")))
#define ZSOFT_FORCE_INLINE inline
#endif //_MSC_VER


#endif
