#include "stdafx.h"

#include "StrategyGridV1.h"

#include "..\hsquantrade\FunctionPub.h"


#include<time.h>

#include "..\hsquantrade\tool.h"

#include <string>


CStrategyGrid::CStrategyGrid(void)
{
	m_strStrategyName = "网格交易策略";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_nTradeCountSum = 0;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //是否跟单,默认写个-1

	m_timeOut = 0;                     //超时时间

	m_jiacha = 0;					       //价差

	m_kp = 0;

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_nCount = 0;

	m_strParamList = "交易合约;合约乘数;交易总数量A1;初始价格P1;网格宽度J1;每网格数量N1;交易方向;开平方向;最大成交数量;最大撤单次数;初始风险;资金回撤;报单间隔;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("合约乘数",""));

	m_params.insert(pair<string, CString>("交易总数量A1",""));

	m_params.insert(pair<string, CString>("初始价格P1",""));

	m_params.insert(pair<string, CString>("网格宽度J1",""));

	m_params.insert(pair<string, CString>("每网格数量N1",""));

	m_params.insert(pair<string, CString>("交易方向","0"));

	m_params.insert(pair<string, CString>("开平方向","0"));

	m_params.insert(pair<string, CString>("最大成交数量",""));

	m_params.insert(pair<string, CString>("最大撤单次数","500"));

	m_params.insert(pair<string, CString>("初始风险","0"));

	m_params.insert(pair<string, CString>("资金回撤","0"));

	m_params.insert(pair<string, CString>("报单间隔","500"));

	m_params.insert(pair<string, CString>("账户",""));

	m_params.insert(pair<string, CString>("是否追单","-1"));

	m_params.insert(pair<string, CString>("超时时间","0"));

	m_params.insert(pair<string, CString>("价差","0"));

	isNewModel = true;

	isAnohterDay = false;

	isSave = false;

	isLoad = false;

	m_Exit = false;


}

CStrategyGrid::CStrategyGrid(int i)
{
	if(i==1)
	{
		setValue();
		m_tOrder = thread( bind( &CStrategyGrid::checkOrder, this ) );

		//指定线程函数
		//m_tStore = thread( bind( &CStrategyGrid::process, this ) );

	}
}


CStrategyGrid::~CStrategyGrid(void)
{
	m_Exit = true;
	m_orderinfo.clear();
	m_mapOrder.clear();
	m_tOrder.join();
	m_tStore.join();
	
}


void  CStrategyGrid::Tick(CTickData * pt)
{

	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	m_lastPrice = pt->lastprice;

	//认为收到第一笔最新价就开盘或者进行集合竞价了
	if((data!=NULL) && ((*data).isReady) )
	{
		//不能直接写成m_tickcount > 1
		if(m_tickcount == 2)
		{	
			m_btick = true;
			//return ;
		}

		m_tickcount++;
	}
	else
	{
		return;
	}

	//m_hignPrice = pt->highestprice;

	m_pricebid1 = pt->bidprice[0];

	m_priceask1 = pt->askprice[0];

	m_fyingkuiPosition = 0;

	/*
	for(int i=0;i<m_arPosition.GetSize();i++)
	{
		TPosition * pos= (TPosition *)m_arPosition[i];

		double temp = 0 ;

		if( pos->direction == '0' )
		{
			temp = ( pt->lastprice - pos->price ) * pos->count * m_nInstrumentValue;
		}
		else if( pos->direction == '1' )
		{
			temp = ( pos->price - pt->lastprice) * pos->count * m_nInstrumentValue;
		}

		m_fyingkuiPosition += temp;
	}	

	//风险和资金管理控制
	if( ( m_fyingkuiPosition + m_fyingkuiClose ) < m_nMaxLost && m_nMaxLost < -1 ) //0 的时候不启用
	{
		m_bRun = 0;

		char msg[128];

		sprintf_s(msg,128,"最大亏损%.3f 超过最大初始风险设置，清仓，停止策略允许",m_fyingkuiPosition + m_fyingkuiClose);

		AddLog(msg);

		ClearPosition();
	}

	if(  ( m_fyingkuiPosition + m_fyingkuiClose ) > m_maxProfit )
	{
		m_maxProfit = ( m_fyingkuiPosition + m_fyingkuiClose );
	}

	float back =  m_maxProfit - ( m_fyingkuiPosition + m_fyingkuiClose );

	if( back > m_fProfitBackStop  && m_fProfitBackStop > 1 ) //0 的时候不启用
	{
		m_bRun = 0;

		char msg[128];

		sprintf_s(msg,128,"资金回撤%.3f 超过资金回撤门限，清仓，停止策略允许",back);

		AddLog(msg);

		ClearPosition();
	}
	*/

}

void CStrategyGrid::BeginRun()
{
	//如果放在这里，快速的增加线程会导致崩溃，放在构造函数里面不会
	//t = thread( bind( &CStrategyGrid::checkOrder, this ) );
	m_tStore = thread( bind( &CStrategyGrid::process, this ) );
	
}

void CStrategyGrid::Order()
{
	char msg[256];

	//开仓
	if(m_kp==0)
	{
		//买开
		if( m_TradeDirection == 0 )
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenBuy(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"初始报单,开多,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0','0');

				price = price - m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//判断价格是否超过跌停价格

			
			}
		}
		//卖开
		else
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenSale(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"初始报单,开空,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1','0');

				price = price + m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//判断价格是否超过涨停价格
			}
		}
	}
	//平仓
	else if(m_kp==1)
	{
		//买平
		if( m_TradeDirection == 0 )
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				dealHoldsinfo(count,price,m_TradeDirection);

				price = price - m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//判断价格是否超过跌停价格

			
			}
		}
		//卖平
		else
		{
			double price = m_beginPrice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{

				dealHoldsinfo(count,price,m_TradeDirection);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > m_nCount )
				{
					break;
				}

				//判断价格是否超过涨停价格
			}
		}

	}


}

void CStrategyGrid::SetParam(CString paramname,CString value)
{

	//"交易合约;交易总数量A1;初始价格P1;网格宽度J1;每网格数量N1;交易方向;最大成交数量;最大撤单次数;最大风险;账户;";

	m_params[(LPCTSTR)paramname] = value;

	//还需要将对应的值更新
	if(paramname=="交易合约")
	{
		m_strInstrument = value;
	}
	else if(paramname=="合约乘数")
	{
		m_nInstrumentValue =  atoi(value);
	}
	else if(paramname=="交易总数量A1")
	{
		if(m_nCount==0)
		{
			m_nCount = atoi(value);
			return;
		}

		int newCount = atoi(value);

		char msg[256];
		//盘中才行
		if(!checkOrdertime())
		{
			
			sprintf(msg,"非交易时间不允许改变总数量...");
			AddLog(msg);
			return;
		}

		if(!m_bRun || m_Exit)
		{
			sprintf(msg,"模型处于非激活状态,不允许改变总数量...");
			AddLog(msg);
			return;
		}


		if(newCount > m_nCount)
		{
			CString s;
			s.Format("交易总数量改为:%d",newCount);

			AddLog(s.GetBuffer(0));

			addGrid(newCount);
		}
		else if(newCount < m_nCount)
		{
			CString s;
			s.Format("交易总数量改为:%d",newCount);

			AddLog(s.GetBuffer(0));

			subGrid(newCount);
		}

		m_nCount = newCount;

	}
	else if(paramname=="初始价格P1")
	{
		m_beginPrice = atof(value);
	}
	else if(paramname=="网格宽度J1")
	{
		m_J1 = atof(value);
	}
	else if(paramname=="每网格数量N1")
	{
		m_N1 = atof(value);
	}
	else if(paramname=="交易方向")
	{
		m_TradeDirection = atoi(value);
	}
	else if(paramname=="开平方向")
	{
		m_kp = atoi(value);
	}
	else if(paramname=="最大成交数量")
	{
		m_nMaxTradeCount = atoi(value);
	}
	else if(paramname=="最大撤单次数")
	{
		m_nMaxChedanCount = atoi(value);
	}
	else if(paramname=="初始风险")
	{
		m_nMaxLost = atoi(value);
	}
	else if(paramname=="资金回撤")
	{
		m_fProfitBackStop = atof(value);
	}
	else if(paramname=="账户")
	{
		m_fundAccount = value;
	}
	else if(paramname=="报单间隔")
	{
		m_iIntervalOrder = atoi(value);
	}
}


void CStrategyGrid::SetParam(CString value)
{
	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	CStringArray listField;

	pub.ParseParam(	m_strParamList,listField,';');

	for(int i=0;i<listField.GetSize()&&i<list.GetSize();i++)
	{
		SetParam(listField[i],list[i]);
	}

	//if( m_TradeDirection == 0 )
	//{
	//	if(m_kp==0)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "开多";     //modelname
	//	else if(m_kp==1)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "平多";     //modelname

	//}
	//else if( m_TradeDirection == 1 )
	//{
	//	if(m_kp==0)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "开空";     //modelname
	//	else if(m_kp==1)
	//		m_strStrategyName = m_strStrategyName + m_strInstrument + "平空";     //modelname
	//}

}

CString CStrategyGrid::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%d;%.4f;%.4f;%.2f;%d;%d;%d;%d;%d;%.2f;%d;%s;",m_strInstrument,m_nInstrumentValue,m_nCount,m_beginPrice,m_J1,m_N1,m_TradeDirection,m_kp,m_nMaxTradeCount,m_nMaxChedanCount,m_nMaxLost,m_fProfitBackStop,m_iIntervalOrder,m_fundAccount);

	return str;
}

CString CStrategyGrid::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyGrid::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString openrice;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	CString yinkuirate;

	CString huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "空仓";

		openrice.Format("%.4f",m_beginPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "买开";

		openrice.Format("%.4f",m_beginPrice);

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "卖开";

		openrice.Format("%.4f",m_beginPrice);

	}

	if( m_bRun == false )
		modelstatus = "暂停";
	else
		modelstatus = "运行";

	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getfutuName(m_strInstrument.GetBuffer(0));

	m_strInstrument.ReleaseBuffer();

	//yinkuirate.Format("%.2f",(m_lastPrice - atof(openrice)) / atof(openrice));

	yinkuirate = "0";

	if(m_hignPrice < 0.001)
	{
		huitiaorate = "0.00";
	}
	else
	{
		huitiaorate.Format("%.2f",((m_hignPrice - m_lastPrice) / m_hignPrice));
	}

	if( m_TradeDirection == 0 )
	{
		if(m_kp==0)
			status = status+"(开多)";     //modelname
		else
			status = status+"(平多)";     //modelname
	}
	else if( m_TradeDirection == 1 )
	{
		if(m_kp==0)
			status = status+"(开空)";     //modelname
		else
			status = status+"(平空)";     //modelname
	}
	

	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%.4f;%s;%.4f;%.4f(%s);%s;%s;",m_strStrategyName,m_strInstrument,codename,m_nCount,status,openrice,yingkuiPos,yinkuirate,m_fyingkuiClose,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,m_strStrategyName);

	//result = "网格交易策略TF1512做多;TF1512;TF1512;10;买开;100;0;空仓;0;0;041249,;";
	return result;
}

/*
处理成交回报
买入方式：
开仓成交-------------立即挂止赢单
平仓成交-------------立即挂开仓单
*/
void CStrategyGrid::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{
	//判断是否属于本策略的报单
	bool bt = false;
	
	bt = isExistRef(orderref,direction,offsetflag,ins);

	int notTradeCount = 0;            //本次回报之前未成交数量

	char msg[256];

	if( bt )
	{
		notTradeCount = m_orderinfo[orderref].ordercount - m_orderinfo[orderref].tradecount;

		if( offsetflag == '0' )//开仓
		{
			if(direction == '0')
			{		
				sprintf_s(msg,256,"[回报]开仓成交 成交价格%.3f,数量%d,报单引用:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();

				double newprice = m_orderinfo[orderref].orderprice + m_J1;
				
				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//需要累加成交数量
				m_orderinfo[orderref].tradecount += vol;
				//已经全部成交，删除
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"全部成交,删除列表引用:%d",orderref);
					AddLog(msg);

				}
				//出现过回报的数量大于报单数量的情况
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"成交(%d)大于报单(%d),全部成交,删除列表引用:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//这里判断先平后开的最大成交数量
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"成交数量 %d 超过门限%d,不再下单 ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					//平空
					JoinOrder(orderref,vol,newprice,'1',CLOSEFLAG_CLOSE_TODAY);

					//开仓回报回来肯定是平今
					//int closeorderref = CloseSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					//AddOrderInfo(closeorderref,vol,newprice,'1',CLOSEFLAG_CLOSE_TODAY);

					//记录开仓的仓位
					//AddOpenPosition(closeorderref,vol,price,direction,ins);

					//sprintf_s(msg,256,"止盈单生成 价格%.3f 数量%d orderref=%d ",newprice,vol,closeorderref);

					//AddLog(msg);
				}

			}
			else
			{
				sprintf_s(msg,256,"[回报]开仓成交 成交价格%.3f,数量%d,报单引用:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==0)
					m_nTradeCountSum += vol;


				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice - m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//需要累加成交数量
				m_orderinfo[orderref].tradecount += vol;
				//已经全部成交，删除
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"全部成交,删除列表引用:%d，",orderref);
					AddLog(msg);
				}
				//出现过回报的数量大于报单数量的情况
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"成交(%d)大于报单(%d),全部成交,删除列表引用:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//这里判断初始平仓的最大成交数量
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==1)
				{
					sprintf_s(msg,256,"成交数量 %d 超过门限%d,不再下单 ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if(m_bRun)
				{
					//平多
					JoinOrder(orderref,vol,newprice,'0',CLOSEFLAG_CLOSE_TODAY);

					//int closeorderref = CloseBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

					//AddOrderInfo(closeorderref,vol,newprice,'0',CLOSEFLAG_CLOSE_TODAY);

					////记录开仓的仓位
					//AddOpenPosition(closeorderref,vol,price,direction,ins);
					//	
					//sprintf_s(msg,256,"止盈单生成 价格%.3f 数量%d orderref=%d ",newprice,vol,closeorderref);

					//AddLog(msg);

				}

			}

		}
		else//平仓
		{
			CalCloseProfit(direction,price,vol);

			if(direction == '1')
			{
				sprintf_s(msg,256,"[回报]平仓成交 成交价格%.3f,数量%d,报单引用:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice - m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//需要累加成交数量
				m_orderinfo[orderref].tradecount += vol;
				//已经全部成交，删除
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"全部成交,删除列表引用:%d，",orderref);
					AddLog(msg);
				}
				//出现过回报的数量大于报单数量的情况
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"成交(%d)大于报单(%d),全部成交,删除列表引用:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}
				*/
				m_CheckSelfMutex.unlock();


				//这里判断初始开仓的最大成交数量
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"成交数量 %d 超过门限%d,不再下单 ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					//开多
					JoinOrder(orderref,vol,newprice,'0','0');
					//int ref = OpenBuy(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					//AddOrderInfo(ref,vol,newprice,'0','0');

					//sprintf_s(msg,256,"开仓单生成 价格%.2f 数量%d orderref=%d ",newprice,vol,ref);

					//AddLog(msg);
				}


			}
			else
			{
				sprintf_s(msg,256,"[回报]平仓成交 成交价格%.2f,数量%d,报单引用:%d",price,vol,orderref);

				AddLog(msg);

				if(m_kp==1)
					m_nTradeCountSum += vol;

				m_CheckSelfMutex.lock();
				double newprice = m_orderinfo[orderref].orderprice + m_J1;

				vol = dealTrade(orderref,vol,notTradeCount);

				if(vol == -1)
					return;

				/*
				//需要累加成交数量
				m_orderinfo[orderref].tradecount += vol;
				//已经全部成交，删除
				if(m_orderinfo[orderref].ordercount==m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"全部成交,删除列表引用:%d，",orderref);
					AddLog(msg);
				}
				//出现过回报的数量大于报单数量的情况
				else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
				{
					RemoveOrder(orderref);	
					m_orderinfo.erase(orderref);
					sprintf_s(msg,256,"成交(%d)大于报单(%d),全部成交,删除列表引用:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
					AddLog(msg);

					vol = notTradeCount;

				}*/
				m_CheckSelfMutex.unlock();

				//这里判断初始开仓的最大成交数量
				if( m_nTradeCountSum  >= m_nMaxTradeCount && m_kp==0)
				{
					sprintf_s(msg,256,"成交数量 %d 超过门限%d,不再下单 ",m_nTradeCountSum,m_nMaxTradeCount);

					AddLog(msg);

					return;
				}

				if( m_bRun )
				{
					//开空
					JoinOrder(orderref,vol,newprice,'1','0');

					//int ref = OpenSale(m_strInstrument.GetBuffer(0),vol,newprice,m_fundAccount.GetBuffer(0));

					//AddOrderInfo(ref,vol,newprice,'1','0');

					//sprintf_s(msg,256,"开仓单生成 价格%.3f 数量%d orderref=%d ",newprice,vol,ref);

					//AddLog(msg);
				}

			}
		}
	}
}



void CStrategyGrid::OnErrorRtn(int code,char * errormsg) 
{
	char msg[128];

	
}


void CStrategyGrid::OnRspOrderInsert(int ref,char entrust_status,int errorid)
{

	/*

	//先判断是否是该模型的报单
	m_CheckSelfMutex.lock();

	if(m_orderinfo.end() == m_orderinfo.find(ref))
	{
		m_CheckSelfMutex.unlock();

		return;
	}

	m_CheckSelfMutex.unlock();

	//处理错误的报单
	if(errorid !=0 )
	{
		CString s;
		s.Format("报单失败,错误码:%d,增加补单,报错委托引用:%d",errorid,ref);
		AddLog(s.GetBuffer(0));

		m_reOrderMutex.lock();

		m_failOrder[ref] = m_orderinfo[ref];

		m_reOrderMutex.unlock();
		//删除指令

		m_CheckSelfMutex.lock();

		m_orderinfo.erase(ref);

		m_CheckSelfMutex.unlock();
	}


	*/
}


void CStrategyGrid::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) 
{
	//如果有撤单需要删除对应的报单引用
	if(isExistRef(orderref,bs,kp,ins))
	{
		if((orderstatus=='5' || orderstatus=='6') && (ordersystemid!=0))
		{
			CString s;
			s.Format("[撤单回报] 删除对应指令,报单引用:%d",orderref);

			m_CheckSelfMutex.lock();

			//201604 begin						
			//判断是否是内部成交的撤单回报
			map<int,COrderInfo>::iterator it = m_orderinfo.find(orderref);
			if(it != m_orderinfo.end())
			{
				if(it->second.selfclose == '1')
				{
					s.Format("[撤单回报] 内部成交发出撤单请求,报单引用:%d",orderref);
					//JoinOrder函数内已经加了锁，避免此种情况程序飞掉
					//m_JoinMutex.lock();

					int newCount = it->second.ordercount - it->second.tradecount;
					double newPrice = it->second.orderprice;
					char bs = it->second.bs;
					char kp = it->second.kp;
					JoinOrder(orderref,newCount,newPrice,bs,kp);
					char msg[256];
					sprintf_s(msg,256,"撤单进入报单队列,引用:%d,价格:%.3f,数量:%d,买卖:%c,开平:%c...",orderref,newPrice,newCount,bs,kp);
					AddLog(msg);

					//m_JoinMutex.unlock();	
				}
			}			
			//201604 end

			m_orderinfo.erase(orderref);

			m_CheckSelfMutex.unlock();

			RemoveOrder(orderref);
			AddLog(s.GetBuffer(0));
		}
		//处理被挡掉的报单
		else if(ordersystemid==0 && orderstatus=='5')
		{
			/*
			CString s;
			s.Format("增加补单,原报单引用:%d",orderref);
			AddLog(s.GetBuffer(0));

			m_reOrderMutex.lock();
			m_failOrder[orderref] = m_orderinfo[orderref];
			m_reOrderMutex.unlock();
			//删除指令

			m_CheckSelfMutex.lock();
			m_orderinfo.erase(orderref);
			m_CheckSelfMutex.unlock();
			*/
		
		}
	}

}


void CStrategyGrid::CalCloseProfit(char direction,double price,int count)
{

	if( direction == '0' )
	{
		
		//BP-找一个开空的持仓
		for(int i=0;i<m_arPosition.GetSize();i++)
		{
			TPosition * pos = (TPosition *)m_arPosition[i];

			if( pos->direction =='1' )
			{
				m_arPosition.RemoveAt(i);
				
				m_fyingkuiClose += (pos->price - price ) * count * m_nInstrumentValue;

				delete pos;

				break;
			}
		}

	}
	else
	{
		//SP
		for(int i=0;i<m_arPosition.GetSize();i++)
		{
			TPosition * pos = (TPosition *)m_arPosition[i];

			if( pos->direction =='0' )
			{
				m_arPosition.RemoveAt(i);
				
				m_fyingkuiClose += (price - pos->price) * count * m_nInstrumentValue;

				delete pos;

				break;
			}
		}
	}
}

//平掉所有仓位，撤销所有未成交单
void CStrategyGrid::ClearPosition(void)
{	
	//先撤单
	for(int i=0;i<m_listOrderRef.GetSize();i++)
	{
		int orderref = m_listOrderRef[i];

		char msg[128];

		sprintf_s(msg,128,"撤单 orderref = %d",orderref);

		AddLog(msg);

		OrderAction(orderref);
	}
	
	//设置下单方式，清仓的时候需要使用追价
	m_follow = "1";

	m_jiacha = 2 * m_J1;

	//平仓
	for(int i=0;i<m_arPosition.GetSize();i++)
	{
		TPosition * pos = (TPosition *)m_arPosition[i];

		if( pos->direction == '0' )
		{			
			CloseSale(m_strInstrument.GetBuffer(0),pos->count,m_pricebid1-m_J1,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
		}
		else
		{
			CloseBuy(m_strInstrument.GetBuffer(0),pos->count,m_priceask1+m_J1,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
		}
	}
}


void CStrategyGrid::RemoveOrder(int orderref)
{
	for(int i=0;i<m_listOrderRef.GetSize();i++)
	{
		if( orderref == m_listOrderRef[i] )
		{
			m_listOrderRef.RemoveAt(i);
			break;
		}
	}
}

bool CStrategyGrid::isExistRef(int ref,char bs,char kp,char * ins)
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

void CStrategyGrid::addGrid(int count)
{
	int addcount = count - m_nCount;

	double price = 0;

	if(m_TradeDirection==0)
	{
		if(m_kp==0)
		{
			 price = m_beginPrice - (m_nCount / m_N1) * m_J1;
		}
		else if(m_kp==1)
		{
			 price = m_beginPrice + (m_nCount / m_N1) * m_J1;
		}
	}
	else if(m_TradeDirection==1)
	{
		if(m_kp==0)
		{
			 price = m_beginPrice + (m_nCount / m_N1) * m_J1;
		}
		else if(m_kp==1)
		{
			 price = m_beginPrice - (m_nCount / m_N1) * m_J1;
		}
	}

	//盘中才行
	if(checkOrdertime())
	{
		newOrder(addcount,price);
	}
	else
	{
		char msg[256];
		sprintf(msg,"非交易时间不允许改变总数量...");
		AddLog(msg);
	}

}


void CStrategyGrid::newOrder(int newcount,double newprice)
{

	char msg[256];

	//开仓
	if(m_kp==0)
	{
		//买开
		if( m_TradeDirection == 0 )
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenBuy(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"新增,开多,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0','0');

				price = price - m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//判断价格是否超过跌停价格

			
			}
		}
		//卖开
		else
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				int orderref = OpenSale(m_strInstrument.GetBuffer(m_strInstrument.GetLength()),count,price,m_fundAccount.GetBuffer(m_fundAccount.GetLength()));

				sprintf_s(msg,256,"新增,开空,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1','0');

				m_listOrderRef.Add(orderref);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//判断价格是否超过涨停价格
			}
		}
	}
	//平仓
	else if(m_kp==1)
	{
		//买平
		if( m_TradeDirection == 0 )
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{
				dealHoldsinfo(count,price,m_TradeDirection);

				price = price - m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//判断价格是否超过跌停价格

			
			}
		}
		//卖平
		else
		{
			double price = newprice;

			int count = m_N1;

			int countAll = 0;

			while(true)
			{

				dealHoldsinfo(count,price,m_TradeDirection);

				price = price + m_J1;

				countAll += count;

				if( countAll + count > newcount )
				{
					break;
				}

				//判断价格是否超过涨停价格
			}
		}

	}

}


void CStrategyGrid::reload()                //重新加载
{
	//原始字符串 c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + m_strStrategyName + ".ini";
	//path = path  + "网格交易策略ru1605做多3023422.ini";

	int amount  = GetPrivateProfileInt("orderinfo", "amount", 0, path);

	CString code,account,count,price,kp,bs;
	string code1,account1;

	//一直返回-1
	//GetPrivateProfileString("modelparams", "交易合约", "", code1.GetBuffer(0), 16, path);

	GetPrivateProfileString("modelparams", "交易合约", "", (char*)code1.c_str(), 16, path);

	//GetPrivateProfileString("modelparams", "账户", "", account1.GetBuffer(0), 16, path);

	GetPrivateProfileString("modelparams", "账户", "", (char*)account1.c_str(), 16, path);

	code = code1.c_str();
	account = account1.c_str();

	for(int i=0;i<amount;i++)
	{
	
		CString k1,k2,k3,k4,v2,v3,v4;

		string a,b,c;

		k1.Format("count%d",i);
		k2.Format("price%d",i);
		k3.Format("bs%d",i);
		k4.Format("kp%d",i);
		
		int v1 = GetPrivateProfileInt("orderinfo", k1, 0, path);
		//GetPrivateProfileString("orderinfo", k2, "", v2.GetBuffer(0), 20, path); 
		//GetPrivateProfileString("orderinfo", k3, "", v3.GetBuffer(0), 2, path); 
		//GetPrivateProfileString("orderinfo", k4, "", v4.GetBuffer(0), 2, path); 

		GetPrivateProfileString("orderinfo", k2, "", (char*)a.c_str(), 20, path); 
		GetPrivateProfileString("orderinfo", k3, "", (char*)b.c_str(), 2, path); 
		GetPrivateProfileString("orderinfo", k4, "", (char*)c.c_str(), 2, path);

		v2 = a.c_str();
		v3 = b.c_str();
		v4 = c.c_str();


		if(v1==0)
			continue;

			int cc = v1;

			double pp = atof(v2);

			if(v3=="0")
			{
				if(v4=="0")
				{
					int ref = OpenBuy(code.GetBuffer(0),cc,pp,account.GetBuffer(0));

					AddOrderInfo(ref,cc,pp,'0','0');

				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseBuy(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					AddOrderInfo(ref,cc,pp,'0','1');

				}
			}
			else if(v3=="1")
			{
				if(v4=="0")
				{
					int ref = OpenSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0));

					AddOrderInfo(ref,cc,pp,'1','0');

				}
				else if(v4=="1" || v4=="3")
				{
					int ref = CloseSale(code.GetBuffer(0),cc,pp,account.GetBuffer(0),CLOSEFLAG_CLOSE);

					AddOrderInfo(ref,cc,pp,'1','1');

				}
			
			}

		}
	

}


void CStrategyGrid::saveModel()            //保存模型
{

	if(m_orderinfo.size()<=0)
		return;

	//原始字符串 c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + m_strStrategyName + ".ini";


	std::map<std::string,CString>::iterator it; 

	//交易合约;合约乘数;交易总数量A1;初始价格P1;网格宽度J1;每网格数量N1;交易方向;最大成交数量;最大撤单次数;初始风险;资金回撤;账户;
	for(it=m_params.begin();it!=m_params.end();it++)
	{
		string key = it->first;
		CString value = it->second;
		WritePrivateProfileString("modelparams", key.c_str(), value, path);
	}

	//保存未成交的开仓和平仓委托
	CString c,count,price,bs,kp;
	c.Format("%d",m_orderinfo.size());
	WritePrivateProfileString("orderinfo", "amount", c, path);

	int i=0;

	map<int,COrderInfo>::iterator it_order;
	for(it_order=m_orderinfo.begin();it_order!=m_orderinfo.end();it_order++)
	{
		COrderInfo c = it_order->second;
		CString k1,k2,k3,k4,v1,v2,v3,v4;

		k1.Format("count%d",i);
		k2.Format("price%d",i); 
		k3.Format("bs%d",i); 
		k4.Format("kp%d",i); 

		v1.Format("%d",c.ordercount-c.tradecount);
		v2.Format("%.4f",c.orderprice);
		v3.Format("%c",c.bs);
		v4.Format("%c",c.kp);

		WritePrivateProfileString("orderinfo", k1, v1, path);
		WritePrivateProfileString("orderinfo", k2, v2, path);
		WritePrivateProfileString("orderinfo", k3, v3, path);
		WritePrivateProfileString("orderinfo", k4, v4, path);

		i++;
	}

	//收盘最后需要清空m_orderinfo
	m_orderinfo.clear();

}


bool CStrategyGrid::checkOrdertime()
{
	
	bool ret = false;
	
	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='3' /*|| a=='5'*/ || a=='2')
		ret =true;

	return ret;

	//bool ret =false;

	//const time_t t = time(NULL);

	//struct tm* current_time = localtime(&t);

	//int h = current_time->tm_hour;
	//int m = current_time->tm_min;
	//int s = current_time->tm_sec;

	//if( (h==16) && (m>=35) )
	//{
	//	ret =  true;
	//}
	//return ret;
}

bool CStrategyGrid::checkMatchTime()
{
	bool ret = false;
	
	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='5')
		ret =true;

	return ret;
}

bool CStrategyGrid::checkothertime()
{
	bool ret =false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='1' || a=='0')
		ret =true;

	return ret;

	//return false;

}

bool CStrategyGrid::checksavetime()
{
	bool ret = false;

	string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));

	char a = (*data).m_sCodeStatus[prefix].status;
	if(a=='6')
		ret =true;

	return ret;

}

bool CStrategyGrid::reLogintime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if( (h==8 || h==20) && (20<m && m<50) )
	{
		ret =  true;
	}
	else if( (h==12 ) && (55<m && m<59) )
	{
		ret =  true;
	}
	else if( (h==13 ) && (25<m && m<29) )
	{
		ret =  true;
	}

	return ret;

}

//bool CStrategyGrid::checkbefore()
//{
//	bool ret =false;
//
//	const time_t t = time(NULL);
//
//	struct tm* current_time = localtime(&t);
//
//	int h = current_time->tm_hour;
//	int m = current_time->tm_min;
//	int s = current_time->tm_sec;
//
//	if( (h==8 || h==20) && (m>=50) )
//	{
//		ret =  true;
//	}
//	else if(h==9 || h==21)
//	{
//		ret =  true;
//	}
//	else if( (h==10) &&(h>=25) )
//	{
//		ret =  true;
//	}
//	else if( (h==12) &&(h>=55) )
//	{
//		ret =  true;
//	}
//	else if( (h==13) &&(h>=25) )
//	{
//		ret =  true;
//	}
//
//	return ret;
//
//}

void CStrategyGrid::checkOrder()
{

	//这里不存在该账户也会直接进入此循环
	while( (data==NULL) || !(*data).isReady)
	{
		AddLog("wait...");
		if((data!=NULL))
		{
			if( (m_kp==1) && ( checkOrdertime()) )
			{
				CString str;
				str.Format("正在查询持仓...");
				AddLog(str.GetBuffer(0));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if(data==NULL)
		{
			//AddLog("无此帐号...");
			//return;
		}

	}

	while(!m_Exit)
	{
		
		//AddLog("222...");
		if(m_Exit)
		{
			CString str;

			str.Format("%s:策略终止，退出",m_strStrategyName);

			this->AddLog(str.GetBuffer(0));

			return;
		}

		if(!m_bRun)
		{
			CString str;

			str.Format("%s:策略暂停，等待",m_strStrategyName);

			this->AddLog(str.GetBuffer(0));

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

			continue;
		}

		if(reLogintime())
		{
			::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,3,0);

			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		}


		if(checkOrdertime() || m_btick)
		{
			//AddLog("333...");
			m_btick = false;

			if(isNewModel)
			{
				AddLog("模型添加成功...");

				if( m_nStrategyStatus == 0 )
				{
					////指定线程函数
					//m_tStore = thread( bind( &CStrategyGrid::process, this ) );

					Order();
				}

				isNewModel = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				continue;
			}

			//报单失败的单子再次报单
			reOrderAgain();

			if(isAnohterDay && !isNewModel && !isLoad)
			{
				this->AddLog("reload...");

				reload();

				isLoad = true;

				isSave = false;

				isAnohterDay = false;

				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}


		//收盘
		if(checksavetime())
		{
			//如果盘后添加则等待
			if(isNewModel)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
				continue;
			}

			if(!isSave && !isAnohterDay)
			{
				AddLog("save...");
				saveModel();
				isSave = true;
				isAnohterDay = true;
				isLoad = false;
				m_listOrderRef.RemoveAll();
				m_orderinfo.clear();

			}
		
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		}

		//一般非交易时间
		if(checkothertime())
		{
			//AddLog("444...");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			continue;
			
		}

		//没有就强制睡3秒
		string prefix = Tool::getCodePrefix(m_strInstrument.GetBuffer(0));
		if((*data).m_sCodeStatus[prefix].status==0 && !m_btick)
			std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));

		//AddLog("555...");
		
	}

	if(m_Exit)
	{
		CString str;

		str.Format("%s:策略终止，退出",m_strStrategyName);

		this->AddLog(str.GetBuffer(0));

		return;
	}


}

void CStrategyGrid::setValue()
{
	m_strStrategyName = "网格交易策略";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_nTradeCountSum = 0;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //是否跟单,默认写个-1

	m_timeOut = 0;                     //超时时间

	m_jiacha = 0;					       //价差

	m_fyingkuiClose = 0;

	m_nInstrumentValue = 1;

	m_maxProfit = 0;

	m_fProfitBackStop = 0;

	m_pricebid1 = 0;

	m_priceask1 = 0;

	m_N1 = 0;

	m_kp = 0;

	m_nCount = 0;

	m_btick = false;

	m_tickcount = 0;

	m_iIntervalOrder = 500;

	m_strParamList = "交易合约;合约乘数;交易总数量A1;初始价格P1;网格宽度J1;每网格数量N1;交易方向;开平方向;最大成交数量;最大撤单次数;初始风险;资金回撤;报单间隔;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("合约乘数",""));

	m_params.insert(pair<string, CString>("交易总数量A1",""));

	m_params.insert(pair<string, CString>("初始价格P1",""));

	m_params.insert(pair<string, CString>("网格宽度J1",""));

	m_params.insert(pair<string, CString>("每网格数量N1",""));

	m_params.insert(pair<string, CString>("交易方向","0"));

	m_params.insert(pair<string, CString>("开平方向","0"));

	m_params.insert(pair<string, CString>("最大成交数量",""));

	m_params.insert(pair<string, CString>("最大撤单次数","500"));

	m_params.insert(pair<string, CString>("初始风险","0"));

	m_params.insert(pair<string, CString>("资金回撤","0"));

	m_params.insert(pair<string, CString>("账户",""));

	m_params.insert(pair<string, CString>("是否追单","-1"));

	m_params.insert(pair<string, CString>("超时时间","0"));

	m_params.insert(pair<string, CString>("价差","0"));

	m_params.insert(pair<string, CString>("报单间隔","500"));

	isNewModel = true;

	isAnohterDay = false;

	isSave = false;

	isLoad = false;

	m_Exit = false;


}


void CStrategyGrid::dealHoldsinfo(int count,double price,int bs)
{
	
	if(data == NULL)
		return;

	int yp = 0;  
	int p = 0;
	int index = 0;

	CString msg;

	m_QueryHoldMutex.lock();

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp(((*data).m_sHoldsInfo)[i].szInstrument,m_strInstrument)!=0)
			continue;

		if(((*data).m_sHoldsInfo)[i].PosiDirection + bs== 1 )
		{
			yp = ((*data).m_sHoldsInfo)[i].YdAvaiable;
			p = ((*data).m_sHoldsInfo)[i].TdAvaiable;
			msg.Format("账户:%s,当前可用今仓:%d,可用昨仓%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
			index = i;
			break;
		}
	
	}

	

	if(yp==0 && p==0)
	{
		msg.Format("可用持仓不足...");
		AddLog(msg.GetBuffer(0));
		msg.ReleaseBuffer();

		m_QueryHoldMutex.unlock();
		return;
	}


	if(bs==0)
	{
		if(p>=count)    //今仓大于下单数量
		{
			int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

			AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);

			((*data).m_sHoldsInfo)[index].TdAvaiable = ((*data).m_sHoldsInfo)[index].TdAvaiable - count;

			int rest = ((*data).m_sHoldsInfo)[index].TdAvaiable;

			msg.Format("平今仓:%d手,剩余今仓:%d手,报单引用:%d",count,rest,orderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

		}
		else if(yp + p >=count && p<count)
		{
			//先平今仓
			int orderref = 0;

			if(p>0)
			{
				orderref =CloseBuy(m_strInstrument.GetBuffer(0),p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
							
				AddOrderInfo(orderref,p,price,'0',CLOSEFLAG_CLOSE_TODAY);
				
			}

			int yorderref=CloseBuy(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			AddOrderInfo(yorderref,count-p,price,'0',CLOSEFLAG_CLOSE);

			((*data).m_sHoldsInfo)[index].TdAvaiable = 0;

			((*data).m_sHoldsInfo)[index].YdAvaiable = ((*data).m_sHoldsInfo)[index].YdAvaiable - (count-p);

			int rest = ((*data).m_sHoldsInfo)[index].YdAvaiable;

			msg.Format("优先平今仓:%d手,剩余今仓:0手,报单引用:%d;再平老仓:%d手,剩余老仓:%d手,报单引用:%d",p,orderref,(count-p),rest,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}
	}
	else if(bs==1)
	{
		if(p>=count)
		{
			int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

			AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);

			((*data).m_sHoldsInfo)[index].TdAvaiable = ((*data).m_sHoldsInfo)[index].TdAvaiable - count;

			int rest = ((*data).m_sHoldsInfo)[index].TdAvaiable;

			msg.Format("平今仓:%d手,剩余今仓:%d手,报单引用:%d",count,rest,orderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

		}
		else if(yp + p>=count && p<count)
		{
			//先平今仓
			int orderref = 0;
			if(p>0)
			{
				orderref =CloseSale(m_strInstrument.GetBuffer(0),p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);

				AddOrderInfo(orderref,p,price,'1',CLOSEFLAG_CLOSE_TODAY);

			}

			int yorderref =CloseSale(m_strInstrument.GetBuffer(0),count-p,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);

			AddOrderInfo(yorderref,count-p,price,'1',CLOSEFLAG_CLOSE);

			((*data).m_sHoldsInfo)[index].TdAvaiable = 0;

			((*data).m_sHoldsInfo)[index].YdAvaiable = ((*data).m_sHoldsInfo)[index].YdAvaiable - (count-p);

			int rest = ((*data).m_sHoldsInfo)[index].YdAvaiable;

			msg.Format("优先平今仓:%d手,剩余今仓:0手,报单引用:%d;再平老仓:%d手,剩余老仓:%d手,报单引用:%d",p,orderref,(count-p),rest,yorderref);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();
		}

	}

	m_QueryHoldMutex.unlock();

}



void CStrategyGrid::setHoldinfo()
{
	CString msg;
	int p = 0;
	int yp = 0;

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp((*data).m_sHoldsInfo[i].szInstrument,m_strInstrument)!=0)
			continue;

		if((*data).m_sHoldsInfo[i].PosiDirection == 0 )
		{
			yp = (*data).m_sHoldsInfo[i].YdAvaiable = (*data).m_sHoldsInfo[i].TdAvaiable + (*data).m_sHoldsInfo[i].YdAvaiable;
			p = (*data).m_sHoldsInfo[i].TdAvaiable = 0;
			msg.Format("盘后,账户:%s,今仓归老仓,当前可用今仓:%d,可用昨仓%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}

		if(((*data).m_sHoldsInfo)[i].PosiDirection == 1 )
		{
			yp = (*data).m_sHoldsInfo[i].YdAvaiable = (*data).m_sHoldsInfo[i].TdAvaiable + (*data).m_sHoldsInfo[i].YdAvaiable;
			p = (*data).m_sHoldsInfo[i].TdAvaiable = 0;
			msg.Format("盘后,账户:%s,今仓归老仓,当前可用今仓:%d,可用昨仓%d...",m_fundAccount,p,yp);
			AddLog(msg.GetBuffer(0));
			msg.ReleaseBuffer();

			break;
		}
	
	}
	
}

void CStrategyGrid::updateHoldinfo(char bs,char kp,char *ins,int count)
{

	CString msg;
	int oldnum = 0;
	int newnum = 0;

	int ibs = bs - 48;
	int ikp = kp - 48;

	for(int i=0;i<(*data).m_sHoldsInfo.size();i++)
	{
		if (strcmp((*data).m_sHoldsInfo[i].szInstrument,ins)!=0)
			continue;

		if(ikp==0)
		{
			if((*data).m_sHoldsInfo[i].PosiDirection == ibs )
			{
				oldnum = (*data).m_sHoldsInfo[i].TdAvaiable ;
				newnum = (*data).m_sHoldsInfo[i].TdAvaiable + count;
				(*data).m_sHoldsInfo[i].TdAvaiable = newnum;

				msg.Format("账户:%s,开仓回报,更新可用持仓.原可用%d,增加%d,新可用%d",m_fundAccount,oldnum,count,newnum);
				AddLog(msg.GetBuffer(0));
				msg.ReleaseBuffer();

				break;
			}

		}
		else if(ikp==1)
		{
			if(((*data).m_sHoldsInfo)[i].PosiDirection == ibs )
			{
				oldnum = (*data).m_sHoldsInfo[i].TdAvaiable ;
				newnum = (*data).m_sHoldsInfo[i].TdAvaiable - count;
				(*data).m_sHoldsInfo[i].TdAvaiable = newnum;

				msg.Format("账户:%s,平仓回报,更新可用持仓.原可用%d,减少%d,新可用%d",m_fundAccount,oldnum,count,newnum);
				AddLog(msg.GetBuffer(0));
				msg.ReleaseBuffer();

				break;
			}

		}

	
	}

}



void CStrategyGrid::reOrderAgain()
{

	int count = 0;
	double price = 0;
	char msg[256];

	m_reOrderMutex.lock();

	if(m_failOrder.size()==0)
	{
		m_reOrderMutex.unlock();
		return;
	}

	map<int,COrderInfo>::iterator it;
	for(it=m_failOrder.begin();it!=m_failOrder.end();it++)
	{
		count = it->second.ordercount;
		price = it->second.orderprice;

		if(it->second.bs=='0' && it->second.kp=='0')
		{
			int orderref = OpenBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			sprintf_s(msg,256,"补单,开多,报单引用:%d",orderref);

			AddLog(msg);

			AddOrderInfo(orderref,count,price,'0','0');		
		
		}
		else if(it->second.bs=='1' && it->second.kp=='0')
		{
			int orderref = OpenSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			sprintf_s(msg,256,"补单,开空,报单引用:%d",orderref);

			AddLog(msg);

			AddOrderInfo(orderref,count,price,'1','0');	
			
		}
		else if(it->second.bs=='0' && it->second.kp=='1')
		{
			int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
			sprintf_s(msg,256,"补单,平多,报单引用:%d",orderref);
			AddLog(msg);

			AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE);	
		
		}
		else if(it->second.bs=='1' && it->second.kp=='1')
		{
			int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
			sprintf_s(msg,256,"补单,平空,报单引用:%d",orderref);
			AddLog(msg);

			AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE);	
			
		}
		else if(it->second.bs=='0' && it->second.kp=='3')
		{
			if(isAnohterDay)
			{
				int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
				sprintf_s(msg,256,"补单,平多,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE);	
		
			}
			else
			{
				int orderref = CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
				sprintf_s(msg,256,"补单,平今多头,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);
				
			}
		}
		else if(it->second.bs=='1' && it->second.kp=='3')
		{
			if(isAnohterDay)
			{
				int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE);
				sprintf_s(msg,256,"补单,平空,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE);

			}
			else
			{
				int orderref = CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
				
				sprintf_s(msg,256,"补单,平今空头,报单引用:%d",orderref);

				AddLog(msg);

				AddOrderInfo(orderref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);

			}
			
		}


	}
	
	m_failOrder.clear();

	m_reOrderMutex.unlock();

}


/*
selfclose ==0 正常报单状态
selfclose ==1 可能自成交，且进入等待队列，本次不报单
selfclose ==2 本次可能自成交但未报单，进入下次报单队列
*/

void CStrategyGrid::OrderTogether()
{

	m_JoinMutex.lock();

	//临时map，存放需要等待撤单成功的报单
	map<int,COrderInfo> tempWait;

	if(m_mapOrder.size()==0)
	{
		m_JoinMutex.unlock();
		return;
	}

	int count = 0;
	double price = 0;
	char msg[256];
	int ref = 0;
	int orderref = 0;

	map<int,COrderInfo>::iterator it;

	//先进行自成交检查
	for(it=m_mapOrder.begin();it!=m_mapOrder.end();it++)
	{
		orderref = it->second.ref;

		//已经检查过，且进入下一次报单队列的报单，跳过
		if(it->second.selfclose == '1')
		{
			continue;
		}

		//待报单自成交检测
		while( CheckSelfNotOrder(orderref,it->second.orderprice,it->second.bs,it->second.kp,it->second.ordercount) )
		{
			sprintf_s(msg,256,"循环检查,报单引用:%d,价格:%.3f,买卖:%c,开平:%c ",orderref,it->second.orderprice,it->second.bs,it->second.kp);
			AddLog(msg);

			//这里认为是价格不相等可能发生的自成交情况
			if(it->second.selfclose == '0')
			{
				sprintf_s(msg,256,"存在价差,可能发生自成交,保存并放在下一次报单,报单引用:%d...",orderref);
				AddLog(msg);

				//代表该指令可能自成交,且需要等待报单
				it->second.selfclose = '1';

				//记录可能自成交的报单信息
				COrderInfo *order = new COrderInfo();
				strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
				order->ref = orderref;
				order->ordercount = it->second.ordercount;
				order->tradecount = 0;              
				order->orderprice = it->second.orderprice;
				order->bs = it->second.bs;
				order->kp = it->second.kp;
				order->selfclose = '2';                  
				tempWait[orderref] = *order;
				delete order;
				

			}
			
		}

		if(it->second.selfclose == '1')
		{
			//说明待报单队列已经检查过自成交，并且是放在下一次报单队列，跳过
			continue;
		}

		//挂单自成交检测
		if( CheckSelfOrder(orderref,it->second.orderprice,it->second.bs,it->second.kp) )
		{
			
			sprintf_s(msg,256,"检查到与挂单自成交,等待报单,报单引用:%d,价格:%.3f,买卖:%c,开平:%c ",orderref,it->second.orderprice,it->second.bs,it->second.kp);
			AddLog(msg);

			//代表该指令可能自成交,且需要等待报单
			it->second.selfclose = '1';

			//记录可能自成交的报单信息
			COrderInfo *order = new COrderInfo();
			strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
			order->ref = orderref;
			order->ordercount = it->second.ordercount;
			order->tradecount = 0;              
			order->orderprice = it->second.orderprice;
			order->bs = it->second.bs;
			order->kp = it->second.kp;
			order->selfclose = '2';                  
			tempWait[orderref] = *order;
			delete order;

	
		}

	}

	//检查完成再报单
	OrderByMap(m_mapOrder);

	m_mapOrder.clear();

	map<int,COrderInfo>::iterator iter; 
	for(iter=tempWait.begin();iter!=tempWait.end();iter++)
	{
		int ref = iter->second.ref;

		m_mapOrder[ref] = iter->second;

		sprintf(msg,"可能自成交报单转入报单队列,等待撤单完成,再进行下次报单:%d",ref);
		AddLog(msg);

	}
	

	m_JoinMutex.unlock();

}


/*
**1.一笔报单分笔成交回来需要组合
**2.不同报单回来但是开平方向、买卖方向、价格一直也需要组合
*/
void CStrategyGrid::JoinOrder(int ref,int vol,double orderprice,char bs,char kp)
{

	m_JoinMutex.lock();

	char msg[256];

	sprintf(msg,"报单整理,成交报单:%d,成交量:%d,新的委托价格:%.3f",ref,vol,orderprice);

	AddLog(msg);

	map<int,COrderInfo>::iterator it; 

	it = m_mapOrder.find(ref);

	int newCount = 0;
	double newPrice = 0.0;

	if(it!=m_mapOrder.end())
	{
		int count = m_mapOrder[ref].ordercount;

		m_mapOrder[ref].ordercount = vol + count;

		sprintf(msg,"更新报单,原报单数量:%d,新增报单数量:%d,总共:%d,报单价格:%.3f,成交报单:%d",count,vol,(vol + count),orderprice,ref);

		AddLog(msg);
	}
	else
	{
		//先寻找之前报单进行合并
		if( JoinDiffRef(ref,vol,orderprice,bs,kp) )
		{	
		}
		else
		{
			//之前没有合并的报单，就新增
			COrderInfo order;
			strcpy(order.szInstrument,m_strInstrument);
			order.ref = ref;
			order.ordercount = vol;
			order.orderprice = orderprice;
			order.tradecount = 0;
			order.bs = bs;
			order.kp = kp;

			m_mapOrder[ref] = order;

			sprintf(msg,"生成报单,报单数量:%d,报单价格:%.3f,成交报单:%d",vol,orderprice,ref);

			AddLog(msg);
		}
	
	}

	m_JoinMutex.unlock();

}

/*
**针对ref不同的报单
*/
bool CStrategyGrid::JoinDiffRef(int ref,int vol,double orderprice,char bs,char kp)
{
	bool ret = false;
	char msg[256];

	map<int,COrderInfo>::iterator iter;
	for(iter = m_mapOrder.begin();iter!= m_mapOrder.end(); iter++)
	{
		int orderref = iter->second.ref;

		if(ref!= orderref && iter->second.orderprice == orderprice && iter->second.bs == bs && iter->second.kp == kp)
		{
			ret = true;

			int oldcount = iter->second.ordercount;
			int newcount = vol + oldcount;

			iter->second.ordercount = newcount;

			sprintf(msg,"合并,本次报单:%d,数量:%d,原报单:%d,数量:%d",ref,vol,orderref,oldcount);
			AddLog(msg);

			sprintf(msg,"新报单参数,报单引用:%d,价格:%.4f,数量:%d,买卖:%c,开平:%c",orderref,orderprice,newcount,bs,kp);
			AddLog(msg);
	
			break;
		}
	}

	return ret;

}


void CStrategyGrid::AddOrderInfo(int ref,int count,double price,char bs,char kp)
{
		m_listOrderRef.Add(ref);

		m_CheckSelfMutex.lock();

		COrderInfo *order = new COrderInfo();
		strcpy(order->szInstrument,m_strInstrument.GetBuffer(0));
		order->ref = ref;
		order->ordercount = count;
		order->tradecount = 0;
		order->orderprice = price;
		order->bs = bs;
		order->kp = kp;
		order->selfclose = '0';
		m_orderinfo[ref] = *order;
		delete order;

		m_CheckSelfMutex.unlock();

}

void CStrategyGrid::AddOpenPosition(int ref,int count,double price,char bs,char *code)
{
		//记录开仓的仓位
		TPosition * pos = new TPosition;

		m_arPosition.Add(pos);

		strcpy_s(pos->ins,code);

		pos->count = count;

		pos->price  = price;

		pos->direction = bs;

}

void CStrategyGrid::process()
{

	//这里不存在该账户也会直接进入此循环
	while( (data==NULL) || !(*data).isReady)
	{
		AddLog("wait...");
		if((data!=NULL))
		{
			if( (m_kp==1) && ( checkOrdertime()) )
			{
				CString str;
				str.Format("正在查询持仓...");
				AddLog(str.GetBuffer(0));
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		if(data==NULL)
		{
			//AddLog("无此帐号...");
			//return;
		}

	}


	while(!m_Exit)
	{
		if(m_Exit)
			return;

		//如果是非交易时间则等待
		if(!checkOrdertime())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}
		//如果集合竞价撮合阶段成交了，则等待
		if(checkMatchTime())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		//test
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		//OrderAction(20007);
		//return;

		//分笔成交报单
		OrderTogether();

		std::this_thread::sleep_for(std::chrono::milliseconds(m_iIntervalOrder));

		//AddLog("process...");

		//自成交报单
		//OrderSelfColse();

	}

}


bool CStrategyGrid::CheckSelfNotOrder(int ref,double price,char bs,char kp,int count)
{
	bool ret = false;

	if(m_mapOrder.size()==0)
		return ret;

	char msg[256];

	double lastPrice = 0.0;

	char lastBS = '0';

	char lastKP = '0';

	int lastRef = 0;

	int lastCount = 0;


	map<int,COrderInfo> ::iterator it;
	for(it = m_mapOrder.begin();it!=m_mapOrder.end();it++)
	{
		lastRef = it->second.ref;
		lastPrice = it->second.orderprice;
		lastBS = it->second.bs;
		lastKP = it->second.kp;
		lastCount = it->second.ordercount;

		//如果lastRef已经检查过了就跳过
		//如果检查过而且内部成交就继续检查
		if( lastRef==ref)
		{
			continue;
		}

		//已经检查过，且进入下一次报单队列的报单，跳过
		if(it->second.selfclose == '1')
		{
			continue;
		}


		//买开 > 卖平
		if( (bs=='0') && (kp=='0') && (price>=lastPrice) && (lastBS=='1') && (lastKP == '1' || lastKP == '3')  )
		{
			ret = true;
			break;
		}
		//卖开 < 买平
		else if( (bs=='1') && (kp=='0') && (price<=lastPrice) && (lastBS=='0') && (lastKP == '1' || lastKP == '3') )
		{
			ret = true;
			break;
		}
		//买平 > 卖开
		else if( (bs=='0') && (kp == '1' || kp == '3') && (price>=lastPrice) && (lastBS=='1') && (lastKP=='0') )
		{
			ret = true;
			break;
		}
		//卖平 < 买开
		else if( (bs=='1') && (kp == '1' || kp == '3') && (price<=lastPrice) && (lastBS=='0') && (lastKP=='0') )
		{
			ret = true;
			break;
		}

	}

	if(ret)
	{
		sprintf(msg,"与未报单校对(队列中,待校验),价格(%.3f,%.3f),买卖(%c,%c),开平(%c,%c),数量(%d,%d),报单引用(%d,%d)",lastPrice,price,lastBS,bs,lastKP,kp,lastCount,count,lastRef,ref);

		AddLog(msg);

		//如果价格相同，则向上向下浮动一格
		//bug...
		//if( (price - lastPrice < 0.0000001) ||  (lastPrice - price < 0.0000001 ) )
		//if( (price - lastPrice < 0.0000001) ||  (price - lastPrice > -0.0000001 ) )
		if( price == lastPrice  )
		{

			if(count < lastCount)
			{
				ReverseOrder(ref,0,count);
				ReverseOrder(lastRef,0,count);
			}
			else if(count > lastCount)
			{
				ReverseOrder(ref,0,lastCount);
				ReverseOrder(lastRef,0,lastCount);
			}
			else
			{
				ReverseOrder(ref,0,count);
				ReverseOrder(lastRef,0,count);
			}

		}
		else
		{
			//201603  begin
			//认为价格一致才能自成交
			//ret = false;
			//201603 end

			//201604 begin
			//标记出价格不相等的可能自成交报单
			//如果ret为true并且价格相等，ReverseOrde函数中  会标记 it->second.selfclose = '1';
			//可利用 ret=true,it->second.selfclose = '0'  判断是否是价格相等可能发生的自成交
			//201604 end
		}

	}


	return ret;

}

bool CStrategyGrid::CheckSelfOrder(int ref,double price,char bs,char kp)
{
	m_CheckSelfMutex.lock();

	bool ret = false;

	if(m_orderinfo.size()==0)
	{
		m_CheckSelfMutex.unlock();
		return ret;
	}

	char msg[256];

	double lastPrice = 0.0;

	char lastBS = '0';

	char lastKP = '0';

	int lastRef = 0;

	int lastCount = 0;

	map<int,COrderInfo> ::iterator iter;
	for(iter = m_orderinfo.begin();iter!=m_orderinfo.end();iter++)
	{
		lastRef = iter->second.ref;
		lastPrice = iter->second.orderprice;
		lastBS = iter->second.bs;
		lastKP = iter->second.kp;
		lastCount = iter->second.ordercount;

		//201604 begin  
		//还是采用延迟报单的处理，因为存在撤单路上成交的情况.还是维持原来处理，因为成交了没关系	
		//为1代表先前检查过，已经发出撤单请求了。撤单应答，会删除该orderinfo
		if ( iter->second.selfclose == '1' )
		{
			sprintf_s(msg,256,"该挂单(%d)已经检查过自成交,已经发出撤单请求...",lastRef);
			AddLog(msg);
			continue;
		}
		

		//买开 > 卖平
		if( (bs=='0') && (kp=='0') && (price>=lastPrice) && (lastBS=='1') && (lastKP == '1' || lastKP == '3')  )
		{
			ret = true;
			break;
		}
		//卖开 < 买平
		else if( (bs=='1') && (kp=='0') && (price<=lastPrice) && (lastBS=='0') && (lastKP == '1' || lastKP == '3') )
		{
			ret = true;
			break;
		}
		//买平 > 卖开
		else if( (bs=='0') && (kp == '1' || kp == '3') && (price>=lastPrice) && (lastBS=='1') && (lastKP=='0') )
		{
			ret = true;
			break;
		}
		//卖平 < 买开
		else if( (bs=='1') && (kp == '1' || kp == '3') && (price<=lastPrice) && (lastBS=='0') && (lastKP=='0') )
		//else if( bs=='1' && (kp == '1' || kp == '3') && (price>=lastPrice) && lastBS=='0' && lastKP=='0'  )
		{
			ret = true;
			break;
		}

	}

	if(ret)
	{
		sprintf(msg,"与挂单校对(挂单,待校验),价格(%.3f,%.3f),买卖(%c,%c),开平(%c,%c),报单引用(%d,%d)",lastPrice,price,lastBS,bs,lastKP,kp,lastRef,ref);

		AddLog(msg);

		
		//if( (price - lastPrice < 0.0000001) ||  (lastPrice - price < 0.0000001 ) )
		//if( (price - lastPrice < 0.0000001) ||  (price - lastPrice > -0.0000001 ) )
		//这里直接撤单,撤单不成功没关系
		//if( price == lastPrice  )
		{
			
			sprintf_s(msg,256,"撤挂单,使其进入报单队列,报单引用:%d",lastRef);

			OrderAction(lastRef);

			iter->second.selfclose = '1';

			AddLog(msg);

		}
		/*
		else
		{
			//价格不相等延迟处理
			//201603 begin
			//ret = false;
			//201603 end

			//201604 begin
			//现在还是返回true
			//201604 end


		}
		*/

	}


	m_CheckSelfMutex.unlock();

	return ret;

}

void CStrategyGrid::OrderSelfColse()
{

	if(m_mapSelfOrder.size()==0)
	{
		return;
	}

	int count = 0;
	double price = 0;
	char msg[256];
	int ref = 0;
	int orderref = 0;

	map<int,COrderInfo>::iterator it; 
	for(it=m_mapSelfOrder.begin();it!=m_mapSelfOrder.end();it++)
	{
		count = it->second.ordercount;
		price = it->second.orderprice;
			
		if(it->second.kp == '0' && it->second.bs == '0')
		{
			ref = OpenBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,price,'0','0');
				
		}
		else if(it->second.kp=='0' && it->second.bs=='1')
		{
			OpenSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,price,'1','0');
		}
		else if(it->second.kp!='0' && it->second.bs=='0')
		{
			CloseBuy(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,price,'0',CLOSEFLAG_CLOSE_TODAY);
			
		}
		else if(it->second.kp!='0' && it->second.bs=='1')
		{
			CloseSale(m_strInstrument.GetBuffer(0),count,price,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,price,'1',CLOSEFLAG_CLOSE_TODAY);
		}

		sprintf(msg,"规避自成交报单,报单引用:%d",ref);
		AddLog(msg);
	}

	m_mapSelfOrder.clear();

}



int CStrategyGrid::dealTrade(int orderref,int vol,int iNotTradeCount)
{
	char msg[256];

	map<int,COrderInfo> ::iterator it = m_orderinfo.find(orderref);

	if(it == m_orderinfo.end())
	{
		sprintf_s(msg,256,"不存在报单引用:%d...",orderref);
		AddLog(msg);
		return -1;
	}

	//需要累加成交数量
	m_orderinfo[orderref].tradecount += vol;
	//已经全部成交，删除
	if(m_orderinfo[orderref].ordercount == m_orderinfo[orderref].tradecount)
	{
		RemoveOrder(orderref);	
		m_orderinfo.erase(orderref);
		sprintf_s(msg,256,"全部成交,删除列表引用:%d",orderref);
		AddLog(msg);

	}
	//出现过回报的数量大于报单数量的情况
	else if(m_orderinfo[orderref].ordercount < m_orderinfo[orderref].tradecount)
	{
		RemoveOrder(orderref);	
		m_orderinfo.erase(orderref);
		sprintf_s(msg,256,"成交(%d)大于报单数量(%d),全部成交,删除列表引用:%d",m_orderinfo[orderref].tradecount,m_orderinfo[orderref].ordercount,orderref);
		AddLog(msg);

		sprintf_s(msg,256,"未成交数量:%d",iNotTradeCount);
		AddLog(msg);

		vol = iNotTradeCount;

	}

	return vol;
}


bool CStrategyGrid::ReverseOrder(int ref,int type,int count)
{

	char msg[256];

	if(type == 0)
	{
		map<int,COrderInfo> ::iterator it = m_mapOrder.find(ref);

		if(it != m_mapOrder.end())
		{
			char bs = it->second.bs;
			char kp = it->second.kp;
			double price = it->second.orderprice;

			char newBs = '0';
			char newKp = '0';
			double newPrice = 0;

			//买开成交
			if(bs == '0' && kp == '0')
			{
				newBs = '1';
				newKp = '3';
				newPrice = price + m_J1;
			}
			//卖开成交
			else if(bs == '1' && kp == '0')
			{
				newBs = '0';
				newKp = '3';
				newPrice = price - m_J1;
			}
			//买平成交
			else if(bs == '0' && (kp == '3' || kp == '1') )
			{
				newBs = '1';
				newKp = '0';
				newPrice = price + m_J1;
			}
			//卖平成交
			else if(bs == '1' && (kp == '3' || kp == '1') )
			{
				newBs = '0';
				newKp = '0';
				newPrice = price - m_J1;
			}

		
			//拆分
			if(count < it->second.ordercount)
			{	
						
				COrderInfo order;
				strcpy(order.szInstrument,m_strInstrument.GetBuffer(0));
				order.ref = it->second.ref * 10;
				order.ordercount = it->second.ordercount - count;
				order.tradecount = 0;
				order.orderprice = it->second.orderprice;
				order.bs = it->second.bs;
				order.kp = it->second.kp;
				order.selfclose = '0';
    
				//这里直接报单
				OrderByInfo(order);

				
				//内部成交部分
				it->second.ordercount = count;
				it->second.bs = newBs;
				it->second.kp = newKp;
				it->second.orderprice = newPrice;
				it->second.tradecount = 0;                     

				sprintf_s(msg,256,"数量拆分,按照原计划先报单:%d,剩余:%d 部分内部成交...",order.ordercount,count);

				AddLog(msg);

			}
			else if(count == it->second.ordercount)
			{
				//数量不变
				it->second.bs = newBs;
				it->second.kp = newKp; 
				it->second.orderprice = newPrice;
				it->second.tradecount = 0;						
			}
			
			it->second.selfclose = '2';   //这里赋值2，表示可以下单

			sprintf_s(msg,256,"内部自成交(原,新),报单引用:%d,买卖(%c,%c),开平(%c,%c),价格(%.3f,%.3f)",ref,bs,it->second.bs,kp,it->second.kp,price,it->second.orderprice);

			AddLog(msg);

		}
	}
	//撤单过后新增报单
	else if(type == 1)
	{
		map<int,COrderInfo> ::iterator it = m_orderinfo.find(ref);

		if(it != m_orderinfo.end())
		{
			char bs = it->second.bs;
			char kp = it->second.kp;
			double price = it->second.orderprice;

			COrderInfo order;
			strcpy(order.szInstrument,m_strInstrument);
			order.ref = ref;
			order.ordercount = it->second.ordercount;
			order.orderprice = price;
			order.tradecount = 0;
			order.bs = bs;
			order.kp = kp;
			order.selfclose = '0';

			m_mapOrder[ref] = order;

			//买开成交
			if(bs == '0' && kp == '0')
			{
				order.bs = '1';
				order.kp = '3';
				order.orderprice = price + m_J1;
			}
			//卖开成交
			else if(bs == '1' && kp == '0')
			{
				order.bs = '0';
				order.kp = '3';
				order.orderprice = price - m_J1;
			}
			//买平成交
			else if(bs == '0' && (kp == '3' || kp == '1') )
			{
				order.bs = '1';
				order.kp = '0';
				order.orderprice = price + m_J1;
			}
			//卖平成交
			else if(bs == '1' && (kp == '3' || kp == '1'))
			{
				order.bs = '0';
				order.kp = '0';
				order.orderprice = price - m_J1;
			}

			sprintf_s(msg,256,"撤挂单,内部自成交(原,新),报单引用:%d,买卖(%c,%c),开平(%c,%c),价格(%.3f,%.3f)",ref,bs,order.bs,kp,order.kp,price,order.orderprice);

			AddLog(msg);

		}
	}

	return true;

}


void CStrategyGrid::OrderByMap(map<int,COrderInfo> orderMap)
{
	char msg[256];

	map<int,COrderInfo> ::iterator it;

	int ref = 0;

	for(it = orderMap.begin(); it!= orderMap.end();it++)
	{

		if(it->second.selfclose == '1')
		{
			sprintf(msg,"可能自成交,先跳过报单,报单引用%d",it->second.ref);
			AddLog(msg);
			continue;
		}
	
		int count = it->second.ordercount;
		double price = it->second.orderprice;
		int orderref = it->second.ref;

		if(it->second.bs == '0' && it->second.kp == '0')
		{
			ref = OpenBuy(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,it->second.orderprice,'0','0');
				
		}
		else if(it->second.bs=='1' && it->second.kp=='0' )
		{
			ref = OpenSale(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0));
			AddOrderInfo(ref,count,it->second.orderprice,'1','0');
		}
		else if(it->second.bs=='0' && it->second.kp!='0')
		{
			ref = CloseBuy(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,it->second.orderprice,'0',CLOSEFLAG_CLOSE_TODAY);
			
		}
		else if(it->second.bs=='1' && it->second.kp!='0')
		{
			ref = CloseSale(m_strInstrument.GetBuffer(0),count,it->second.orderprice,m_fundAccount.GetBuffer(0),CLOSEFLAG_CLOSE_TODAY);
			AddOrderInfo(ref,count,it->second.orderprice,'1',CLOSEFLAG_CLOSE_TODAY);
		}

		if(it->second.selfclose == '2')
		{
			sprintf(msg,"自成交队列中报单,原报单引用:%d,新生成报单引用%d,删除...",orderref,ref);
			AddLog(msg);
			it->second.selfclose = '0';
		}

		sprintf(msg,"止盈单生成,报单引用:%d",ref);
		AddLog(msg);

	}


}


void CStrategyGrid::OrderByInfo(COrderInfo orderinfo)
{
	map<int,COrderInfo> map;

	map[orderinfo.ref] = orderinfo;

	OrderByMap(map);

}