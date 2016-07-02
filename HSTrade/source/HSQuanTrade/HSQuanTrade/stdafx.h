#pragma once

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  从 Windows 头文件中排除极少使用的信息
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#define _ATL_NO_AUTOMATIC_NAMESPACE             // 避免类名称冲突

#include <afxwin.h> 
#include <afxctl.h>         // ActiveX 控件的 MFC 支持
#include <afxext.h>         // MFC 扩展

#include <afxdisp.h>        // MFC 自动化类

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

// 如果不希望使用 MFC 数据库类，
//  请删除下面的两个包含文件
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT



#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifdef _DEBUG
//#include "vld.h"
#endif

#define WM_MESSAGE_TRADE WM_USER + 1000
#define WM_MESSAGE_PRICE WM_USER + 1001

#define WM_MESSAGE_ENTRUST WM_USER + 1002

#define WM_MESSAGE_SOCK WM_USER + 1003

//#define WM_MODEL_RESULT_MSG    WM_USER+1004 /*has been moved to TradeStatistic2.h.*/

#define WM_MESSAGE_BACKTEST_STEP WM_USER + 1005


#define	LOG_LEVEL_FORMAT	-1
#define	LOG_LEVEL_MAIN		0
#define	LOG_LEVEL_SUB_1		1
#define	LOG_LEVEL_SUB_2		2
#define	LOG_LEVEL_SUB_3		3



#endif // _WIN64





