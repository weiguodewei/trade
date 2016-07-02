#include "stdafx.h"
#include "FeimaTraderSpi.h"
#include <string.h>
#include <stdio.h>
using namespace std;
#include <iostream>
#include <vector>




void CFeimaTraderSpi::OnFrontConnected()
{
	strcpy(ORDER_REF,"\0");

	strcpy(m_szMyInvestorID,"\0");

	m_iRequestID = 0;

	m_OrderActionID = 0;

	CUstpFtdcReqUserLoginField req;

	memset(&req,0,sizeof(CUstpFtdcReqUserLoginField));

	strcpy(req.TradingDay,m_pUserApiTrade->GetTradingDay());

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.Password,m_szPwd);

	strcpy(req.UserProductInfo, "fuhaoming hft1.0");

	m_pUserApiTrade->ReqUserLogin(&req,m_iRequestID++);

	m_nOrderRefQry = -1;
	
}
	
///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CFeimaTraderSpi::OnFrontDisconnected(int nReason)
{
}
		
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CFeimaTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
}
///报文回调开始通知。当API收到一个报文后，首先调用本方法，然后是各数据域的回调，最后是报文回调结束通知。
///@param nTopicID 主题代码（如私有流、公共流、行情流等）
///@param nSequenceNo 报文序号
void CFeimaTraderSpi::OnPackageStart(int nTopicID, int nSequenceNo)
{
}	
///报文回调结束通知。当API收到一个报文后，首先调用报文回调开始通知，然后是各数据域的回调，最后调用本方法。
///@param nTopicID 主题代码（如私有流、公共流、行情流等）
///@param nSequenceNo 报文序号
void CFeimaTraderSpi::OnPackageEnd(int nTopicID, int nSequenceNo)
{
}


///错误应答
void CFeimaTraderSpi::OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}

///风控前置系统用户登录应答
void CFeimaTraderSpi::OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( bIsLast && !IsErrorRspInfo(pRspInfo) && pRspUserLogin  )
	{
		m_iNextOrderRef = atoi( pRspUserLogin->MaxOrderLocalID)+1+m_nOrderRefOffset;	
	
		InitOrderReq();
	}
}
///用户退出应答
void CFeimaTraderSpi::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

///报单录入应答
void CFeimaTraderSpi::OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}
///报单操作应答
void CFeimaTraderSpi::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	IsErrorRspInfo(pRspInfo);
}
///数据流回退通知
void CFeimaTraderSpi::OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) 
{
}


///成交回报
void CFeimaTraderSpi::OnRtnTrade(CUstpFtdcTradeField *pTrade) 
{
	if( pTrade && strcmp(pTrade->UserID,m_szUserID) == 0 )//添加userid的判断来过滤非本帐号的成交单。需要调试验证是否有效
	{
	}
}

bool CFeimaTraderSpi::IsTradingOrder(CUstpFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != USTP_FTDC_OS_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != USTP_FTDC_OS_Canceled));
}

///报单回报
void CFeimaTraderSpi::OnRtnOrder(CUstpFtdcOrderField *pOrder) 
{
	if( !pOrder )
		return; 


}

///报单录入错误回报
void CFeimaTraderSpi::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo) 
{
	IsErrorRspInfo(pRspInfo);
}

///报单操作错误回报
void CFeimaTraderSpi::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo) 
{
	if( !pOrderAction )
		return;

	char temp[128];
	sprintf(temp,"OnErrRtnOrderAction  %s %s %s",pOrderAction->OrderSysID,pOrderAction->UserOrderActionLocalID,pOrderAction->UserOrderLocalID);


	IsErrorRspInfo(pRspInfo);
}

///合约交易状态通知
void CFeimaTraderSpi::OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus) 
{
}

///账户出入金回报
void CFeimaTraderSpi::OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) 
{
}

///报单查询应答
//添加盘中查询的过滤，只对查询的orderref进行返回。
void CFeimaTraderSpi::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !IsErrorRspInfo(pRspInfo) && pOrder )
	{
		if( m_nOrderRefQry == -1 || atoi(pOrder->UserOrderLocalID )== m_nOrderRefQry )
		{
			
			
		}		
	}
}

///成交单查询应答
void CFeimaTraderSpi::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{	
	if( pTrade && !IsErrorRspInfo(pRspInfo)  )
	{
		if( m_nTradeQry == -1 || m_nTradeQry == atoi( pTrade->UserOrderLocalID )  )
		{
			

		}
	}
}

///可用投资者账户查询应答
void CFeimaTraderSpi::OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///交易编码查询应答
void CFeimaTraderSpi::OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}

///投资者资金账户查询应答
void CFeimaTraderSpi::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !pRspInvestorAccount )
		return; 

	strcpy(m_szMyInvestorID, pRspInvestorAccount->InvestorID);
	

}

///合约查询应答
void CFeimaTraderSpi::OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///交易所查询应答
void CFeimaTraderSpi::OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	
}

//查询持仓
void CFeimaTraderSpi::RspQryInvestorPosition()
{
	m_vectorPos.clear();

	CUstpFtdcQryInvestorPositionField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorPositionField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InstrumentID,m_szInstrumentID);
	strcpy(req.UserID,m_szUserID);
	strcpy(req.InvestorID,m_szMyInvestorID);
	m_pUserApiTrade->ReqQryInvestorPosition(&req,++m_iRequestID);
}

///投资者持仓查询应答
void CFeimaTraderSpi::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( !IsErrorRspInfo(pRspInfo) && pRspInvestorPosition )
	{
	
	}
}


///订阅主题应答
void CFeimaTraderSpi::OnRspSubscribeTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}


///合规参数查询应答
void CFeimaTraderSpi::OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///主题查询应答
void CFeimaTraderSpi::OnRspQryTopic(CUstpFtdcDisseminationField *pDissemination, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
}
///投资者手续费率查询应答
void CFeimaTraderSpi::OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(!IsErrorRspInfo(pRspInfo) && pInvestorFee )
	{
	}
}
///投资者保证金率查询应答
void CFeimaTraderSpi::OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if(!IsErrorRspInfo(pRspInfo) && pInvestorMargin )
	{
	}
}


int CFeimaTraderSpi::ReqOrderAction(CUstpFtdcOrderField *pOrder)
{
	return ReqOrderAction(atoi(pOrder->UserOrderLocalID));	
}

int CFeimaTraderSpi::ReqOrderAction(int localID)
{
	CUstpFtdcOrderActionField req;

	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_szBroker);
	///投资者代码
	strcpy(req.InvestorID, m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	///报单引用，被撤订单的本地报单编号
	sprintf(req.UserOrderLocalID,"%012d",localID);
	///操作标志
	req.ActionFlag = USTP_FTDC_AF_Delete;

	strcpy(req.ExchangeID,"CFFEX");
		
	strcpy(req.OrderSysID,"\0");
	
	//和CTP不一样的地方，撤单也需要进行递增，会导致OrderRtn中ORDER_REF和UserOrderLocalID不一致的情况。
	m_iNextOrderRef++;

	sprintf(req.UserOrderActionLocalID, "%012d", m_iNextOrderRef);

	int iResult = m_pUserApiTrade->ReqOrderAction(&req, ++m_iRequestID);

	InitOrderAction();

	return m_iNextOrderRef;
}


int CFeimaTraderSpi::ReqOrderInsert(char * szHeyue,TUstpFtdcOffsetFlagType flagtype,TUstpFtdcDirectionType dir,double price,int count,TUstpFtdcTimeConditionType type )
{
	if( szHeyue[0] == 'I' )
		price = int((price+0.001)/0.2)*0.2;

	CUstpFtdcInputOrderField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.InstrumentID, szHeyue);

	strcpy(req.ExchangeID,"CFFEX");

	m_iNextOrderRef++;

	sprintf(req.UserOrderLocalID, "%012d", m_iNextOrderRef);
	
	req.OrderPriceType = USTP_FTDC_OPT_LimitPrice;

	req.Direction = dir;

	req.OffsetFlag = flagtype;

	req.HedgeFlag = USTP_FTDC_CHF_Speculation;

	req.LimitPrice = price;

	req.Volume = count;

	req.TimeCondition = type;
	
	req.VolumeCondition = USTP_FTDC_VC_AV;

	req.MinVolume = 1;

	req.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	req.IsAutoSuspend = 0;

	int iResult = m_pUserApiTrade->ReqOrderInsert(&req, ++m_iRequestID);

	return m_iNextOrderRef;
}

bool CFeimaTraderSpi::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
	}
	return bResult;
}

void CFeimaTraderSpi::ReqQryInvestorAccount(void)
{
	CUstpFtdcQryInvestorAccountField req;

	memset(&req,0,sizeof(CUstpFtdcQryInvestorAccountField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.UserID,m_szUserID);

	m_pUserApiTrade->ReqQryInvestorAccount(&req,m_iRequestID++);
}


void CFeimaTraderSpi::ReqQryOrder(int orderref)
{
	m_nOrderRefQry = orderref;

	CUstpFtdcQryOrderField req;

	memset(&req,0,sizeof(CUstpFtdcQryOrderField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.ExchangeID,"CFFEX");

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	//strcpy(req.InstrumentID,m_szInstrumentID);//测试不填写，是不是返回所有合约的）

	int nt = m_pUserApiTrade->ReqQryOrder(&req,m_iRequestID++);

	char msg[128];

	sprintf(msg,"ReqQryOrder %d %d",nt,orderref);


	while( nt != 0 )
	{
		Sleep(1000);

		nt = m_pUserApiTrade->ReqQryOrder(&req,m_iRequestID++);

		sprintf(msg,"ReqQryOrder %d %d",nt,orderref);

	}
}

void CFeimaTraderSpi::ReqQryTrader(int orderref)
{
	m_nTradeQry = orderref;

	CUstpFtdcQryTradeField req;

	memset(&req,0,sizeof(CUstpFtdcQryTradeField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.ExchangeID,"CFFEX");

	strcpy(req.InvestorID,m_szMyInvestorID);

	strcpy(req.UserID,m_szUserID);

	strcpy(req.InstrumentID,m_szInstrumentID);

	int nt = m_pUserApiTrade->ReqQryTrade(&req,m_iRequestID++);	

	if( nt != 0 )
	{
		char temp[128];

		sprintf(temp,"ReqQryTrader Error %d",nt);

	}
}


void CFeimaTraderSpi::ReqQryInvestFee(void)
{
	CUstpFtdcQryInvestorFeeField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorFeeField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InvestorID,m_szMyInvestorID);
	strcpy(req.UserID,m_szUserID);
	m_pUserApiTrade->ReqQryInvestorFee(&req,m_iRequestID++);	
}

void CFeimaTraderSpi::ReqQryInvestMargin(void)
{

	CUstpFtdcQryInvestorMarginField req;
	memset(&req,0,sizeof(CUstpFtdcQryInvestorMarginField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.ExchangeID,"CFFEX");
	strcpy(req.InvestorID,m_szMyInvestorID);
	strcpy(req.UserID,m_szUserID);
	m_pUserApiTrade->ReqQryInvestorMargin(&req,m_iRequestID++);
	
}

void CFeimaTraderSpi::ModifyPwd(char * pwd1, char * pwd2)
{
	CUstpFtdcUserPasswordUpdateField req;
	memset(&req,0,sizeof(CUstpFtdcUserPasswordUpdateField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.UserID,m_szUserID);
	strcpy(req.OldPassword,pwd1);
	strcpy(req.NewPassword,pwd2);

	m_pUserApiTrade->ReqUserPasswordUpdate(&req,m_iRequestID++);
}


void CFeimaTraderSpi::OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !IsErrorRspInfo(pRspInfo) &&bIsLast )
	{

	}
}


void CFeimaTraderSpi::InitOrderReq(void)
{
	memset(&m_reqOrderInsert, 0, sizeof(CUstpFtdcInputOrderField));

	strcpy(m_reqOrderInsert.BrokerID, m_szBroker);

	strcpy(m_reqOrderInsert.InvestorID,m_szMyInvestorID);

	strcpy(m_reqOrderInsert.UserID,m_szUserID);

	strcpy(m_reqOrderInsert.InstrumentID, m_szInstrumentID);

	strcpy(m_reqOrderInsert.ExchangeID,"CFFEX");

	m_iNextOrderRef++;

	sprintf(m_reqOrderInsert.UserOrderLocalID, "%012d", m_iNextOrderRef);
	
	m_reqOrderInsert.OrderPriceType = USTP_FTDC_OPT_LimitPrice;

	m_reqOrderInsert.HedgeFlag = USTP_FTDC_CHF_Speculation;

	m_reqOrderInsert.VolumeCondition = USTP_FTDC_VC_AV;

	m_reqOrderInsert.MinVolume = 1;

	m_reqOrderInsert.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	m_reqOrderInsert.IsAutoSuspend = 0;
}


void CFeimaTraderSpi::InitOrderAction(void)
{
	memset(&m_reqOrderAction, 0, sizeof(CUstpFtdcOrderActionField));

	strcpy(m_reqOrderAction.BrokerID, m_szBroker);

	strcpy(m_reqOrderAction.InvestorID, m_szMyInvestorID);

	strcpy(m_reqOrderAction.UserID,m_szUserID);
	
	m_reqOrderAction.ActionFlag = USTP_FTDC_AF_Delete;

	strcpy(m_reqOrderAction.ExchangeID,"CFFEX");
		
	strcpy(m_reqOrderAction.OrderSysID,"\0");	
}
