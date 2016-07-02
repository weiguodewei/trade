#include "stdafx.h"
#include "FundManager.h"
#include "typedefine.h"
#include "StrategyBase.h"

CFundPolicyBase::CFundPolicyBase()
{
	m_iFuli = 1;

}
CFundPolicyBase::~CFundPolicyBase()
{

}
bool CFundPolicyBase::GetAvailableFund(double& dbFund)
{
	dbFund = m_dbInitFund;
	if (m_mpTrade.size() > 0)
	{
		m_mpTrade.clear();
	}
	return true;
}

void CFundPolicyBase::SetParams(CString paramname,CString value)
{

}
CString CFundPolicyBase::GetParams(CString paramname)
{
	return "";
}
void CFundPolicyBase::SetInitFund(double dbInitFund)
{
	m_dbInitFund = dbInitFund;
	m_dbFund = dbInitFund;
}

void CFundPolicyBase::OpenBuy(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(0, 0, szCode, dbPrice, iCount);
}
void CFundPolicyBase::OpenSell(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(0, 1, szCode, dbPrice, iCount);
}
void CFundPolicyBase::CloseBuy(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(1, 0, szCode, dbPrice, iCount);
}
void CFundPolicyBase::CloseSell(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(1, 1, szCode, dbPrice, iCount);
}

//记录每天的开仓
void CFundPolicyBase::RecordTrade(int iOpenClose, int iDir, const char* szCode, double dbPrice, int iCount)
{
	if (0 == iOpenClose)//open
	{
		CTradeRecord2 oRec;
		strcpy(oRec.heyue, szCode);
		oRec.count = iCount;
		oRec.price = dbPrice;

		m_mpTrade[szCode] = oRec;
	}
	else
	{
		std::map<string, CTradeRecord2>::iterator itRec = m_mpTrade.find(szCode);
		if (itRec != m_mpTrade.end())
		{
			m_mpTrade.erase(itRec);
		}
	}
}




////CFundAvgPolicy

CFundAvgPolicy::CFundAvgPolicy()
{
	m_nPositionCount = 5;
	m_nPositionAcc = 0;
}
CFundAvgPolicy::~CFundAvgPolicy()
{

}
bool CFundAvgPolicy::GetAvailableFund(double& dbFund)
{
	if (m_nPositionAcc >= m_nPositionCount)
	{
		return false;
	}

	if(m_iFuli)
	{
		dbFund = m_dbFund / (m_nPositionCount - m_nPositionAcc);
	}
	else
	{
		dbFund = m_dbInitFund / m_nPositionCount ;    //始终一样的值
	}
	return true;
}

void CFundAvgPolicy::SetParams(CString paramname,CString value)
{
	if (0 == paramname.Compare("同时开仓数"))
	{
		m_nPositionCount = atoi(value);
	}
	else if (0 == paramname.Compare("是否复利"))
	{
		m_iFuli = atoi(value);
	}
}

CString CFundAvgPolicy::GetParams(CString paramname)
{
	CString strValue;
	if (0 == paramname.Compare("同时开仓数"))
	{
		strValue.Format("%d", m_nPositionCount);
	}
	else if (0 == paramname.Compare("是否复利"))
	{
		strValue.Format("%d", m_iFuli);
	}

	return strValue;
}

void CFundAvgPolicy::OpenBuy(const char* szCode, double dbPrice, int iCount)
{
	double dbFund = dbPrice * iCount;
	m_dbFund -= dbFund;
	m_dbFund -= dbFund * m_dbCostRate;
	m_nPositionAcc++;

	RecordTrade(0, 0, szCode, dbPrice, iCount);
}

void CFundAvgPolicy::CloseSell(const char* szCode, double dbPrice, int iCount)
{
	double dbFund = dbPrice * iCount;
	m_dbFund += dbFund;
	m_dbFund -= dbFund * m_dbCostRate;
	m_nPositionAcc--;
	RecordTrade(1, 1, szCode, dbPrice, iCount);
}

void CFundAvgPolicy::OpenSell(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(0, 1, szCode, dbPrice, iCount);
}
void CFundAvgPolicy::CloseBuy(const char* szCode, double dbPrice, int iCount)
{
	RecordTrade(1, 0, szCode, dbPrice, iCount);
}

int CFundAvgPolicy::GetPostionCount()
{
	return m_nPositionAcc;
}

CStockEquity CFundAvgPolicy::GetEveryDayEquity(CStrategyBase* pStrategy,const char* szDate)
{
	CStockEquity oStockEquity;
	double dbEveryDayEquity = 0.0;
	double dbFloatProfit = 0.0;

#ifdef _DEBUG
	if(0 == strcmp(szDate, "20151225"))
	{
		cout<<"debug everyday equity...";
	}
#endif
	if (pStrategy)
	{
		
		std::map<string, CTradeRecord2>::const_iterator itRec = m_mpTrade.cbegin();
		while (itRec != m_mpTrade.cend())
		{
			CKStrdata kData;
			CTradeRecord2 oRec = itRec->second;
			if (pStrategy->m_pKManager->getDayKData(itRec->first.c_str(), szDate, kData))
			{
				
				if (0 == oRec.openorclose)
				{
					dbFloatProfit += kData.priceS  * oRec.count;
				}
				m_mapLastClosePrice[itRec->first] = kData.priceS;
			}
			else
			{
				if (m_mapLastClosePrice.find(itRec->first) != m_mapLastClosePrice.end())
				{
					dbFloatProfit += m_mapLastClosePrice[itRec->first] * oRec.count;
				}
			}
			
			
			itRec++;
		}
	}
	oStockEquity.m_dbStockEquity = dbFloatProfit;
	dbEveryDayEquity = m_dbFund + dbFloatProfit;
	oStockEquity.m_dbTotalEquity = dbEveryDayEquity;
	
	return oStockEquity;
}



////CFundManager
///////

CFundManager::CFundManager(void)
{
}


CFundManager::~CFundManager(void)
{
}


CFundPolicyBase* CFundManager::GetPolicyInstance(int iFundPolicy)
{
	CFundPolicyBase* pPolicy = NULL;
	switch ((EFundPolicyType)iFundPolicy)
	{
	case  FUND_NO_POLICY:
		pPolicy = new CFundPolicyBase();
		break;
	case FUND_POLICY_STOCK_AVG:
		{
			pPolicy = new CFundAvgPolicy();
		}
		break;
	default:
		pPolicy = new CFundPolicyBase();
		break;
	}

	return pPolicy;
}
