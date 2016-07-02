#include "stdafx.h"
#include "MdSpiCTP.h"
#include <iostream>
#include "typedefine.h"
#include "StragegyManager.h"
#include "StrategyBase.h"
#include "Order.h"
#include "TradeInterface.h"

using namespace std;

#pragma warning(disable : 4996)



void CMdSpiCTP::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}

void CMdSpiCTP::OnFrontDisconnected(int nReason)
{
}
		
void CMdSpiCTP::OnHeartBeatWarning(int nTimeLapse)
{
}

void CMdSpiCTP::OnFrontConnected()
{
	CString log ="MDCTP->行情前置连接成功";
	m_MdStrategy_CTP->Log(log.GetBuffer(0));

	m_iRequestID = 0;
	///用户登录请求
	ReqUserLogin();
}

void CMdSpiCTP::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_szBroker);
	strcpy(req.UserID, m_szUserID);
	strcpy(req.Password, m_szPwd);
	int iResult = m_pUserApiHQ_CTP->ReqUserLogin(&req, ++m_iRequestID);

	cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpiCTP::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		strcpy(m_szDate, pRspUserLogin->TradingDay);
		SubscribeMarketData();	
	}
}

void CMdSpiCTP::SubscribeMarketData()
{
	int iResult = m_pUserApiHQ_CTP->SubscribeMarketData(m_ppInstrumentID, m_nInsNum);
}

void CMdSpiCTP::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CString log = pSpecificInstrument->InstrumentID;
	log ="MDCTP->" + log + "订阅成功";

	if(!IsErrorRspInfo(pRspInfo))
	{
		if(pSpecificInstrument)
		{
			m_MdStrategy_CTP->Log(log.GetBuffer(0));
			cout<<"["<<pSpecificInstrument->InstrumentID<<"] 订阅成功"<<endl;
		}
	}
}

void CMdSpiCTP::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

//行情报价
void CMdSpiCTP::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	CTickData  pd ;

	memset(&pd, 0, sizeof(CTickData));

	strcpy(pd.szInstrument, pDepthMarketData->InstrumentID);

	strcpy(pd.szDate, m_szDate);
	strcpy(pd.szTime, pDepthMarketData->UpdateTime);

	pd.bidprice[0] = pDepthMarketData->BidPrice1;
	pd.bidprice[1] = pDepthMarketData->BidPrice2;
	pd.bidprice[2] = pDepthMarketData->BidPrice3;
	pd.bidprice[3] = pDepthMarketData->BidPrice4;
	pd.bidprice[4] = pDepthMarketData->BidPrice5;

	pd.bidvol[0] = pDepthMarketData->BidVolume1;
	pd.bidvol[1] = pDepthMarketData->BidVolume2;
	pd.bidvol[2] = pDepthMarketData->BidVolume3;
	pd.bidvol[3] = pDepthMarketData->BidVolume4;
	pd.bidvol[4] = pDepthMarketData->BidVolume5;

	pd.askprice[0] = pDepthMarketData->AskPrice1;
	pd.askprice[1] = pDepthMarketData->AskPrice2;
	pd.askprice[2] = pDepthMarketData->AskPrice3;
	pd.askprice[3] = pDepthMarketData->AskPrice4;
	pd.askprice[4] = pDepthMarketData->AskPrice5;

	pd.askvol[0] = pDepthMarketData->AskVolume1;
	pd.askvol[1] = pDepthMarketData->AskVolume2;
	pd.askvol[2] = pDepthMarketData->AskVolume3;
	pd.askvol[3] = pDepthMarketData->AskVolume4;
	pd.askvol[4] = pDepthMarketData->AskVolume5;

	pd.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	pd.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;

	pd.Volume = pDepthMarketData->Volume;

	pd.lastprice = pDepthMarketData->LastPrice;

	pd.highestprice = pDepthMarketData->HighestPrice;
	pd.preClosePrice = pDepthMarketData->PreClosePrice;

	m_MdStrategy_CTP->ModelTick(&pd);

	//处理类型为1的订单
	m_MdStrategy_CTP->m_pTradeInterface->OrderTick(&pd);

}

bool CMdSpiCTP::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		//TRACE(" ErrorID= %d ErrorMsg=%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);
	}
	return bResult;
}