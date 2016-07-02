#include "stdafx.h"

#include "COrderEx.h"

#include "BaseTradeApi.h"

#include "StrategyBase.h"

#include<windows.h>

#include "TradeInterface.h"

#include "tool.h"

#include "CTPTrade.h"

//////////////
//COrderEx
//////////////
COrderEx::COrderEx(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref)
{
	
	strcpy(m_account,account);

	strcpy(m_szIntrument,instrument);

	m_count = count;

	m_direction = direction;

	m_offsetflag = offsetflag;

	m_price= price;

	m_nordertype = ordertype;

	m_ntimeout = timeout;

	m_jiacha = jiacha;

	m_pApi = api;

	m_ptrade = ptrade;

	m_nOrderRef = orderref;

	m_bWithDrawING = false;

	m_iTradeCount = 0;

	m_nEntrustNo = 0;

	m_status = 'w';				//表示初始值

	m_bDone = false;

	m_bReOrder = false;

	time_t rawtime;
	time ( &rawtime );
	m_timestamp = (long)rawtime;

	
}


COrderEx::~COrderEx()
{
	m_bDone = true;
}


void COrderEx::RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins)
{
	if(strcmp(m_szIntrument,ins) !=0)
		return ;

	if(m_nordertype==-1)
		return;

	if(m_bDone)
		return;

	char msg[128];

	CString code = ins;

	CString bs,kp,st;

	bs = Tool::TransBS(m_direction,m_pApi);

	kp = Tool::TransKP(m_offsetflag,m_pApi);

	st = Tool::TransStatus(status,m_pApi);

	m_status = status; 


	//如果m_nEntrustNo为0 ctp接口直接返回 没有应答
	//针对ctp接口
	if( (m_nEntrustNo==0) && (ordersysid >0) && (m_pApi->GetInterfaceType()==TRADE_CTP))
		m_nEntrustNo = orderref;


	sprintf(msg,"委托回报->合约 %s  委托号 %d  状态 %s  状态id %c",code,m_nEntrustNo,st,status);

	WRITE_LOG(1,msg);

 
	if(m_nordertype== 1 || m_nordertype== 2)  //6已撤,5部撤 需要追单
	{
		if(m_pApi->IsBuChe(status) || m_pApi->IsYiChe(status))
		{
			sprintf(msg,"追价->合约:%s,委托号:%d,收到撤单回报,追单",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//追单
			int newcount = m_count - m_iTradeCount;
			
			sprintf(msg,"追价->合约 %s %s %s,委托号 %d  数量 %d,价格 %.4f 追单...",m_szIntrument,bs,kp,m_nEntrustNo,newcount,m_price);

			WRITE_LOG(1,msg);

			m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nordertype,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //撤单完成，
		

		
		}

	}
	else if(m_nordertype==0)
	{	
		if(m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"超时撤单->合约:%s,委托号:%d,收到撤单回报,订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}
		else if(m_pApi->IsBuChe(m_status) )
		{
			sprintf(msg,"超时撤单->合约:%s,委托号:%d,收到部撤回报,订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}

	}

}

void COrderEx::RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	if(strcmp(m_szIntrument,ins) !=0)
		return ;

	if(m_nordertype==-1)
		return;

	char msg[128];

	CString code = ins;

	m_iTradeCount = m_iTradeCount + vol;
	
	sprintf(msg,"成交回报->合约 %s  委托引用 %d  委托数量 %d  成交数量 %d  成交价格 %.4f",code,m_nEntrustNo,m_count,vol,price);

	WRITE_LOG(1,msg);

	if( (m_iTradeCount==m_count)  && (price > 0.00001) )    //总成交数量==下单数量
	{
		sprintf(msg,"成交回报->委托引用 %d  全部成交 订单终止",m_nEntrustNo);

		m_bDone = true;

		WRITE_LOG(1,msg);

	}
	else if( (m_iTradeCount < m_count) && (price > 0.00001) )
	{
		sprintf(msg,"成交回报->合约 %s  委托引用 %d  部分成交",code,m_nEntrustNo);

		WRITE_LOG(1,msg);

		//m_bDone = true;
	}

	if(m_bDone)
		return;

	//认为成交价格为0  是撤单回报 
	if(price < 0.00001)
	{
		if( m_nordertype==1 || m_nordertype==2)
		{

			sprintf(msg,"追价->合约 %s  委托号 %d  收到撤单回报 追单",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//追单 数量为此次成交数量
			int newcount = vol;

			double old_price = m_price;

			double newprice = 0.0;

			if(m_direction == '0')
				newprice = m_price /*+ m_jiacha*/;
			else if(m_direction == '1')
				newprice = m_price /*- m_jiacha*/;
			
			sprintf(msg,"追价->合约 %s  委托号 %d  数量 %d  价格 %.4f(原价格 %.4f)  追单...",m_szIntrument,m_nEntrustNo,newcount,newprice,old_price);

			WRITE_LOG(1,msg);

			//test
			//if(m_nordertype!=2)
			m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nordertype,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //撤单完成，
				
		}
		else if(m_nordertype==0)
		{
			sprintf(msg,"超时撤单->合约 %s  委托号 %d  收到撤单回报 订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
		}
	
	}

}

/*解决不支持报单引用的接口
	  entrustno != -9999 表示不支持报单引用需要更新entrustno
*/
void COrderEx::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	/*if(strcmp(m_szIntrument,ins) !=0)
		return ;*/

	char msg[256];

	if(errorid !=0 )
	{
		m_bDone = true;
		return;
	}

	//支持报单引用的接口
	if(entrustno==-9999)
	{
		sprintf(msg,"报单应答->报单引用 %d  状态 %c ",m_nOrderRef,entrust_status);
		WRITE_LOG(1,msg);
		m_nEntrustNo = m_nOrderRef;
	}
	else
	{
		sprintf(msg,"报单应答->委托号 %d  状态 %c ",entrustno,entrust_status);
		WRITE_LOG(1,msg);
		m_nEntrustNo = entrustno;
		//投资赢家回报里面只有委托号 所以这里把委托引用更新为委托号 
		//这里就存在隐患，如果回报先于报单应答回来 委托引用就是原来的，不是委托号 无法匹配
		//m_nOrderRef = entrustno;

		//20160628 begin
		//之前这里为了兼容投资赢家  这样更改m_nOrderRef = entrustno 在主推回调里面 返回的是两个 entrustno  entrustno来判断
		//存在隐患 修改为下面

		//投资赢家智能接口
		if(m_pApi->GetInterfaceType()==TRADE_UFX_AUTO)
		{
			m_nOrderRef = entrustno;
		}
		else
		{
		
		}

	}

	m_status = entrust_status;

}


bool COrderEx::DealPrice(CTickData *pd)
{
	if(strcmp(m_szIntrument,pd->szInstrument) !=0)
		return true;

	bool ret = true;

	double bid1 = pd->bidprice[0];
	double bid2 = pd->bidprice[1];
	double ask1 = pd->askprice[0];
	double ask2 = pd->askprice[1];

	if(m_nordertype !=ORDER_TYPE_ZUIJIA_NEWPRICE && m_nordertype !=ORDER_TYPE_ZUIJIA_JIACHA)   //下单类型
		return true;

	if (m_bWithDrawING)    //撤单未完成 不允许再次撤单;
		return true;

	if(m_bDone)
		return true;

	//必须先等委托应答或者回报回来
	if(m_nEntrustNo==0)
		return true;

	char msg[128];

	//有些报单有报单撤单时间间隔监管
	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;

	if(timestamp - m_timestamp <m_ntimeout)
	{
		sprintf(msg,"报单时间与当前时间间隔小于 %d",m_ntimeout);

		WRITE_LOG(1,msg);
		//return true;
	}


	//已成；已撤；部撤；废单  不允许撤单
	if( m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status)|| m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status) )
	{
		m_bDone = true;
		return true;
	}

	

	//是否满足条件
	bool need = false; 

	double old_price = m_price;

	if(m_nordertype == ORDER_TYPE_ZUIJIA_NEWPRICE)
	{
		if(m_direction == '0')
		{
			if( (ask1 > m_price) ) 
			{	
				//价差是否过大
				if( (ask1 - m_price) >= (2*m_jiacha))
				{
					sprintf(msg,"报单 %d  滑点 %.4f  允许价差 %.4f",m_nEntrustNo,ask1 - m_price,2*m_jiacha);
					WRITE_LOG(1,msg);
					ret = false;
				}

				m_price = ask1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (bid1 < m_price) ) 
			{	
				//价差是否过大
				if( (m_price - bid1) >= (2*m_jiacha))
				{
					sprintf(msg,"报单 %d  滑点 %.4f  允许价差 %.4f",m_nEntrustNo,m_price - bid1,2*m_jiacha);
					WRITE_LOG(1,msg);
					ret = false;
				}

				m_price = bid1;

				need = true;
			}
		}
	}
	else if(m_nordertype == ORDER_TYPE_ZUIJIA_JIACHA)
	{
		if(m_direction == '0')
		{
			if( (ask1 - m_price) > m_jiacha ) 
			{								
				sprintf(msg,"模式2 卖一价与委托价差 %.4f  价差参数 %.4f",(ask1 - m_price),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = ask1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (m_price - bid1) > m_jiacha ) 
			{							
				sprintf(msg,"模式2 委托价与买一价差 %.4f  价差参数 %.4f",(m_price - bid1),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = bid1;

				need = true;

			}
		}
	
	}

	if(need)
	{
		if(!m_bWithDrawING)
		{

			//m_bWithDrawING = true;

			//m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			if(WithDraw(m_nEntrustNo))
			{
				sprintf(msg,"追价->发出撤单指令  委托引用 %d  合约 %s  原价格 %.4f  新价格 %.4f 现买一价 %.4f 现卖一价 %.4f",m_nEntrustNo,m_szIntrument,old_price,m_price,bid1,ask1);
		
				WRITE_LOG(1,msg);
			}

		}
		else
		{
			sprintf(msg,"报单 %d  追价撤单失败，该报单已经处于撤单状态",m_nEntrustNo);
			WRITE_LOG(1,msg);
		}

	}

	return ret;
}


bool COrderEx::IsDone()
{
	return m_bDone;
}


void COrderEx::SetDone(bool done)
{
	m_bDone = done;

}


bool COrderEx::IsWithDraw()
{

	return m_bWithDrawING;
}


void COrderEx::SetWithDraw(bool withdraw)
{
	m_bWithDrawING = withdraw;
}


bool COrderEx::IsReOrder()
{
	return m_bReOrder;
}


void COrderEx::SetReOrder(bool order)
{
	m_bReOrder = order;
}


void COrderEx::RervseOrder(CTickData *pd)
{

	if(strcmp(m_szIntrument,pd->szInstrument) !=0)
		return ;

	//下单数量是成交数量
	//int count = m_iTradeCount;
	//如果出现过追单 报单是撤单 反向开仓数量如果用成交数量是0
	int count = m_count;

	char bs = '0';
	char kp = '0';

	double newprice = 0;

	if(m_direction == '0')
	{
		bs = '1';
		newprice = pd->bidprice[0];

	}
	else if(m_direction == '1')
	{
		bs = '0';
		newprice = pd->askprice[0];
	}

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
	{
		if(m_offsetflag == '0')
			kp = '3';
	}
	else
	{
		if(m_offsetflag == '0')
				kp = '1';
	}

	char msg[256];
	sprintf(msg,"平仓->报单 %d  合约 %s  原价格 %.4f  原数量 %d  新开仓价格 %.4f  新开仓数量 %d",m_nOrderRef,m_szIntrument,m_price,m_count,newprice,count);
	WRITE_LOG(1,msg);
	
	m_ptrade->InsertOrder(m_szIntrument,count,bs,kp,newprice,m_nordertype,m_ntimeout,m_jiacha,m_account);

	m_bReOrder = true;
}



int COrderEx::WithDraw(int ref)
{
	int ret = 1;
	m_lockWithDraw.lock();

	if( (ref == m_nEntrustNo) && (!m_bWithDrawING))
	{
		m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);
		char msg[128];
		sprintf(msg,"撤单 %d",m_nEntrustNo);
		WRITE_LOG(1,msg);
		m_bWithDrawING = true;
		ret = -1;
	}

	m_lockWithDraw.unlock();
	return ret;
}


int COrderEx::GetEntrustAmount()
{
	return m_count;
}


int COrderEx::GetTradeAmount()
{
	return m_iTradeCount;
}


double COrderEx::GetEntrustPrice()
{
	return m_price;
}



//////////////////////
//COrderGroup
//////////////////////
COrderGroup::COrderGroup(int id)
{
	m_iGrpId = id;
}



COrderGroup::~COrderGroup()
{


}


void COrderGroup:: OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{

	bool found = false;
	if( m_mapOrder.end() != m_mapOrder.find(ref) )
	{
		m_mapOrder[ref]->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);
		found = true;
	}

	//20160615
	//为了兼容投资赢家的接口 什么都干的出来。
	//ctp接口报单正常情况没有OnRspOrderInsert调用


	//20160629  原来是兼容投资赢家m_nOrderRef = entrustno;
	//现在只会在只能接口这样做  所以用不到下面了  将注释掉

	//if(found && errorid==0)
	//{
	//	m_mapOrder[entrustno] =  m_mapOrder[ref];
	//	m_mapOrder.erase(ref);
	//	char msg[128];
	//	sprintf(msg,"原报单 %d 更新为 %d",ref,entrustno);
	//	WRITE_LOG(1,msg);
	//}

}


void COrderGroup::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins)
{
	char msg[256];

	if( m_mapOrder.end() != m_mapOrder.find(orderref) )
	{
		//ctp接口撤单走委托回报	
		bool need = false;
		if(m_mapOrder[orderref]->m_pApi->IsYiChe(status))
		{
			for(unsigned int i=0;i<m_vecChedan.size(); i++)
			{
				//存在撤单不追单队列
				if(orderref == m_vecChedan[i])
				{
					sprintf(msg,"报单 %d 由于价差过大，撤单过后不再追单...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);
					need = true;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i])
				{
					sprintf(msg,"报单 %d 未成交报单撤单成功,撤单过后不再追单...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);
					m_lockNotTrade.unlock();
					return;
				}		
			}
			m_lockNotTrade.unlock();


			if(need)
			{
				DealReverse(orderref);
				return;
			}
		}


		//如果未成交报单撤单失败，反而该报单成交了	
		/*
		if(m_mapOrder[orderref]->m_pApi->IsYiCheng(status))
		{
			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{			
				if(orderref == m_vecNotTrade[i] && m_mapOrder[orderref]->IsWithDraw())
				{
					sprintf(msg,"报单 %d 未成交报单撤单失败，该报单成交了...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);
				
					if(!m_mapOrder[orderref]->IsReOrder())
					{
						string code = m_mapOrder[orderref]->m_szIntrument;
						m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					}
					m_lockNotTrade.unlock();
					return;
				}
				
			}

			m_lockNotTrade.unlock();
		}*/
		

		m_mapOrder[orderref]->RtnOrder(orderref,ordersysid,status,bs,kp,ins);
	}

}


void COrderGroup::RtnTrade(int orderref,int ordersysid,double price,char bs,char kp,char * tradetime,int vol,char *ins)
{
	char msg[256];

	if( m_mapOrder.end() != m_mapOrder.find(orderref) )
	{

		bool need = false;

		//表示撤单回报
		if(price < 0.0000001)
		{
			for(unsigned int i=0;i<m_vecChedan.size(); i++)
			{
				//存在撤单不追单队列
				if(orderref == m_vecChedan[i])
				{
					sprintf(msg,"报单 %d 由于价差过大，撤单过后不再追单...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);
					need = true;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i])
				{
					sprintf(msg,"报单 %d 未成交报单撤单成功,终止...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);					
					m_lockNotTrade.unlock();
					return;
				}
			
			}
			m_lockNotTrade.unlock();

		}

		if(need)
		{
			DealReverse(orderref);
			return;
		}

		//如果未成交报单撤单失败，反而该报单成交了
		int temp = m_mapOrder[orderref]->GetTradeAmount();
		temp += vol;
		//成交回报 按组下单只会下一笔
		if(/*temp==m_mapOrder[orderref]->GetTradeAmount() &&*/ (price  > 0.000001) )
		{
			m_lockNotTrade.lock();
			for(unsigned int i=0;i<m_vecNotTrade.size(); i++)
			{
				if(orderref == m_vecNotTrade[i] && m_mapOrder[orderref]->IsWithDraw())
				{
					sprintf(msg,"报单 %d 未成交报单撤单失败，该报单成交了...",orderref);
					WRITE_LOG(1,msg);
					m_mapOrder[orderref]->SetDone(true);//终止状态
					m_mapOrder[orderref]->SetWithDraw(false);
				
					if(!m_mapOrder[orderref]->IsReOrder())
					{
						string code = m_mapOrder[orderref]->m_szIntrument;
						m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					}

					m_lockNotTrade.unlock();
					return;
				}
			
			}
			m_lockNotTrade.unlock();

			//如果前面都撤单了，后面成交过来 需要反向平仓
			if( (m_vecNotTrade.size()>0) || (m_vecChedan.size() >0) )
			{
				if(!m_mapOrder[orderref]->IsReOrder())
				{
					string code = ins;
					m_mapOrder[orderref]->RervseOrder(&m_mapTick[code]);
					sprintf(msg,"报单 %d  后面报单成交 需要反向平仓",orderref);
					WRITE_LOG(1,msg);
				}

			}
		}


		m_mapOrder[orderref]->RtnTrade(orderref,ordersysid,price,bs,kp,tradetime,vol,ins);
	}

}


/*
出现需要追单的情况，如果价差过大，按照如下处理
1.组内其他订单成交了 就平仓
2.如果没成交就撤单
*/
void COrderGroup:: Tick(CTickData *pd)
{

#ifdef _DEBUG

	char msg[256];
	sprintf(msg,"收到tick  %s",pd->szInstrument);
	//WRITE_LOG(1,msg);

#endif 
	string code = pd->szInstrument;
	m_mapTick[code] = (*pd);

	map<int,COrderEx*>::iterator it = m_mapOrder.begin();
	for(;it != m_mapOrder.end();it++)
	{
		if(strcmp(pd->szInstrument,it->second->m_szIntrument)!=0)
			continue;

		//不是终止状态
		if(!it->second->IsDone())
		{
			if(!it->second->DealPrice(pd))
			{
				//追价价差很大
				m_vecChedan.push_back(it->first);

				return;
			}
		}
	}

}


void COrderGroup::AddOrder(int ref, COrderEx *order)
{
	m_mapOrder[ref] = order;
}


int COrderGroup::GetGrpId()
{
	return m_iGrpId;
}


void COrderGroup::DealReverse(int orderref)
{
	char msg[256];

	//对其他报单进行撤单或者反向开仓
	map<int,COrderEx*>::iterator iter = m_mapOrder.begin();
	for(;iter != m_mapOrder.end();iter++)
	{
		int ref = iter->first;

		//非终止状态撤单，并且当前没有处于撤单状态
		if(!(iter->second->IsDone()))
		{
			if(!m_mapOrder[ref]->IsWithDraw())
			{
				iter->second->WithDraw(ref);
				m_lockNotTrade.lock();
				m_vecNotTrade.push_back(ref);	
				m_lockNotTrade.unlock();
				sprintf(msg,"报单 %d  添加进入未成交报单队列...",ref);
				WRITE_LOG(1,msg);
			}
			else
			{
				sprintf(msg,"报单 %d  正在撤单...",ref);
				WRITE_LOG(1,msg);
				continue;
			}
		}
		//终止状态平仓
		else
		{
			bool found = false;
			for(unsigned int k=0;k<m_vecChedan.size(); k++)
			{
				if( (ref == m_vecChedan[k]) )
				{
					found = true;
					break;
				}
			}

			m_lockNotTrade.lock();
			for(unsigned int k=0;k<m_vecNotTrade.size(); k++)
			{
				if( (ref == m_vecNotTrade[k]) )
				{
					found = true;
					break;
				}
			}
			m_lockNotTrade.unlock();

			if(found)
				continue;

			//不在撤单队列就平仓
			if(!m_mapOrder[ref]->IsReOrder())
			{
				string code = m_mapOrder[ref]->m_szIntrument;
				if( m_mapTick.end() !=m_mapTick.find(code) )
					m_mapOrder[ref]->RervseOrder(&m_mapTick[code]);
				else
				{
					CTickData pd;
					strcpy(pd.szInstrument,code.c_str());
					pd.askprice[0] = m_mapOrder[ref]->GetEntrustPrice();
					pd.bidprice[0] = m_mapOrder[ref]->GetEntrustPrice();
					m_mapOrder[ref]->RervseOrder(&pd);
					sprintf(msg,"报单 %d  当前没有 %s 的行情，按照原委托价  %.4f  报单...",ref,code.c_str(),m_mapOrder[ref]->GetEntrustPrice());
					WRITE_LOG(1,msg);
				}
			}

		}
			
	}

}


