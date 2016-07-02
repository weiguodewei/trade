#include "stdafx.h"
#include "StrategyArbitrageImpl.h"
#include "..\HSQuanTrade\include\HundsunMdCenterStruct.h"
#include "..\FunctionPub.h"
#include "..\TradeInterface.h"

//收盘前5分钟停止交易
#define HS_STOP_TRADE_MINUTE 5

St_Condition_Type CStrategyArbitrageImpl::s_arCondtionType[] = {
	{">", 0},{">=", 1},{"<", 2},{"<=", 3}
};
CStrategyArbitrageImpl::CStrategyArbitrageImpl(void)
{
	m_bRun = false;

	m_strStrategyName = "商品套利策略";

	m_strParamList = "合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号;开仓方向";



	m_params.insert(pair<string, CString>("合约一",""));
	m_params.insert(pair<string, CString>("合约二",""));
	m_params.insert(pair<string, CString>("是否标准合约",""));
	m_params.insert(pair<string, CString>("开仓条件",""));
	m_params.insert(pair<string, CString>("价差门限",""));
	m_params.insert(pair<string, CString>("开仓数量",""));
	m_params.insert(pair<string, CString>("平仓条件一",""));
	m_params.insert(pair<string, CString>("平仓价差一",""));
	m_params.insert(pair<string, CString>("平仓条件二", ""));
	m_params.insert(pair<string, CString>("平仓价差二", ""));//



	m_params.insert(pair<string, CString>("启用策略执行下单",""));
	m_params.insert(pair<string, CString>("优先开仓合约",""));
	m_params.insert(pair<string, CString>("每天开仓次数小于", ""));//
	m_params.insert(pair<string, CString>("下单账号", ""));


	m_iMinutesPreStop = 5;

	m_iLastRef = -1;

	m_iLeg1OpenPosAcc = 0;
	m_iLeg2OpenPosAcc = 0;
	m_iLeg1ClosePosAcc = 0;
	m_iLeg2ClosePosAcc = 0;

	m_iOpenPosAcc = 0;
	m_iClosePosAcc = 0;

	m_timeOut = 1;

	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_nLeg1Status = STRATEGY_STATUS_INIT;
	m_nLeg2Status = STRATEGY_STATUS_INIT;

	m_dbLeg1PriceSum = 0.0;
	m_dbLeg2PriceSum = 0.0;
	
	m_iLeg1VolMul = 0;
	m_iLeg2VolMul = 0;
	m_iTodayDayOpenNum = 0;

	m_bEnabled = true;

	m_pPolicy = CFundManager::GetPolicyInstance(FUND_POLICY_STOCK_AVG);
}


CStrategyArbitrageImpl::~CStrategyArbitrageImpl(void)
{
}

void CStrategyArbitrageImpl::BeginRun()
{

}

void CStrategyArbitrageImpl::OnTradeReturn(char * ins,char * time,int orderref,int ordersystemid,char direction,char offsetflag,int vol,double price)
{
	char szMsg[256];
	if (m_arCodeSubs.end() == m_arCodeSubs.find(ins))
	{
		return;
	}
	if (m_bIsStdArbitrage)
	{

		if (HS_THOST_FTDC_OF_Open == offsetflag)
		{
			if (m_iLastRef == orderref && (STRATEGY_STATUS_WAIT_BK == m_nStrategyStatus || STRATEGY_STATUS_WAIT_SK ==m_nStrategyStatus))
			{
				if (0 == m_strCode1.Compare(ins))
				{
					m_iLeg1OpenPosAcc += vol;
					m_dbLeg1PriceSum += price * vol;
					if (m_iLeg1OpenPosAcc == m_iOpenNum)
					{
						m_strLeg1OpenTime= CFunctionPub::GetFullDateString(time);
						double dbAvgPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg1OpenPosAcc, dbAvgPrice, m_strLeg1OpenTime);
					}

				}
				else if (0 == m_strCode2.Compare(ins))
				{
					m_iLeg2OpenPosAcc += vol;
					m_dbLeg2PriceSum += price * vol;
					if (m_iLeg2OpenPosAcc == m_iOpenNum)
					{
						m_strLeg2OpenTime = CFunctionPub::GetFullDateString(time);
						double dbAvgPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg2OpenPosAcc, dbAvgPrice, m_strLeg2OpenTime);
					}
				}
				
				
				if (m_iLeg1OpenPosAcc == m_iOpenNum && m_iLeg2OpenPosAcc == m_iOpenNum)
				{
					m_iOpenPosAcc = m_iOpenNum;
					if (POSTION_DIRECTION_BUY == direction)
					{
						m_nStrategyStatus = STRATEGY_STATUS_BK;
						m_nLeg1Status = STRATEGY_STATUS_BK;
						m_nLeg2Status = STRATEGY_STATUS_SK;
					}
					else
					{
						m_nStrategyStatus = STRATEGY_STATUS_SK;
						m_nLeg1Status = STRATEGY_STATUS_SK;
						m_nLeg2Status = STRATEGY_STATUS_BK;
					}
				}
				
			}

			
		}

		else if (HS_THOST_FTDC_OF_Close == offsetflag ||
			HS_THOST_FTDC_OF_ForceClose == offsetflag ||
			HS_THOST_FTDC_OF_CloseToday == offsetflag ||
			HS_THOST_FTDC_OF_CloseYesterday == offsetflag ||
			HS_THOST_FTDC_OF_ForceOff == offsetflag ||
			HS_THOST_FTDC_OF_LocalForceClose == offsetflag)
		{
			if (0 == m_strCode1.CompareNoCase(ins) && ((m_iOpenDir == 1 && m_nLeg1Status == STRATEGY_STATUS_WAIT_BP) || ( m_iOpenDir == 0 && m_nLeg1Status == STRATEGY_STATUS_WAIT_SP)))
			{
				m_iLeg1ClosePosAcc += vol;
				if (m_iLeg1ClosePosAcc == m_iLeg1OpenPosAcc)
				{

					m_nLeg1Status = STRATEGY_STATUS_INIT;
					OnNotifyPostion(ins, 1/*close*/, direction == '0' ? 0 : 1, m_iLeg1ClosePosAcc, 0, time);

					m_iLeg1OpenPosAcc = 0;

					m_dbLeg1PriceSum = 0;
					m_iLeg1ClosePosAcc = 0;

					if (m_nLeg1Status == STRATEGY_STATUS_INIT && m_nLeg2Status == STRATEGY_STATUS_INIT)
					{
						m_iTodayDayOpenNum++;
						memset(szMsg, 0, 256);
						sprintf(szMsg, "已开平仓%d次\n", m_iTodayDayOpenNum);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}
				}

			}
			else if (0 == m_strCode2.CompareNoCase(ins)&& 
				((m_iOpenDir == 0 && m_nLeg2Status == STRATEGY_STATUS_WAIT_BP) || ( m_iOpenDir == 1 && m_nLeg2Status == STRATEGY_STATUS_WAIT_SP)))
			{
				m_iLeg2ClosePosAcc += vol;
				if (m_iLeg2ClosePosAcc == m_iLeg2OpenPosAcc)
				{

					m_nLeg2Status = STRATEGY_STATUS_INIT;
					OnNotifyPostion(ins, 1/*close*/, direction == '0' ? 0 : 1, m_iLeg2ClosePosAcc, 0, time);
					m_iLeg2OpenPosAcc = 0;
					m_iLeg2ClosePosAcc = 0;
					m_dbLeg2PriceSum = 0;

					if (m_nLeg1Status == STRATEGY_STATUS_INIT && m_nLeg2Status == STRATEGY_STATUS_INIT)
					{
						m_iTodayDayOpenNum++;
						memset(szMsg, 0, 256);
						sprintf(szMsg, "已开平仓%d次\n", m_iTodayDayOpenNum);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}
				}
			}

			
		}
	}
	else
	{
		if (HS_THOST_FTDC_OF_Open == offsetflag)
		{
			switch (m_iFirstOpenFlag)
			{
			case 0://1st leg first
				{
					if (m_iLeg1Ref == orderref && 0 == m_strCode1.CompareNoCase(ins) && (STRATEGY_STATUS_WAIT_BK == m_nLeg1Status || STRATEGY_STATUS_WAIT_SK == m_nLeg1Status))
					{
						m_iLeg1OpenPosAcc += vol;
						m_dbLeg1PriceSum += vol * price;
						m_strLeg1OpenTime = CFunctionPub::GetFullDateString(time);
						if (m_iLeg1OpenPosAcc == m_iOpenNum )
						{
							if (POSTION_DIRECTION_BUY == direction)
							{
								m_nLeg1Status = STRATEGY_STATUS_BK;
							}
							else
							{
								m_nLeg1Status = STRATEGY_STATUS_SK;
							}
							
							double dbAvgPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
							OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iOpenNum, dbAvgPrice, m_strLeg1OpenTime);

							
							//开第二腿
							if (0 == m_iOpenDir)
							{
								m_iLeg2Ref = OpenSale(m_oLeg2Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg2Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
								memset(szMsg, 0, 256);
								sprintf(szMsg, "卖开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg2Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg2Tick.bidprice[0]);
								AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							}
							else
							{

								m_iLeg2Ref = OpenBuy(m_oLeg2Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg2Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
								memset(szMsg, 0, 256);
								sprintf(szMsg, "买开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg2Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg2Tick.askprice[0]);
								AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							}
						}
					}
					else if (/*m_iLeg2Ref == orderref && */0 == m_strCode2.CompareNoCase(ins))//第二手可能是撤单后重新下单的
					{
						if ((m_nLeg2Status == STRATEGY_STATUS_WAIT_BK && '0' == direction) || (m_nLeg2Status == STRATEGY_STATUS_WAIT_SK && '1' == direction))
						{
							if (vol + m_iLeg2OpenPosAcc <= m_iLeg1OpenPosAcc)
							{
								m_iLeg2OpenPosAcc += vol;

								m_strLeg2OpenTime = time;
								m_dbLeg2PriceSum += vol * price;
								if (m_iLeg2OpenPosAcc == m_iLeg1OpenPosAcc)
								{
									if (POSTION_DIRECTION_BUY == direction)
									{
										m_nLeg2Status = STRATEGY_STATUS_BK;
									}
									else
									{
										m_nLeg2Status = STRATEGY_STATUS_SK;
									}
									m_iTodayDayOpenNum++;
									m_strLeg2OpenTime = CFunctionPub::GetFullDateString(time);
									double dbAvgPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
									OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg2OpenPosAcc, dbAvgPrice, m_strLeg2OpenTime);
								}
							}
						}

					
					}
				}
				break;
			case 1://2nd leg first
				{
					if (m_iLeg2Ref == orderref && 0 == m_strCode2.CompareNoCase(ins) && (STRATEGY_STATUS_WAIT_BK == m_nLeg2Status || STRATEGY_STATUS_WAIT_SK == m_nLeg2Status))
					{
						m_iLeg2OpenPosAcc += vol;
						m_dbLeg2PriceSum += vol * price;
						m_strLeg2OpenTime = CFunctionPub::GetFullDateString(time);
						if (m_iLeg2OpenPosAcc == m_iOpenNum)
						{
							
							if (POSTION_DIRECTION_BUY == direction)
							{
								m_nLeg2Status = STRATEGY_STATUS_BK;
							}
							else
							{
								m_nLeg2Status = STRATEGY_STATUS_SK;
							}
							double dbAvgPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
							OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg2OpenPosAcc, dbAvgPrice, m_strLeg2OpenTime);

							//开第一腿
							if (0 == m_iOpenDir)
							{
								m_nLeg1Status = STRATEGY_STATUS_WAIT_BK;
								m_iLeg1Ref = OpenBuy(m_oLeg1Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg1Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
								memset(szMsg, 0, 256);
								sprintf(szMsg, "卖开第一腿:%s, %d手, 对价:%.2f\n", m_oLeg1Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg1Tick.askprice[0]);
								AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							}
							else
							{
								m_nLeg1Status = STRATEGY_STATUS_WAIT_SK;
								m_iLeg1Ref = OpenSale(m_oLeg1Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg1Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
								memset(szMsg, 0, 256);
								sprintf(szMsg, "买开第一腿:%s, %d手, 对价:%.2f\n", m_oLeg1Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg1Tick.bidprice[0]);
								AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							}

						}
					}
					else if (/*m_iLeg1Ref == orderref && */0 == m_strCode1.CompareNoCase(ins))//第二手可能是撤单后重新下单的
					{
						if ((m_nLeg1Status == STRATEGY_STATUS_WAIT_BK && '0' == direction) || (m_nLeg1Status == STRATEGY_STATUS_WAIT_SK && '1' == direction))
						{
							if (vol + m_iLeg1OpenPosAcc <= m_iLeg2OpenPosAcc)
							{
								m_iLeg1OpenPosAcc += vol;

								m_dbLeg1PriceSum += vol * price;
								if (m_iLeg1OpenPosAcc == m_iLeg2OpenPosAcc)
								{
									m_strLeg1OpenTime = CFunctionPub::GetFullDateString(time);
									if (POSTION_DIRECTION_BUY == direction)
									{
										m_nLeg1Status = STRATEGY_STATUS_BK;
									}
									else
									{
										m_nLeg1Status = STRATEGY_STATUS_SK;
									}
									m_iTodayDayOpenNum++;
									double dbAvgPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
									OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg1OpenPosAcc, dbAvgPrice, m_strLeg1OpenTime);

								}
							}
						}
						
					}
					
					
				}
				break;
			case 2://at the same time
				if (m_iLeg1Ref == orderref && 0 == m_strCode1.CompareNoCase(ins) && (STRATEGY_STATUS_WAIT_BK == m_nLeg1Status || STRATEGY_STATUS_WAIT_SK == m_nLeg1Status))
				{
					m_iLeg1OpenPosAcc += vol;
					m_dbLeg1PriceSum += vol * price;
					if (m_iLeg1OpenPosAcc == m_iOpenNum)
					{
						m_strLeg1OpenTime = CFunctionPub::GetFullDateString(time);
						if (POSTION_DIRECTION_BUY == direction)
						{
							m_nLeg1Status = STRATEGY_STATUS_BK;
						}
						else
						{
							m_nLeg1Status = STRATEGY_STATUS_SK;
						}
						double dbAvgPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg1OpenPosAcc, dbAvgPrice, m_strLeg1OpenTime);
					}
					
				}
				else if (m_iLeg2Ref == orderref && 0 == m_strCode2.CompareNoCase(ins) && (STRATEGY_STATUS_WAIT_BK == m_nLeg2Status || STRATEGY_STATUS_WAIT_SK == m_nLeg2Status))
				{
					
					m_iLeg2OpenPosAcc += vol;
					m_dbLeg2PriceSum += vol * price;
					if (m_iLeg2OpenPosAcc == m_iOpenNum)
					{
						m_strLeg2OpenTime = CFunctionPub::GetFullDateString(time);
						if (POSTION_DIRECTION_BUY == direction)
						{
							m_nLeg2Status = STRATEGY_STATUS_BK;
						}
						else
						{
							m_nLeg2Status = STRATEGY_STATUS_SK;
						}
						double dbAvgPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, direction == '0' ? 0 : 1, m_iLeg2OpenPosAcc, dbAvgPrice, m_strLeg2OpenTime);
					}
				}

				if ((m_nLeg1Status == STRATEGY_STATUS_BK || m_nLeg1Status == STRATEGY_STATUS_SK ) && ( m_nLeg2Status == STRATEGY_STATUS_BK || m_nLeg2Status == STRATEGY_STATUS_SK))
				{
					m_iTodayDayOpenNum++;
				}
				break;
			default:
				break;
			}
		}
		else if (HS_THOST_FTDC_OF_Close == offsetflag ||
			HS_THOST_FTDC_OF_ForceClose == offsetflag ||
			HS_THOST_FTDC_OF_CloseToday == offsetflag ||
			HS_THOST_FTDC_OF_CloseYesterday == offsetflag ||
			HS_THOST_FTDC_OF_ForceOff == offsetflag ||
			HS_THOST_FTDC_OF_LocalForceClose == offsetflag)
		{
			if (0 == m_strCode1.CompareNoCase(ins) && 
				((m_iOpenDir == 1 && m_nLeg1Status == STRATEGY_STATUS_WAIT_BP) || ( m_iOpenDir == 0 && m_nLeg1Status == STRATEGY_STATUS_WAIT_SP)))
			{
				m_iLeg1ClosePosAcc += vol;
				if (m_iLeg1ClosePosAcc == m_iLeg1OpenPosAcc)
				{

					m_nLeg1Status = STRATEGY_STATUS_INIT;
					OnNotifyPostion(ins, 1/*close*/, direction == '0' ? 0 : 1, m_iLeg1ClosePosAcc, 0, time);

					m_iLeg1OpenPosAcc = 0;

					m_dbLeg1PriceSum = 0;
					m_iLeg1ClosePosAcc = 0;
					if (m_nLeg1Status == STRATEGY_STATUS_INIT && m_nLeg2Status == STRATEGY_STATUS_INIT)
					{
						memset(szMsg, 0, 256);
						sprintf(szMsg, "已开平仓%d次\n", m_iTodayDayOpenNum);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}
				}

			}
			else if (0 == m_strCode2.CompareNoCase(ins)&& 
				((m_iOpenDir == 0 && m_nLeg2Status == STRATEGY_STATUS_WAIT_BP) || ( m_iOpenDir == 1 && m_nLeg2Status == STRATEGY_STATUS_WAIT_SP)))
			{
				m_iLeg2ClosePosAcc += vol;
				if (m_iLeg2ClosePosAcc == m_iLeg2OpenPosAcc)
				{
					m_nLeg2Status = STRATEGY_STATUS_INIT;
					OnNotifyPostion(ins, 1/*close*/, direction == '0' ? 0 : 1, m_iLeg2ClosePosAcc, 0, time);
					m_iLeg2OpenPosAcc = 0;
					m_iLeg2ClosePosAcc = 0;
					m_dbLeg2PriceSum = 0;

					if (m_nLeg1Status == STRATEGY_STATUS_INIT && m_nLeg2Status == STRATEGY_STATUS_INIT)
					{
						memset(szMsg, 0, 256);
						sprintf(szMsg, "已开平仓%d次\n", m_iTodayDayOpenNum);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}


				}

			}


	

			
		}


	}
}

void CStrategyArbitrageImpl::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{
	char szMsg[256];
	if (m_arCodeSubs.end() == m_arCodeSubs.find(ins))
	{
		return;
	}
	if (m_bIsStdArbitrage)//标准价差合约
	{
		if (HS_ORDER_CANCELED == orderstatus)
		{
			if (m_iLastRef == orderref)
			{
				if ((STRATEGY_STATUS_WAIT_SK == m_nStrategyStatus || STRATEGY_STATUS_WAIT_BK == m_nStrategyStatus) 
					&& m_iLeg1OpenPosAcc == m_iLeg2OpenPosAcc && 0 == m_iLeg1OpenPosAcc)
				{
					m_nStrategyStatus = STRATEGY_STATUS_INIT;
				}
			}
			
		}
	}
	else
	{
		if(HS_ORDER_HASREPORT == orderstatus)
		{
			switch (m_iFirstOpenFlag)
			{
			case 0:
				{
					if (0 == m_strCode2.Compare(ins))
					{
						m_iLeg2Ref = orderref;
					}
				}
				break;
			case 1:
				{
					if (0 == m_strCode1.Compare(ins))
					{
						m_iLeg1Ref = orderref;
					}
				}
				break;
			case 2:
				{

				}
				break;
			}
		}
		else if (HS_ORDER_CANCELED == orderstatus)
		{
			switch (m_iFirstOpenFlag)
			{
			case 0:
				if (m_iLeg1Ref == orderref && kp == '0')
				{
					

				
					if (m_iLeg1OpenPosAcc > 0)
					{
						if (0 == m_iOpenDir)
						{
							m_nLeg1Status = STRATEGY_STATUS_BK;
						}
						else
						{
							m_nLeg1Status = STRATEGY_STATUS_SK;
						}
						double dbAvgPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, bs == '0' ? 0 : 1, m_iLeg1OpenPosAcc, dbAvgPrice, m_strLeg1OpenTime);

						//开第二腿
						if (0 == m_iOpenDir)
						{
							m_nLeg2Status = STRATEGY_STATUS_WAIT_SK;
							m_iLeg2Ref = OpenSale(m_oLeg2Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg2Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "卖开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg2Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg2Tick.bidprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						}
						else
						{

							m_nLeg2Status = STRATEGY_STATUS_WAIT_BK;
							m_iLeg2Ref = OpenBuy(m_oLeg2Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg2Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "买开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg2Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg2Tick.askprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						}
					}
					else
					{
						m_nLeg1Status = STRATEGY_STATUS_INIT;
					}

				}
				break;
			case 1:
				if (m_iLeg2Ref == orderref && kp == '0')
				{
					
					if (m_iLeg2OpenPosAcc > 0)
					{
						if (0 == m_iOpenDir)
						{
							m_nLeg2Status = STRATEGY_STATUS_SK;
						}
						else
						{
							m_nLeg2Status = STRATEGY_STATUS_BK;
						}
						double dbAvgPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
						OnNotifyPostion(ins, 0/*open*/, bs == '0' ? 0 : 1, m_iLeg2OpenPosAcc, dbAvgPrice, m_strLeg2OpenTime);


						//开第一腿
						if (0 == m_iOpenDir)
						{
							m_nLeg1Status = STRATEGY_STATUS_WAIT_BK;
							m_iLeg1Ref = OpenBuy(m_oLeg1Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg1Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "买开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg1Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg1Tick.askprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							
						}
						else
						{
							m_nLeg1Status = STRATEGY_STATUS_WAIT_SK;
							m_iLeg1Ref = OpenSale(m_oLeg1Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg1Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "卖开第二腿:%s, %d手, 对价:%.2f\n", m_oLeg1Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg1Tick.bidprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						}
					}
					else
					{
						m_nLeg2Status = STRATEGY_STATUS_INIT;
					}

				}
				
				break;
			case 2:
				break;
			}

		}
		
	}


}

void CStrategyArbitrageImpl::OnErrorRtn(int code,char * errormsg)
{
	
	TRACE(errormsg);
}
void CStrategyArbitrageImpl::OnSubcode()
{
	
	//m_iOpenPosAcc = 0;//每天开盘前重置


	char sMsg[256];

	if (m_bIsStdArbitrage)
	{

		sprintf(sMsg, "subcode#%s;FUTURES;CTP;#", m_strStdCode);
		::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sMsg);

		sprintf(sMsg, "已订阅 %s 标准备价差行情", m_strStdCode);
		m_arCodeSubs[string(m_strStdCode)] = 0;

		memset(sMsg, 0, 256);
		sprintf(sMsg, "subcode#%s$%s;FUTURES;CTP;#", m_strCode1, m_strCode2);
		::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sMsg);

		memset(sMsg, 0, 256);
		sprintf(sMsg, "已订阅 %s, %s 行情", m_strCode1, m_strCode2);
		AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);

		m_arCodeSubs[string(m_strCode1)] = 0;
		m_arCodeSubs[string(m_strCode2)] = 0;

	}
	else
	{
		sprintf(sMsg, "subcode#%s$%s;FUTURES;CTP;#", m_strCode1, m_strCode2);
		::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sMsg);

		
		memset(sMsg, 0, 256);
		sprintf(sMsg, "已订阅 %s, %s 行情", m_strCode1, m_strCode2);
		AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);

		m_arCodeSubs[string(m_strCode1)] = 0;
		m_arCodeSubs[string(m_strCode2)] = 0;
	}	


}

bool CStrategyArbitrageImpl::IsRightOpenPostionTime(const char* szTime)
{
	char szMsg[256];
	memset(szMsg, 0, 256);
	if (STRATEGY_STATUS_INIT == m_nStrategyStatus)//收盘前n分钟不开仓
	{
		char szHour[8];
		char szMin[8];
		char szSec[8];
		memset(szHour, 0, 8);
		memset(szMin, 0, 8);
		memset(szSec, 0, 8);

		CTime tmNow = CTime::GetCurrentTime();
		char szCloseTime1[32], szCloseTime2[32], sz21Clock[32];
		sprintf(szCloseTime1, "%s %s", tmNow.Format("%Y-%m-%d"), m_szCloseTime1);
		CTime tmClose1, tmClose2, tm21Clock;
		CFunctionPub::String2Time(szCloseTime1, tmClose1);
		sprintf(szCloseTime2, "%s %s", tmNow.Format("%Y-%m-%d"), m_szCloseTime2);
		CFunctionPub::String2Time(szCloseTime2, tmClose2);
		sprintf(sz21Clock, "%s %s", tmNow.Format("%Y-%m-%d"), "21::00::00");
		CFunctionPub::String2Time(sz21Clock, tm21Clock);

		CTime tmTick;
		char szTickTime[32];
		if (6 == strlen(szTime))
		{
			strncpy(szHour, szTime, 2);
			strncpy(szMin, szTime + 2, 2);
			strncpy(szSec, szTime + 4, 2);
			sprintf(szTickTime, "%s %s:%s:%s", tmNow.Format("%Y-%m-%d"), szHour, szMin, szSec);
		}
		else if (5 == strlen(szTime))
		{
			strncpy(szHour, szTime, 1);
			strncpy(szMin, szTime + 1, 2);
			strncpy(szSec, szTime + 3, 2);
			sprintf(szTickTime, "0%s %s:%s:%s", tmNow.Format("%Y-%m-%d"), szHour, szMin, szSec);
		}

		CFunctionPub::String2Time(szTickTime, tmTick);


		CTime tmClose;//取两腿先收盘的时间
		if (tmClose1 >= tm21Clock && tmClose2 > tm21Clock)//两腿都在0点以前
		{
			if (tmClose1 >= tmClose2) //第二腿先收盘
			{
				tmClose = tmClose2;
			}
			else
			{
				tmClose = tmClose1;
			}
		}
		else if (tmClose1 > tm21Clock && tm21Clock > tmClose2 )//一腿在0点以前，一腿在0点以后, 且第一腿先收盘
		{
			tmClose = tmClose1;
		}
		else if (tmClose2 > tm21Clock && tm21Clock > tmClose1)//一腿在0点以前，一腿在0点以后, 且第二腿先收盘
		{
			tmClose = tmClose2;
		}
		else if (tm21Clock > tmClose1 && tm21Clock > tmClose2)
		{
			if (tmClose1 >= tmClose2)
			{
				tmClose = tmClose2;
			}
			else
			{
				tmClose = tmClose1;
			}

		}

		int iCloseHour = tmClose.GetHour();
		int iCloseMin = tmClose.GetMinute();
		CTimeSpan spanTick;
		if (tmClose > tmTick)
		{
			spanTick = tmClose - tmTick;
		}
		else
		{
			spanTick = tmTick - tmClose;
		}

		if (spanTick.GetTotalMinutes() <= m_iMinutesPreStop)
		{
			sprintf(szMsg, "收盘前%d分钟结束", m_iMinutesPreStop);
			TRACE(szMsg);
			return false;
		}

	}

}

bool CStrategyArbitrageImpl::IsRightTickTime(const char* szTime)
{

	if (szTime)//92900
	{
		char szHour[8];
		memset(szHour, 0, 8);
		int iHour = 0;

		if (5 == strlen(szTime))
		{

			strncpy(szHour, szTime, 1);
			iHour = atoi(szHour);
			if (iHour< 9 && iHour > 2)
			{
				return false;
			}
		}
		if (6 == strlen(szTime))//205900
		{
			strncpy(szHour, szTime, 2);
			iHour = atoi(szHour);

			char szMin[8];
			memset(szMin, 0, 8);
			strncpy(szMin, szTime + 2, 2);
			int iMin = atoi(szMin);
			if (iHour == 11 && iMin >= (30 - HS_STOP_TRADE_MINUTE))//收盘前5分钟
			{
				return false;
			}
			if (iHour == 14 && iMin >= 60 - HS_STOP_TRADE_MINUTE)
			{
				return false;
			}
			if (iHour > 15 && iHour < 21)
			{
				return false;
			}

			
		}
		return true;

	}
	return false;
}

void CStrategyArbitrageImpl::ReadCloseTimeCfg()
{
	//to be continued...
	strcpy(m_szCloseTime1, "23:00:00");
	strcpy(m_szCloseTime1, "01:00:00");
}

double CStrategyArbitrageImpl::CalcDivPrice(CTickData* pt)
{
	double dbDiv = 99999;
	if (m_bIsStdArbitrage && 0 == m_strStdCode.Compare(pt->szInstrument))
	{
		dbDiv = (pt->askprice[0] + pt->bidprice[0]) / 2.0;
	}
	else
	{
		if (abs(m_oLeg1Tick.lastprice ) < 99999 && abs(m_oLeg1Tick.lastprice ) > 0 && abs(m_oLeg2Tick.lastprice) < 99999 && abs(m_oLeg2Tick.lastprice) > 0)
		{
			dbDiv = m_oLeg1Tick.lastprice - m_oLeg2Tick.lastprice;
		}
	}
	return dbDiv;
}
bool CStrategyArbitrageImpl::ConditionJudge(double dbDiv, int bOpenOrClose)
{
	bool bCond = false;
	int iCondType;
	double dbDestDivPrice = 99999.0;
	if (0 == bOpenOrClose)
	{
		iCondType = GetConditionType(m_sOpenCondtion);
		dbDestDivPrice = m_dbOpenPriceDiv;

	}
	else if (1 == bOpenOrClose)//止盈
	{
		iCondType = GetConditionType(m_sCloseCondtion1);
		if (-1 == iCondType)
		{
			return false;
		}
		dbDestDivPrice = m_dbClosePriceDiv1;
	}
	else if (2 == bOpenOrClose)//止损
	{
		iCondType = GetConditionType(m_sCloseCondtion2);
		if (-1 == iCondType)
		{
			return false;
		}
		dbDestDivPrice = m_dbClosePriceDiv2;
	}

	if (dbDestDivPrice != 99999.0)
	{
		switch (iCondType)
		{
		case 0://>
			if (dbDiv > dbDestDivPrice)
			{
				bCond = true;
			}
			break;
		case 1://>=
			if (dbDiv >= dbDestDivPrice)
			{
				bCond = true;
			}
			break;
		case 2://<
			if (dbDiv < dbDestDivPrice)
			{
				bCond = true;
			}
			break;
		case 3://<=		
			if (dbDiv <= dbDestDivPrice)
			{
				bCond = true;
			}
			break;
		default:
			if (dbDiv > dbDestDivPrice)
			{
				bCond = true;
			}
			break;
		}
	}


	return bCond;
}
bool CStrategyArbitrageImpl::CanOpenPostion(CTickData* pt, double &dbDiv)
{
	dbDiv = CalcDivPrice(pt);
	if (!m_bEnabled || 99999 == dbDiv)
	{
		return false;
	}
	bool bOpen = false;
	
	if (dbDiv >= 99999.0)
	{
		return false;
	}
	if (m_bIsStdArbitrage)
	{

		if (ConditionJudge(dbDiv, 0))
		{
			bOpen = true;
		}
	}
	else
	{

		switch (m_iFirstOpenFlag)
		{
		case 0://先成交第一腿
			if (0 == m_strCode2.Compare(pt->szInstrument))//由慢行情驱动
			{
				if (ConditionJudge(dbDiv, 0))
				{
					bOpen = true;
				}

			}
			break;
		case 1://先成交第二腿
			if (0 == m_strCode1.Compare(pt->szInstrument))//由慢行情驱动
			{
				if (ConditionJudge(dbDiv, 0))
				{
					bOpen = true;
				}
			}
			break;
		case 2://两腿同时开仓
			if (ConditionJudge(dbDiv, 0))
			{
				bOpen = true;
			}
			break;
		default:
			if (ConditionJudge(dbDiv, 0))
			{
				bOpen = true;
			}
			break;
		}
	}


	return bOpen;
}
bool CStrategyArbitrageImpl::CanClosePostion(CTickData* pt, double &dbDiv)
{
	dbDiv = CalcDivPrice(pt);
	if (!m_bEnabled || 99999 == dbDiv)
	{
		return false;
	}
	bool bClose = false;
	

	if (m_bIsStdArbitrage)
	{
		if (ConditionJudge(dbDiv, 1))
		{
			bClose = true;
		}
		else if (!m_sCloseCondtion2.IsEmpty())//是否设置止损条件
		{
			if (ConditionJudge(dbDiv, 2))
			{
				bClose = true;
			}
		}

	}
	else
	{
		switch (m_iFirstOpenFlag)
		{
		case 0://先成交第一腿
			{

				if (0 == m_strCode2.Compare(pt->szInstrument))//由慢行情驱动
				{

					if (ConditionJudge(dbDiv, 1))
					{
						bClose = true;
					}
					else if (!m_sCloseCondtion2.IsEmpty())//是否设置止损条件
					{
						if (ConditionJudge(dbDiv, 2))
						{
							bClose = true;
							break;
						}
					}

				}
			}

			break;
		case 1://先成交第二腿
			if (0 == m_strCode1.Compare(pt->szInstrument))//由慢行情驱动
			{
				if (ConditionJudge(dbDiv, 1))
				{
					bClose = true;
				}
				else if (!m_sCloseCondtion2.IsEmpty())
				{
					if (ConditionJudge(dbDiv, 2))
					{
						bClose = true;
						break;
					}
				}
			}
			break;
		case 2://两腿同时开仓
			if (ConditionJudge(dbDiv, 1))
			{
				bClose = true;
			}
			else if (!m_sCloseCondtion2.IsEmpty())
			{
				if (ConditionJudge(dbDiv, 2))
				{
					bClose = true;
					break;
				}
			}
			break;
		default:
			if (ConditionJudge(dbDiv, 1))
			{
				bClose = true;
			}
			else if (!m_sCloseCondtion2.IsEmpty())
			{
				if (ConditionJudge(dbDiv, 2))
				{
					bClose = true;
					break;
				}
			}
			break;
		}
	}
	return bClose;
}
void CStrategyArbitrageImpl::OnStdArbitrageTick(CTickData* pt)
{
	double dbDiv;
	char szMsg[256];

	memset(szMsg, 0, 256);
	int iVolMul = GetContractVolMul(pt->szInstrument);
	dbDiv = CalcDivPrice(pt);
	if (99999 == dbDiv || 0 == iVolMul)
	{
		return;
	}

	if (0 != m_strStdCode.Compare(pt->szInstrument))
	{
		return;
	}
	
	if (abs(m_oLeg1Tick.lastprice ) < 99999 && abs(m_oLeg1Tick.lastprice ) > 0 && abs(m_oLeg2Tick.lastprice) < 99999 && abs(m_oLeg2Tick.lastprice) > 0)
	{
		sprintf(szMsg, "1,%s,%.2f,%.2f,%.2f,%d", pt->szTime, dbDiv, m_oLeg1Tick.lastprice, m_oLeg2Tick.lastprice, iVolMul);
	}
	else
	{
		sprintf(szMsg, "0,%s,%.2f, %d", pt->szTime,dbDiv, iVolMul);
	}
	//sprintf(szMsg, "%s %s, 合约名:%s, 当前价差:%.2f\n, ", pt->szDate, pt->szTime, pt->szInstrument,dbDiv);
	AddLog(szMsg, TRADE_MSG_TYPE_ARBITRAGE_HQ);

	
	if (STRATEGY_STATUS_INIT == m_nStrategyStatus)
	{

		if (CanOpenPostion(pt, dbDiv))
		{
			if (m_iTodayDayOpenNum >= m_iMaxDayOpenNum)
			{
				memset(szMsg, 0, 256);
				sprintf(szMsg, "当天开仓次数超过上限%d\n", m_iMaxDayOpenNum);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				return;
			}
			
			if (0 == m_iOpenDir)//做多
			{
				
				m_nStrategyStatus = STRATEGY_STATUS_WAIT_BK;
				m_iLastRef = OpenBuy(pt->szInstrument, m_iOpenNum, pt->askprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);

			}
			else
			{
				
				m_nStrategyStatus = STRATEGY_STATUS_WAIT_SK;
				m_iLastRef = OpenSale(pt->szInstrument, m_iOpenNum, pt->bidprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);

			}
		}

	}
	else if (CanClosePostion(pt, dbDiv) && 
		(m_nLeg1Status == STRATEGY_STATUS_BK || m_nLeg1Status == STRATEGY_STATUS_SK) && 
		(m_nLeg2Status == STRATEGY_STATUS_BK || m_nLeg2Status == STRATEGY_STATUS_SK))
	{

		if (0 == m_iOpenDir)
		{
			if (m_iLeg1OpenPosAcc > 0)
			{
				char cFlag = GetFutuCloseFlag(m_strCode1, m_strLeg1OpenTime);
				m_nLeg1Status = STRATEGY_STATUS_WAIT_SP;
				m_iLeg1Ref = CloseSale(m_oLeg1Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg1Tick.bidprice[0], m_fundAccount,  cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);

				memset(szMsg, 0, 256);
				sprintf(szMsg, "当前价差%.2f, 卖平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg1Tick.bidprice[0]);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
			}

			if (m_iLeg2OpenPosAcc > 0)
			{
				char cFlag = GetFutuCloseFlag(m_strCode2, m_strLeg2OpenTime);
				m_nLeg2Status = STRATEGY_STATUS_WAIT_BP;
				m_iLeg2Ref = CloseBuy(m_oLeg2Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg2Tick.askprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);

				memset(szMsg, 0, 256);
				sprintf(szMsg, "当前价差%.2f, 买平第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg2Tick.askprice[0]);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
			}


		}
		else
		{
			if (m_iLeg1OpenPosAcc > 0)
			{
				char cFlag = GetFutuCloseFlag(m_strCode1, m_strLeg1OpenTime);
				m_nLeg1Status = STRATEGY_STATUS_WAIT_BP;
				m_iLeg1Ref = CloseBuy(m_oLeg1Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg1Tick.askprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);

				memset(szMsg, 0, 256);
				sprintf(szMsg, "当前价差%.2f, 买平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg1Tick.askprice[0]);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
			}

			if (m_iLeg2OpenPosAcc > 0)
			{
				char cFlag = GetFutuCloseFlag(m_strCode2, m_strLeg2OpenTime);
				m_nLeg2Status = STRATEGY_STATUS_WAIT_SP;
				m_iLeg2Ref = CloseSale(m_oLeg2Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg2Tick.bidprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);

				memset(szMsg, 0, 256);
				sprintf(szMsg, "当前价差%.2f, 卖平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg2Tick.bidprice[0]);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
			}


		}
	}



#if 0
	//标准价差合约的平仓由单腿平仓替代
	else if(STRATEGY_STATUS_SK == m_nStrategyStatus)
	{
		int iPostion = m_iOpenPosAcc;
		if (CanClosePostion(pt, dbDiv))
		{
			m_nStrategyStatus = STRATEGY_STATUS_WAIT_BP;
			CloseBuy(pt->szInstrument, iPostion, pt->askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
		}
	}
	else if (STRATEGY_STATUS_BK == m_nStrategyStatus)
	{
		int iPostion = m_iOpenPosAcc;
		if (CanClosePostion(pt, dbDiv))
		{
			m_nStrategyStatus = STRATEGY_STATUS_WAIT_SP;
			CloseSale(pt->szInstrument, iPostion, pt->bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
		}
	}

#endif

	

}

void  CStrategyArbitrageImpl::Tick(CTickData * pt)
{
	char szMsg[256];
	double dbDiv;
	if (m_arCodeSubs.end() == m_arCodeSubs.find(pt->szInstrument))
	{
		return;
	}


	if (m_iOpenNum - m_iPostionNum > 0)//仍有开仓余量
	{

	}

	if (!IsRightTickTime(pt->szTime))
	{
		return;
	}

	if (0 == m_strCode1.Compare(pt->szInstrument))
	{
		m_oLeg1Tick = *pt;
	}
	else if (0 == m_strCode2.Compare(pt->szInstrument))
	{
		m_oLeg2Tick = *pt;
	}

	if (m_bIsStdArbitrage)
	{
		OnStdArbitrageTick(pt);
	}
	else//两腿合约
	{

		if (CanOpenPostion(pt, dbDiv) &&
			m_nLeg1Status == STRATEGY_STATUS_INIT  && 
			m_nLeg2Status == STRATEGY_STATUS_INIT )
		{
			if (m_iTodayDayOpenNum >= m_iMaxDayOpenNum)
			{
				memset(szMsg, 0, 256);
				sprintf(szMsg, "当天开仓次数超过上限%d\n", m_iMaxDayOpenNum);
				AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				return;
			}
			switch (m_iFirstOpenFlag)
			{
			case 0://先成交第一腿
				{
					if (STRATEGY_STATUS_INIT == m_nLeg1Status)
					{
						if (0 == m_iOpenDir)
						{
							m_nLeg1Status = STRATEGY_STATUS_WAIT_BK;
							m_iLeg1Ref = OpenBuy(m_oLeg1Tick.szInstrument, m_iOpenNum, m_oLeg1Tick.askprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "当前价差%.2f, 买开第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iOpenNum, m_oLeg1Tick.askprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							
						}
						else
						{
							m_nLeg1Status = STRATEGY_STATUS_WAIT_SK;
							m_iLeg1Ref = OpenSale(m_oLeg1Tick.szInstrument, m_iOpenNum, m_oLeg1Tick.bidprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);
							memset(szMsg, 0, 256);
							sprintf(szMsg, "当前价差%.2f, 卖开第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iOpenNum, m_oLeg1Tick.bidprice[0]);
							AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
							
						}
					}

				}
				break;
			case 1://先成交第二腿
				if (STRATEGY_STATUS_INIT == m_nLeg2Status)
				{
					if (0 == m_iOpenDir)
					{
						m_nLeg2Status = STRATEGY_STATUS_WAIT_BK;
						m_iLeg2Ref = OpenSale(m_oLeg2Tick.szInstrument, m_iOpenNum, m_oLeg2Tick.bidprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 买开第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iOpenNum, m_oLeg2Tick.bidprice[0]);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						
					}
					else
					{
						m_nLeg2Status = STRATEGY_STATUS_WAIT_SK;
						m_iLeg2Ref = OpenBuy(m_oLeg2Tick.szInstrument, m_iOpenNum, m_oLeg2Tick.askprice[0], m_fundAccount, ORDER_TYPE_TIMEOUT_ABORT);
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 卖开第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iOpenNum, m_oLeg2Tick.askprice[0]);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						
					}
				}

				break;
			case 2://两腿同时开仓
				if (STRATEGY_STATUS_INIT == m_nLeg1Status && STRATEGY_STATUS_INIT == m_nLeg2Status)
				{
					if (0 == m_iOpenDir)
					{
						m_nLeg1Status = STRATEGY_STATUS_WAIT_BK;
						m_iLeg1Ref = OpenBuy(m_oLeg1Tick.szInstrument, m_iOpenNum, m_oLeg1Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
						
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 买开第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iOpenNum, m_oLeg2Tick.askprice[0]);

						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
						m_nLeg2Status = STRATEGY_STATUS_WAIT_SK;
						m_iLeg2Ref = OpenSale(m_oLeg2Tick.szInstrument, m_iOpenNum, m_oLeg2Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
						
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 卖开第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iOpenNum, m_oLeg2Tick.bidprice[0]);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}
					else
					{
						m_nLeg1Status = STRATEGY_STATUS_WAIT_SK;
						m_iLeg1Ref = OpenSale(m_oLeg1Tick.szInstrument, m_iOpenNum, m_oLeg1Tick.bidprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
						
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 卖开第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iOpenNum, m_oLeg1Tick.bidprice[0]);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);

						m_nLeg2Status = STRATEGY_STATUS_WAIT_BK;
						m_iLeg2Ref = OpenBuy(m_oLeg2Tick.szInstrument, m_iOpenNum, m_oLeg2Tick.askprice[0], m_fundAccount, ORDER_TYPE_ZUIJIA_NEWPRICE);
						
						memset(szMsg, 0, 256);
						sprintf(szMsg, "当前价差%.2f, 买开第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iOpenNum, m_oLeg2Tick.askprice[0]);
						AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
					}
				}

				break;
			default:
				break;
			}
		}
		else if (CanClosePostion(pt, dbDiv) && 
			(m_nLeg1Status == STRATEGY_STATUS_BK || m_nLeg1Status == STRATEGY_STATUS_SK) && 
			(m_nLeg2Status == STRATEGY_STATUS_BK || m_nLeg2Status == STRATEGY_STATUS_SK))
		{
			
			if (0 == m_iOpenDir)
			{
				if (m_iLeg1OpenPosAcc > 0)
				{
					char cFlag = GetFutuCloseFlag(m_strCode1, m_strLeg1OpenTime);
					m_nLeg1Status = STRATEGY_STATUS_WAIT_SP;
					m_iLeg1Ref = CloseSale(m_oLeg1Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg1Tick.bidprice[0], m_fundAccount,  cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);
					
					memset(szMsg, 0, 256);
					sprintf(szMsg, "当前价差%.2f, 卖平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg1Tick.bidprice[0]);
					AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				}
				
				if (m_iLeg2OpenPosAcc > 0)
				{
					char cFlag = GetFutuCloseFlag(m_strCode2, m_strLeg2OpenTime);
					m_nLeg2Status = STRATEGY_STATUS_WAIT_BP;
					m_iLeg2Ref = CloseBuy(m_oLeg2Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg2Tick.askprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);
					
					memset(szMsg, 0, 256);
					sprintf(szMsg, "当前价差%.2f, 买平第二腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg2Tick.askprice[0]);
					AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				}

				
			}
			else
			{
				if (m_iLeg1OpenPosAcc > 0)
				{
					char cFlag = GetFutuCloseFlag(m_strCode1, m_strLeg1OpenTime);
					m_nLeg1Status = STRATEGY_STATUS_WAIT_BP;
					m_iLeg1Ref = CloseBuy(m_oLeg1Tick.szInstrument, m_iLeg1OpenPosAcc, m_oLeg1Tick.askprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);
					
					memset(szMsg, 0, 256);
					sprintf(szMsg, "当前价差%.2f, 买平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg1Tick.szInstrument,m_iLeg1OpenPosAcc, m_oLeg1Tick.askprice[0]);
					AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				}

				if (m_iLeg2OpenPosAcc > 0)
				{
					char cFlag = GetFutuCloseFlag(m_strCode2, m_strLeg2OpenTime);
					m_nLeg2Status = STRATEGY_STATUS_WAIT_SP;
					m_iLeg2Ref = CloseSale(m_oLeg2Tick.szInstrument, m_iLeg2OpenPosAcc, m_oLeg2Tick.bidprice[0], m_fundAccount, cFlag, ORDER_TYPE_ZUIJIA_NEWPRICE);
					
					memset(szMsg, 0, 256);
					sprintf(szMsg, "当前价差%.2f, 卖平第一腿:%s, %d手, 对价:%.2f\n", dbDiv, m_oLeg2Tick.szInstrument,m_iLeg2OpenPosAcc, m_oLeg2Tick.bidprice[0]);
					AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
				}

				
			}
		}
	}




	
	if (!m_bIsStdArbitrage)
	{
		if (m_oLeg1Tick.lastprice > 0 && m_oLeg1Tick.lastprice > 0)
		{
			memset(szMsg, 0, 256);
			int iVolMul = GetContractVolMul(pt->szInstrument);
			sprintf(szMsg, "1,%s,%.2f,%.2f,%.2f,%d", pt->szTime,dbDiv, m_oLeg1Tick.lastprice, m_oLeg2Tick.lastprice, iVolMul);
			/*sprintf(szMsg, "%s %s, 合约一:%s, 合约一价格:%.2f, 合约二:%s,合约二价格:%.2f, 当前价差:%.2f\n, ", pt->szDate, pt->szTime, 
			m_oLeg1Tick.szInstrument, m_oLeg1Tick.lastprice,
			m_oLeg2Tick.szInstrument, m_oLeg2Tick.lastprice, 
			dbDiv);*/
			AddLog(szMsg, TRADE_MSG_TYPE_ARBITRAGE_HQ);
		}

	}



}


void CStrategyArbitrageImpl::SetParam(CString paramname,CString value)
{

	//"合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号";
	if (paramname == "合约一")
	{
		m_strCode1 = value;
	}
	else if (paramname == "合约二")
	{
		m_strCode2 = value;
	}
	else if (paramname == "是否标准合约")
	{
		m_bIsStdArbitrage = (0 == atoi(value) ? false : true);
	}
	else if (paramname == "开仓条件")
	{
		m_sOpenCondtion = value;
	}
	else if (paramname == "价差门限")
	{
		m_dbOpenPriceDiv = atof(value);
	}
	else if (paramname == "开仓数量")
	{
		m_iOpenNum = atoi(value);
	}
	else if (paramname == "平仓条件一")
	{
		m_sCloseCondtion1 = value;
	}
	else if (paramname == "平仓价差一")
	{
		m_dbClosePriceDiv1 = atof(value);
	}
	else if (paramname == "平仓条件二")
	{
		m_sCloseCondtion2 = value;
	}
	else if (paramname == "平仓价差二")
	{
		m_dbClosePriceDiv2 = atof(value);
	}
	else if (paramname == "启用策略执行下单")
	{
		m_bStrategyOrder = (0 == atoi(value) ? false : true);
	}
	else if (paramname == "优先开仓合约")
	{
		m_iFirstOpenFlag = atoi(value);
	}
	else if (paramname == "每天开仓次数小于")
	{
		m_iMaxDayOpenNum = atoi(value);
	}
	else if(paramname=="下单账号")
	{
		m_fundAccount = value;
	}
	else if (paramname == "开仓方向")
	{
		m_iOpenDir = atoi(value);
	}
	else if (paramname == "使能")
	{
		m_bEnabled = (0 == atoi(value) ? false : true);
	}

}

void CStrategyArbitrageImpl::SetParam(CString value)
{
	CFunctionPub pub;
	CStringArray list;
	pub.ParseParam(value,list,';');
	//"合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号";
	if( list.GetSize() >= 14 )
	{
		m_params["合约一"] = list[0];
		m_strCode1 = list[0];
		if (!m_strCode1.IsEmpty())
		{
			m_arCodeSubs[string(m_strCode1)] = 0;
		}

		m_params["合约二"] = list[1];
		m_strCode2 = list[1];

		if (!m_strCode2.IsEmpty())
		{
			m_arCodeSubs[string(m_strCode2)] = 1;
		}


		m_params["是否标准合约"] = list[2];
		m_bIsStdArbitrage = (0 == atoi(list[2]) ? false : true);

		if (m_bIsStdArbitrage)
		{
			m_strStdCode = m_strCode1;
			m_arCodeSubs[(string)m_strStdCode] = 2;

			int nt = m_strStdCode.Find("&");
			if (nt > 0)
			{
				m_strCode2 = m_strStdCode.Right(m_strStdCode.GetLength() - nt - 1);
				int iLen = m_strCode2.GetLength();
				m_strCode1 = m_strStdCode.Mid(nt - iLen, iLen);

				//subscribe 2 legs' contract
				CEasyThread *pThread = new CEasyThread();
				if (pThread)
				{
					pThread->Start(Task_Subcode, 0, this);
				}
			}

		}

		m_params["开仓条件"] = list[3];
		m_sOpenCondtion = list[3];

		m_params["价差门限"] = list[4];
		m_dbOpenPriceDiv = atof(list[4]);

		m_params["开仓数量"] = list[5];
		m_iOpenNum = atoi(list[5]);

		m_params["平仓条件一"] = list[6];
		m_sCloseCondtion1 = list[6];
		m_params["平仓价差一"] = list[7];
		m_dbClosePriceDiv1 = atof(list[7]);
		m_params["平仓条件二"] = list[8];
		m_sCloseCondtion2 = list[8];
		m_params["平仓价差二"] = list[9];
		m_dbClosePriceDiv2 = atof(list[9]);

		m_params["启用策略执行下单"] = list[10];
		m_bStrategyOrder = (0 == atoi(list[10]) ? false : true);
		m_params["优先开仓合约"] = list[11];
		m_iFirstOpenFlag = atoi(list[11]);

		m_params["每天开仓次数小于"] = list[12];
		m_iMaxDayOpenNum = atoi(list[12]);

		m_params["下单账号"] = list[13];
		m_fundAccount = list[13];

		m_params["开仓方向"] = list[14];
		m_iOpenDir = atoi(list[14]);

	}
}

CString CStrategyArbitrageImpl::GetParamValue()
{
	CString str;


	CString strValue = m_pPolicy->GetParams("同时开仓数");

	int nPostionCount = atoi(strValue);
	//"合约一;合约二;是否标准合约;开仓条件;价差门限;开仓数量;平仓条件一;平仓价差一;平仓条件二;平仓价差二;启用策略执行下单;优先开仓合约;每天开仓次数小于;下单账号;开仓方向";
	double dbFund = m_pPolicy->GetCurrentFund();
	if(m_bIsStdArbitrage)
	{
		str.Format("%s;%s;%d;%s;%.2f;%d;%s;%.2f;%s;%.2f;%d;%d;%d;%s;%d",
			m_strStdCode,
			"",
			m_bIsStdArbitrage,
			m_sOpenCondtion,
			m_dbOpenPriceDiv,

			m_iOpenNum,
			m_sCloseCondtion1,
			m_dbClosePriceDiv1,
			m_sCloseCondtion2,
			m_dbClosePriceDiv2,

			m_bStrategyOrder,
			m_iFirstOpenFlag,
			m_iMaxDayOpenNum,
			m_fundAccount,
			m_iOpenDir
			);
	}
	else
	{
		str.Format("%s;%s;%d;%s;%.2f;%d;%s;%.2f;%s;%.2f;%d;%d;%d;%s;%d",
			m_strCode1,
			m_strCode2,
			m_bIsStdArbitrage,
			m_sOpenCondtion,
			m_dbOpenPriceDiv,

			m_iOpenNum,
			m_sCloseCondtion1,
			m_dbClosePriceDiv1,
			m_sCloseCondtion2,
			m_dbClosePriceDiv2,

			m_bStrategyOrder,
			m_iFirstOpenFlag,
			m_iMaxDayOpenNum,
			m_fundAccount,
			m_iOpenDir
			);
	}

	

	return str;
}
CString CStrategyArbitrageImpl::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

void CStrategyArbitrageImpl::CreateStatisticObject()
{
	m_pStatistic = new CStockTradeStatistic();
}

int CStrategyArbitrageImpl::GetConditionType(const char* szSymbol)
{
	for (int i = 0; i < sizeof(s_arCondtionType) / sizeof(St_Condition_Type); i++)
	{
		if (0 == strcmp(s_arCondtionType[i].m_szSymbol, szSymbol))
		{
			return s_arCondtionType[i].m_iType;
		}
	}

	return -1;
}




//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
void CStrategyArbitrageImpl::ResumePostionInfo(CString strPosInfo)
{

	CStringArray arInfo;
	if (strPosInfo.IsEmpty())
	{
		m_iLastRef = -1;

		m_iLeg1OpenPosAcc = 0;
		m_iLeg2OpenPosAcc = 0;
		m_iLeg1ClosePosAcc = 0;
		m_iLeg2ClosePosAcc = 0;

		m_iOpenPosAcc = 0;
		m_iClosePosAcc = 0;

		m_timeOut = 1;

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		m_nLeg1Status = STRATEGY_STATUS_INIT;
		m_nLeg2Status = STRATEGY_STATUS_INIT;

		m_dbLeg1PriceSum = 0.0;
		m_dbLeg2PriceSum = 0.0;

		m_iLeg1VolMul = 0;
		m_iLeg2VolMul = 0;
		m_iTodayDayOpenNum = 0;

		return;
	}
	CFunctionPub::ParseParam(strPosInfo, arInfo, '$');
	CString info; 

	char szMsg[256];
	memset(szMsg, 0, 256);

	if (arInfo.GetSize() != 2)
	{
		sprintf(szMsg, "错误的持仓信息");
		AddLog(szMsg, TRADE_MSG_TYPE_LOG_PRV);
		return;
	}
	CStringArray arPos;
	CFunctionPub::ParseParam(arInfo[0], arPos, ';');

	//first leg
	if (arPos.GetSize() >= 6)
	{
		m_strCode1 = arPos[0];
		
		int iPos = atoi(arPos[2]);
		m_iLeg1OpenPosAcc = iPos;

		double dbPrice = atof(arPos[3]);
		m_dbLeg1PriceSum = dbPrice * iPos;

		int iDir = atoi(arPos[4]);
		if (0 == iDir)
		{
			m_nLeg1Status = STRATEGY_STATUS_BK;
			m_nStrategyStatus = STRATEGY_STATUS_BK;
		}
		else
		{
			m_nLeg1Status = STRATEGY_STATUS_SK;
			m_nStrategyStatus = STRATEGY_STATUS_SK;
		}
		m_iTodayDayOpenNum = 1;
		CString strTime = arPos[5];

		m_strLeg1OpenTime = strTime;
		sprintf(szMsg,"模型恢复  合约 %s  数量 %d  成本价 %.4f  方向 %d  开仓时间 %s ",m_strCode1,iPos,dbPrice,iDir,strTime);
		AddLog(szMsg);
	}
	

	//second leg
	arPos.RemoveAll();
	CFunctionPub::ParseParam(arInfo[1], arPos, ';');
	if (arPos.GetSize() >= 6)
	{
		m_strCode2 = arPos[0];

		int iPos = atoi(arPos[2]);
		m_iLeg2OpenPosAcc = iPos;

		double dbPrice = atof(arPos[3]);
		m_dbLeg2PriceSum = dbPrice * iPos;

		int iDir = atoi(arPos[4]);
		if (0 == iDir)
		{
			m_nLeg2Status = STRATEGY_STATUS_BK;
		}
		else
		{
			m_nLeg2Status = STRATEGY_STATUS_SK;
		}
		CString strTime = arPos[5];

		m_strLeg2OpenTime = strTime;
		sprintf(szMsg,"模型恢复  合约 %s  数量 %d  成本价 %.4f  方向 %d  开仓时间 %s ",m_strCode1,iPos,dbPrice,iDir,strTime);
		AddLog(szMsg);

	}

	


}

//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
CString CStrategyArbitrageImpl::GetPostionInfo()
{
	CString strContext = "";


	//leg1
	if (m_iLeg1OpenPosAcc > 0)
	{
		CString strName = "";
		int iPostion = 0;
		double dbPrice = 0.0;
		int iDir = 0;
		CString strTime = "";
		char szBuf[64] = {0};
		strContext += m_strCode1;
		strContext += ";";

		if (m_pStrategyCodeInfo)
		{
			strName = GetFutuName(m_strCode1);

		}
		strContext += strName;
		strContext += ";";

		strContext += itoa(m_iLeg1OpenPosAcc, szBuf, 10);
		strContext += ";";

		dbPrice = m_dbLeg1PriceSum / (double)m_iLeg1OpenPosAcc;
		sprintf(szBuf, "%.4f", dbPrice);
		strContext += szBuf;
		strContext += ";";


		if (0 == m_iOpenDir)
		{
			iDir = 0;
		}
		else
		{
			iDir = 1;
		}

		strContext += itoa(iDir, szBuf, 10);
		strContext += ";";


		strContext += m_strLeg1OpenTime;

		strContext += "$";
	}

	//leg2
	if (m_iLeg2OpenPosAcc > 0)
	{
		CString strName = "";
		int iPostion = 0;
		double dbPrice = 0.0;
		int iDir = 0;
		CString strTime = "";
		char szBuf[64] = {0};
		strContext += m_strCode2;
		strContext += ";";

		if (m_pStrategyCodeInfo)
		{
			strName = GetFutuName(m_strCode2);
		}
		strContext += strName;
		strContext += ";";

		strContext += itoa(m_iLeg2OpenPosAcc, szBuf, 10);
		strContext += ";";

		dbPrice = m_dbLeg2PriceSum / (double)m_iLeg2OpenPosAcc;
		sprintf(szBuf, "%.4f", dbPrice);
		strContext += szBuf;
		strContext += ";";


		if (0 == m_iOpenDir)
		{
			iDir = 1;
		}
		else
		{
			iDir = 0;
		}

		strContext += itoa(iDir, szBuf, 10);
		strContext += ";";


		strContext += m_strLeg2OpenTime;

		strContext += "$";
	}


	

	return strContext;
}


void CStrategyArbitrageImpl::OnNotifyPostion(const char* szCode, int iOpenClose, int iBuySell, int iPosNum, double dbAvgPrice, const char* szOpenTime)
{
	char szMsg[256];
	memset(szMsg, 0, 256);
	sprintf(szMsg, ";%s;%d;%d;%d;%.2f;%s", szCode, iOpenClose, iBuySell, iPosNum, dbAvgPrice, szOpenTime);

	AddLog(szMsg, TRADE_MSG_TYPE_POSTION_INFO);
}

int CStrategyArbitrageImpl::GetContractVolMul(const char* szCode)
{
	int iVolMul = 0;
	std::map<std::string,FutuCodeInfo> *pMapFutuCodeInfo = &CPubData::m_sFutuCodeInfo;
	if (m_bIsStdArbitrage)
	{
		if (pMapFutuCodeInfo->find(szCode) != pMapFutuCodeInfo->end())
		{
			FutuCodeInfo oFutuInfo = pMapFutuCodeInfo->at(szCode);

			iVolMul = oFutuInfo.iVolumeMultiple;

		}
	}
	else
	{
		if (0 == m_strCode1.Compare(szCode))
		{
			if (m_iLeg1VolMul > 0)
			{
				iVolMul = m_iLeg1VolMul;
			}
			else
			{
				if (pMapFutuCodeInfo->find(szCode) != pMapFutuCodeInfo->end())
				{
					FutuCodeInfo oFutuInfo = pMapFutuCodeInfo->at(szCode);

					m_iLeg1VolMul = oFutuInfo.iVolumeMultiple;
					iVolMul = m_iLeg1VolMul;
				}
			}
		}
		else if (0 == m_strCode2.Compare(szCode))
		{
			if (m_iLeg2VolMul > 0)
			{
				iVolMul = m_iLeg2VolMul;
			}
			else
			{
				if (pMapFutuCodeInfo->find(szCode) != pMapFutuCodeInfo->end())
				{
					FutuCodeInfo oFutuInfo = pMapFutuCodeInfo->at(szCode);

					m_iLeg2VolMul = oFutuInfo.iVolumeMultiple;
					iVolMul = m_iLeg2VolMul;
				}
			}
		}
	}
	
	return iVolMul;
}
