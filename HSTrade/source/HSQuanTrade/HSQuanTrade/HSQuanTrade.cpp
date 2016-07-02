// HSQuanTrade.cpp : CHSQuanTradeApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "HSQuanTrade.h"
#include "data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHSQuanTradeApp theApp;

const GUID CDECL _tlid = { 0x7868F789, 0x7146, 0x404C, { 0xA4, 0x24, 0xB, 0x8D, 0x3B, 0xDE, 0xCE, 0x50 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CHSQuanTradeApp::InitInstance - DLL 初始化

BOOL CHSQuanTradeApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	CPubData::m_mapExchaneId["SHFE"] = 0;
	CPubData::m_mapExchaneId["DCE"] = 0;
	CPubData::m_mapExchaneId["CZCE"] = 0;
	

	return bInit;
}



// CHSQuanTradeApp::ExitInstance - DLL 终止

int CHSQuanTradeApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
