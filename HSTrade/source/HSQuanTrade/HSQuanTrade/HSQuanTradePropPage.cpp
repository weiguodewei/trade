// HSQuanTradePropPage.cpp : CHSQuanTradePropPage 属性页类的实现。

#include "stdafx.h"
#include "HSQuanTrade.h"
#include "HSQuanTradePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CHSQuanTradePropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(CHSQuanTradePropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CHSQuanTradePropPage, "HSQUANTRADE.HSQuanTradePropPage.1",
	0xb1b8ac58, 0xc93c, 0x4033, 0xb5, 0xb4, 0x8b, 0x5e, 0x97, 0x95, 0x7b, 0xb9)

// CHSQuanTradePropPage::CHSQuanTradePropPageFactory::UpdateRegistry -
// 添加或移除 CHSQuanTradePropPage 的系统注册表项

BOOL CHSQuanTradePropPage::CHSQuanTradePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_HSQUANTRADE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CHSQuanTradePropPage::CHSQuanTradePropPage - 构造函数

CHSQuanTradePropPage::CHSQuanTradePropPage() :
	COlePropertyPage(IDD, IDS_HSQUANTRADE_PPG_CAPTION)
{
}

// CHSQuanTradePropPage::DoDataExchange - 在页和属性间移动数据

void CHSQuanTradePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CHSQuanTradePropPage 消息处理程序
