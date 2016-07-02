#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CHsquantradectrl1 包装类

class CHsquantradectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CHsquantradectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xAD4ABDEA, 0x8D04, 0x4EBE, { 0x89, 0xDE, 0xDA, 0x1, 0xE1, 0xEF, 0x35, 0x5D } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 特性
public:


// 操作
public:

// _DHSQuanTrade

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetAllStrategy(long param1, LPCTSTR param2)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, param1, param2);
		return result;
	}
	CString GetStrategyParamInfo(LPCTSTR strategyname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strategyname);
		return result;
	}
	CString SetStrategyParam(LPCTSTR modelname, LPCTSTR paramname, LPCTSTR paramvalue)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, modelname, paramname, paramvalue);
		return result;
	}
	CString AddRunStrategy(LPCTSTR paramname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, paramname);
		return result;
	}
	CString StopStrategy(LPCTSTR strategynname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strategynname);
		return result;
	}
	CString PauseStrategy(LPCTSTR strategyname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strategyname);
		return result;
	}
	CString RunStrategy(LPCTSTR strategyname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strategyname);
		return result;
	}
	CString GetStrategyStatusInfo(LPCTSTR strategy)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strategy);
		return result;
	}

	CString Init(LPCTSTR param1)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, param1);
		return result;
	}
	CString Exit()
	{
		CString result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetParamValue(LPCTSTR modelname, LPCTSTR paramname)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, modelname, paramname);
		return result;
	}

	CString GetStrategyLog(LPCTSTR modelName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, modelName);
		return result;
	}

// Properties
//



};
