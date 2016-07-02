#pragma once

// HSQuanTradePropPage.h : CHSQuanTradePropPage 属性页类的声明。


// CHSQuanTradePropPage : 有关实现的信息，请参阅 HSQuanTradePropPage.cpp。

class CHSQuanTradePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CHSQuanTradePropPage)
	DECLARE_OLECREATE_EX(CHSQuanTradePropPage)

// 构造函数
public:
	CHSQuanTradePropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_HSQUANTRADE };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

