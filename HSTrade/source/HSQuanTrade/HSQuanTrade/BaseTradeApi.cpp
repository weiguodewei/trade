#include "stdafx.h"

#include "BaseTradeApi.h"

#include "TradeInterface.h"




CBaseTradeApi::CBaseTradeApi()
{
	m_nOrderRefOffset = 0;

	strcpy_s(m_szBroker,"\0");

	strcpy_s(m_szUserID,"\0");

	strcpy_s(m_szBroker,"\0");

	strcpy_s(m_szPwd,"\0");

	strcpy_s(m_szTradeServer,"\0");

	m_bRisk = 0;

}


CBaseTradeApi::~CBaseTradeApi(void)
{

}


void CBaseTradeApi::Log(char * msg)
{
	::SendMessage(m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)msg);
}

void CBaseTradeApi::SetInterface(CTradeInterface * pt)
{
	m_pTradeInterface = pt;

}

bool CBaseTradeApi::CheckRisk(char *code,int count,double price,char bs,char kp,int type)
{
	if(!m_bRisk)
		return true;

	return data.CheckRisk(code,count,price,bs,kp,type);
}



void CBaseTradeApi::UpdateRiskInfo(char *code,int vol,double price,char bs,char kp)
{
	if(!m_bRisk)
		return;

	data.UpdateRiskInfo(code,vol,price,bs,kp);
}


bool CBaseTradeApi::ExistOrder(int orderref)
{
	if(!m_bRisk)
		return false;

	return data.ExistOrder(orderref);
}


void CBaseTradeApi::AddOrder(int orderref)
{
	if(!m_bRisk)
		return ;

	data.AddOrder(orderref);
}

void CBaseTradeApi::SetRiskInfo(double margin,int count,double pl,int open)
{
	m_bRisk = open;
	data.SetRiskInfo(margin,count,pl);
}

