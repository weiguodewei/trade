#ifndef _H5API_STD_DEF_H__
	#define _H5API_STD_DEF_H__
	#include <stdio.h>
	#ifdef _WIN32
		typedef char				int8;
		typedef short				int16;
		typedef int					int32;
		typedef __int64				int64;
		typedef unsigned char		uint8;
		typedef unsigned short		uint16;
		typedef unsigned int		uint32;
		typedef unsigned __int64	uint64;
	#else
		#include <sys/types.h>
		#if defined(__linux__)
			#include <stdint.h>
		#endif
		typedef int8_t				int8;
		typedef int16_t				int16;
		typedef int32_t				int32;
		typedef int64_t				int64;
		typedef uint8_t				uint8;
		typedef uint16_t			uint16;
		typedef uint32_t			uint32;
		typedef uint64_t			uint64;
	#endif


	#ifdef _WIN32
		#if !defined( FUNCTION_CALL_MODE )
			#define FUNCTION_CALL_MODE	__stdcall
		#endif
	#else
		#define FUNCTION_CALL_MODE
	#endif
		
	#ifndef HS_SID
		#define HS_SID	const char *
	#endif
	#ifndef IHS_KNOWN
		#define IHS_KNOWN
		struct IHSKnown
		{
			///接口查询
			/**查询与当前接口相关的其他接口，例如可以查到 IIoC, IManager 等
			 *@param HS_SID  iid  接口全局唯一标识
			 *@param IKnown **ppv 返回iid对应的接口指针
			 *@return I_OK 成功，I_NONE 未查到iid 相应接口
			 */
			virtual unsigned long  FUNCTION_CALL_MODE QueryInterface( HS_SID iid, IHSKnown **ppv ) = 0;
			///引用接口，引用计数加一(多线程引用时，方法实现代码里要对计数值加锁后修改)
			virtual unsigned long  FUNCTION_CALL_MODE AddRef() = 0;
			///释放接口，引用计数减一，计数为0时释放接口的实现对象(多线程引用时，方法实现代码里要对计数值加锁后修改)
			virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
		};
	#endif
#endif
