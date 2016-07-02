#include "stdafx.h"
#include "StrategyBase.h"
#include <windows.h>

#include<string>
#include "tool.h"
#include "FunctionPub.h"

using namespace std;


CStrategyBase::CStrategyBase(void)
{
	m_bRun = false;

	m_Exit = false;

	m_follow = "-1";                   //是否跟单

	m_timeOut = 0;                     //超时时间

	m_jiacha = 0;					       //价差

	m_bBackTest = false;
	m_dbBeginQuanyi = 200000;
	//m_nSignalStatus = TRADE_STATUS_INIT;

	m_iPriceOrderType = 0;//默认为指令价

	m_bRealTrade = true;


	m_pThdSubcode = NULL;
	

	
}


CStrategyBase::~CStrategyBase(void)
{

	
	
	
}

void CStrategyBase::StartTick()
{
	m_thdTickProcess.Start(Task_TickProcess, 0, this);
}

CString CStrategyBase::GetStatusInfo(void)
{



	return CString();
}

void CStrategyBase::SetParam(CString value)
{


}

void CStrategyBase::BackTest(CKStrdata *pk, const char* sCode)
{

}

CString CStrategyBase::GetParamValue()
{

	return "";
}

CString CStrategyBase::GetParamValue(CString paramname)
{
	return "";
}



int CStrategyBase::OpenBuy(char * instrument, int count, double price,const char * account, int follow)
{
	//信号
	CString  code = instrument;

	code = code + "|" + m_pStrategyCodeInfo->getName(instrument);

	CString  p ;

	//p.Format("%.4f",m_lastPrice);
	p.Format("%.4f",price);

	string msg = code + "|" + "BK" + "|" + p + "|";

	AddSignal(msg.c_str());


	//日志
	CString a = account;

	CString  f ,s;

	f.Format("%.4f",price);

	s.Format("%i",count);

	CString loginfo = "" + a + "|买开|" + code + "|" + f + "|" + s ;

	AddLog(loginfo.GetBuffer(0), TRADE_MSG_TYPE_SIG);

	
	//return m_SPTrade->InsertOrder(instrument,count,'0','0',price,atoi(m_follow),m_timeOut,jiacha,account);

	//改为发消息下单
	COrderInfo *c = new COrderInfo();
	strcpy(c->szInstrument,instrument);
	c->ordercount = count;
	c->bs = '0';
	c->kp = '0';
	c->orderprice = price;
	c->ordertype = follow;
	c->timeOut = m_timeOut;
	c->jiacha = m_jiacha;
	strcpy(c->account,account);

	if (m_bRealTrade)
	{
		m_pStrategyManager->m_LockOrder.lock();
		::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,0,(LPARAM)c);
		m_pStrategyManager->m_LockOrder.unlock();
	}
	

	int ref = c->ref;
	delete c;
	if(m_bBackTest && false == m_bRun)
	{
		
		char szDay[9] = "\0";
		strncpy(szDay, m_szTickTime, 8);
		char szTime[16] = "\0";
		if (strlen(m_szTickTime) >= 13)
		{
			strncpy(szTime, m_szTickTime + 9, 6);
		}
		

		m_pStatistic->AddOpreate(szDay,szTime,price,count,0,0, instrument);

		
		
	}
	time_t tLastTrade;
	CFunctionPub::String2Time(m_szTickTime, tLastTrade);
	SetOpenPostionTime(instrument, tLastTrade);
	if (m_pPolicy)
	{
		m_pPolicy->OpenBuy(instrument, price, count);
	}
	SetSignalStatus(instrument, TRADE_STATUS_BK_TRADE);

	CMediaAux::PlayWavSound(E_HS_SOUND_SIGNAL);

	return ref;

}

int CStrategyBase::OpenSale(char * instrument, int count, double price,const char * account, int follow)
{

	//信号
	CString  code = instrument;

	code = code + "|" + m_pStrategyCodeInfo->getName(instrument);

	CString  p ;

	p.Format("%.4f",price);

	string msg = code + "|" + "SK" + "|" + p + "|";

	AddSignal(msg.c_str());

	//日志
	CString a = account;

	CString  f ,s;

	//p.Format("%.4f",m_lastPrice);
	f.Format("%.4f",price);

	s.Format("%i",count);


	CString loginfo = "" + a + "|卖开|" + code + "|" + f + "|" + s ;

	AddLog(loginfo.GetBuffer(0), TRADE_MSG_TYPE_SIG);

	//return m_SPTrade->InsertOrder(instrument,count,'1','0',price,atoi(m_follow),m_timeOut,jiacha,account);

	//改为发消息下单
	COrderInfo *c = new COrderInfo();
	strcpy(c->szInstrument,instrument);
	c->ordercount = count;
	c->bs = '1';
	c->kp = '0';
	c->orderprice = price;
	c->ordertype = follow;
	c->timeOut = m_timeOut;
	c->jiacha = m_jiacha;
	strcpy(c->account,account);

	if(m_bRealTrade)
	{
		m_pStrategyManager->m_LockOrder.lock();
		::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,0,(LPARAM)c);
		m_pStrategyManager->m_LockOrder.unlock();
	}
	int ref = c->ref;
	delete c;

	if(m_bBackTest && false == m_bRun)
	{

		char szDay[9] = "\0";
		strncpy(szDay, m_szTickTime, 8);
		char szTime[16] = "\0";
		if (strlen(m_szTickTime) >= 13)
		{
			strncpy(szTime, m_szTickTime + 9, 6);
		}

		m_pStatistic->AddOpreate(szDay, szTime, price, count, 0, 1, instrument);
	}
	SetSignalStatus(instrument, TRADE_STATUS_SK_TRADE);

	CMediaAux::PlayWavSound(E_HS_SOUND_SIGNAL);
	return ref;
}

int CStrategyBase::CloseBuy(char * instrument,int count, double price,const char * account,char closeflag, int follow)
{

	//信号
	CString  code = instrument;

	code = code + "|" + m_pStrategyCodeInfo->getName(instrument);

	CString  p ;

	//p.Format("%.4f",m_lastPrice);
	p.Format("%.4f",price);

	string msg = code + "|" + "BP" + "|" + p + "|";

	AddSignal(msg.c_str());

	//日志
	CString a = account;

	CString  f ,s;

	f.Format("%.4f",price);

	s.Format("%i",count);

	CString loginfo = "" + a + "|买平|" + code + "|" + f + "|" + s ;

	AddLog(loginfo.GetBuffer(0), TRADE_MSG_TYPE_SIG);

	//return m_SPTrade->InsertOrder(instrument,count,'0',closeflag,price,atoi(m_follow),m_timeOut,jiacha,account);

	//改为发消息下单
	COrderInfo *c = new COrderInfo();
	strcpy(c->szInstrument,instrument);
	c->ordercount = count;
	c->bs = '0';
	c->kp = closeflag;
	c->orderprice = price;
	c->ordertype = follow;
	c->timeOut = m_timeOut;
	c->jiacha = m_jiacha;
	strcpy(c->account,account);

	if(m_bRealTrade)
	{
		m_pStrategyManager->m_LockOrder.lock();
		::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,0,(LPARAM)c);
		m_pStrategyManager->m_LockOrder.unlock();
	}
	int ref = c->ref;
	delete c;
	if(m_bBackTest && false == m_bRun)
	{

		char szDay[9] = "\0";
		strncpy(szDay, m_szTickTime, 8);
		char szTime[16] = "\0";
		if (strlen(m_szTickTime) >= 13)
		{
			strncpy(szTime, m_szTickTime + 9, 6);
		}

		m_pStatistic->AddOpreate(szDay, szTime, price, count, 1, 0, instrument);
	}

	SetSignalStatus(instrument, TRADE_STATUS_INIT);

	CMediaAux::PlayWavSound(E_HS_SOUND_SIGNAL);
	return ref;
}

int CStrategyBase::CloseSale(char * instrument, int count,double price,const char * account,char closeflag, int follow)
{
	//信号
	CString  code = instrument;

	code = code + "|" + m_pStrategyCodeInfo->getName(instrument);

	CString  p ;

	//p.Format("%.4f",m_lastPrice);
	p.Format("%.4f",price);

	string msg = code + "|" + "SP" + "|" + p + "|";

	AddSignal(msg.c_str());


	//日志
	CString a = account;

	CString  f ,s;

	f.Format("%.4f",price);

	s.Format("%i",count);

	CString loginfo = "" + a + "|卖平|" + code + "|" + f + "|" + s ;

	AddLog(loginfo.GetBuffer(0), TRADE_MSG_TYPE_SIG);

	//return m_SPTrade->InsertOrder(instrument,count,'1',closeflag,price,atoi(m_follow),m_timeOut,jiacha,account);

	//改为发消息下单
	COrderInfo *c = new COrderInfo();
	strcpy(c->szInstrument,instrument);
	c->ordercount = count;
	c->bs = '1';
	c->kp = closeflag;
	c->orderprice = price;
	c->ordertype = follow;
	c->timeOut = m_timeOut;
	c->jiacha = m_jiacha;
	strcpy(c->account,account);

	if(m_bRealTrade)
	{
		m_pStrategyManager->m_LockOrder.lock();
		::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,0,(LPARAM)c);
		m_pStrategyManager->m_LockOrder.unlock();
	}
	int ref = c->ref;
	delete c;
	if(m_bBackTest && false == m_bRun)
	{
		char szDay[9] = "\0";
		strncpy(szDay, m_szTickTime, 8);
		char szTime[16] = "\0";
		if (strlen(m_szTickTime) >= 13)
		{
			strncpy(szTime, m_szTickTime + 9, 6);
		}

		m_pStatistic->AddOpreate(szDay, szTime, price, count, 1, 1, instrument);
	}
	if (m_pPolicy)
	{
		m_pPolicy->CloseSell(instrument, price, count);
	}
	SetSignalStatus(instrument, TRADE_STATUS_INIT);

	CMediaAux::PlayWavSound(E_HS_SOUND_SIGNAL);
	
	return ref;
}


void CStrategyBase::OrderAction(int orderref)
{
	CMediaAux::PlayWavSound();

	CEntrustInfo *c= new CEntrustInfo();
	char * ac = (char*)m_fundAccount.GetBuffer(0);
	//strcpy(c->account,ac);
	c->account = ac;
	c->entrustno = orderref;
	strcpy(c->szInstrument,m_strInstrument.GetBuffer(0));

	CString log;
	log.Format("账户:%s,撤单,委托号:%d",ac,orderref);
	AddLog(log.GetBuffer(0));
	//AddSignal(log.GetBuffer(0));
	
	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,1,(LPARAM)c);
	delete c;
}


void CStrategyBase::QueryPosition()
{
	//m_SPTrade->QueryPosition(account);
	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,2,(LPARAM)m_fundAccount.GetBuffer(0));

}

void CStrategyBase::QueryOrder()
{
	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,4,(LPARAM)m_fundAccount.GetBuffer(0));

}

void CStrategyBase::QueryTrade()
{

	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,5,(LPARAM)m_fundAccount.GetBuffer(0));

}

void CStrategyBase::QueryFund()
{

	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,6,(LPARAM)m_fundAccount.GetBuffer(0));

}



void CStrategyBase::AddLog( const char * log, int iMsgType)
{

	string msg(log);

	CString sStragtegyName = m_strStrategyName + m_sStrategyId;
	string ss(sStragtegyName.GetBuffer(0));
	sStragtegyName.ReleaseBuffer();
	if(iMsgType == TRADE_MSG_TYPE_LOG)
		WRITE_LOG(1,ss+"   "+msg);

	string time = Tool::getTimeStr();


	msg = time + "|"+ msg + "#";            //加一个分隔符便于前台获取

	if (!m_sModuleId.IsEmpty())
	{
		CString strMsg(msg.c_str());
		strMsg += "$";
		strMsg += m_sModuleId;
		
		::SendMessage(m_hMsgWnd,WM_MESSAGE_TRADE, iMsgType, (long)strMsg.GetBuffer());
		strMsg.ReleaseBuffer();
	}

	m_LockMutex.lock();

	//数据多了先删除
	if(m_listLog.size()>100)
		m_listLog.clear();

	m_listLog.push_back(msg);

	m_LockMutex.unlock();
}

CString CStrategyBase::GetLogInfo()
{
	string log = "";

	vector<string>::iterator it;

	m_LockMutex.lock();

	for(it = m_listLog.begin();it!=m_listLog.end();it++)
	{
		log = log +  (*it);
	}

	m_LockMutex.unlock();

	return log.c_str();
}

void CStrategyBase::AddSignal(const char * sig)
{
	string signal(sig);
	CString sStragtegyName = m_strStrategyName + m_sStrategyId;
	string ss(sStragtegyName.GetBuffer(0));
	sStragtegyName.ReleaseBuffer();
	WRITE_LOG(1,ss+"   "+signal);

	string time = Tool::getTimeStr();

	signal = time + "|"+ signal + "#";            //加一个分隔符便于前台获取

	m_LockMutex.lock();

	//数据多了先删除
	if(m_listSignal.size()>100)
		m_listSignal.clear();

	m_listSignal.push_back(signal);

	m_LockMutex.unlock();
}

CString CStrategyBase::GetSignalInfo()
{
	string signal = "";

	vector<string>::iterator it;

	m_LockMutex.lock();

	for(it = m_listSignal.begin();it!=m_listSignal.end();it++)
	{
		signal = signal +  (*it);
	}

	m_LockMutex.unlock();

	return signal.c_str();
}





void CStrategyBase::SetWndHwnd(HWND hWnd)
{
	m_hMsgWnd = hWnd;

	if(m_pStatistic != NULL)
	{
		m_pStatistic->SetMsgWnd(hWnd);
	}
}

void CStrategyBase::SetStrategyManager(CStrategyManager* pStragyManager)
{
	m_pStrategyManager = pStragyManager;
}

CStrategyManager* CStrategyBase::GetStrategyManager()
{
	return m_pStrategyManager;
}

void CStrategyBase::SetBeginQuanyi(double dbQuanyi)
{
	if (!m_pPolicy)
	{
		m_pPolicy = CFundManager::GetPolicyInstance(FUND_NO_POLICY);
	}
	if (m_pStatistic)
	{
		m_pStatistic->SetBeginQuanyi(dbQuanyi);
	}
	m_pPolicy->SetInitFund(dbQuanyi);
	m_dbBeginQuanyi = dbQuanyi;
}

bool CStrategyBase::ApplyForFund(double& dbFund)
{
	bool bSuc = true;
	if (!m_pPolicy)
	{
		dbFund = m_dbBeginQuanyi;
	}
	else
	{
		bSuc = m_pPolicy->GetAvailableFund(dbFund);
	}
	return bSuc;
}

int CStrategyBase::GetPositionCount()
{
	if (!m_pPolicy)
	{
		return 0;
	}

	return m_pPolicy->GetPostionCount();
}

int CStrategyBase::GetSignalStatus(const char* szCode)
{
	if (m_mapSignalStatus.find(szCode) != m_mapSignalStatus.end())
	{
		return m_mapSignalStatus[szCode];
	}
	else
	{
		return TRADE_STATUS_INIT;
	}
}
void CStrategyBase::SetSignalStatus(const char* szCode, int iSignalStatus)
{
	m_mapSignalStatus[szCode] = iSignalStatus;
}

int CStrategyBase::GetStockCount(const char* szCode)
{
	int iCount = 0;
	if (m_mapStockCount.find(szCode) != m_mapStockCount.end())
	{
		iCount = m_mapStockCount[szCode];
	}

	return iCount;
}
void CStrategyBase::SetStockCount(const char* szCode, int iCount)
{
	m_mapStockCount[szCode] = iCount;
}

time_t CStrategyBase::GetOpenPostionTime(const char* szCode)
{
	time_t tOpen;
	if (m_mapOpenTime.find(szCode) != m_mapOpenTime.end())
	{
		tOpen = m_mapOpenTime[szCode];
	}
	return tOpen;
}
void CStrategyBase::SetOpenPostionTime(const char* szCode, time_t tOpen)
{
	m_mapOpenTime[szCode] = tOpen;
}

double CStrategyBase::GetAvgPrice(const char* szCode)
{
	double dbAvgPrice;
	if(m_mapAvgPrice.find(szCode) != m_mapAvgPrice.end())
	{
		dbAvgPrice = m_mapAvgPrice[szCode];
	}
	return dbAvgPrice;
}
void CStrategyBase::SetAvgPrice(const char* szCode, double dbAvgPrice)
{
	m_mapAvgPrice[szCode] = dbAvgPrice;
}


double CStrategyBase::GetHgPrice(const char* szCode)
{
	double dprice = 0;
	if(m_mapHighPrice.find(szCode) != m_mapHighPrice.end())
	{
		dprice = m_mapHighPrice[szCode];
	}

	return dprice;
}



void CStrategyBase::SetHgPrice(const char* szCode, double dbHgPrice)
{
	m_mapHighPrice[szCode] = dbHgPrice;
}


//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
void CStrategyBase::ResumePostionInfo(CString strPosInfo)
{
	CStringArray arInfo;
	CFunctionPub::ParseParam(strPosInfo, arInfo, '$');
	CString info; 

	char msg[256];
	for (int i = 0; i < arInfo.GetSize(); i++)
	{
		CStringArray arPos;
		CFunctionPub::ParseParam(arInfo[i], arPos, ';');
		if (arPos.GetSize() >= 6)
		{
			CString strCode = arPos[0];
			int iPos = atoi(arPos[2]);
			SetStockCount(strCode, iPos);

			double dbPrice = atof(arPos[3]);
			SetAvgPrice(strCode, dbPrice);

			if (m_pPolicy)
			{
				m_pPolicy->OpenBuy(strCode, dbPrice, iPos);
			}

			int iDir = atoi(arPos[4]);
			if (0 == iDir)
			{
				SetSignalStatus(strCode, TRADE_STATUS_BK_TRADE);
			}
			else
			{
				SetSignalStatus(strCode, TRADE_STATUS_SK_TRADE);
			}
			CString strTime = arPos[5];
			time_t tmOpen;
			CFunctionPub::String2Time(strTime, tmOpen);
			SetOpenPostionTime(strCode, tmOpen);
			
			//最高价
			CString strHighPrice = "0";
			if(arPos.GetSize()>=7)
			{
				strHighPrice =  arPos[6];
				double temp = atof(strHighPrice.GetBuffer(0));
				SetHgPrice(strCode,temp);

				/*if(temp > 0.00001)
					info += strCode + ":" + strHighPrice + ":#";*/

				strHighPrice.ReleaseBuffer();
			}

			sprintf(msg,"模型恢复  合约 %s  数量 %d  成本价 %.4f  方向 %d  开仓时间 %s  最高价 %s",strCode.GetBuffer(0),iPos,dbPrice,iDir,strTime.GetBuffer(0),strHighPrice.GetBuffer(0));
			AddLog(msg);
		}
	}

	/*if(info!="")
	{
		reload(info);
	}*/

}

//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
CString CStrategyBase::GetPostionInfo()
{
	CString strContext = "";
	
	std::map<string, int>::const_iterator itCode = m_mapSignalStatus.cbegin();
	while (itCode != m_mapSignalStatus.cend())
	{
		if (TRADE_STATUS_BK_TRADE == itCode->second || TRADE_STATUS_SK_TRADE == itCode->second)
		{
			CString strName = "";
			int iPostion = 0;
			double dbPrice = 0.0;
			int iDir = 0;
			CString strTime = "";
			char szBuf[64] = {0};
			strContext += itCode->first.c_str();
			strContext += ";";
			
			if (m_pStrategyCodeInfo)
			{
				strName = m_pStrategyCodeInfo->getName((char*)itCode->first.c_str());

			}
			strContext += strName;
			strContext += ";";
			if (m_mapStockCount.end() != m_mapStockCount.find(itCode->first))
			{
				iPostion = m_mapStockCount[itCode->first];
			}
			strContext += itoa(iPostion, szBuf, 10);
			strContext += ";";

			if (m_mapAvgPrice.end() != m_mapAvgPrice.find(itCode->first))
			{
				dbPrice = m_mapAvgPrice[itCode->first];
			}
			sprintf(szBuf, "%.4f", dbPrice);
			strContext += szBuf;
			strContext += ";";

			
			if (TRADE_STATUS_BK_TRADE == itCode->second)
			{
				iDir = 0;
			}
			else
			{
				iDir = 1;
			}

			strContext += itoa(iDir, szBuf, 10);
			strContext += ";";

			if (m_mapOpenTime.end() != m_mapOpenTime.find(itCode->first))
			{
				time_t tOpen = m_mapOpenTime[itCode->first];

				CTime tmOpen(tOpen);
				strTime = tmOpen.Format("%Y%m%d %H%M%S");
			}
			
			strContext += strTime;

			//买入过后最高价
			strContext += ";";
			sprintf(szBuf, "%.4f", GetHgPrice(itCode->first.c_str()));
			strContext += szBuf;

			strContext += "$";
			
		}
		
		itCode++;
	}

	return strContext;
}
void CStrategyBase::JoinAllTask()
{
	m_thdTickProcess.Join();

	if (m_pThdSubcode)
	{

		m_pThdSubcode->Join();
	}
}
void CStrategyBase::TickPeek(CTickData * pt)
{
	
	if (pt)
	{
		if (pt->askprice[0] - pt->bidprice[0] < 0.0001 )//忽略集合竞价
		{
			//char sMsg[128];
			//sprintf("忽略集合竞价, 合约:%s, 时间:%s",pt->szInstrument, pt->szTime);
			//WRITE_LOG(1, sMsg);
			return;
		}
		m_mtxTick.Lock();
		m_vTicks.push_back(*pt);
		m_mtxTick.Unlock();
	}
	
}


void CStrategyBase::Task_TickProcess(void *lpData)
{
	CStrategyBase *pStrategy = (CStrategyBase*)lpData;
	if (pStrategy)
	{

		for (;;)
		{
			if (!pStrategy->m_bRun)
			{
				break;
			}
			pStrategy->m_mtxTick.Lock();
			
			std::vector<CTickData>::iterator itTick = pStrategy->m_vTicks.begin();
			while (itTick != pStrategy->m_vTicks.end())
			{
				CTickData pt = *itTick;
				pStrategy->Tick(&pt);
				
				itTick++;
			}

			pStrategy->m_vTicks.clear();
			/*{
				std::vector<CTickData> vTemp;

				pStrategy->m_vTicks.swap(vTemp);
			}*/

			pStrategy->m_mtxTick.Unlock();
			Sleep(1);
		}
		
					
	}
	
}

void CStrategyBase::Task_Subcode(void *lpData)
{
	CStrategyBase *pStrategy = (CStrategyBase*)lpData;
	if (pStrategy)
	{
		
		pStrategy->OnSubcode();
	}
}
void CStrategyBase::StartSubcode()
{
	m_pThdSubcode = new CEasyThread();
	m_pThdSubcode->Start(Task_Subcode, 0, this);
}

//9:30以后

bool CStrategyBase::IsRightTickTime(const char* szTime)
{
	if (szTime)//92900
	{
		if (5 == strlen(szTime))
		{
			char szHour[8];
			memset(szHour, 0, 8);
			strncpy(szHour, szTime, 1);
			int iHour = atoi(szHour);
			if (iHour< 9)
			{
				return false;
			}
			else if (9 == iHour)
			{
				char szMin[8];
				memset(szMin, 0, 8);
				strncpy(szMin, szTime + 1, 2);
				int iMin = atoi(szMin);
				if (iMin < 30)
				{
					return false;
				}
			}
		}
		
		return true;
		
	}
	return true;
}

char CStrategyBase::GetFutuCloseFlag(const char* szCode, const char* szOpenTime)
{
	char cFlag = '1'; //平昨
	std::map<std::string,FutuCodeInfo> *pMapFutuCodeInfo = &CPubData::m_sFutuCodeInfo;

	if (pMapFutuCodeInfo)
	{
		if (pMapFutuCodeInfo->find(szCode) != pMapFutuCodeInfo->end())
		{
			FutuCodeInfo oFutuInfo = pMapFutuCodeInfo->at(szCode);
			if (0 == strcmp(oFutuInfo.exchid, "SHFE"))
			{
//#ifdef _DEBUG
				//cFlag = '3';
//#else
				CTime tmOpen;
				if (0 == CFunctionPub::String2Time(szOpenTime, tmOpen))
				{
					CTime tmNow = CTime::GetCurrentTime();
					CTimeSpan oTmSpan = tmNow - tmOpen;

					if (oTmSpan.GetDays() < 1)
					{
						//夜盘
						if ((tmOpen.GetHour() >= 21 && tmOpen.GetHour() <= 23 )|| (tmOpen.GetHour() < 3 && tmOpen.GetHour() >= 0))
						{
							if (tmNow.GetHour() <= 15)
							{
								cFlag = '3';
							}

						}
						else if (tmOpen.GetHour() >= 9 && tmOpen.GetHour() <= 15)//白天
						{
							if (tmNow.GetHour() >= 9 && tmNow.GetHour() <= 15)
							{
								cFlag = '3';
							}
						}	
					}
					else
					{
						//周五夜盘
						if (6 == tmOpen.GetDayOfWeek() && oTmSpan.GetDays() <= 3 && 2 == tmNow.GetDayOfWeek() && tmNow.GetHour() <= 15)
						{
							cFlag = '3';
						}
						
					}
				}

//#endif
			}

		}
	}
	
	

	return cFlag;
}

char* CStrategyBase::GetFutuName(const char* szCode)
{
	char *pName = NULL;
	std::map<std::string,FutuCodeInfo> *pMapFutuCodeInfo = &CPubData::m_sFutuCodeInfo;
	if (pMapFutuCodeInfo->find(szCode) != pMapFutuCodeInfo->end())
	{
		FutuCodeInfo oFutuInfo = pMapFutuCodeInfo->at(szCode);
		pName = oFutuInfo.name;
	}

	return pName;
}