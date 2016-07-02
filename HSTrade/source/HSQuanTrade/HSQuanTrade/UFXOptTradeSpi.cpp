#include "stdafx.h"
#include "UFXOptTradeSpi.h"
#include "BaseTradeApi.h"
#include "StrategyBase.h"

bool CUFXOptTraderSpi::IsErrorRspInfo(CHundsunRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorNo != 0));
	if (ret)
	{
		printf("IsErrorRspInfo\n");
		cerr<<" 响应 | "<<pRspInfo->ErrorInfo<<endl;
		

		CString msg;

		msg.Format("error id %d error msg %s",pRspInfo->ErrorNo,pRspInfo->ErrorInfo);

		m_pTrade->Log(msg.GetBuffer(msg.GetLength()));

		WRITE_LOG(1,msg.GetBuffer(0));
	}
	return ret;
}

int CUFXOptTraderSpi::ReqClientLogin(char* szFundCount, char* szPassword, char cEntrustWay)
{
	CHundsunReqFuncClientLoginField req;
	memset(&req, 0, sizeof(CHundsunReqFuncClientLoginField));
	req.OpEntrustWay = cEntrustWay;
	req.InputContent = '1';
	hs_strncpy(req.AccountContent, szFundCount, sizeof(req.AccountContent));
	req.PasswordType = '2';
	hs_strncpy(req.Password, szPassword, sizeof(req.Password));
	return lpUserApi->ReqFuncClientLogin(&req, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFutuContractbailQry()
{
	CHundsunReqFuncFutuContractbailQryField Reqfunc_futu_contractbail_qry;
	memset(&Reqfunc_futu_contractbail_qry, 0, sizeof(CHundsunReqFuncFutuContractbailQryField));
	return lpUserApi->ReqFuncFutuContractbailQry(&Reqfunc_futu_contractbail_qry, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice)
{
	CHundsunReqFuncOrderinsertField ReqFuncOrderinsert;
	memset(&ReqFuncOrderinsert, 0, sizeof(CHundsunReqFuncOrderinsertField));
	hs_strncpy(ReqFuncOrderinsert.FutuExchType, szExchType, sizeof(ReqFuncOrderinsert.FutuExchType));
	hs_strncpy(ReqFuncOrderinsert.FutuCode, szFutuCode, sizeof(ReqFuncOrderinsert.FutuCode));
	ReqFuncOrderinsert.EntrustBs = cEntrustBS;
	ReqFuncOrderinsert.FuturesDirection = cEntrustDirection;
	ReqFuncOrderinsert.HedgeType = cHedgeType;
	ReqFuncOrderinsert.EntrustAmount = nEntrustAmount;
	ReqFuncOrderinsert.FutuEntrustPrice = dEntrustPrice;
	return lpUserApi->ReqFuncOrderinsert(&ReqFuncOrderinsert, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOrderQry()
{
	CHundsunReqFuncOrderQryField ReqFuncOrderQry;
	memset(&ReqFuncOrderQry, 0, sizeof(CHundsunReqFuncOrderQryField));
	return lpUserApi->ReqFuncOrderQry(&ReqFuncOrderQry, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOptOptcodeQry()
{
	CHundsunReqFuncOptOptcodeQryField ReqFuncOptOptcode_qry;
	memset(&ReqFuncOptOptcode_qry, 0, sizeof(CHundsunReqFuncOptOptcodeQryField));
	return lpUserApi->ReqFuncOptOptcodeQry(&ReqFuncOptOptcode_qry, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOptEnTrade(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, double dEntrustPrice)
{
	CHundsunReqFuncOptEnTradeField ReqFuncOptEnTrade;
	memset(&ReqFuncOptEnTrade, 0, sizeof(CHundsunReqFuncOptEnTradeField));

	ReqFuncOptEnTrade.AssetProp = 'B';
	hs_strncpy(ReqFuncOptEnTrade.ExchangeType, szExchType, sizeof(ReqFuncOptEnTrade.ExchangeType));
	hs_strncpy(ReqFuncOptEnTrade.OptionCode, szOptionCode, sizeof(ReqFuncOptEnTrade.OptionCode));
	hs_strncpy(ReqFuncOptEnTrade.EntrustProp, "0", sizeof(ReqFuncOptEnTrade.EntrustProp));
	ReqFuncOptEnTrade.OptEntrustPrice = dEntrustPrice;
	ReqFuncOptEnTrade.EntrustBs = cEntrustBS;
	ReqFuncOptEnTrade.EntrustOc = cEntrustOC;

	return lpUserApi->ReqFuncOptEnTrade(&ReqFuncOptEnTrade, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOptEntrust(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, 
								  int nEntrustAmount, double dEntrustPrice)
								  //"2", "20003928", '1', 'O', 1, 16
{
	CHundsunReqFuncOptEntrustField ReqFuncOptEntrust;
	memset(&ReqFuncOptEntrust, 0, sizeof(CHundsunReqFuncOptEntrustField));

	ReqFuncOptEntrust.AssetProp = 'B';
	hs_strncpy(ReqFuncOptEntrust.ExchangeType, szExchType, sizeof(ReqFuncOptEntrust.ExchangeType));
	hs_strncpy(ReqFuncOptEntrust.OptionCode, szOptionCode, sizeof(ReqFuncOptEntrust.OptionCode));
	hs_strncpy(ReqFuncOptEntrust.EntrustProp, "0", sizeof(ReqFuncOptEntrust.EntrustProp));
	ReqFuncOptEntrust.EntrustAmount = nEntrustAmount;
	ReqFuncOptEntrust.OptEntrustPrice = dEntrustPrice;

	if(cEntrustBS == '0')
	{
		ReqFuncOptEntrust.EntrustBs = '1';
	}
	else if(cEntrustBS == '1')
	{
		ReqFuncOptEntrust.EntrustBs = '2';
	}

	if(cEntrustOC == '0')
	{
		ReqFuncOptEntrust.EntrustOc = 'O';
	}
	else if(cEntrustOC == '1')
	{
		ReqFuncOptEntrust.EntrustOc = 'C';
	}

	//ReqFuncOptEntrust.EntrustBs = cEntrustBS;
	//ReqFuncOptEntrust.EntrustOc = cEntrustOC;

	ReqFuncOptEntrust.BatchNo = m_orderref;

	//下单
	lpUserApi->ReqFuncOptEntrust(&ReqFuncOptEntrust, m_orderref);

	return m_orderref;
}

int CUFXOptTraderSpi::ReqFuncOptWithdraw(int EntrustNo)
{
	CHundsunReqFuncOptWithdrawField ReqFuncOptWithdraw;
	memset(&ReqFuncOptWithdraw, 0, sizeof(CHundsunReqFuncOptWithdrawField));

	ReqFuncOptWithdraw.EntrustNo = EntrustNo;
	//hs_strncpy(ReqFuncOptWithdraw.ExchangeType, "2", sizeof(ReqFuncOptWithdraw.ExchangeType));
	return lpUserApi->ReqFuncOptWithdraw(&ReqFuncOptWithdraw, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOptEntrustQry()
{
	CHundsunReqFuncOptEntrustQryField ReqFuncOptEntrustQry;
	memset(&ReqFuncOptEntrustQry, 0, sizeof(CHundsunReqFuncOptEntrustQryField));

	return lpUserApi->ReqFuncOptEntrustQry(&ReqFuncOptEntrustQry, ++m_RequestID);
}

int CUFXOptTraderSpi::ReqFuncOptRealtimeQry()
{
	CHundsunReqFuncOptRealtimeQryField ReqFuncOptRealtimeQry;
	memset(&ReqFuncOptRealtimeQry, 0, sizeof(CHundsunReqFuncOptRealtimeQryField));

	return lpUserApi->ReqFuncOptRealtimeQry(&ReqFuncOptRealtimeQry, ++m_RequestID);
}


int CUFXOptTraderSpi::ReqFuncOptEntrustSubscribe()
{
	return lpUserApi->ReqFuncOptEntrustSubscribe();
}
int CUFXOptTraderSpi::ReqFuncOptDealSubscribe()
{
	return lpUserApi->ReqFuncOptDealSubscribe();
}

void CUFXOptTraderSpi::OnFrontConnected()
{
	printf("[期权交易]连接成功...\n");

	ReqClientLogin(m_szUserID,m_szPwd,'7');
}

void CUFXOptTraderSpi::OnFrontDisconnected(int nReason)
{
	printf("[期权交易]连接断开...\n");
}

void CUFXOptTraderSpi::OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<"欢迎["<<pfunc_client_login->FundAccount<<"]使用UFX系统"<<endl; 
		
		m_pTrade->Log("期权登陆成功...");
	}

	//订阅
	ReqFuncOptEntrustSubscribe();

	ReqFuncOptDealSubscribe();
}

void CUFXOptTraderSpi::OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(bIsLast)
		{
			cout<<"查询结束"<<endl; 
		}
		else
		{
			cout<<pRspfunc_futu_contractbail_qry->FutuCode<<"-"<<pRspfunc_futu_contractbail_qry->FutuName<<endl; 
		}
	}	
}

void CUFXOptTraderSpi::OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<"[委托应答]...委托号="<<pRspFuncOrderinsert->EntrustNo<<endl;

		//m_pTrade->Log("下单成功...");
		
	}
}

void CUFXOptTraderSpi::OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(bIsLast)
		{
			cout<<"查询结束"<<endl; 
		}
		else
		{
			cout<<pRspFuncOrderQry->EntrustNo<<"-"<<pRspFuncOrderQry->FutuCode<<":委托状态["<<pRspFuncOrderQry->EntrustStatus<<"]"<<endl; 
		}
	}	
}

void CUFXOptTraderSpi::OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush)
{
	cout<<"[委托主推]...委托号="<<pFuncQhEntrustPush->EntrustNo<<" 委托状态="<<pFuncQhEntrustPush->EntrustStatus<<endl; 
}

void CUFXOptTraderSpi::OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush)
{
	cout<<"[成交主推]...成交编号="<<pFuncQhRealPush->BusinessNo<<" 成交数量="<<pFuncQhRealPush->BusinessAmount<<" 委托状态="<<pFuncQhRealPush->EntrustStatus<<endl; 
}

void CUFXOptTraderSpi::OnRtnFuncOptDealPush(CHundsunRtnFuncOptDealPushField *pFuncOptDealPush)
{
	if(pFuncOptDealPush)
	{
		cout<<"[成交主推]...成交编号="<<pFuncOptDealPush->BusinessId<<" 成交数量="<<pFuncOptDealPush->BusinessAmount<<" 委托状态="<<pFuncOptDealPush->EntrustStatus<<endl; 
	
		m_pTrade->Log("成交主推...");
		int orderref = pFuncOptDealPush->BatchNo;
		int entrustno = pFuncOptDealPush->EntrustNo;
		double price = pFuncOptDealPush->OptBusinessPrice;
		char bs = pFuncOptDealPush->EntrustBs;
		char kp = pFuncOptDealPush->EntrustOc;
		//char *time = pFuncOptDealPush->BusinessTime;
		int vol = pFuncOptDealPush->BusinessAmount;
		char *code =pFuncOptDealPush->OptionCode;

		//转换符合ctp格式
		if(bs == '1')
			bs = '0';
		else if(bs == '2')
			bs = '1';

		if(kp == 'O')
			kp = '0';
		else if(kp == 'C')
			kp = '1';

		//BatchNo 作为报单引用，通过BatchNo对应找到entrustno   201604 
		//m_pTrade->RtnTrade(orderref,entrustno,price,kp,bs,"0",vol,code);

		//由于投资赢家回报里面智能通过entrust_no对应本地委托和回报 所以在委托应答里面把orderref赋值成entrust_no
		//这里为了兼容，需要把orderref改为entrustno            201605
		//m_pTrade->RtnTrade(pFuncOptDealPush->FundAccount,entrustno,entrustno,price,kp,bs,"0",vol,code);

		//20160628
		m_pTrade->RtnTrade(pFuncOptDealPush->FundAccount,orderref,entrustno,price,kp,bs,"0",vol,code);
	}
}

void CUFXOptTraderSpi::OnRtnFuncOptEntrustPush(CHundsunRtnFuncOptEntrustPushField *pFuncOptEntrustPush)
{
	if(pFuncOptEntrustPush)
	{

		cout<<"[委托主推]...委托编号="<<pFuncOptEntrustPush->EntrustNo<<" 委托数量="<<pFuncOptEntrustPush->EntrustAmount<<" 委托状态="<<pFuncOptEntrustPush->EntrustStatus<<endl;

		m_pTrade->Log("委托主推...");
		int orderref = pFuncOptEntrustPush->BatchNo;
		int entrustno = pFuncOptEntrustPush->EntrustNo;
		char bs = pFuncOptEntrustPush->EntrustBs;
		char kp = pFuncOptEntrustPush->EntrustOc;
		char status = pFuncOptEntrustPush->EntrustStatus;
		char *code = pFuncOptEntrustPush->OptionCode;
		int entrustamount = pFuncOptEntrustPush->EntrustAmount;

		//转换符合ctp格式
		if(bs == '1')
			bs = '0';
		else if(bs == '2')
			bs = '1';

		if(kp == 'O')
			kp = '0';
		else if(kp == 'C')
			kp = '1';


		//BatchNo 作为报单引用，通过BatchNo对应找到entrustno   201604 
		//m_pTrade->RtnOrder(orderref,entrustno,status,bs,kp,code);

		//由于投资赢家回报里面智能通过entrust_no对应本地委托和回报 所以在委托应答里面把orderref赋值成entrust_no
		//这里为了兼容，需要把orderref改为entrustno            201605
		//m_pTrade->RtnOrder(entrustno,entrustno,status,bs,kp,code,entrustamount,-9999,-9999);

		//20160628
		m_pTrade->RtnOrder(orderref,entrustno,status,bs,kp,code,entrustamount,-9999,-9999);
	}

}

void CUFXOptTraderSpi::OnRspFuncOptOptcodeQry(CHundsunRspFuncOptOptcodeQryField *pRspFuncOptOptcodeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(bIsLast)
		{
			cout<<"查询结束"<<endl; 
		}
		else
		{
			cout<<"OptcontractId="<<pRspFuncOptOptcodeQry->OptcontractId<<"|stock_code="<<pRspFuncOptOptcodeQry->StockCode<<"|OptionCode="<<pRspFuncOptOptcodeQry->OptionCode<<endl; 
		}
	}
}

void CUFXOptTraderSpi::OnRspFuncOptEnTrade(CHundsunRspFuncOptEnTradeField *pRspFuncOptEnTrade, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<"EnableAmount="<<pRspFuncOptEnTrade->EnableAmount<<endl; 
	}
}

void CUFXOptTraderSpi::OnRspFuncOptEntrust(CHundsunRspFuncOptEntrustField *pRspFuncOptEntrust, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	if(pRspFuncOptEntrust && pRspInfo)
	{
		if(!IsErrorRspInfo(pRspInfo))
		{

			cout<<"EntrustNo="<<pRspFuncOptEntrust->EntrustNo<<endl; 

			m_pTrade->Log("下单成功...");
		}

		int orderref = pRspFuncOptEntrust->BatchNo;

		int entrustno = pRspFuncOptEntrust->EntrustNo;

		int requestid = nRequestID;

		//如果没初始化，下单委托号为0，报错处理，利用nRequestID
		if(entrustno==0)
			m_pTrade->OnRspOrderInsert(nRequestID,-9999,'0',pRspInfo->ErrorNo);
		else
			m_pTrade->OnRspOrderInsert(orderref,entrustno,'0',pRspInfo->ErrorNo);
	}
}

void CUFXOptTraderSpi::OnRspFuncOptWithdraw(CHundsunRspFuncOptWithdrawField *pRspFuncOptWithdraw, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<"EntrustNo="<<pRspFuncOptWithdraw->EntrustStatusOld<<endl; 
	}
}

void CUFXOptTraderSpi::OnRspFuncOptEntrustQry(CHundsunRspFuncOptEntrustQryField *pRspFuncOptEntrustQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(bIsLast)
		{
			cout<<"查询结束"<<endl; 
		}
		else
		{
			cout<<"EntrustNo="<<pRspFuncOptEntrustQry->EntrustNo<<"|OptcontractId="<<pRspFuncOptEntrustQry->OptcontractId<<"|EntrustStatus="<<pRspFuncOptEntrustQry->EntrustStatus<<endl; 
		}
	}
}

void CUFXOptTraderSpi::OnRspFuncOptRealtimeQry(CHundsunRspFuncOptRealtimeQryField *pRspFuncOptRealtimeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(bIsLast)
		{
			cout<<"查询结束"<<endl; 
		}
		else
		{
			cout<<"BusinessId="<<pRspFuncOptRealtimeQry->BusinessId<<"|OptcontractId="<<pRspFuncOptRealtimeQry->OptcontractId<<"|BusinessAmount="<<pRspFuncOptRealtimeQry->BusinessAmount<<endl; 
		}
	}
}


void CUFXOptTraderSpi::OnRspFuncOptDealSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		m_pTrade->Log("成交回报订阅成功...");
	}
}

void CUFXOptTraderSpi::OnRspFuncOptEntrustSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		m_pTrade->Log("委托回报订阅成功...");
	}
}