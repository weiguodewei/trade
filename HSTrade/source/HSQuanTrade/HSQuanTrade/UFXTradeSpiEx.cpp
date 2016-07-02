#include "stdafx.h"
#include "UFXTradeSpiEx.h"
#include "BaseTradeApi.h"
#include "StrategyBase.h"
#include "FunctionPub.h"
#include "TraderSpi.h"


bool CUFXTraderSpiEx::IsErrorRspInfo(CHundsunRspInfoField *pRspInfo)
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


int CUFXTraderSpiEx::ReqClientLogin(char* szFundCount, char* szPassword, char cEntrustWay)
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


int CUFXTraderSpiEx::ReqFutuContractbailQry()
{
	CHundsunReqFuncFutuContractbailQryField Reqfunc_futu_contractbail_qry;
	memset(&Reqfunc_futu_contractbail_qry, 0, sizeof(CHundsunReqFuncFutuContractbailQryField));
	strcpy(Reqfunc_futu_contractbail_qry.PositionStr,"");
	Reqfunc_futu_contractbail_qry.RequestNum = 100;
	return lpUserApi->ReqFuncFutuContractbailQry(&Reqfunc_futu_contractbail_qry, ++m_RequestID);
}



int CUFXTraderSpiEx::ReqOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice)
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
	char temp[32];
	sprintf(temp,"%d",m_orderref);
	strcpy_s(ReqFuncOrderinsert.EntrustReference,temp);

	if(cEntrustBS=='0')
	{
		ReqFuncOrderinsert.EntrustBs = '1';
	}
	else if(cEntrustBS=='1')
	{
		ReqFuncOrderinsert.EntrustBs = '2';
	}

	if(cEntrustDirection == '0')
	{
		ReqFuncOrderinsert.FuturesDirection = '1';
	}
	else if(cEntrustDirection == '1')
	{
		ReqFuncOrderinsert.FuturesDirection = '2';
	}
	else if(cEntrustDirection == '3')
	{
		ReqFuncOrderinsert.FuturesDirection = '4';
	}


	lpUserApi->ReqFuncOrderinsert(&ReqFuncOrderinsert, ++m_RequestID);

	return m_orderref;
}


int CUFXTraderSpiEx::ReqArbOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice)
{
	CHundsunReqFuncArgorderinsertField req;

	memset(&req, 0, sizeof(CHundsunReqFuncArgorderinsertField));
	hs_strncpy(req.FutuExchType, szExchType, sizeof(req.FutuExchType));
	hs_strncpy(req.ArbitCode, szFutuCode, sizeof(req.ArbitCode));
	req.EntrustBs = cEntrustBS;
	req.FuturesDirection = cEntrustDirection;
	req.HedgeType = cHedgeType;
	req.EntrustAmount = nEntrustAmount;
	req.FutuEntrustPrice = dEntrustPrice;
	char temp[32];
	sprintf(temp,"%d",m_orderref);
	strcpy_s(req.EntrustReference,temp);

	if(cEntrustBS=='0')
	{
		req.EntrustBs = '1';
	}
	else if(cEntrustBS=='1')
	{
		req.EntrustBs = '2';
	}

	if(cEntrustDirection == '0')
	{
		req.FuturesDirection = '1';
	}
	else if(cEntrustDirection == '1')
	{
		req.FuturesDirection = '2';
	}
	else if(cEntrustDirection == '3')
	{
		req.FuturesDirection = '4';
	}

	//req.WeaveType = '7';
	ArbitrageCodeInfo arbcodeinfo;
	if(CPubData::m_mapArbitrageCode.end() != CPubData::m_mapArbitrageCode.find(szFutuCode))
	{
		arbcodeinfo = CPubData::m_mapArbitrageCode[szFutuCode];
		string arbcode = arbcodeinfo.sCode;

		//郑州
		if(strcmp(arbcodeinfo.sExchid,"F1") ==0)
		{
			hs_strncpy(req.FutuExchType, "F1", sizeof(req.FutuExchType));
			int index = arbcode.find("&");
			string code2 = arbcode.substr(index+1);
			int lencode1 = arbcode.length() - code2.length() - 1;
			string code1 = arbcode.substr(0,lencode1);
			strcpy(req.FutuCode,code1.c_str());
			strcpy(req.SecondCode,code2.c_str());

			int i = 0;
			for(;i<code1.length();i++)
			{
				char c =  code1[i];
				if( c > '1' && c <= '9')
					break;
			}
			string prefix1 = code1.substr(0,i);
			string prefix2 = code2.substr(0,i);
			
			//跨期
			if(prefix1 == prefix2)
				req.WeaveType = '7';
			//跨品种 
			else
				req.WeaveType = '8';

			strcpy(req.EntrustProp,"F7");
			
		}
		//大连
		else if(strcmp(arbcodeinfo.sExchid,"F2") ==0)
		{
			hs_strncpy(req.FutuExchType, "F2", sizeof(req.FutuExchType));
			req.WeaveType = '1';
			strcpy(req.EntrustProp,"F0");

			int index = arbcode.find("&");
			string code2 = arbcode.substr(index+1);
			int lencode1 = arbcode.length() - code2.length() - 1;
			string temp = arbcode.substr(0,lencode1);
			int index2 = temp.find(" ");
			string code1 = temp.substr(index2+1);

			strcpy(req.FutuCode,code1.c_str());
			strcpy(req.SecondCode,code2.c_str());
		}
	}

	lpUserApi->ReqFuncArgorderinsert(&req, ++m_RequestID);

	return m_orderref;


}


int CUFXTraderSpiEx::ReqFuncOrderQry()
{
	CHundsunReqFuncOrderQryField ReqFuncOrderQry;
	memset(&ReqFuncOrderQry, 0, sizeof(CHundsunReqFuncOrderQryField));
	return lpUserApi->ReqFuncOrderQry(&ReqFuncOrderQry, ++m_RequestID);
}


int CUFXTraderSpiEx::ReqFuncWithdraw(int EntrustNo)
{
	CHundsunReqFuncOrderactionField ReqFuncWithDraw;
	memset(&ReqFuncWithDraw, 0, sizeof(CHundsunReqFuncOrderactionField));
	ReqFuncWithDraw.EntrustNo = EntrustNo;
	return lpUserApi->ReqFuncOrderaction(&ReqFuncWithDraw, ++m_RequestID);

}


int CUFXTraderSpiEx::ReqFuncQueryCode()
{

	CHundsunReqFuncFutuContractbailQryField Req;
	memset(&Req, 0, sizeof(CHundsunReqFuncOrderactionField));
	return lpUserApi->ReqFuncFutuContractbailQry(&Req, ++m_RequestID);

}


int CUFXTraderSpiEx::ReqFuncQueryArbCode()
{
	if(CTraderSpi:: bHaveQryInstrument)
		return 0;

	CTraderSpi:: bHaveQryInstrument = true;


	//clear file first
	CString sFileName = "arbitrage_code.dat";

	sFileName = CFunctionPub::GetModuleDir() + "\\" + sFileName;

	CStdioFile fResult;

	if(!fResult.Open(sFileName,CFile::modeCreate ))
		return 1;

	fResult.Close();

	CHundsunReqFuncArginstrumentQryField  Req;
	memset(&Req, 0, sizeof(CHundsunReqFuncArginstrumentQryField));
	strcpy(Req.FutuExchType,"F3");
	strcpy(Req.PositionStr,"");
	Req.RequestNum = 100;
	int iResult =  lpUserApi->ReqFuncArginstrumentQry(&Req, ++m_RequestID);

	while(0 != iResult)
	{
		Sleep(1000);
		iResult = lpUserApi->ReqFuncArginstrumentQry(&Req, ++m_RequestID);

	}
	CPubData::m_mapExchaneId["F3"] = 1;

	return iResult;

}


void CUFXTraderSpiEx::OnFrontConnected()
{
	printf("[UFX期货交易]连接成功...\n");

	ReqClientLogin(m_szUserID,m_szPwd,'7');

	char msg[128];
	sprintf(msg,"账户 %s  UFX期货交易连接成功...",m_szUserID);
	WRITE_LOG(1,msg);
}


void CUFXTraderSpiEx::OnFrontDisconnected(int nReason)
{
	printf("[UFX期货交易]连接断开...\n");

	char msg[128];
	sprintf(msg,"账户 %s  UFX期货交易连接断开...",m_szUserID);
	WRITE_LOG(1,msg);
}


void CUFXTraderSpiEx::OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<"欢迎["<<pfunc_client_login->FundAccount<<"]使用UFX系统"<<endl; 
		
		m_pTrade->Log("UFX期货登陆成功...");

		char msg[128];
		sprintf(msg,"账户 %s  UFX期货登陆成功...",m_szUserID);
		WRITE_LOG(1,msg);

		m_SessionId = pfunc_client_login->SessionNo;

		//查询组合合约代码
		ReqFuncQueryArbCode();
		//ReqFutuContractbailQry();
	}


}


void CUFXTraderSpiEx::OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush)
{
	if(pFuncQhEntrustPush && IsMyOrder(pFuncQhEntrustPush->SessionNo) )
	{
		m_pTrade->Log("委托主推...");
		int orderref = atoi(pFuncQhEntrustPush->EntrustReference);
		int entrustno = pFuncQhEntrustPush->EntrustNo;
		char bs = pFuncQhEntrustPush->EntrustBs;
		char kp = pFuncQhEntrustPush->FuturesDirection;
		char status = pFuncQhEntrustPush->EntrustStatus;
		char *code = pFuncQhEntrustPush->ArbitCode;
		int entrustamount = pFuncQhEntrustPush->EntrustAmount;

		//转换符合ctp格式
		if(bs == '1')
			bs = '0';
		else if(bs == '2')
			bs = '1';

		if(kp == '1')
			kp = '0';
		else if(kp == '2')
			kp = '1';
		else if(kp == '4')
			kp = '3';

		m_pTrade->RtnOrder(orderref,entrustno,status,bs,kp,code,entrustamount,-9999,-9999);
	}

}


void CUFXTraderSpiEx::OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush)
{

	if(pFuncQhRealPush && IsMyOrder(pFuncQhRealPush->SessionNo))
	{		
		m_pTrade->Log("成交主推...");
		int orderref = atoi(pFuncQhRealPush->EntrustReference);
		int entrustno = pFuncQhRealPush->EntrustNo;
		double price = pFuncQhRealPush->FutuBusinessPrice;
		char bs = pFuncQhRealPush->EntrustBs;
		char kp = pFuncQhRealPush->FuturesDirection;
		//char *time = pFuncOptDealPush->BusinessTime;
		int vol = pFuncQhRealPush->BusinessAmount;
		char *code =pFuncQhRealPush->FutuCode;

		//转换符合ctp格式
		if(bs == '1')
			bs = '0';
		else if(bs == '2')
			bs = '1';

		if(kp == '1')
			kp = '0';
		else if(kp == '2')
			kp = '1';
		else if(kp == '4')
			kp = '3';

		m_pTrade->RtnTrade(pFuncQhRealPush->FundAccount,orderref,entrustno,price,kp,bs,"0",vol,code);
	}
}


void CUFXTraderSpiEx::OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo) && pRspfunc_futu_contractbail_qry)
	{

		if (CPubData::m_sFutuCodeInfo.find(pRspfunc_futu_contractbail_qry->FutuCode) == CPubData::m_sFutuCodeInfo.end())
		{
			FutuCodeInfo oFutuInfo;
			strcpy(oFutuInfo.code, pRspfunc_futu_contractbail_qry->FutuCode);
			strcpy(oFutuInfo.exchid, pRspfunc_futu_contractbail_qry->FutuExchType);
			strcpy(oFutuInfo.name, pRspfunc_futu_contractbail_qry->FutuName);
			oFutuInfo.iVolumeMultiple = pRspfunc_futu_contractbail_qry->AmountPerHand;
			//只有一个保证金比例返回
			oFutuInfo.LMarginRatio = pRspfunc_futu_contractbail_qry->OpenRatio;
			oFutuInfo.SMarginRatio = pRspfunc_futu_contractbail_qry->OpenRatio;
			oFutuInfo.PriceTick = pRspfunc_futu_contractbail_qry->FutuPriceStep;

			CPubData::m_sFutuCodeInfo[pRspfunc_futu_contractbail_qry->FutuCode] = oFutuInfo;

		}
	
	}

}


void CUFXTraderSpiEx::OnRspFuncArginstrumentQry(CHundsunRspFuncArginstrumentQryField *pRspFuncArginstrumentQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo) && pRspFuncArginstrumentQry)
	{
		if (strstr(pRspFuncArginstrumentQry->ArbitCode, "&"))
		{
		
			if (CPubData::m_mapArbitrageCode.find(pRspFuncArginstrumentQry->ArbitCode) == CPubData::m_mapArbitrageCode.end())
			{
				ArbitrageCodeInfo oCodeInfo; 
				strcpy(oCodeInfo.sCode, pRspFuncArginstrumentQry->ArbitCode);
				strcpy(oCodeInfo.sExchid,pRspFuncArginstrumentQry->FutuExchType);
				//ufx没有这两个字段
				oCodeInfo.iMinTradeVol = 0;
				oCodeInfo.iMaxTradeVol = 0;
				CPubData::m_mapArbitrageCode[pRspFuncArginstrumentQry->ArbitCode] = oCodeInfo;
				CPubData::s_vFutuCodeInfo.push_back(oCodeInfo);
			}
					
		}

		if (bIsLast)
		{
			//if (0 == strcmp(pRspFuncArginstrumentQry->ArbitCode, "F1"))
			//因为UFX是根据数据是否为空来判断是否最后一条数据
			int last = 0;
			if(!CPubData::s_vFutuCodeInfo.empty())
				last = CPubData::s_vFutuCodeInfo.size();
			if( (last!=0) && (strcmp(CPubData::s_vFutuCodeInfo[last-1].sExchid,"F1")==0) )
			{
				CStdioFile fResult;
				CString sFileName = "arbitrage_code.dat";
				sFileName = CFunctionPub::GetModuleDir() + "\\" + sFileName;

				if(!fResult.Open(sFileName,CFile::modeNoTruncate|CFile::modeWrite ))
					return;

				fResult.SeekToEnd();


				std::vector<ArbitrageCodeInfo>::const_iterator itCode = CPubData::s_vFutuCodeInfo.cbegin();

				//map<std::string, ArbitrageCodeInfo>::const_iterator itCode = CPubData::m_mapArbitrageCode.cbegin();
				while (itCode != CPubData::s_vFutuCodeInfo.cend())
				{
					CString strLine;

				
					strLine.Format("%s,%s,%d,%d", itCode->sCode, itCode->sExchid, itCode->iMinTradeVol, itCode->iMaxTradeVol);
					fResult.WriteString(strLine);
					fResult.WriteString("\n");
					itCode++;
				}
				fResult.Close();

				if (m_pTrade)
				{
					::PostMessage(m_pTrade->m_hMsgWnd, WM_MESSAGE_TRADE, TRADE_MSG_TYPE_CODE_QUERY_DONE, 0);
				}

				//查询合约信息  UFX接口组合合约和单腿合约是分开保存的
				ReqFutuContractbailQry();
			
			}

			int iFlag = CPubData::m_mapExchaneId["F2"];
			if(!iFlag)
			{
				CPubData::m_mapExchaneId["F2"] = 1;
				CHundsunReqFuncArginstrumentQryField req;
				memset(&req, 0, sizeof(req));		
				strcpy(req.FutuExchType, "F2");
				strcpy(req.PositionStr,"");
				req.RequestNum = 100;
			
				int iResult = lpUserApi->ReqFuncArginstrumentQry(&req, ++m_RequestID);

				while (0 != iResult)
				{
					Sleep(1000);
					iResult = lpUserApi->ReqFuncArginstrumentQry(&req, ++m_RequestID);

				}
			}
			else
			{
				iFlag = CPubData::m_mapExchaneId["F1"];
				if (!iFlag)
				{
					CPubData::m_mapExchaneId["F1"] = 1;
					CHundsunReqFuncArginstrumentQryField req;
					memset(&req, 0, sizeof(req));
					strcpy(req.FutuExchType, "F1");	
					strcpy(req.PositionStr,"");
					req.RequestNum = 100;

					int iResult = lpUserApi->ReqFuncArginstrumentQry(&req, ++m_RequestID);

					while(0 != iResult)
					{
						Sleep(1000);
						iResult = lpUserApi->ReqFuncArginstrumentQry(&req, ++m_RequestID);

					}

				}
			}
		

		}

	
	}

}
	


void CUFXTraderSpiEx::OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspFuncOrderinsert && pRspInfo)
	{
		if(!IsErrorRspInfo(pRspInfo))
		{		
			m_pTrade->Log("下单成功...");
		
		}

		int orderref = atoi(pRspFuncOrderinsert->EntrustReference);

		int entrustno = pRspFuncOrderinsert->EntrustNo;

		int requestid = nRequestID;

		//如果没初始化，下单委托号为0，报错处理，利用nRequestID
		if(entrustno==0)
			m_pTrade->OnRspOrderInsert(nRequestID,-9999,'0',pRspInfo->ErrorNo);
		else
			m_pTrade->OnRspOrderInsert(orderref,entrustno,'0',pRspInfo->ErrorNo);

	}

}


void CUFXTraderSpiEx::OnRspFuncArgorderinsert(CHundsunRspFuncArgorderinsertField *pRspFuncArgorderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspFuncArgorderinsert && pRspInfo)
	{
		if(!IsErrorRspInfo(pRspInfo))
		{		
			m_pTrade->Log("下单成功...");
		
		}

		int orderref = atoi(pRspFuncArgorderinsert->EntrustReference);

		int entrustno = pRspFuncArgorderinsert->EntrustNo;

		int requestid = nRequestID;

		//如果没初始化，下单委托号为0，报错处理，利用nRequestID
		if(entrustno==0)
			m_pTrade->OnRspOrderInsert(nRequestID,-9999,'0',pRspInfo->ErrorNo);
		else
			m_pTrade->OnRspOrderInsert(orderref,entrustno,'0',pRspInfo->ErrorNo);

	}


}



void CUFXTraderSpiEx::OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{



}
	

bool CUFXTraderSpiEx::IsMyOrder(int sessionid)
{
	if(sessionid == m_SessionId)
		return true;
	else
		return false;

}