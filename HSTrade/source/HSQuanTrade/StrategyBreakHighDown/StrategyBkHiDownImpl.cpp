#include "stdafx.h"
#include "StrategyBkHiDownImpl.h"
#include <string>
#include "..\HSQuanTrade\FunctionPub.h"

#define STOCK_TOTAL_CAPITAL 2000000000

CStrategyBkHiDownImpl::CStrategyBkHiDownImpl(void)
{
	m_bRun = false;

	m_strStrategyName = "创新高回调买入策略";

	m_dbDownRate = 0.05;
	m_dbLoseRate = 0.08;
	m_fyingkuiPosition = 0;
	//m_dbAvgPrice = 0.01;

	//m_nCount = 100;
	m_iRefHighDay = 30;
	m_iFallDays = 3;
	m_dbProfitRate = 0.20;
	m_dbShiftLost = 0.08;
	m_dbFallRate = 0.15;
	m_bStrictlyFall = false;
	m_iWaitBreakDownDays = 5;

	m_strParamList = "初始权益;止损率;止盈率;移动止损率;换手率;回调率;账户;同时开仓数;手续费率;退出信号为指令价或收盘价;是否下单;创N天新高;N天回调;回落幅度;单调回落;是否参考20日均线";


	m_params.insert(pair<string, CString>("初始权益",""));
	m_params.insert(pair<string, CString>("止损率",""));
	m_params.insert(pair<string, CString>("止盈率",""));
	m_params.insert(pair<string, CString>("移动止损率",""));
	m_params.insert(pair<string, CString>("换手率",""));
	m_params.insert(pair<string, CString>("回调率",""));
	m_params.insert(pair<string, CString>("账户",""));
	m_params.insert(pair<string, CString>("同时开仓数",""));
	m_params.insert(pair<string, CString>("手续费率",""));
	m_params.insert(pair<string, CString>("退出信号为指令价或收盘价", ""));
	m_params.insert(pair<string, CString>("是否下单", ""));//
	
	

	m_params.insert(pair<string, CString>("创N天新高","30"));
	m_params.insert(pair<string, CString>("N天回调","3"));
	m_params.insert(pair<string, CString>("回落幅度", ""));//
	m_params.insert(pair<string, CString>("单调回落", ""));
	m_params.insert(pair<string, CString>("是否参考20日均线", "1"));

	m_params.insert(pair<string, CString>("价差","0"));
	

	m_pPolicy = CFundManager::GetPolicyInstance(FUND_POLICY_STOCK_AVG);


}


CStrategyBkHiDownImpl::~CStrategyBkHiDownImpl(void)
{
}


void CStrategyBkHiDownImpl::OnSubcode()
{
	vector<string> arCodes;
	char sMsg[256];

	//step1: 检查开仓股票
	m_arCodeSubs.clear();

	std::map<string, int>::const_iterator itSignal = m_mapSignalStatus.cbegin();
	while (itSignal != m_mapSignalStatus.cend())
	{
		if (TRADE_STATUS_BK_TRADE == itSignal->second || TRADE_STATUS_SK_TRADE == itSignal->second)
		{

			char sSubcode[256];

			sprintf(sSubcode, "subcode#%s;STOCK;UFX;#",  itSignal->first.c_str());
			sprintf(sMsg, "已订阅 %s 行情",  itSignal->first.c_str());
			::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);

			m_arCodeSubs[itSignal->first] = itSignal->second;
		}
		itSignal++;
	}

	CTime tmLastTradeDate;
	if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
	{
		WRITE_LOG(1, "上证指数历史日线数据不存在", TRADE_MSG_TYPE_LOG_PRV);
		return;
	}
	CString strTime = tmLastTradeDate.Format("%Y%m%d");
	if (m_pKManager->GetCodeList(arCodes) > 0)
	{
		vector<string>::const_iterator itCode = arCodes.cbegin();

		while (itCode != arCodes.cend())
		{

			if (m_arCodeSubs.end() != m_arCodeSubs.find(*itCode))
			{
				itCode++;
				continue;
			}
			
#ifdef _DEBUG
			if (0 == itCode->compare("603988"))
			{
				WRITE_LOG(1, "debug");
			}
#endif	

			double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)itCode->c_str());
			if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //流通股本大于20亿股或者流通市值大于1000亿的票排除。
			{
				itCode++;
				continue;
			}
			CKStrdata oHighKData;
			CKStrdata oLowKDay;
			if (m_iRefHighDay > 0)
			{
				
				//新高条件
				bool bSeekRefKDay = true;

				double dbRefHighPrice = 0;
				for (int i = 1; i <= m_iRefHighDay; i++)
				{
					CKStrdata oRefNDay;

					if (!GetPreNDayKData(itCode->c_str(), strTime, oRefNDay, - i))
					{
						sprintf(sMsg, "获取前%d日K线数据失败:%s", i, itCode->c_str());        
						WRITE_LOG(1, sMsg);
						bSeekRefKDay = false;
						break;
					}
					if (oRefNDay.priceH > dbRefHighPrice)
					{
						dbRefHighPrice = oRefNDay.priceH;
						oHighKData = oRefNDay;
					}
				}
				if (!bSeekRefKDay || dbRefHighPrice > oHighKData.priceH)
				{
					itCode++;
					continue;
				}

				int iDownDays = m_pKManager->CalDaysBetween2DayK(itCode->c_str(), oHighKData.curDate, strTime);
				if (iDownDays > m_iFallDays)
				{
#ifdef _DEBUG
					//sprintf(sMsg, "股票%s创新高回调天数:%d, 预设%d天", itCode->c_str(), iDownDays, m_iDownDays);       
					//AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
#endif
					itCode++;
					continue;
				}
				bool bStrictlyFall = true;
				//寻找低价
				oLowKDay = oHighKData;
				CKStrdata oPrevData = oHighKData;
				for (int i = 1; i <= iDownDays; i++)
				{
					CKStrdata oCurKData;
					if (!GetPreNDayKData(itCode->c_str(), oHighKData.curDate, oCurKData, i))
					{
						sprintf(sMsg, "获取新高后%d日K线数据失败:%s", i, itCode->c_str());        
						WRITE_LOG(1, sMsg);
					}
					if (oLowKDay.priceL > oCurKData.priceL)
					{
						oLowKDay = oCurKData;
					}
					if (m_bStrictlyFall)//单调回落
					{
						if (oPrevData.priceH < oCurKData.priceH)
						{
							bStrictlyFall = false;
							break;
						}
					}
					oPrevData = oCurKData;
				}

				if (m_bStrictlyFall)
				{
					if (!bStrictlyFall)
					{
						itCode++;
						continue;
					}
				}
			}
			else
			{
				CKStrdata oPrevKData, oCurKData;
				
				if (!GetPreNDayKData(itCode->c_str(), strTime, oLowKDay, 0))
				{
					sprintf(sMsg, "获取昨日K线数据失败:%s", itCode->c_str());        
					WRITE_LOG(1, sMsg);
					
					itCode++;
					continue;
				}
				bool bStrictlyFall = true;
				for (int i = 1; i <= m_iFallDays; i++)
				{
					if (!GetPreNDayKData(itCode->c_str(), strTime, oPrevKData, -i))
					{
						sprintf(sMsg, "获取前%d日K线数据失败:%s", i, itCode->c_str());
						WRITE_LOG(1, sMsg);
						return;
					}
					if (m_bStrictlyFall)
					{
						if (oCurKData.priceH > oPrevKData.priceH)
						{
							bStrictlyFall = false;
						}
					}
					if (oPrevKData.priceH * (1 - m_dbFallRate) >= oLowKDay.priceL)
					{
						if (m_bStrictlyFall)
						{
							if (bStrictlyFall)
							{
								oHighKData = oPrevKData; 
							}
						}
						else
						{
							oHighKData = oPrevKData; 
						}
					}

					oCurKData = oPrevKData;
					
				}
			}

			if (oHighKData.priceH * (1 - m_dbFallRate) >= oLowKDay.priceL)
			{
				//订阅行情
				char sSubcode[256];

				sprintf(sSubcode, "subcode#%s;STOCK;UFX;#", itCode->c_str());
				sprintf(sMsg, "已订阅 %s 行情, 新高日:%s, 新低日:%s", itCode->c_str(), oHighKData.curDate, oLowKDay.curDate);
				::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
#ifdef _DEBUG
				AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
#else
				WRITE_LOG(1, sMsg);
#endif
				m_arCodeSubs[*itCode] = 0;
				
			}
			itCode++;
			
		}

		sprintf(sMsg, "本次订阅共遍历股票%d个, 其中成功订阅股票%d个", arCodes.size(), m_arCodeSubs.size());
		AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
	}


}


void CStrategyBkHiDownImpl::Tick(CTickData * pt)
{
	if (m_arCodeSubs.end() == m_arCodeSubs.find(pt->szInstrument))
	{
		return;
	}

	if (!IsRightTickTime(pt->szTime))
	{
		return;
	}



	int iSignalStatus = GetSignalStatus(pt->szInstrument);

	//记录最高价
	double dbHighPrice = GetHighPrice(pt->szInstrument);
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		dbHighPrice = 0;
		SetHighPrice(pt->szInstrument, dbHighPrice);
	}
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus && dbHighPrice < pt->highestprice)
	{
		dbHighPrice = pt->highestprice;
		SetHighPrice(pt->szInstrument, dbHighPrice);
	}

	char sMsg[256];
	CTime tmLastTradeDate;
	if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
	{
		AddLog("上证指数历史日线数据不存在", TRADE_MSG_TYPE_LOG_PRV);
		return;
	}
	CString strLastTradeDate = tmLastTradeDate.Format("%Y%m%d");


	CKStrdata oPreKData1;
	if (!GetPreNDayKData(pt->szInstrument, strLastTradeDate, oPreKData1, 0))
	{
		WRITE_LOG(1, "获取前一日K线数据失败");
		return;
	}
	
	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument);
	if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //流通股本大于20亿股或者流通市值大于1000亿的票排除。
	{
		return;
	}

	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		
		if (m_bMa20Ref)
		{
			CKStrdata oIndexData;
			bool bSuc = m_pKManager->getDayKData("999999", strLastTradeDate, oIndexData);
			double dbMa20 = 0;
			if (!m_pKManager->CalcMa20("999999", strLastTradeDate, dbMa20))
			{
				return;
			}
			if (!bSuc || oIndexData.priceS <= dbMa20)
			{
				return;
			}
		}
		
		E_Bk_Hi_DownStep eSigStatus = GetSigStatus(pt->szInstrument);
		if (0 == m_iRefHighDay)//不要新高条件
		{
			if (E_SEEK_HIGH == eSigStatus)
			{
				eSigStatus = E_WAIT_FALL;
				SetSigStatus(pt->szInstrument, E_WAIT_FALL);
			}
		}

		if(E_SEEK_HIGH == eSigStatus)
		{
			CString strTime = pt->szDate;
			CKStrdata oHighKData;

			//新高条件
			bool bSeekRefKDay = true;
			double dbRefHighPrice = 0;
			for (int i = 1; i <= m_iRefHighDay; i++)
			{
				CKStrdata oRefNDay;

				if (!GetPreNDayKData(pt->szInstrument, strTime, oRefNDay, -i))
				{
					sprintf(sMsg, "获取前%d日K线数据失败:%s", i, pt->szInstrument);
					WRITE_LOG(1, sMsg);
					bSeekRefKDay = false;
					break;
				}
				if (oRefNDay.priceH > dbRefHighPrice)
				{

					dbRefHighPrice = oRefNDay.priceH;
					oHighKData = oRefNDay;
				}
			}
			if (!bSeekRefKDay || dbRefHighPrice > oHighKData.priceH)
			{
				return;
			}
			SetSigStatus(pt->szInstrument, E_WAIT_FALL);
			SetHighPriceKData(pt->szInstrument, oHighKData);
		}
		else if (E_WAIT_FALL == eSigStatus)
		{
			//回调条件

			if (0 == m_iRefHighDay)//不要新高条件
			{


				CKStrdata oPrevKData, oCurKData;
				
				bool bStrictlyFall = true;
				for (int i = 1; i <= m_iFallDays; i++)
				{
					if (!GetPreNDayKData(pt->szInstrument, pt->szDate, oPrevKData, -i))
					{
						sprintf(sMsg, "获取前%d日K线数据失败:%s", i, pt->szInstrument);
						WRITE_LOG(1, sMsg);
						return;
					}
					if (m_bStrictlyFall)
					{
						if (oCurKData.priceH > oPrevKData.priceH)
						{
							bStrictlyFall = false;
						}
					}
					if (oPrevKData.priceH * (1 - m_dbFallRate) >= pt->lastprice)
					{
						CKStrdata oLowKData;
						oLowKData.priceL = pt->lastprice;
						strcpy(oLowKData.curDate, pt->szDate);
						oLowKData.priceH = pt->highestprice;
						if (m_bStrictlyFall)
						{
							if (bStrictlyFall)
							{
								
								SetLowPriceKData(pt->szInstrument, oLowKData);
								SetSigStatus(pt->szInstrument, E_WAIT_ARISE);
								eSigStatus = E_WAIT_ARISE;
							}
						}
						else
						{
							SetLowPriceKData(pt->szInstrument, oLowKData);
							SetSigStatus(pt->szInstrument, E_WAIT_ARISE);
							eSigStatus = E_WAIT_ARISE;
						}
					}

					oCurKData = oPrevKData;

				}

			}
			else
			{
				CKStrdata oHiKData;
				if (!GetHighPriceKData(pt->szInstrument, oHiKData))
				{
					return;
				}

				//找到最高价出现的交易日到当前交易日的最低价

				if (pt->lastprice > oHiKData.priceH)
				{
					SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
					//SetHighPriceKData(sCode, *pk);
					return;
				}

				//回落天数
				int iDownDays = m_pKManager->CalDaysBetween2DayK(pt->szInstrument, oHiKData.curDate, pt->szDate);
				if (iDownDays > m_iFallDays)
				{
					SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
					return;
				}

				if (m_bStrictlyFall)//检查是否单调回落
				{
					CKStrdata oKPrevData = oHiKData;
					CKStrdata oKCurData;
					bool bStrictly = true;
					for (int i = 1; i <= iDownDays; i++)
					{
						if (!GetPreNDayKData(pt->szInstrument, oHiKData.curDate, oKCurData, i))
						{
							sprintf(sMsg, "获取前%d日K线数据失败:%s", i, pt->szInstrument);
							WRITE_LOG(1, sMsg);

							return;
						}
						if (oKCurData.priceH > oKPrevData.priceH)
						{
							sprintf(sMsg, "股票:%s, 在新高日:%s不是单调回落", pt->szInstrument, oKCurData.curDate);
							WRITE_LOG(1, sMsg);
							bStrictly = false;
							break;
						}
						oKPrevData = oKCurData;
					}

					if (!bStrictly)
					{
						SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
						return;

					}

				}

				CKStrdata oLowKData;
				if (GetLowPriceKData(pt->szInstrument, oLowKData))
				{
					if (oLowKData.priceL > pt->lastprice)
					{
						CKStrdata oCurKData;
						oCurKData.priceL = pt->lastprice;
						strcpy(oCurKData.curDate, pt->szDate);
						oCurKData.priceH = pt->highestprice;
						//记录回调最低K线
						SetLowPriceKData(pt->szInstrument, oCurKData);
					}
				}
				else
				{	
					//记录回调最低K线
					CKStrdata oCurKData;
					oCurKData.priceL = pt->lastprice;
					strcpy(oCurKData.curDate, pt->szDate);
					oCurKData.priceH = pt->highestprice;
					//记录回调最低K线
					SetLowPriceKData(pt->szInstrument, oCurKData);
				}

				if (oHiKData.priceH * (1 - m_dbFallRate)  > pt->lastprice)
				{
					SetSigStatus(pt->szInstrument, E_WAIT_ARISE);
					eSigStatus = E_WAIT_ARISE;
				}
			}

		}
		else if (E_WAIT_ARISE == eSigStatus)
		{
			CKStrdata oLowKData;
			if (GetLowPriceKData(pt->szInstrument, oLowKData))
			{

				if(pt->lastprice < oLowKData.priceL)
				{
					CKStrdata oCurKData;
					oCurKData.priceL = pt->lastprice;
					strcpy(oCurKData.curDate, pt->szDate);
					oCurKData.priceH = pt->highestprice;
					SetLowPriceKData(pt->szInstrument, oCurKData);
				}
				else if (pt->lastprice > oLowKData.priceH)
				{
					CKStrdata oHiKData;
					if (!GetHighPriceKData(pt->szInstrument, oHiKData))
					{
						return;
					}
					int iFallDays =  m_pKManager->CalDaysBetween2DayK(pt->szInstrument, oHiKData.curDate, pt->szDate);
					if (iFallDays > m_iWaitBreakDownDays)
					{
						sprintf_s(sMsg,256,"%s 创新高后%d天没有见底", pt->szInstrument, m_iWaitBreakDownDays);
						AddLog(sMsg);
						SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
						RemoveHighPriceKData(pt->szInstrument);
						RemoveLowPriceKData(pt->szInstrument);
						return;
					}

					int iUpDays = m_pKManager->CalDaysBetween2DayK(pt->szInstrument, oLowKData.curDate, pt->szDate);
					if (iUpDays > m_iWaitBreakDownDays)
					{
						sprintf_s(sMsg,256,"%s 创新低后%d天没有突破", pt->szInstrument, m_iWaitBreakDownDays);
						AddLog(sMsg);
						SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
						RemoveHighPriceKData(pt->szInstrument);
						RemoveLowPriceKData(pt->szInstrument);
						return;
					}
					

					if (pt->lastprice > oLowKData.priceH && oPreKData1.priceS * 1.1 - pt->lastprice > 0.001 /*非涨停*/)
					{
						double dbOpenPrice = pt->askprice[0];

						double dbFund;
						if (!ApplyForFund(dbFund))
						{
							sprintf_s(sMsg,256,"%s 出信号,但资金不够， 当前开仓数: %d", pt->szInstrument, GetPositionCount());
							AddLog(sMsg);
							SetSigStatus(pt->szInstrument, E_SEEK_HIGH);
							RemoveHighPriceKData(pt->szInstrument);
							RemoveLowPriceKData(pt->szInstrument);
							return;
						}

						int iCount = ((int)(dbFund / dbOpenPrice) / 100 ) * 100;
						SetStockCount(pt->szInstrument, iCount);
						int iOrderRef = OpenBuy((char*)pt->szInstrument, iCount, dbOpenPrice, m_fundAccount.GetBuffer(0));
						SetSigStatus(pt->szInstrument, E_SEEK_HIGH);



						CKStrdata oHiKData;
						TRACE("\n");
						if (GetHighPriceKData(pt->szInstrument, oHiKData))
						{
							sprintf_s(sMsg, 256, "******股票代码:%s, 开仓日:%s,创新高日:%s, 最低价日:%s*****\n", pt->szInstrument, pt->szDate, oHiKData.curDate, oLowKData.curDate);
						}
						else
						{
							sprintf_s(sMsg, 256, "******股票代码:%s, 开仓日:%s, 最低价日:%s*****\n", pt->szInstrument, pt->szDate, oLowKData.curDate);
						}
						WRITE_LOG(1, sMsg);
#ifdef DEBUG
						TRACE(sMsg);
#endif // DEBUG


						if(m_bBackTest || false == m_bRun)
						{
							//	m_dbAvgPrice = pt->lastprice;
							SetAvgPrice(pt->szInstrument, dbOpenPrice);
						}

						sprintf_s(sMsg,256,"开多,报单引用:%d",iOrderRef);
						AddLog(sMsg);
						AddOrderInfo(iOrderRef,iCount,dbOpenPrice,'0','0', pt->szInstrument);
					}
				}
				//else{
				//	SetSigStatus(sCode, E_SEEK_HIGH);
				//	return;
				//}
			}
		}

	}
	


}


int CStrategyBkHiDownImpl::CalcPositionDays(time_t tOpen, time_t tNow)
{
	struct tm *tmFmt = localtime(&tOpen);
	struct tm tmOpen = *tmFmt;

	tmFmt = localtime(&tNow);
	struct tm tmNow = *tmFmt;

	int iDaySpan = 0;
	if (tmOpen.tm_year == tmNow.tm_year) //20151223 vs 20151225
	{
		iDaySpan = tmNow.tm_yday - tmOpen.tm_yday;

	}
	else if (tmOpen.tm_year < tmNow.tm_year) //20151231 vs 20160104
	{
		if (0 == tmOpen.tm_year % 4) //润年
		{
			iDaySpan = 366 - tmOpen.tm_yday + tmNow.tm_yday;
		}
		else
		{
			iDaySpan = 365 - tmOpen.tm_yday + tmNow.tm_yday;
		}
	}


	int iWorkDaySpan = 0;
	time_t tNextDay = tOpen;
	for (int i = 0; i < iDaySpan; i++)
	{
		tNextDay = tNextDay + 60*60*24;
		struct tm* tmNext = localtime(&tNextDay);
		if (tmNext->tm_wday > 0 && tmNext->tm_wday < 6)
		{
			iWorkDaySpan++;
		}
	}

	return iWorkDaySpan;

}


bool CStrategyBkHiDownImpl::GetHighPriceKData(const char* szCode, CKStrdata& oKData)
{
	bool bSuc = false;
	if (m_mapHighPriceKData.find(szCode) != m_mapHighPriceKData.end())
	{
		oKData = m_mapHighPriceKData[szCode];
		bSuc = true;
	}

	return bSuc;
}

void CStrategyBkHiDownImpl::SetHighPriceKData(const char* szCode, CKStrdata oKData)
{
	m_mapHighPriceKData[szCode] = oKData;
}

bool CStrategyBkHiDownImpl::GetLowPriceKData(const char* szCode, CKStrdata& oKData)
{
	bool bSuc = false;
	if (m_mapLowPriceKData.find(szCode) != m_mapLowPriceKData.end())
	{
		oKData = m_mapLowPriceKData[szCode];
		bSuc = true;
	}

	return bSuc;
}
void CStrategyBkHiDownImpl::SetLowPriceKData(const char*szCode, CKStrdata oKData)
{
	m_mapLowPriceKData[szCode] = oKData;
}


E_Bk_Hi_DownStep CStrategyBkHiDownImpl::GetSigStatus(const char* szCode)
{
	if (m_mapSigStatus.end() != m_mapSigStatus.find(szCode))
	{
		return m_mapSigStatus[szCode];
	}
	return E_SEEK_HIGH;
}
void CStrategyBkHiDownImpl::SetSigStatus(const char* szCode, E_Bk_Hi_DownStep eStatus)
{
	m_mapSigStatus[szCode] = eStatus;
	
	
}

void CStrategyBkHiDownImpl::RemoveHighPriceKData(const char* szCode)
{
	std::map<string, CKStrdata>::iterator itKData = m_mapHighPriceKData.find(szCode);
	if (itKData != m_mapHighPriceKData.end())
	{
		m_mapHighPriceKData.erase(itKData);
	}
}

void CStrategyBkHiDownImpl::RemoveLowPriceKData(const char* szCode)
{
	std::map<string, CKStrdata>::iterator itKData = m_mapLowPriceKData.find(szCode);
	if (itKData != m_mapLowPriceKData.end())
	{
		m_mapLowPriceKData.erase(itKData);
	}
}

double CStrategyBkHiDownImpl::GetHighPrice(const char* szCode)
{
	double dbHigh = 0.0;
	if (m_mapHighPrice.find(szCode) != m_mapHighPrice.end())
	{
		dbHigh = m_mapHighPrice[szCode];
	}

	return dbHigh;
}
void CStrategyBkHiDownImpl::SetHighPrice(const char* szCode, double dbPrice)
{
	m_mapHighPrice[szCode] = dbPrice;
}


void CStrategyBkHiDownImpl::BackTest(CKStrdata *pk, const char* sCode)
{
	char sMsg[256];


	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)sCode);
	if (dbActiveCaptital > STOCK_TOTAL_CAPITAL) //流通股本大于20亿股或者流通市值大于1000亿的票排除。
	{
		return;
	}



	int iSignalStatus = GetSignalStatus(sCode);
	int iCount = GetStockCount(sCode);


	//记录最高价
	double dbHighPrice = GetHighPrice(sCode);
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		dbHighPrice = 0;
		SetHighPrice(sCode, dbHighPrice);
	}
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus && dbHighPrice < pk->priceH)
	{
		dbHighPrice = pk->priceH;
		SetHighPrice(sCode, dbHighPrice);
	}


#ifdef _DEBUG
	if (0 == strcmp("20160420", pk->curDate))
	{
		WRITE_LOG(1,"DEBUG");
	}
#endif

	int iOrderRef;
	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		if (m_bMa20Ref)
		{
			CKStrdata oIndexData;
			bool bSuc = m_pKManager->getDayKData("999999", pk->curDate, oIndexData);
			double dbMa20 = 0;
			if (!m_pKManager->CalcMa20("999999", pk->curDate, dbMa20))
			{
				return;
			}
			if (!bSuc || oIndexData.priceS <= dbMa20)
			{
				return;
			}

		}
		
		E_Bk_Hi_DownStep eSigStatus = GetSigStatus(sCode);
		if (0 == m_iRefHighDay)//不要新高条件
		{
			 if (E_SEEK_HIGH == eSigStatus)
			 {
				 eSigStatus = E_WAIT_FALL;
				 SetSigStatus(sCode, E_WAIT_FALL);
			 }
			
		}
		
		if(E_SEEK_HIGH == eSigStatus)
		{
			CString strTime = pk->curDate;
			CKStrdata oHighKData;
			
			//新高条件
			bool bSeekRefKDay = true;
			double dbRefHighPrice = 0;
			for (int i = 1; i <= m_iRefHighDay; i++)
			{
				CKStrdata oRefNDay;

				if (!GetPreNDayKData(sCode, strTime, oRefNDay, -i))
				{
					sprintf(sMsg, "获取前%d日K线数据失败:%s", i, sCode);
					WRITE_LOG(1, sMsg);
					bSeekRefKDay = false;
					break;
				}
				if (oRefNDay.priceH > dbRefHighPrice)
				{

					dbRefHighPrice = oRefNDay.priceH;
					oHighKData = oRefNDay;
				}
			}
			if (!bSeekRefKDay || dbRefHighPrice > oHighKData.priceH)
			{
				return;
			}
			SetSigStatus(sCode, E_WAIT_FALL);
			SetHighPriceKData(sCode, oHighKData);
		}
		else if (E_WAIT_FALL == eSigStatus)
		{
			//回调条件

			if (0 == m_iRefHighDay)//不要新高条件
			{
				

				CKStrdata oPrevKData, oCurKData;
				oCurKData = *pk;
				bool bStrictlyFall = true;
				for (int i = 1; i <= m_iFallDays; i++)
				{
					if (!GetPreNDayKData(sCode, pk->curDate, oPrevKData, -i))
					{
						sprintf(sMsg, "获取前%d日K线数据失败:%s", i, sCode);
						WRITE_LOG(1, sMsg);
						return;
					}
					if (m_bStrictlyFall)
					{
						if (oCurKData.priceH > oPrevKData.priceH)
						{
							bStrictlyFall = false;
						}
					}
					

					oCurKData = oPrevKData;
					
				}
				if (oPrevKData.priceH * (1 - m_dbFallRate) >= pk->priceL)
				{
					if (m_bStrictlyFall)
					{
						if (bStrictlyFall)
						{
							SetLowPriceKData(sCode, *pk);
							SetSigStatus(sCode, E_WAIT_ARISE);
							eSigStatus = E_WAIT_ARISE;
						}
					}
					else
					{
						SetLowPriceKData(sCode, *pk);
						SetSigStatus(sCode, E_WAIT_ARISE);
						eSigStatus = E_WAIT_ARISE;
					}
				}
			}
			else
			{
				CKStrdata oHiKData;
				if (!GetHighPriceKData(sCode, oHiKData))
				{
					return;
				}

				//找到最高价出现的交易日到当前交易日的最低价

				if (pk->priceH > oHiKData.priceH)
				{
					SetSigStatus(sCode, E_SEEK_HIGH);
					//SetHighPriceKData(sCode, *pk);
					return;
				}

				//回落天数
				int iDownDays = m_pKManager->CalDaysBetween2DayK(sCode, oHiKData.curDate, pk->curDate);
				if (iDownDays > m_iFallDays)
				{
					SetSigStatus(sCode, E_SEEK_HIGH);
					return;
				}

				CKStrdata oKPrevData = oHiKData;
				CKStrdata oKCurData;
				bool bStrictlyFall = true;
				for (int i = 1; i <= iDownDays; i++)
				{
					if (!GetPreNDayKData(sCode, oHiKData.curDate, oKCurData, i))
					{
						sprintf(sMsg, "获取前%d日K线数据失败:%s", i, sCode);
						WRITE_LOG(1, sMsg);

						return;
					}
					if (m_bStrictlyFall)//检查是否单调回落
					{
						if (oKCurData.priceH > oKPrevData.priceH)
						{
							bStrictlyFall = false;
						}

					}
					

					oKPrevData = oKCurData;
				}

				if (oHiKData.priceH * (1 - m_dbFallRate) >= pk->priceL)
				{
					if (m_bStrictlyFall)
					{
						if (bStrictlyFall)
						{
							SetLowPriceKData(sCode, *pk);
							SetSigStatus(sCode, E_WAIT_ARISE);
							eSigStatus = E_WAIT_ARISE;
						}
					}
					else
					{
						SetLowPriceKData(sCode, *pk);
						SetSigStatus(sCode, E_WAIT_ARISE);
						eSigStatus = E_WAIT_ARISE;
					}
				}
			}
			
		}
		else if (E_WAIT_ARISE == eSigStatus)
		{
			CKStrdata oLowKData;
			if (GetLowPriceKData(sCode, oLowKData))
			{

				if(pk->priceL < oLowKData.priceL)
				{
					SetLowPriceKData(sCode, *pk);
				}
				else if (pk->priceH > oLowKData.priceH)
				{
					CKStrdata oHiKData;
					if (!GetHighPriceKData(sCode, oHiKData))
					{
						return;
					}
					int iFallDays =  m_pKManager->CalDaysBetween2DayK(sCode, oHiKData.curDate, pk->curDate);
					if (iFallDays > m_iWaitBreakDownDays)
					{
						sprintf_s(sMsg,256,"%s 创新高后%d天没有见底", sCode, m_iWaitBreakDownDays);
						AddLog(sMsg);
						SetSigStatus(sCode, E_SEEK_HIGH);
						RemoveHighPriceKData(sCode);
						RemoveLowPriceKData(sCode);
						return;
					}


					int iDays = m_pKManager->CalDaysBetween2DayK(sCode, oLowKData.curDate, pk->curDate);
					if (iDays > m_iWaitBreakDownDays)
					{
						sprintf_s(sMsg,256,"%s 创新低后%d天没有突破", sCode, m_iWaitBreakDownDays);
						AddLog(sMsg);
						SetSigStatus(sCode, E_SEEK_HIGH);
						RemoveHighPriceKData(sCode);
						RemoveLowPriceKData(sCode);
						return;
					}

					if ((pk->priceH == pk->priceK && pk->priceH == pk->priceL && pk->priceH == pk->priceS))
					{
						SetSigStatus(sCode, E_SEEK_HIGH);
						RemoveHighPriceKData(sCode);
						RemoveLowPriceKData(sCode);
						return;
					}
					else if (pk->priceH > oLowKData.priceH)
					{
						double dbOpenPrice;
						if (pk->priceH >= oLowKData.priceH &&  pk->priceL <= oLowKData.priceH)
						{
							dbOpenPrice = oLowKData.priceH;
						}
						else
						{
							dbOpenPrice = pk->priceK;
						}

						
						double dbFund;
						if (!ApplyForFund(dbFund))
						{
							sprintf_s(sMsg,256,"%s 出信号,但资金不够， 当前开仓数: %d", sCode, GetPositionCount());
							AddLog(sMsg);
							SetSigStatus(sCode, E_SEEK_HIGH);
							RemoveHighPriceKData(sCode);
							RemoveLowPriceKData(sCode);
							return;
						}

						iCount = ((int)(dbFund / dbOpenPrice) / 100 ) * 100;
						SetStockCount(sCode, iCount);
						iOrderRef = OpenBuy((char*)sCode, iCount, dbOpenPrice, m_fundAccount.GetBuffer(0));
						SetSigStatus(sCode, E_SEEK_HIGH);



						CKStrdata oHiKData;
						TRACE("\n");
						if (GetHighPriceKData(sCode, oHiKData))
						{
							sprintf_s(sMsg, 256, "******股票代码:%s, 开仓日:%s,创新高日:%s, 最低价日:%s*****\n", sCode, pk->curDate, oHiKData.curDate, oLowKData.curDate);
						}
						else
						{
							sprintf_s(sMsg, 256, "******股票代码:%s, 开仓日:%s, 最低价日:%s*****\n", sCode, pk->curDate, oLowKData.curDate);
						}
						WRITE_LOG(1, sMsg);
#ifdef DEBUG
						TRACE(sMsg);
#endif // DEBUG


						if(m_bBackTest || false == m_bRun)
						{
							//	m_dbAvgPrice = pt->lastprice;
							SetAvgPrice(sCode, dbOpenPrice);
						}

						sprintf_s(sMsg,256,"开多,报单引用:%d",iOrderRef);
						AddLog(sMsg);
						AddOrderInfo(iOrderRef,iCount,dbOpenPrice,'0','0', sCode);
					}
				}
				//else{
				//	SetSigStatus(sCode, E_SEEK_HIGH);
				//	return;
				//}
			}
		}
		
	}
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		iCount = GetStockCount(sCode);

		time_t tNow;
		CFunctionPub::String2Time(pk->curDate, tNow);

		time_t tLastTrade = GetOpenPostionTime(sCode);
		int iTradeDays = CalcPositionDays(tLastTrade, tNow) + 1;
		if (iTradeDays > 1)
		{
			CKStrdata oPreKData1;
			if (!GetPreNDayKData(sCode, pk->curDate, oPreKData1, -1))
			{
				AddLog("获取前一日K线数据失败");
				return;
			}

			CKStrdata oPreKData2;
			if (!GetPreNDayKData(sCode, pk->curDate, oPreKData2, -2))
			{
				WRITE_LOG(1, "获取前二日K线数据失败");
				return;
			}
			double dbPreDayClosePrice = oPreKData1.priceS;

			//跌停卖不出
			if (pk->priceL - dbPreDayClosePrice * 0.9 <= 0.001 &&
				pk->priceH == pk->priceS && 
				pk->priceH ==pk->priceL &&
				pk->priceH == pk->priceK )
			{
				AddLog("跌停价卖不出");
				return;
			}


			double dbAvgPrice = GetAvgPrice(sCode);
			double dbLimitUpPrice = dbPreDayClosePrice * 1.1;
			

			double dbLostPrice = dbAvgPrice * (1 - m_dbLoseRate);
			/*CKStrdata oLowKData;
			if (GetLowPriceKData(sCode, oLowKData))
			{
			dbLostPrice = oLowKData.priceL * 0.99;
			}*/

			double dbZhiyinPrice = dbAvgPrice * ( 1 + m_dbProfitRate);
			double dbHuitiaoPrice = dbHighPrice * (1- m_dbShiftLost);

			if (pk->priceL <= dbLostPrice)
			{
				CloseSale((char*)sCode, iCount, dbLostPrice, m_fundAccount.GetBuffer(0),'1');
				SetSigStatus(sCode, E_SEEK_HIGH);
				RemoveHighPriceKData(sCode);
				RemoveLowPriceKData(sCode);
				
			}
			else
			{
				if (0 == m_iPriceOrderType)
				{
					//止盈
					if (pk->priceH > dbZhiyinPrice)
					{
						CloseSale((char*)sCode, iCount, dbZhiyinPrice, m_fundAccount.GetBuffer(0),'1');
						SetSigStatus(sCode, E_SEEK_HIGH);
						RemoveHighPriceKData(sCode);
						RemoveLowPriceKData(sCode);
					}
					else//回调
					{
						if (pk->priceS <= dbHuitiaoPrice)
						{
							CloseSale((char*)sCode, iCount, dbHuitiaoPrice, m_fundAccount.GetBuffer(0),'1');
							SetSigStatus(sCode, E_SEEK_HIGH);
							RemoveHighPriceKData(sCode);
							RemoveLowPriceKData(sCode);
						}
					}

				}
				else //收盘价
				{
					//止盈
					if (pk->priceS > dbZhiyinPrice)
					{
						CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
						SetSigStatus(sCode, E_SEEK_HIGH);
						RemoveHighPriceKData(sCode);
						RemoveLowPriceKData(sCode);
					}
					else//回调
					{
						if (pk->priceS <= dbHuitiaoPrice)
						{
							CloseSale((char*)sCode, iCount, pk->priceS, m_fundAccount.GetBuffer(0),'1');
							SetSigStatus(sCode, E_SEEK_HIGH);
							RemoveHighPriceKData(sCode);
							RemoveLowPriceKData(sCode);
						}

					}
				}
			}
		}
	}
}


void CStrategyBkHiDownImpl::AddOrderInfo(int ref,int count,double price,char bs,char kp, const char* szCode)
{
	m_listOrderRef.Add(ref);

	m_CheckSelfMutex.lock();

	COrderInfo order;
	strcpy(order.szInstrument, szCode);
	order.ref = ref;
	order.ordercount = count;
	order.tradecount = 0;
	order.orderprice = price;
	order.bs = bs;
	order.kp = kp;
	m_orderinfo[ref] = order;


	m_CheckSelfMutex.unlock();

}

void CStrategyBkHiDownImpl::SetParam(CString paramname,CString value)
{
	m_params[(LPCTSTR)paramname] = value;

	if (0 == paramname.Compare("交易合约"))
	{
		if (value.GetLength() > 6)
		{
			value = value.Right(6);

		}
		m_strInstrument = value;
	}

	//还需要将对应的值更新
	else if(paramname=="止损率")
	{
		m_dbLoseRate = atof(value);
	}
	else if (paramname=="止盈率")
	{
		m_dbProfitRate = atof(value);
	}
	else if (paramname == "移动止损率")
	{
		m_dbShiftLost = atof(value);
	}
	else if(paramname=="换手率")
	{
		m_dbChangeRate = atof(value);
	}
	else if(paramname=="跌破收盘价比率1")
	{
		m_dbDownRate = atof(value);
	}
	else if(paramname=="账户")
	{
		m_fundAccount = value;
	}
	else if(paramname=="初始权益")
	{
		double dbBeginQuanyi = atof(value);
		SetBeginQuanyi(dbBeginQuanyi);
	}
	else if (paramname == "同时开仓数")
	{

		m_pPolicy->SetParams("同时开仓数", value);
	}
	else if (paramname == "手续费率")
	{

		m_dbCostRate = atof(value);
		m_pStatistic->m_feilv = m_dbCostRate;
		m_pPolicy->SetCostRate(m_dbCostRate);
	}
	else if (paramname == "退出信号为指令价或收盘价")
	{
		m_iPriceOrderType = atoi(value);
	}
	else if(paramname == "是否下单")
	{
		m_bRealTrade = (0 == atoi(value) ? false : true);
	}
	else if(paramname == "创N天新高")
	{
		m_iRefHighDay =  atoi(value);
	}
	else if (paramname == "N天回调")
	{
		m_iFallDays = atoi(value);
	}
	else if (paramname == "回落幅度")
	{
		m_dbFallRate = atof(value);
	}
	else if (paramname == "单调回落")
	{
		m_bStrictlyFall = (0 == atoi(value) ? false : true);
	}
	else if (paramname == "是否参考20日均线")
	{
		m_bMa20Ref = (0 == atoi(value) ? false : true);
	}

	
}

void CStrategyBkHiDownImpl::SetParam(CString value)
{
	//格式是600570;10;40;0.01;0.02;
	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	if( list.GetSize() >= 3 )
	{

		double dbBeginQuanyi = atof(list[0]);
		SetBeginQuanyi(dbBeginQuanyi);

		m_params["初始权益"] = list[0];

		m_dbLoseRate = atof(list[1]);

		m_params["止损率"] = list[1];                      //亏损率

		m_dbProfitRate = atof(list[2]);
		m_params["止盈率"] = list[2];

		m_params["移动止损率"] = list[3];
		m_dbShiftLost = atof(list[3]);

		m_dbChangeRate = atof(list[4]);
		m_params["换手率"] = list[4];		               //换手率


		m_dbDownRate = atof(list[5]);

		m_params["回调率"] = list[5];						//回调率

		m_fundAccount = list[6];							//帐号   用逗号隔开的  a,b,c,d,
		m_params["账户"] = list[6];				

		double nPostionCount = atoi(list[7]);
		m_pPolicy->SetParams("同时开仓数", list[7]);
		m_params["同时开仓数"] = list[9];

		m_dbCostRate = atof(list[8]);
		m_params["手续费率"] = list[8];
		m_pStatistic->m_feilv = m_dbCostRate;
		m_pPolicy->SetCostRate(m_dbCostRate);
		//m_strModelName = m_strStrategyName + m_strInstrument;     //modelname

		m_params["退出信号为指令价或收盘价"] = list[9];
		m_iPriceOrderType = atoi(list[9]);

		m_params["是否下单"] = list[10];
		m_bRealTrade = (0 == atoi(list[10]) ? false : true);

		m_params["创N天新高"] = list[11];
		m_iRefHighDay =  atoi(list[11]);


		m_params["N天回调"] = list[12];
		m_iFallDays = atoi(list[12]);

		m_params["回落幅度"] = list[13];
		m_dbFallRate = atof(list[13]);

		m_params["单调回落"] = list[14];
		m_bStrictlyFall = (0 == atoi(list[14]) ? false : true);

		m_params["是否参考20日均线"] = list[15];
		m_bMa20Ref = (0 == atoi(list[15]) ? false : true);
	

	}
}
CString CStrategyBkHiDownImpl::GetParamValue()
{
	CString str;


	CString strValue = m_pPolicy->GetParams("同时开仓数");

	int nPostionCount = atoi(strValue);
	//"初始权益;止损率;止盈率;移动止损率;换手率;跌破收盘价比率1;跌破收盘价比率2;涨停类型;账户;同时开仓数;手续费率;退出信号为指令价或收盘价;是否下单;创N天新高;N天回调;回落幅度;单调回落,是否参考20日均线";
	double dbFund = m_pPolicy->GetCurrentFund();
	str.Format("%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%s;%d;%.2f;%d;%d,%d,%d,%.2f,%d,%d",
		m_dbBeginQuanyi,//初始权益
		m_dbLoseRate,//止损率
		m_dbProfitRate,//止盈率
		m_dbShiftLost,//移动止损率
		m_dbChangeRate,//换手率
		m_dbDownRate, //回调率
		m_fundAccount,//账户
		nPostionCount,//同时开仓数
		m_dbCostRate,//手续费率

		m_iPriceOrderType,//退出信号为指令价或收盘价
		m_bRealTrade,//是否下单

		m_iRefHighDay,
		m_iFallDays,

		m_dbFallRate,
		m_bStrictlyFall,
		m_bMa20Ref
		);

		/*
	m_params.insert(pair<string, CString>("回落幅度", ""));//
	m_params.insert(pair<string, CString>("单调回落", ""));*/
	return str;
}

CString CStrategyBkHiDownImpl::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyBkHiDownImpl::GetStatusInfo(void)
{
	return "";
}
void CStrategyBkHiDownImpl::BeginRun()
{

}
void CStrategyBkHiDownImpl::OnTradeReturn(char * ins,char * sTime,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{
	bool bt = false;

	bt = isExistRef(orderref,direction,offsetflag,ins);
	if (bt)
	{
		if ('0' == offsetflag && '0' == direction)
		{
			time_t tLastTrade = time(NULL);
			SetOpenPostionTime(ins, tLastTrade);
			m_orderinfo[orderref].tradecount += vol;
			if (m_orderinfo[orderref].ordercount == m_orderinfo[orderref].tradecount)
			{
				//m_dbAvgPrice = price;
				SetAvgPrice(ins, price);
			}

		}

	}
}
void CStrategyBkHiDownImpl::OnErrorRtn(int code,char * errormsg) 
{

}

bool CStrategyBkHiDownImpl::isExistRef(int ref,char bs,char kp,char * ins)
{
	map<int,COrderInfo> ::iterator iter;

	m_CheckSelfMutex.lock();

	iter = m_orderinfo.find(ref);
	if(iter!=m_orderinfo.end())
	{
		//先把kp去掉，返回1 ，3 ，4 也是醉了
		if( (strcmp(iter->second.szInstrument,ins)==0) && (iter->second.bs==bs) /*&& (iter->second.kp==kp)*/ )
		{
			m_CheckSelfMutex.unlock();
			return true;
		}
	}
	m_CheckSelfMutex.unlock();
	return false;

}

void CStrategyBkHiDownImpl::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}
int CStrategyBkHiDownImpl::CloseAll(char* szInstrument, double dbPrice)
{
	int iSignalStatus = GetSignalStatus(szInstrument);
	if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		int iCount = GetStockCount(szInstrument);
		CloseSale(szInstrument, iCount, dbPrice, m_fundAccount.GetBuffer(0),'1');
	}
	return 0;
}
void CStrategyBkHiDownImpl::CreateStatisticObject()
{
	m_pStatistic = new CStockTradeStatistic();
}
void CStrategyBkHiDownImpl::reload()                //重新加载
{

}
void CStrategyBkHiDownImpl::saveModel()            //保存模型
{

}


bool CStrategyBkHiDownImpl::GetDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData)
{
	ZeroMemory(&oKData, sizeof(oKData));
	if (m_pKManager)
	{
		m_pKManager->getDayKData(szCode, szCurDate, oKData);
		return true;
	}

	return false;
}

bool CStrategyBkHiDownImpl::GetPreNDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData, int iDay)
{
	ZeroMemory(&oKData, sizeof(oKData));
	if (m_pKManager)
	{
		return m_pKManager->getNDayKData(szCode, szCurDate, oKData, iDay);
	}

	return false;
}