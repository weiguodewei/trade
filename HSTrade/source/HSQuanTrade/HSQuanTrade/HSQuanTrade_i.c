

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Tue Jun 28 09:40:53 2016
 */
/* Compiler settings for HSQuanTrade.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_HSQuanTradeLib,0x7868F789,0x7146,0x404C,0xA4,0x24,0x0B,0x8D,0x3B,0xDE,0xCE,0x50);


MIDL_DEFINE_GUID(IID, DIID__DHSQuanTrade,0x5E64DA04,0x4094,0x4246,0x9A,0xD6,0xD7,0x6D,0x53,0x47,0xB0,0xA1);


MIDL_DEFINE_GUID(IID, DIID__DHSQuanTradeEvents,0xE2482DB0,0x6A90,0x4C95,0x9C,0x65,0x3A,0xBB,0xAB,0x4D,0x14,0xB4);


MIDL_DEFINE_GUID(CLSID, CLSID_HSQuanTrade,0xAD4ABDEA,0x8D04,0x4EBE,0x89,0xDE,0xDA,0x01,0xE1,0xEF,0x35,0x5D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



