#include "stdafx.h"

#include <iostream>

#include "ThostFtdcTraderApi.h"

#include "TraderSpi.h"

#include "BaseTradeApi.h"

#include "TradeInterface.h"

#include "tool.h"
#include "data.h"
#include "FunctionPub.h"

using namespace std;

#pragma warning(disable : 4996)

// USER_API参数
// 配置参数

// 会话参数
//TThostFtdcFrontIDType	FRONT_ID;	//前置编号
//TThostFtdcSessionIDType	SESSION_ID;	//会话编号

std::atomic_int CTraderSpi::m_iNextOrderRef;

std::mutex m_testLog;

bool CTraderSpi:: bHaveQryInstrument = false;

void CTraderSpi::OnFrontConnected()
{
	m_iRequestID = 0;

	m_nOrderRef = -1;

	m_nTradeQry = -1;

	m_nOrderRefOffset = 10000;

	m_pTrade->Log("CTP->OnFrontConnected");

	string account = m_szUserID;

	WRITE_LOG(1,account + "  交易连接建立...");

	m_bLogin = false;

	ReqUserLogin();
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID, m_szUserID);

	int iResult =m_pUserApiTrade->ReqQryTradingAccount(&req, ++m_iRequestID);
}
void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.UserID, m_szUserID);

	strcpy(req.Password, m_szPwd);

	int iResult = m_pUserApiTrade->ReqUserLogin(&req, ++m_iRequestID);

}

int CTraderSpi::ReqUserLogout()
{
	CThostFtdcUserLogoutField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.UserID,m_szUserID);

	int iResult = m_pUserApiTrade->ReqUserLogout(&req,++m_iRequestID);

	return iResult;
}



void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, 
					 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		string msg = pUserLogout->UserID;
		WRITE_LOG(1,msg +"  登出成功...");
	
	}

}


void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//保存会话参数
		FRONT_ID = pRspUserLogin->FrontID;

		SESSION_ID = pRspUserLogin->SessionID;

		//这样才能让ref唯一，因为每次短线重连都会从头开始产生新的ref
		if(CTraderSpi::m_iNextOrderRef < m_nOrderRefOffset)
		{
			CTraderSpi::m_iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef) + (Tool::getDayofWeek()) * m_nOrderRefOffset;
		}

		CTraderSpi::m_iNextOrderRef++;
		///投资者结算结果确认

		CString log;

		log.Format("CTP->%s 登录成功",m_szUserID);

		m_pTrade->Log(log.GetBuffer(0));

		string account = pRspUserLogin->UserID;

		WRITE_LOG(1,account + "  登录成功");

		ReqSettlementInfoConfirm();

		//bHaveQryInstrument = false;

		ReqQryInstrument();   //查询合约

		//ReqQryInvestorPosition();   //查询持仓

		//ReqQryInvestMargin();

		ReqQryTradingAccount();

		m_bLogin = true;

	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID, m_szUserID);

	int iResult = m_pUserApiTrade->ReqSettlementInfoConfirm(&req, ++m_iRequestID);

	
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo)  )
	{
		m_pTrade->Log("CTP->OnRspSettlementInfoConfirm");
	}
}

void CTraderSpi::ReqQryInstrument()
{
	if( bHaveQryInstrument )
		return;

	bHaveQryInstrument = true;

	//clear file first
	CString sFileName = "arbitrage_code.dat";

	sFileName = CFunctionPub::GetModuleDir() + "\\" + sFileName;

	CStdioFile fResult;

	if(!fResult.Open(sFileName,CFile::modeCreate ))
		return;

	fResult.Close();

	CThostFtdcQryInstrumentField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.ExchangeID, "SHFE");

	strcpy(req.InstrumentID, "");

	int iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);

#ifdef _DEBUG
	char szMsg[256];
	memset(szMsg, 0, 256);
	sprintf(szMsg, "************************************ReqQryInstrument[SHFE]: %d\n", iResult);
	TRACE(szMsg);
#endif

	while(0 != iResult)
	{
		Sleep(1000);
		iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);
#ifdef _DEBUG

		memset(szMsg, 0, 256);
		sprintf(szMsg, "************************************ReqQryInstrument[SHFE] again: %d\n", iResult);
		TRACE(szMsg);
#endif
	}
	CPubData::m_mapExchaneId["SHFE"] = 1;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	if (strstr(pInstrument->InstrumentID, "&"))
	{
		
		if (CPubData::m_mapArbitrageCode.find(pInstrument->InstrumentID) == CPubData::m_mapArbitrageCode.end())
		{
			ArbitrageCodeInfo oCodeInfo; 
			strcpy(oCodeInfo.sCode, pInstrument->InstrumentID);
			strcpy(oCodeInfo.sExchid,pInstrument->ExchangeID);
			oCodeInfo.iMinTradeVol = pInstrument->MinMarketOrderVolume;
			oCodeInfo.iMaxTradeVol = pInstrument->MaxMarketOrderVolume;
			CPubData::m_mapArbitrageCode[pInstrument->InstrumentID] = oCodeInfo;
			CPubData::s_vFutuCodeInfo.push_back(oCodeInfo);
		}
		

		
		
	}
	
	if (CPubData::m_sFutuCodeInfo.find(pInstrument->InstrumentID) == CPubData::m_sFutuCodeInfo.end())
	{
		FutuCodeInfo oFutuInfo;
		strcpy(oFutuInfo.code, pInstrument->InstrumentID);
		strcpy(oFutuInfo.exchid, pInstrument->ExchangeID);
		strcpy(oFutuInfo.name, pInstrument->InstrumentName);
		oFutuInfo.iVolumeMultiple = pInstrument->VolumeMultiple;
		oFutuInfo.LMarginRatio = pInstrument->LongMarginRatio;
		oFutuInfo.SMarginRatio = pInstrument->ShortMarginRatio;
		oFutuInfo.PriceTick = pInstrument->PriceTick;

		CPubData::m_sFutuCodeInfo[pInstrument->InstrumentID] = oFutuInfo;

#ifdef _DEBUG
		if(strcmp(pInstrument->InstrumentID,"RM609")==0)
		{
			int i = 0;
			i++;
		}
#endif

	}

	if (bIsLast)
	{
		if (0 == strcmp(pInstrument->ExchangeID, "CZCE"))
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
			//查询保证金
			//ReqQryInvestMargin();
		}
		int iFlag = CPubData::m_mapExchaneId["DCE"];
		if(!iFlag)
		{
			CPubData::m_mapExchaneId["DCE"] = 1;
			CThostFtdcQryInstrumentField req;
			memset(&req, 0, sizeof(req));
			strcpy(req.ExchangeID, "DCE");
			strcpy(req.InstrumentID, "");


			int iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);
#ifdef _DEBUG
			char szMsg[256];
			memset(szMsg, 0, 256);
			sprintf(szMsg, "************************************ReqQryInstrument[DCE]: %d\n", iResult);
			TRACE(szMsg);
#endif
			while (0 != iResult)
			{
				Sleep(1000);
				iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);
#ifdef _DEBUG

				memset(szMsg, 0, 256);
				sprintf(szMsg, "************************************ReqQryInstrument[DCE] again: %d\n", iResult);
				TRACE(szMsg);
#endif
			}
		}
		else
		{
			iFlag = CPubData::m_mapExchaneId["CZCE"];
			if (!iFlag)
			{
				CPubData::m_mapExchaneId["CZCE"] = 1;
				CThostFtdcQryInstrumentField req;
				memset(&req, 0, sizeof(req));
				strcpy(req.ExchangeID, "CZCE");
				strcpy(req.InstrumentID, "");


				int iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);
#ifdef _DEBUG
				char szMsg[256];
				memset(szMsg, 0, 256);
				sprintf(szMsg, "************************************ReqQryInstrument[CZCE]: %d\n", iResult);
				TRACE(szMsg);
#endif
				while(0 != iResult)
				{
					Sleep(1000);
					iResult = m_pUserApiTrade->ReqQryInstrument(&req, ++m_iRequestID);
#ifdef _DEBUG

					memset(szMsg, 0, 256);
					sprintf(szMsg, "************************************ReqQryInstrument[CZCE] again: %d\n", iResult);
					TRACE(szMsg);
#endif
				}

			}
		}
		

	}

#if 0
	if(!bIsLast)
	{

		FutuCodeInfo * fc = new FutuCodeInfo();        //存入结构体
		memset(fc,0,sizeof(FutuCodeInfo));
		strcpy(fc->code,pInstrument->InstrumentID);
		strcpy(fc->exchid,pInstrument->ExchangeID);
		strcpy(fc->name,pInstrument->InstrumentName);
		//CPubData::m_sFutuCodeInfo[fc->code] = *fc;
		delete fc;
	}
	else if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		FutuCodeInfo * fc = new FutuCodeInfo();        //存入结构体
		memset(fc,0,sizeof(FutuCodeInfo));
		strcpy(fc->code,pInstrument->InstrumentID);
		strcpy(fc->exchid,pInstrument->ExchangeID);
		strcpy(fc->name,pInstrument->InstrumentName);
		//CPubData::m_sFutuCodeInfo[fc->code] = *fc;
		delete fc;
		
	}
#endif
}


void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && !IsErrorRspInfo(pRspInfo) && pTradingAccount )
	{
	
	}

}


void CTraderSpi::ReqQryInvestorPosition()
{
	m_listPos.clear();

	m_pTrade->data.m_sHoldsInfo.clear();    //清空之前持仓
	
	CThostFtdcQryInvestorPositionField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID, m_szUserID);

	//strcpy(req.InstrumentID, m_szInstrumentID);
	strcpy(req.InstrumentID, "");

	int iResult = m_pUserApiTrade->ReqQryInvestorPosition(&req, ++m_iRequestID);
}


void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !IsErrorRspInfo(pRspInfo) && pInvestorPosition )
	{
		if (bIsLast )
		{
			if( pInvestorPosition->Position > 0 )
			{
				int index = -1;
				for(unsigned int i=0;i<m_pTrade->data.m_sHoldsInfo.size();i++)
				{
					if (strcmp(m_pTrade->data.m_sHoldsInfo[i].szInstrument,pInvestorPosition->InstrumentID)!=0)
						continue;

					if(pInvestorPosition->PosiDirection=='2' && m_pTrade->data.m_sHoldsInfo[i].PosiDirection==0)
					{
						index = i;
						break;
					}
					else if(pInvestorPosition->PosiDirection=='3' && m_pTrade->data.m_sHoldsInfo[i].PosiDirection==1)
					{
						index = i;
						break;
					}

				}

				if(index==-1)
				{
					CHoldsinfo *hold = new CHoldsinfo();
					strcpy(hold->szInstrument,pInvestorPosition->InstrumentID);

					if(pInvestorPosition->PosiDirection=='2')
						hold->PosiDirection=0;
					else if(pInvestorPosition->PosiDirection=='3')
						hold->PosiDirection=1;

					hold->Position = pInvestorPosition->Position;   //今老仓还未平掉的仓位
					hold->TdPosition = pInvestorPosition->TodayPosition;
					hold->YdPosition = pInvestorPosition->YdPosition;
					hold->LongFrozen = pInvestorPosition->LongFrozen;
					hold->ShortFrozen = pInvestorPosition->ShortFrozen;
					hold->TdAvaiable = 0;
					hold->YdAvaiable = 0;

					if(pInvestorPosition->TodayPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							hold->TdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							hold->TdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}
					else if(pInvestorPosition->YdPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							hold->YdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							hold->YdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}

					m_pTrade->data.m_sHoldsInfo.push_back(*hold);	
					delete hold;
				
				}
				else
				{
					if(pInvestorPosition->TodayPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							m_pTrade->data.m_sHoldsInfo[index].TdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							m_pTrade->data.m_sHoldsInfo[index].TdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					
					}
					else if(pInvestorPosition->YdPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							m_pTrade->data.m_sHoldsInfo[index].YdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							m_pTrade->data.m_sHoldsInfo[index].YdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}

				}


			}

			m_pTrade->data.m_bReady= true;

		}
		else
		{
			if(  pInvestorPosition->Position > 0 )
			{
				int index = -1;
				for(unsigned int i=0;i<m_pTrade->data.m_sHoldsInfo.size();i++)
				{
					if (strcmp(m_pTrade->data.m_sHoldsInfo[i].szInstrument,pInvestorPosition->InstrumentID)!=0)
						continue;

					if(pInvestorPosition->PosiDirection=='2' && m_pTrade->data.m_sHoldsInfo[i].PosiDirection==0)
					{
						index = i;
						break;
					}
					else if(pInvestorPosition->PosiDirection=='3' && m_pTrade->data.m_sHoldsInfo[i].PosiDirection==1)
					{
						index = i;
						break;
					}

				}

				if(index==-1)
				{
					CHoldsinfo *hold = new CHoldsinfo();
					strcpy(hold->szInstrument,pInvestorPosition->InstrumentID);

					if(pInvestorPosition->PosiDirection=='2')
						hold->PosiDirection=0;
					else if(pInvestorPosition->PosiDirection=='3')
						hold->PosiDirection=1;

					hold->Position = pInvestorPosition->Position;   //今老仓还未平掉的仓位
					hold->TdPosition = pInvestorPosition->TodayPosition;
					hold->YdPosition = pInvestorPosition->YdPosition;
					hold->LongFrozen = pInvestorPosition->LongFrozen;
					hold->ShortFrozen = pInvestorPosition->ShortFrozen;
					hold->TdAvaiable = 0;
					hold->YdAvaiable = 0;

					if(pInvestorPosition->TodayPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							hold->TdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							hold->TdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}
					else if(pInvestorPosition->YdPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							hold->YdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							hold->YdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}

					m_pTrade->data.m_sHoldsInfo.push_back(*hold);	
					delete hold;
				
				}
				else
				{
					if(pInvestorPosition->TodayPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							m_pTrade->data.m_sHoldsInfo[index].TdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							m_pTrade->data.m_sHoldsInfo[index].TdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					
					}
					else if(pInvestorPosition->YdPosition>0)
					{
						if(pInvestorPosition->PosiDirection=='2')
						{
							m_pTrade->data.m_sHoldsInfo[index].YdAvaiable = pInvestorPosition->Position - pInvestorPosition->ShortFrozen;
						}
						else if(pInvestorPosition->PosiDirection=='3')
						{
							m_pTrade->data.m_sHoldsInfo[index].YdAvaiable = pInvestorPosition->Position - pInvestorPosition->LongFrozen;
						}
					}

				}

			}

		}
	}
}


int CTraderSpi::ReqOrderInsert(char * szHeyue,TThostFtdcOffsetFlagType flagtype,TThostFtdcDirectionType dir,double price,int count,TThostFtdcTimeConditionType timecondition,char fakfok)
{	
	if( szHeyue[0] == 'I' )
	{
		price = int((price+0.001)/0.2)*0.2;
	}
	else if( szHeyue[0] == 'T' && szHeyue[1] == 'F' )
	{
		price = int((price+0.0001)/0.005)*0.005;
	}

	CThostFtdcInputOrderField req;

	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_szBroker);
	///投资者代码
	strcpy(req.InvestorID, m_szUserID);
	///合约代码
	strcpy(req.InstrumentID, szHeyue);

	//CTraderSpi::m_iNextOrderRef++;

	sprintf(req.OrderRef, "%d", CTraderSpi::m_iNextOrderRef);

	//WRITE_LOG(1,req.OrderRef);

	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	req.Direction = dir;

	req.CombOffsetFlag[0] = flagtype;

	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	req.LimitPrice = price;

#if 0

	//测试第二退超时不成交
	//if ( dir == '0' )
	//	req.LimitPrice = price - 5;
	//else
	//	req.LimitPrice = price + 5;
	

	//测试部分成交。
	int nt = count / 2 ;

	if( nt <= 0 )
	{

	}
	else
	{
		count = nt;
	}

	m_direction = dir;

	m_offsetflag = flagtype;

	if(req.CombOffsetFlag[0] =='1')
	req.CombOffsetFlag[0] = '3';

#endif

	req.VolumeTotalOriginal = count;

	req.TimeCondition = timecondition;	

	if( timecondition == THOST_FTDC_TC_IOC )
	{
		//if( fakfok == FOK_ORDER )
		{
			//req.VolumeCondition = THOST_FTDC_VC_CV;
			//req.MinVolume = 1;	
		}
		//else
		{
			//成交最小数量，其他自动撤销
			req.VolumeCondition = THOST_FTDC_VC_MV;
			req.MinVolume = 1;	
		}
	}
	else
	{
		req.VolumeCondition = THOST_FTDC_VC_AV;
		req.MinVolume = 1;
	}

	req.ContingentCondition = THOST_FTDC_CC_Immediately;

	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	req.IsAutoSuspend = 0;

	req.UserForceClose = 0;

	int iResult = m_pUserApiTrade->ReqOrderInsert(&req, ++m_iRequestID);

	return CTraderSpi::m_iNextOrderRef;
}


void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int a = 0;
	//如果正确应答
	if(! IsErrorRspInfo(pRspInfo))
	{
		/*string refstr = pInputOrder->OrderRef;
		int ref = atoi(refstr.c_str());
		char entrust_status= '3';
		m_pTrade->OnRspOrderInsert(ref,-9999,'3',pRspInfo->ErrorID);*/

	}
	//需要处理下下单失败
	else
	{
		string refstr = pInputOrder->OrderRef;
		int ref = atoi(refstr.c_str());
		m_pTrade->OnRspOrderInsert(ref,-9999,'0',pRspInfo->ErrorID);

	}

//#ifdef _DEBUG		
		char msg[256];
		sprintf(msg,"合约 %s  报单引用 %d  错误代码 %d  错误信息 %s",pInputOrder->InstrumentID,atoi(pInputOrder->OrderRef),pRspInfo->ErrorID,pRspInfo->ErrorMsg);
		WRITE_LOG(1,msg);
//#endif
}

int CTraderSpi::ReqOrderAction(int orderid)
{
	CThostFtdcInputOrderActionField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID,m_szUserID);

	sprintf(req.OrderRef,"%d",orderid);

	req.FrontID = FRONT_ID;

	req.SessionID = SESSION_ID;

	req.ActionFlag = THOST_FTDC_AF_Delete;

	strcpy(req.InstrumentID,m_szInstrumentID);

	int iResult = m_pUserApiTrade->ReqOrderAction(&req, ++m_iRequestID);

	return orderid;
}


int CTraderSpi::ReqOrderAction(int orderid,char *sCode)
{
	char msg[256];

	CThostFtdcInputOrderActionField req;

	memset(&req, 0, sizeof(req));

	strcpy(req.BrokerID, m_szBroker);

	strcpy(req.InvestorID,m_szUserID);

	sprintf(req.OrderRef,"%d",orderid);

	req.FrontID = FRONT_ID;

	req.SessionID = SESSION_ID;

	req.ActionFlag = THOST_FTDC_AF_Delete;

	strcpy(req.InstrumentID, sCode);

	int iResult = m_pUserApiTrade->ReqOrderAction(&req, ++m_iRequestID);

	sprintf(msg,"CTP撤单  合约 %s  报单引用 %d",sCode,orderid);
	WRITE_LOG(1,msg);

#ifdef _DEBUG
	m_testLog.lock();
	//测试部分成交，需要模拟一个撤单成功回报给下单模块，启动一个线程来处理，50ms后发回
	m_orderref = orderid;

	strcpy(m_szInstrumentID,sCode);

	if(strcmp(m_szInstrumentID,"rb1610")==0)
	{
		int u =0;
		u++;
		char xx[128];
		sprintf(xx,"%d11111111",m_orderref);
		//WRITE_LOG(1,xx);
	}


	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadRtnProcess,(void*)this,0,NULL);
	m_testLog.unlock();

#endif
	return orderid;
}


unsigned int CTraderSpi::ThreadRtnProcess(void * pt)
{
	return -1;
	m_testLog.lock();
	CTraderSpi * pSpi = (CTraderSpi *)pt;
	
	Sleep(50);

	//发送一个撤单回报
#ifdef _DEBUG
	if(strcmp(pSpi->m_szInstrumentID,"rb1610")==0)
	{
		int u =0;
		u++;
		char xx[128];
		sprintf(xx,"%d222222222",pSpi->m_orderref);
		//WRITE_LOG(1,xx);
	}
#endif
	pSpi->m_pTrade->RtnOrder(pSpi->m_orderref,1111,'5',pSpi->m_direction,pSpi->m_offsetflag , pSpi->m_szInstrumentID,0,0,0);
	
	m_testLog.unlock();
	return 0;
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	ReqOrderAction(atoi(pOrder->OrderSysID));
}


void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	int id = atoi(pOrder->OrderSysID);
	if( pOrder && IsMyOrder(pOrder) )
	{		
		//OrderSysID如果为空，并且OrderStatus为撤单，则认为报单未成功，需要重新录入
		m_pTrade->RtnOrder(atoi(pOrder->OrderRef),atoi(pOrder->OrderSysID),pOrder->OrderStatus,pOrder->Direction,pOrder->CombOffsetFlag[0], pOrder->InstrumentID,pOrder->VolumeTotalOriginal,pOrder->VolumeTraded,pOrder->VolumeTotal);
		
//#ifdef _DEBUG
		printf("%s %c\n",pOrder->OrderRef,pOrder->OrderStatus);
		char msg[256];
		sprintf(msg,"合约 %s  报单引用 %d  报单编号 %d  报单状态 %c  价格 %.4f   数量 %d  今成交数量 %d  剩余数量 %d ",pOrder->InstrumentID,atoi(pOrder->OrderRef),atoi(pOrder->OrderSysID),pOrder->OrderStatus,pOrder->LimitPrice,pOrder->VolumeTotalOriginal,pOrder->VolumeTraded,pOrder->VolumeTotal);
		WRITE_LOG(1,msg);
//#endif
	}
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	if( pTrade )
	{
		m_pTrade->RtnTrade(pTrade->UserID,atoi(pTrade->OrderRef),atoi(pTrade->OrderSysID),pTrade->Price,pTrade->OffsetFlag,pTrade->Direction,pTrade->TradeTime,pTrade->Volume,pTrade->InstrumentID);

//#ifdef _DEBUG		
		char msg[256];
		sprintf(msg,"合约 %s  报单引用 %d  报单编号 %d  价格 %.4f  成交数量 %d ",pTrade->InstrumentID,atoi(pTrade->OrderRef),atoi(pTrade->OrderSysID),pTrade->Price,pTrade->Volume);
		WRITE_LOG(1,msg);
//#endif
	}
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	m_bLogin = false;

	m_pTrade->Log("CTP->OnFrontDisconnected");

	string account = m_szUserID;

	WRITE_LOG(1,account + "  交易连接断开...");
}
	

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	IsErrorRspInfo(pRspInfo);
}


bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		CString msg;

		msg.Format("error id %d error msg %s",pRspInfo->ErrorID,pRspInfo->ErrorMsg);

		m_pTrade->Log(msg.GetBuffer(msg.GetLength()));

		WRITE_LOG(1,msg.GetBuffer(0));
		
	}
	return bResult;
}


bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) );
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) );

}


void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !pOrder || IsErrorRspInfo(pRspInfo) )
		return ;

	if( m_nOrderRef == -1 || atoi(pOrder->OrderRef) == m_nOrderRef )
	{
		WRITE_LOG(1,"报单查询...");
		OnRtnOrder(pOrder);
	}
	else
	{
		return;
	}
	
}


void CTraderSpi::ReqQryOrder(int orderref)
{
	CThostFtdcQryOrderField req;
	memset(&req,0,sizeof(CThostFtdcQryOrderField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.InvestorID,m_szUserID);
	if( orderref >=0 )
		sprintf(req.OrderSysID,"%d",orderref);

	m_nOrderRef = orderref;

	int nt = m_pUserApiTrade->ReqQryOrder(&req,++m_iRequestID);

}


void CTraderSpi::ReqQryInvestFee(void)
{
	CThostFtdcQryInstrumentCommissionRateField req;
	memset(&req,0,sizeof(CThostFtdcQryInstrumentCommissionRateField));
	strcpy(req.BrokerID,m_szBroker);
	strcpy(req.InvestorID,m_szUserID);
	strcpy(req.InstrumentID,"IF1512");

	m_pUserApiTrade->ReqQryInstrumentCommissionRate(&req,++m_iRequestID);
}


void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if( !IsErrorRspInfo(pRspInfo) && pInstrumentCommissionRate )
	{
		//m_pTrade->InvestorFee(pInstrumentCommissionRate->InstrumentID,pInstrumentCommissionRate->OpenRatioByMoney,pInstrumentCommissionRate->OpenRatioByVolume,pInstrumentCommissionRate->CloseRatioByMoney,pInstrumentCommissionRate->CloseRatioByVolume);
	}
}


void CTraderSpi::ReqQryInvestMargin(void)
{
	CThostFtdcQryInstrumentMarginRateField req;

	memset(&req,0,sizeof(CThostFtdcQryInstrumentMarginRateField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.InvestorID,m_szUserID);

	strcpy(req.InstrumentID,"RM609");
	//strcpy(req.InstrumentID,"");

	req.HedgeFlag = THOST_FTDC_HF_Speculation;

	int iResult = m_pUserApiTrade->ReqQryInstrumentMarginRate(&req,++m_iRequestID);
}


void CTraderSpi::Init(void)
{
	//OrderInsert
	memset(&m_reqOrderIndset, 0, sizeof(m_reqOrderIndset));

	strcpy(m_reqOrderIndset.BrokerID, m_szBroker);

	strcpy(m_reqOrderIndset.InvestorID, m_szUserID);

	///报单引用
	CTraderSpi::m_iNextOrderRef++;

	sprintf(m_reqOrderIndset.OrderRef, "%d", CTraderSpi::m_iNextOrderRef);

	///报单价格条件: 限价
	m_reqOrderIndset.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	
	m_reqOrderIndset.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	
	m_reqOrderIndset.VolumeCondition = THOST_FTDC_VC_AV;
	
	m_reqOrderIndset.MinVolume = 1;
	
	m_reqOrderIndset.ContingentCondition = THOST_FTDC_CC_Immediately;
		
	m_reqOrderIndset.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	
	m_reqOrderIndset.IsAutoSuspend = 0;

	m_reqOrderIndset.UserForceClose = 0;

	//OrderAction
	memset(&m_reqOrderAction, 0, sizeof(m_reqOrderAction));

	strcpy(m_reqOrderAction.BrokerID, m_szBroker);

	strcpy(m_reqOrderAction.InvestorID,m_szUserID);

	m_reqOrderAction.FrontID = FRONT_ID;

	m_reqOrderAction.SessionID = SESSION_ID;

	m_reqOrderAction.ActionFlag = THOST_FTDC_AF_Delete;

	strcpy(m_reqOrderAction.InstrumentID,m_szInstrumentID);
}


void CTraderSpi::ReqQryTrade(int orderref)
{
	m_nTradeQry = orderref;

	CThostFtdcQryTradeField req;

	memset(&req,0,sizeof(CThostFtdcQryTradeField));

	strcpy(req.BrokerID,m_szBroker);

	strcpy(req.InvestorID,m_szUserID);

	strcpy(req.ExchangeID,"CFFEX");

	//strcpy(req.InstrumentID,m_szInstrumentID);

	//填写时间,最近n分钟，如果orderref大于-1，

	if( orderref > -1 )
	{
		
	}

	m_pUserApiTrade->ReqQryTrade(&req,++m_iRequestID);
}


void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	if( !IsErrorRspInfo(pRspInfo) && pOrderAction )
	{

	}
}


void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if( pTrade && !IsErrorRspInfo(pRspInfo)  )
	{
		if( m_nTradeQry == -1 || m_nTradeQry == atoi( pTrade->OrderRef )  )
		{	

		}
	}
}

//合约交易状态通知
void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	if(pInstrumentStatus)
	{
		//char c = pInstrumentStatus->InstrumentStatus;
		string a = pInstrumentStatus->ExchangeID;
		string b = pInstrumentStatus->ExchangeInstID;
		string c = pInstrumentStatus->SettlementGroupID;
		string d = pInstrumentStatus->InstrumentID;
		char e = pInstrumentStatus->InstrumentStatus;
		int f = pInstrumentStatus->TradingSegmentSN;
		string h = pInstrumentStatus->EnterTime;
		char i = pInstrumentStatus->EnterReason;

		CCodeStatus *cs = new CCodeStatus();
		strcpy(cs->szInstrument,pInstrumentStatus->InstrumentID);
		cs->status = pInstrumentStatus->InstrumentStatus;
		strcpy(cs->entertime,pInstrumentStatus->EnterTime);

		string aaa,ccc;
		char bbb[8];
		//sprintf((char*)aaa.c_str(),"%c",e);
		aaa = e;
		sprintf(bbb,"%d",f);
		//sprintf((char*)ccc.c_str(),"%c",i);
		ccc = i;

		string log = a + "  " +b + "  " + c + "  " + d + " status: " + aaa + "  " + bbb + "  "+ h + "  "+ ccc;

		//WRITE_LOG(1,log);

		//独立网格交易调整
		//CStrategyGrid::m_sCodeStatus[d] = *cs;
		m_pTrade->data.m_sCodeStatus[d] = *cs;
		delete cs;

	}

}


///请求查询合约保证金率响应
void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrumentMarginRate)
	{
		if(strcmp(pInstrumentMarginRate->InstrumentID,"RM609")==0)
		{
			int i = 0;
			i++;
		}
	}
}


void CTraderSpi::ReLogin()
{
	if(!m_bLogin)
	{
		string account = m_szUserID;
		WRITE_LOG(1,account+"  重新登录...");
		ReqUserLogin();
	}
	else
	{
		string account = m_szUserID;
		WRITE_LOG(1,account+"  已经登录，不在登录...");
	}

}


int CTraderSpi::LogOut()
{
	if(!m_bLogin)
	{
		string account = m_szUserID;
		WRITE_LOG(1,account+"  登出...");
		return ReqUserLogout();
	}
	else
	{
		string account = m_szUserID;
		WRITE_LOG(1,account+"  已经登录,不需要登出...");
	}

	return -1;
}
