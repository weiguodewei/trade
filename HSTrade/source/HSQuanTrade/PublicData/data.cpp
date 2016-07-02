#include "stdafx.h"
#include "data.h"
#include "string"
#include "tool.h"
#include "typedefine.h"

////
//CAccData
////
CAccData::CAccData()
{
	m_bReady = false;

	m_dEnableMoney = 0.0;

	m_hMsgWnd = 0;

	m_dMargin = 99999999;

	m_iCheDanCount = 500;

	m_dPL = 0;

	m_dUseMargin = 0;

	m_dUsePL = 0;
}


void CAccData::SetRiskInfo(double margin,int count,double pl)
{
	m_dMargin = margin;
	m_iCheDanCount = count;
	m_dPL = pl;

}


double CAccData::UpdateHoldsInfo(char *code,int vol,double price,char bs,char kp)
{
	char msg[256];

	double ret = 0; 
	//头一次开仓
	if(m_sHoldsReal.find(code) == m_sHoldsReal.end())
	{
		CHoldsEx ohold;
		strcpy(ohold.szInstrument,code);
		//ohold.PosiDirection = bs;
		if(bs==POSTION_DIRECTION_BUY)
		{
			ohold.dLongTradePrice = price;
			ohold.LongPosition = vol;
			ohold.dLongMargin = vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].LMarginRatio;
		}
		else if(bs==POSTION_DIRECTION_SELL)
		{
			ohold.dShortTradePrice = price;
			ohold.ShortPosition = vol;
			ohold.dShortMargin = vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].SMarginRatio;
		}

		m_sHoldsReal[code] = ohold;

	}
	else
	{
		int newcount = 0;	
		//开仓
		if(kp == HS_THOST_FTDC_OF_Open)
		{
			//计算开仓均价	
			if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_BUY)
			{
				m_sHoldsReal[code].dLongMargin += vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].LMarginRatio;
				newcount = m_sHoldsReal[code].LongPosition + vol;
				m_sHoldsReal[code].dLongTradePrice = (m_sHoldsReal[code].LongPosition * m_sHoldsReal[code].dLongTradePrice + vol * price) / newcount;
				m_sHoldsReal[code].LongPosition = newcount;
			}
			else if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_SELL)
			{
				m_sHoldsReal[code].dShortMargin += vol * price  * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple * CPubData::m_sFutuCodeInfo[code].SMarginRatio;
				newcount = m_sHoldsReal[code].ShortPosition + vol;
				m_sHoldsReal[code].dShortTradePrice = (m_sHoldsReal[code].ShortPosition * m_sHoldsReal[code].dShortTradePrice + vol * price) / newcount;
				m_sHoldsReal[code].ShortPosition = newcount;
			}
		}
		//平仓 
		else
		{
			double AveMargin = 0;
						
			//平空
			if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_BUY)
			{
				if(m_sHoldsReal[code].ShortPosition <=0)
				{
					WRITE_LOG(1,"持仓不匹配...");
					return 0;
				}

				//保证金
				AveMargin = m_sHoldsReal[code].dShortMargin / m_sHoldsReal[code].ShortPosition;
				m_sHoldsReal[code].dShortMargin -= AveMargin * vol;

				newcount = m_sHoldsReal[code].ShortPosition - vol;	

				//盈亏
				ret = (m_sHoldsReal[code].dShortTradePrice - price) * vol;

				//持仓均价
				if(newcount!=0)
					m_sHoldsReal[code].dShortTradePrice = (m_sHoldsReal[code].ShortPosition * m_sHoldsReal[code].dShortTradePrice - vol * price) / newcount;
				else
					m_sHoldsReal[code].dShortTradePrice = 0;

				//持仓量
				m_sHoldsReal[code].ShortPosition = newcount;
			}
			//平多
			else if(/*ohold.PosiDirection==bs && */bs == POSTION_DIRECTION_SELL)
			{
				if(m_sHoldsReal[code].LongPosition <=0)
				{
					WRITE_LOG(1,"持仓不匹配...");
					return 0;
				}

				//保证金
				AveMargin = m_sHoldsReal[code].dLongMargin / m_sHoldsReal[code].LongPosition;
				m_sHoldsReal[code].dLongMargin -= AveMargin * vol;

				newcount = m_sHoldsReal[code].LongPosition - vol;

				//盈亏
				ret = (price- m_sHoldsReal[code].dLongTradePrice) * vol;

				//持仓均价
				if(newcount!=0)
					m_sHoldsReal[code].dLongTradePrice = (m_sHoldsReal[code].LongPosition * m_sHoldsReal[code].dLongTradePrice - vol * price) / newcount;
				else
					m_sHoldsReal[code].dLongTradePrice = 0;

				//持仓量
				m_sHoldsReal[code].LongPosition = newcount;
			}
		}
	
	}

#ifdef _DEBUG
	map<string,CHoldsEx> ::iterator it = m_sHoldsReal.begin();
	char heyue[32];
	int lcount = 0;
	double lprice = 0;
	int scount = 0;
	double sprice = 0;
	double lmargin = 0;
	double smargin = 0;
	for(;it != m_sHoldsReal.end();it++)
	{
		strcpy(heyue,it->second.szInstrument);
		lcount = it->second.LongPosition;
		lprice = it->second.dLongTradePrice;
		lmargin = it->second.dLongMargin;
		scount = it->second.ShortPosition;
		sprice = it->second.dShortTradePrice;
		smargin = it->second.dShortMargin;
		sprintf(msg,"持仓信息  合约 %s  多仓 %d  多头均价 %.4f  多头保证金 %.4f  空仓 %d  空头均价 %.4f  空头保证金 %.4f",heyue,lcount,lprice,lmargin,scount,sprice,smargin);
		WRITE_LOG(1,msg);
	}

#endif

	return ret;

}


double CAccData::GetMargin(char *code)
{
	double ret = 0;
	map<string,CHoldsEx> ::iterator it = m_sHoldsReal.begin();

	for(;it != m_sHoldsReal.end();it++)
	{
		//if(bs == POSTION_DIRECTION_BUY)
		//{
		//	if(it->second.LongPosition >=0)
		//		ret = it->second.dLongMargin;
		//}
		//else if(bs == POSTION_DIRECTION_SELL)
		//{
		//	if(it->second.ShortPosition >=0)
		//		ret = it->second.dShortMargin;
		//}

		ret = ret = it->second.dLongMargin + it->second.dShortMargin;

	}

	return ret;
}


void CAccData::AddOrder(int orderref)
{
	m_vOrderRef.push_back(orderref);
}


bool CAccData::ExistOrder(int orderref)
{
	bool ret = false;
	for(size_t i=0; i< m_vOrderRef.size(); i++)
	{
		if(orderref == m_vOrderRef[i])
		{
			ret = true;
			break;
		}
	}

	return ret;
}


void CAccData::UpdateRiskInfo(char *code,int vol,double price,char bs,char kp)
{
	char msg[256];

#ifdef _DEBUG
	WRITE_LOG(1,"---------------------------riskinfo--------------------------");

	sprintf(msg,"成交信息  合约 %s  数量 %d  成交价格 %.4f  买卖 %c  开平 %c",code,vol,price,bs,kp);
	WRITE_LOG(1,msg);
#endif


	//获取保证金率
	double rate = 0;
	if(bs == POSTION_DIRECTION_BUY)
	{
		rate = CPubData::m_sFutuCodeInfo[code].LMarginRatio;
	}
	else if(bs == POSTION_DIRECTION_SELL)
	{
		rate = CPubData::m_sFutuCodeInfo[code].SMarginRatio;
	}

	//更新持仓信息
	double pl = UpdateHoldsInfo(code,vol,price,bs,kp) * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple;

	//累计盈亏
	double oldpl = m_dUsePL;
	m_dUsePL += pl;

	double oldmargin = m_dUseMargin;
	//开仓 加
	if(kp == HS_THOST_FTDC_OF_Open)
	{
		//m_dUseMargin += rate * price * vol; 
		m_dUseMargin = GetMargin(code);
		sprintf(msg,"原已用保证金 %.4f  本次增加保证金 %.4f  现已用保证金 %.4f",oldmargin,m_dUseMargin-oldmargin,m_dUseMargin);
	}
	//平仓 减
	else/* if(kp == HS_THOST_FTDC_OF_Close || kp == HS_THOST_FTDC_OF_CloseToday)*/
	{
		//m_dUseMargin -= rate * price * vol;
		m_dUseMargin = GetMargin(code);
		sprintf(msg,"原已用保证金 %.4f  本次释放保证金 %.4f  现已用保证金 %.4f",oldmargin,oldmargin-m_dUseMargin,m_dUseMargin);
	}

#ifdef _DEBUG	
	WRITE_LOG(1,msg);

	sprintf(msg,"原平仓盈亏 %.4f  本次成交盈亏 %.4f  现平仓盈亏 %.4f",oldpl,pl,m_dUsePL);
	WRITE_LOG(1,msg);

	WRITE_LOG(1,"---------------------------riskinfo--------------------------");
#endif
}


bool CAccData::CheckRisk(char *code,int count,double price,char bs,char kp,int type) //type=0 下单  type=1 撤单
{
	bool ret = true;
	char msg[256];

	//记录单合约撤单次数
	if(type == 1)
	{
		if(m_mapUseCheDan.find(code)==m_mapUseCheDan.end())
			m_mapUseCheDan[code] = 1;
		else
		{		
			if(m_mapUseCheDan[code] >= m_iCheDanCount)		
				ret = false;
			if(ret)
				m_mapUseCheDan[code]++;

		}

		sprintf(msg,"合约 %s  撤单次数 %d  撤单次数上限 %d",code,m_mapUseCheDan[code],m_iCheDanCount);
		WRITE_LOG(1,msg);

	}
	//下单
	else if(type == 0)
	{
		//平仓
		if(kp == HS_THOST_FTDC_OF_Close)
			return ret;

		double rate = 0;
		if(bs == POSTION_DIRECTION_BUY)
		{
			rate = CPubData::m_sFutuCodeInfo[code].LMarginRatio;
		}
		else if(bs == POSTION_DIRECTION_SELL)
		{
			rate = CPubData::m_sFutuCodeInfo[code].SMarginRatio;
		}

		double prefreez = price * count * rate * CPubData::m_sFutuCodeInfo[code].iVolumeMultiple;

		//检查保证金占用
		if(prefreez + m_dUseMargin >= m_dMargin)
		{
			sprintf(msg,"本次委托预冻结 %.4f  当前占用保证金 %.4f  超出保证金限额  %.4f",prefreez,prefreez + m_dUseMargin,m_dMargin);
			WRITE_LOG(1,msg);
			ret = false;
		}

		//检查盈亏
		if(m_dUsePL > m_dPL)
		{
			sprintf(msg,"当前平仓盈亏 %.4f  超过平仓盈亏限额 %.4f",m_dUsePL,m_dPL);
			WRITE_LOG(1,msg);
			ret = false;
		}
	}

	return ret;

}



////
//CPubData
////
CPubData::CPubData()
{


}



////
//CStockInfo
////
CStockInfo::CStockInfo()
{

}

void CStockInfo::LoadCodeInfo()
{
	CStdioFile file;

	std::string path = Tool::GetProgramDir() + "\\StockInfo.txt";

	CString pathstr = path.c_str();

	if(!file.Open(pathstr,CFile::modeRead))
		return;

	CString strCodeInfo;
	CString strSubString0 =_T(",");
	CString strSubString1 =_T(",");
	CString strSubString2 =_T(",");
	CString strSubString3 =_T(",");

	while(file.ReadString(strCodeInfo))
	{

		AfxExtractSubString (strSubString0,(LPCTSTR)strCodeInfo,0,',');
		AfxExtractSubString (strSubString1,(LPCTSTR)strCodeInfo,1,',');
		AfxExtractSubString (strSubString2,(LPCTSTR)strCodeInfo,2,',');
		AfxExtractSubString (strSubString3,(LPCTSTR)strCodeInfo,6,',');


		StockInfo * sc = new StockInfo();        //存入结构体
		memset(sc,0,sizeof(StockInfo));
		strcpy(sc->code,(LPCTSTR)strSubString0);
		strcpy(sc->Name,(LPCTSTR)strSubString1);
		sc->J_start = atol(strSubString2);
		sc->ActiveCapital = atof(strSubString3);
		//判断一下code属于哪一个市场
		if (sc->code[0]=='6' && sc->code[1]=='0')
		{
			sc->exchid = 1;
		}
		else
		{
			sc->exchid = 2;
		}

		m_PubData.m_sCodeInfo[sc->code] = *sc;
		delete sc;
		
	}
	file.Close();

}


std::map<std::string,StockInfo> CPubData::m_sCodeInfo;           //股票合约

std::map<std::string,FutuCodeInfo> CPubData::m_sFutuCodeInfo;    //期货合约
std::vector<ArbitrageCodeInfo> CPubData::s_vFutuCodeInfo;

std::map<std::string,StockOpt> CPubData::m_mapOptCode;			//股票期权代码

std::map<std::string, ArbitrageCodeInfo> CPubData::m_mapArbitrageCode;		//标准套利合约

std::map<std::string, int> CPubData::m_mapExchaneId;