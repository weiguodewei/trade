#include "stdafx.h"
#include "FeimaMduserSpi.h"
#include <string.h>
#include <iostream>
using namespace std;

void CFeimaMduserSpi::OnFrontConnected()
{
	CUstpFtdcReqUserLoginField req;
	memset(&req,0,sizeof(CUstpFtdcReqUserLoginField));
	strcpy(req.TradingDay,pUserApiHQ_FM->GetTradingDay());
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.UserID,m_szInvertID);
	strcpy(req.Password,m_szPwd);
	strcpy(req.UserProductInfo, "fuhaoming hft1.0");
	pUserApiHQ_FM->ReqUserLogin(&req,0);
}
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
void CFeimaMduserSpi::OnFrontDisconnected(int nReason)
{
}
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
void CFeimaMduserSpi::OnHeartBeatWarning(int nTimeLapse)
{
}
	
	///报文回调开始通知。当API收到一个报文后，首先调用本方法，然后是各数据域的回调，最后是报文回调结束通知。
	///@param nTopicID 主题代码（如私有流、公共流、行情流等）
	///@param nSequenceNo 报文序号
void CFeimaMduserSpi::OnPackageStart(int nTopicID, int nSequenceNo)
{
}
	
	///报文回调结束通知。当API收到一个报文后，首先调用报文回调开始通知，然后是各数据域的回调，最后调用本方法。
	///@param nTopicID 主题代码（如私有流、公共流、行情流等）
	///@param nSequenceNo 报文序号
void CFeimaMduserSpi::OnPackageEnd(int nTopicID, int nSequenceNo)
{
}


	///错误应答
void CFeimaMduserSpi::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///风控前置系统用户登录应答
void CFeimaMduserSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) )
	{
		SubscribeMarketData();
	}
}

void CFeimaMduserSpi::SubscribeMarketData()
{
	pUserApiHQ_FM->SubMarketData(m_ppInstrumentID,m_nInsNum);
}

	///用户退出应答
void CFeimaMduserSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///订阅主题应答
void CFeimaMduserSpi::OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///主题查询应答
void CFeimaMduserSpi::OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

	///深度行情通知
void CFeimaMduserSpi::OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData) 
{


	
}

	///订阅合约的相关信息
void CFeimaMduserSpi::OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) )
	{
		
	}
}

	///退订合约的相关信息
void CFeimaMduserSpi::OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

bool CFeimaMduserSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		char temp[128];

		sprintf(temp," ErrorID= %d ErrorMsg=%s\n",pRspInfo->ErrorID,pRspInfo->ErrorMsg);

	}
	return bResult;
}
