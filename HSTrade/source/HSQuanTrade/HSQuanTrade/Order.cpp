#include "stdafx.h"

#include "Order.h"

#include "BaseTradeApi.h"

#include "StrategyBase.h"

#include<windows.h>

#include "TradeInterface.h"

#include "tool.h"

#include "CTPTrade.h"

#include "FunctionPub.h"


COrder::COrder(char *account,char *instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref)
{

	strcpy(m_account,account);

	strcpy(m_szIntrument,instrument);

	m_count = count;

	m_direction = direction;

	m_offsetflag = offsetflag;

	m_price= price;

	m_nOrderType = ordertype;

	m_ntimeout = timeout;

	m_jiacha = jiacha;

	m_pApi = api;

	m_ptrade = ptrade;

	m_nOrderRef = orderref;

	m_bWithDrawING = false;

	m_iTradeCount = 0;

	m_nEntrustNo = 0;

	m_nOrderSysID = 0;

	m_status = 'w';				//表示初始值

	m_bDone = false;

	//超时撤单模式
	if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
	{

		m_tTrd = thread(std::bind(&COrder::DealOrder, this));

	}

	//记录生成时间
	time_t rawtime;
	time ( &rawtime );
	m_timestamp = (long)rawtime;


}


COrder::~COrder(void)
{
	m_bDone = true;

	//WRITE_LOG(1,"析构");

	if(m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT)
	{
		m_tTrd.join();
	}
}


/*
//还是放在下单应答里面处理
如果是支持报单引用的接口,orderref 就是报单引用, ordersysid一般无意义
如果不支持报单引用的接口,orderref 采用batch_no, ordersysid是委托号
CTP，期货UFX撤单回报走的是委托回报
股票期权UFX撤单成功是走的成交回报
*/
void COrder::RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char *ins)
{

	if(m_nOrderType == ORDER_TYPE_DONOTHING )
		return;

	if(m_bDone)
		return;

	char msg[128];

	CString code = ins;

	//CString bs,kp,st;
	//bs = Tool::TransBS(m_direction,m_pApi);
	//kp = Tool::TransKP(m_offsetflag,m_pApi);
	//st = Tool::TransStatus(status,m_pApi);

	string bs,kp,st;
	bs = m_pApi->TransBS(m_direction);
	kp = m_pApi->TransKP(m_offsetflag);
	st = m_pApi->TransStatus(status);

	//如果状态已经是终止状态 状态就不能改变  比如如果当前状态已成，不能在变成已报
	if(m_pApi->IsBuChe(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsYiChe(m_status) || m_pApi->IsYiCheng(m_status))
	{
		sprintf(msg,"委托回报->合约 %s  委托号 %d  当前状态状态 %s 为终止状态 本次回报状态 %s",code,m_nEntrustNo,m_pApi->TransStatus(m_status).c_str(),m_pApi->TransStatus(status).c_str());
		WRITE_LOG(1,msg);
		m_bDone = true;

#ifdef _DEBUG
		int id = GetCurrentThreadId();
		sprintf(msg,"1111线程id  %d",id);
		WRITE_LOG(1,msg);

#endif
		return;
		//m_status = status; 
	}
	else
	{
		m_status = status;
	}

	m_nOrderSysID = ordersysid;

	//如果m_nEntrustNo为0 ctp接口直接返回 没有应答
	//针对ctp接口
	if( (m_nEntrustNo==0) && (ordersysid >0) && (m_pApi->GetInterfaceType()==TRADE_CTP))
		m_nEntrustNo = orderref;


	sprintf(msg,"委托回报->合约 %s  委托号 %d  状态 %s  状态id %c",code,m_nEntrustNo,st.c_str(),status);

	WRITE_LOG(1,msg);

 
	if(m_nOrderType== ORDER_TYPE_ZUIJIA_NEWPRICE || m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA )  //6已撤,5部撤 需要追单
	{
		//if(Tool::isBuChe(m_status,m_pApi) ||  Tool::isYiChe(m_status,m_pApi))
		if(m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			if(!m_bWithDrawING/* || (ordersysid <=0) */)
			{
				sprintf(msg,"非订单管理撤单  合约 %s  委托引用 %d  本地单号 %d",code,m_nEntrustNo,ordersysid);
				WRITE_LOG(1,msg);
				return;
			}

			sprintf(msg,"追价->合约:%s,委托号:%d,收到撤单回报,追单",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//追单
			int newcount = m_count - m_iTradeCount;
			
			sprintf(msg,"追价->合约 %s %c %c,委托号 %d  数量 %d,价格 %.4f 追单...",m_szIntrument,bs1,kp1,m_nEntrustNo,newcount,m_price);

			WRITE_LOG(1,msg);

			int result = m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nOrderType,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;

			m_bWithDrawING = false;   //撤单完成，

			if(result == -1)
			{
				WRITE_LOG(1,"委托失败,原因可能是账户设置了保证金限额...");
				//m_bDone = true;//订单结束状态
			}

		
		}

	}
	else if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
	{
		//if(Tool::isYiChe(m_status,m_pApi))	
		if(m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"超时撤单->合约:%s,委托号:%d,收到撤单回报,订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;		
		}
		//else if(Tool::isBuChe(m_status,m_pApi))
		else if(m_pApi->IsBuChe(m_status))
		{
			sprintf(msg,"超时撤单->合约:%s,委托号:%d,收到部撤回报,订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;			
		}

	}

}


/*
//股票期权撤单成功是走的成交回报
//其他接口是走的委托回报
//这里判断成交价格来判断是否是成交回报还是撤单回报
CTP接口下套利单，成交回报是单合约回来两笔
*/
void COrder::RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	if( m_nOrderType == ORDER_TYPE_DONOTHING )
		return;

	if(m_bDone)
		return;

	char msg[128];

	string code = ins;

	string heyue = m_szIntrument;

	bool taoli = false;
	//套利合约 回报取第二腿
	if(heyue.find("&") != -1)
	{
		int index = heyue.find("&");

		string substr = heyue.substr(index+1);

		strcpy(m_szLeg2,substr.c_str());

		taoli = true;
	}

	//只有收到第二腿才累加成交数量
	if(taoli)
	{
		if(strcmp(m_szLeg2,ins)==0)
		{
			m_iTradeCount = m_iTradeCount + vol;
		}
		//收到第一腿就只是记录下就返回
		else
		{
			sprintf(msg,"成交回报->合约 %s  委托引用 %d  委托数量 %d  成交数量 %d  成交价格 %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);
			WRITE_LOG(1,msg);
			return;
		}
	
	}
	else
		m_iTradeCount = m_iTradeCount + vol;
	

	//成交回报
	if(price > 0.00001)
	{
		sprintf(msg,"成交回报->合约 %s  委托引用 %d  委托数量 %d  成交数量 %d  成交价格 %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);

		WRITE_LOG(1,msg);

		if( (m_iTradeCount==m_count))    //总成交数量==下单数量
		{

			//if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
			if(m_pApi->GetInterfaceType()==TRADE_CTP)
				m_status = '0';     //已成
			else
				m_status = '8';

			sprintf(msg,"成交回报->合约 %s  委托引用 %d  全部成交 订单终止",m_szIntrument,m_nEntrustNo);

			m_bDone = true;
			WRITE_LOG(1,msg);			

		}
		else if( (m_iTradeCount < m_count) )
		{

			//if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(m_pApi))
			if(m_pApi->GetInterfaceType()==TRADE_CTP)
				m_status = '1';     //部成 多笔回来，需要判断总成交数量
			else
				m_status = '7';     

			sprintf(msg,"成交回报->合约 %s  委托引用 %d  部分成交",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

		}

	}
	//认为成交价格为0  是撤单回报 
	else if(price < 0.00001)
	{
		sprintf(msg,"撤单回报->合约 %s  委托引用 %d  委托数量 %d  撤单数量 %d  成交价格 %.4f",code.c_str(),m_nEntrustNo,m_count,vol,price);

		WRITE_LOG(1,msg);

		if( m_nOrderType == ORDER_TYPE_ZUIJIA_NEWPRICE || m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA)
		{
			sprintf(msg,"追价->合约 %s  委托号 %d  收到撤单回报 追单",m_szIntrument,m_nEntrustNo);

			WRITE_LOG(1,msg);

			//数量为此次成交数量
			int newcount = vol;

			double newprice = m_price;
		
			sprintf(msg,"追价->合约 %s  委托号 %d  数量 %d  价格 %.4f  追单...",m_szIntrument,m_nEntrustNo,newcount,newprice);

			WRITE_LOG(1,msg);

			//test
			//if(m_nordertype!=2)
			int result=m_ptrade->InsertOrder(m_szIntrument,newcount,m_direction,m_offsetflag,m_price,m_nOrderType,m_ntimeout,m_jiacha,m_account);

			m_bDone = true;		

			m_bWithDrawING = false;   //撤单完成，

			if(result == -1)
			{
				WRITE_LOG(1,"委托失败,原因可能是账户设置了保证金限额...");
				//m_bDone = true;//订单结束状态
			}	
		
		}
		else if( m_nOrderType == ORDER_TYPE_TIMEOUT_ABORT )
		{
			sprintf(msg,"超时撤单->合约 %s  委托号 %d  收到撤单回报 订单终止",m_szIntrument,m_nEntrustNo);
			WRITE_LOG(1,msg);
			m_bDone = true;
			
		}
	
	}
}

/*
委托应答也会更新委托号
*/
void COrder::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{

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

		//201604   begin
		//投资赢家回报里面只有委托号 所以这里把委托引用更新为委托号 
		//这里就存在隐患，如果回报先于报单应答回来 委托引用就是原来的，不是委托号 无法匹配
		//m_nOrderRef = entrustno;
		//201604   end

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

		//本地单号
		m_nOrderSysID = entrustno;
		

	}

	m_status = entrust_status;

}


//处理超时则撤单类型
void COrder::DealOrder()
{
	//如果超时时间是0，就认为是当天有效不撤单
	if(m_ntimeout==0)
		return;

	if(m_nOrderType != ORDER_TYPE_TIMEOUT_ABORT )
		return;

	if(m_bDone)
		return;

	char msg[256];

	while (!m_bDone)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(m_ntimeout*1000));

		if(m_bDone)
			return;

		//必须先等委托应答或者回报回来
		if(m_nEntrustNo==0)
			continue;

		int ref = m_nOrderRef;

		//这里需要判断下订单状态
		//已成；废单;部撤；已撤但是不追单
		//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
		if(m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"超时撤单->委托引用 %d 合约 %s %c %c  价格 %.4f  数量 %d  状态 %s  线程终止",m_nEntrustNo,m_szIntrument,/*Tool::TransBS(m_direction,m_pApi)*/m_direction,/*Tool::TransKP(m_offsetflag,m_pApi)*/m_offsetflag,m_price,m_count,/*Tool::TransStatus(m_status,m_pApi)*/m_pApi->TransStatus(m_status).c_str());

			WRITE_LOG(1,msg);

			m_bDone = true;			

			return;    //已成或者撤单的话就退出线程

		}

		//撤单
		if(!m_bWithDrawING)
		{
			int result = m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			m_bWithDrawING = true;

			sprintf(msg,"超时撤单->委托引用 %d 合约 %s 发出撤单指令",m_nEntrustNo,m_szIntrument);

			WRITE_LOG(1,msg);

			if(result==-1)
			{
				WRITE_LOG(1,"撤单失败,可能是账户设置了撤单次数限制");
				m_bDone = true;//订单结束状态				
			}
		}
	}
	
}

/*
1 代表下单之后下一笔行情来了未成交 就撤单 然后对价追单
2 代表下单之后行情价差达到指定参数值，就撤单 然后对价追单
*/
void COrder::DealPrice(double buy1,double sale1)
{

	if(m_nOrderType != ORDER_TYPE_ZUIJIA_NEWPRICE && m_nOrderType != ORDER_TYPE_ZUIJIA_JIACHA)   //下单类型
		return;

	if (m_bWithDrawING)    //撤单未完成 不允许再次撤单;
		return;

	if(m_bDone)
		return;

	//必须先等委托应答或者回报回来
	if(m_nEntrustNo==0)
		return;

	char msg[128];

	//有些报单有报单撤单时间间隔监管
	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;
	bool timeout = false;

	if(timestamp - m_timestamp <m_ntimeout)
	{
		sprintf(msg,"报单时间与当前时间间隔小于 %d",m_ntimeout);

		WRITE_LOG(1,msg);
		//return;
	}
	{
		timeout = true;
	}

#ifdef _DEBUG
		int id = GetCurrentThreadId();
		sprintf(msg,"2222线程id  %d",id);
		WRITE_LOG(1,msg);

#endif


	//已成；已撤；部撤；废单  不允许撤单
	//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
	if( (m_pApi->IsYiCheng(m_status) && m_count==m_iTradeCount) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
	{
		m_bDone = true;		
		return;
	}

	

	//是否满足条件
	bool need = false; 

	double old_price = m_price;

	if(m_nOrderType == ORDER_TYPE_ZUIJIA_NEWPRICE)
	{
		if(m_direction == '0')
		{
			if( (sale1 > m_price) || timeout) 
			{				
				m_price = sale1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (buy1 < m_price) || timeout) 
			{				
				m_price = buy1;

				need = true;

			}
		}
	}
	else if( m_nOrderType == ORDER_TYPE_ZUIJIA_JIACHA )
	{
		if(m_direction == '0')
		{
			if( (sale1 - m_price) > m_jiacha ) 
			{								
				sprintf(msg,"模式2 卖一价与委托价差 %.4f  价差参数 %.4f",(sale1 - m_price),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = sale1;

				need = true;
			}
		}
		else if(m_direction == '1')
		{
			if( (m_price - buy1) > m_jiacha ) 
			{							
				sprintf(msg,"模式2 委托价与买一价差 %.4f  价差参数 %.4f",(m_price - buy1),m_jiacha);

				WRITE_LOG(1,msg);

				m_price = buy1;

				need = true;

			}
		}
	
	}

	if(need)
	{
		m_bWithDrawING = true;

		//test
		//if(m_nordertype!=2)
		int result=m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

		sprintf(msg,"追价->发出撤单指令  委托引用 %d  合约 %s  原价格 %.4f  新价格 %.4f 现买一价 %.4f 现卖一价 %.4f",m_nEntrustNo,m_szIntrument,old_price,m_price,buy1,sale1);
		
		WRITE_LOG(1,msg);

		if(result==-1)
		{
			WRITE_LOG(1,"撤单失败,可能是账户设置了撤单次数限制");
			m_bDone = true;//订单结束状态			
		}

#ifdef _DEBUG
		if(strcmp(m_szIntrument,"rb1610")==0)
		{
			int u =0;
			u++;	
		}
		Sleep(20);
#endif
		
	}


}


void COrder::QueryOrder()
{

	//沉睡100ms
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//仍然是初始值 需要查询委托
	if(m_status == 'w')
	{
		m_ptrade->QueryEntrust(m_account,0);
	}

}



void COrder::DealOrderEx()
{

	if(m_nOrderType != ORDER_TYPE_ZUIJIA_NEWPRICE &&  m_nOrderType != ORDER_TYPE_ZUIJIA_JIACHA)
		return;

	if(m_bDone)
		return;

	char msg[256];

	while (!m_bDone)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(m_ntimeout*1000));

		//必须先等委托应答或者回报回来
		if(m_nEntrustNo==0)
			continue;

		int ref = m_nOrderRef;

		//这里需要判断下订单状态
		//已成；废单;部撤；已撤但是不追单
		//if( Tool::isYiCheng(m_status,m_pApi) || Tool::isFeiDan(m_status,m_pApi)|| Tool::isBuChe(m_status,m_pApi) || Tool::isYiChe(m_status,m_pApi) )
		if(m_pApi->IsYiCheng(m_status) || m_pApi->IsFeiDan(m_status) || m_pApi->IsBuChe(m_status) || m_pApi->IsYiChe(m_status))
		{
			sprintf(msg,"其他模式超时撤单->委托引用 %d 合约 %s %c %c  价格 %.4f  数量 %d  状态 %s  线程终止",m_nEntrustNo,m_szIntrument,m_direction,m_offsetflag,m_price,m_count,m_pApi->TransStatus(m_status).c_str());

			WRITE_LOG(1,msg);

			m_bDone = true;

			return;    //已成或者撤单的话就退出线程

		}

		//撤单
		if(!m_bWithDrawING)
		{
			int result = m_ptrade->WithDraw(m_account,m_nEntrustNo,m_szIntrument);

			m_bWithDrawING = true;

			sprintf(msg,"超时撤单->委托引用 %d 合约 %s 发出撤单指令",m_nEntrustNo,m_szIntrument);

			WRITE_LOG(1,msg);

			//更新价格
			if(m_direction==POSTION_DIRECTION_BUY)
				m_price = m_price + CPubData::m_sFutuCodeInfo[m_szIntrument].PriceTick;
			else if(m_direction==POSTION_DIRECTION_SELL)
				m_price = m_price - CPubData::m_sFutuCodeInfo[m_szIntrument].PriceTick;

			if(result==-1)
			{
				WRITE_LOG(1,"撤单失败,可能是账户设置了撤单次数限制");
				m_bDone = true;//订单结束状态
				sprintf(msg,"%d 9",m_nOrderRef);
				WRITE_LOG(1,msg);
			}
		}
	}

}






