#include "stdafx.h"
#include "StrategyLimitupBuy_imp.h"
#include <map>
#include <string>
#include "..\hsquantrade\FunctionPub.h"
#include<time.h>


CStrategyLimitupBuy::CStrategyLimitupBuy()
{

	m_strStrategyName = "抢涨停板买入卖出";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //是否跟单,默认写个-1

	m_timeOut = 0;                     //超时时间

	jiacha = 0;					       //价差

	m_nCount = 0;

	m_sEntrustInfo = "";

	m_rate = 0.5;

	m_dChangeRate = 0.01;

	m_Exit = false;

	m_bRun = false;

	m_done = false;

	m_huigou = false;

	m_strParamList = "交易合约;数量;价格;交易方向;开平方向;买一买二比;换手率;委托信息;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("数量","0"));

	m_params.insert(pair<string, CString>("价格","0"));

	m_params.insert(pair<string, CString>("交易方向","0"));

	m_params.insert(pair<string, CString>("开平方向","0"));

	m_params.insert(pair<string, CString>("委托信息","0"));

	m_params.insert(pair<string, CString>("买一买二比","0.5"));

	m_params.insert(pair<string, CString>("换手率","0.01"));

	m_params.insert(pair<string, CString>("账户",""));

	m_params.insert(pair<string, CString>("是否追单","-1"));

	m_params.insert(pair<string, CString>("超时时间","0"));

	m_params.insert(pair<string, CString>("价差","0"));

	
}

CStrategyLimitupBuy::~CStrategyLimitupBuy()
{

	m_bRun = false;

	m_tWorkThread.join();

}


void CStrategyLimitupBuy::Tick(CTickData * pt)
{
	//if(strcmp(m_strInstrument,pt->szInstrument)!=0)
	//	return;

	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;


	double buy1 = pt->bidprice[0];
	double buy2 = pt->bidprice[1];
	double buy5 = pt->bidprice[4];
	double sale1 = pt->askprice[0];
	double sale2 = pt->askprice[1];
	double limitup = pt->UpperLimitPrice;
	double lastprice = pt->lastprice;
	char*code = pt->szInstrument;
	double buy1count = pt->bidvol[0];
	double buy2count = pt->bidvol[1];
	int sale1count = pt->askvol[0];

	m_hignPrice = pt->highestprice;  //最高价

	CString price;

	price.Format("合约:%s,最新价:%.2f,买一价:%.2f,买一量:%.1f,买二价:%.2f,买二量:%.1f,卖一价:%.2f,卖一量:%d,卖二价:%.2f,成交量:%ld"
			,code,lastprice,buy1,buy1count,buy2,buy2count,sale1,sale1count,sale2,pt->Volume);

	WRITE_LOG(1,price.GetBuffer(0));

	if( (data==NULL) || (!(*data).isReady) )
		return;

	//9:15左右没盘口不处理
	if(buy1 - 0.00 < 0.1)
		return;

	//9:20~9:30
	if(isCollectiveBidTime())
		return;

	std::vector<CEntrustInfo>::iterator it;
	for(it=(*data).m_sEntrustInfo.begin();it!=(*data).m_sEntrustInfo.end();it++)
	{
		if(strcmp( it->szInstrument,code)==0)
		{
			if(lastprice<limitup)
			{
				//撤单
				CString log;
				log.Format("合约:%s,最新价:%.3f,跌破涨停价,撤单",code,lastprice);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;
			}

			if(sale2>0)
			{
				//撤单
				CString log;
				log.Format("合约:%s,卖二价(%.3f) > 0,撤单",code,sale2);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;
			}

			if(buy1==sale1 && sale1==limitup && (buy1count/buy2count>=m_rate))
			{

				//撤单
				CString log;
				log.Format("合约:%s,买一(%f)/买二(%f)突破比例(%.3f),撤单",code,buy1count,buy2count,m_rate);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;

			}

			//涨停板上，换手率大于1%
			//防止基本数据没更新的情况
			double rate = 0.0;
			if(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument) > 10)
			{
				rate = pt->Volume/(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument));
			}
			if((pt->UpperLimitPrice == pt->lastprice) &&(  rate> m_dChangeRate) )
			{
				//撤单
				CString log;
				log.Format("合约:%s,当前换手率(%.3f)大于%.3f,撤单",code,rate,m_dChangeRate);
				AddLog(log.GetBuffer(0));
				OrderAction(it->entrustno);
				(*data).m_sEntrustInfo.erase(it);
				break;

			}
		
		}
	}

	//通过查询持仓卖出独立出dll了，这里注释掉
	/*
	//9:35前不操作
	if(isDoNothingTime())
	{
		return;
	}

	char msg[256];
	std::vector<CHoldsinfo>::iterator ithold;
	for(ithold=(*data).m_sHoldsInfo.begin();ithold!=(*data).m_sHoldsInfo.end();ithold++)
	{
		if(strcmp( ithold->szInstrument,code)==0)
		{
			//14:55，亏损5%市价卖出
			if( (isCloeTime()) && ( ithold->dTradePrice*(1-0.05) > pt->lastprice) )
			{
				playsound();

				//OpenSale(code,ithold->TdAvaiable,buy5,m_fundAccount.GetBuffer(0));

				sprintf(msg,"14:55,合约:%s,亏损大于0.05,市价卖出...成本价%.4f,当前价格%.4f",code,ithold->dTradePrice,pt->lastprice);

				AddLog(msg);

				(*data).m_sHoldsInfo.erase(ithold);

				break;
			}

			//最高价回调5%
			//if( (m_hignPrice * (1-0.05)) > pt->lastprice )
			if( (m_hignPrice * (1-0.05)) > pt->lastprice )
			{
				sprintf(msg,"合约:%s,最高价:%.4f,最新价:%.4f,回调超过比例,记录...",code,m_hignPrice,pt->lastprice);

				AddLog(msg);

				if(m_sSignalTime[code] == -9999 || m_sSignalTime[code] == 0 )
				{
					m_sSignalTime[code] = timestamp;
					break;
				}

				//持续一分钟的卖出信号
				if( (timestamp - m_sSignalTime[code] > 60) && (m_sSignalTime[code]!=0) )
				{
					playsound();

					sprintf(msg,"合约:%s,当前时刻:%ld,最初时刻%ld,持续一分钟,卖出...",code,timestamp,m_sSignalTime[code]);

					AddLog(msg);

					//OpenSale(code,ithold->TdAvaiable,pt->lastprice,m_fundAccount.GetBuffer(0));

					(*data).m_sHoldsInfo.erase(ithold);
				}

				break;
			}
			//没有卖出信号
			else
			{
				m_sSignalTime[code] = -9999;

				break;
			}
		}

	}
	*/


	//回购的行情
	if(m_huigou)
	{
		if(strcmp("131810",code)==0)
		{
			//1000的整数倍
			int count = (int(data->m_dEnableMoney / 1000)) * 10;

			AddLog("回购...");

			OpenSale(code,count,buy2,m_fundAccount.GetBuffer(0));

			m_huigou = false;
		}
	}


}


void CStrategyLimitupBuy::SetParam(CString paramname,CString value)
{
	//"交易合约;数量;价格;买卖方向;开平方向;委托信息;账户;";

	m_params[(LPCTSTR)paramname] = value;

	//还需要将对应的值更新
	if(paramname=="交易合约")
	{
		m_strInstrument = value;
	}
	else if(paramname=="数量")
	{
		m_nCount = atoi(value);
	}
	else if(paramname=="价格")
	{
		m_fOpenPrice = atof(value);
	}
	else if(paramname=="买卖方向")
	{
		m_kp = ( (char*)value.GetBuffer())[0];
	}
	else if(paramname=="开平方向")
	{
		m_kp = ( (char*)value.GetBuffer())[0];
	}
	else if(paramname=="委托信息")
	{
		m_sEntrustInfo = value;
	}
	else if(paramname=="买一买二比")
	{
		m_rate = atof(value);
	}
	else if(paramname=="换手率")
	{
		m_dChangeRate = atof(value);
	}
	else if(paramname=="账户")
	{
		m_fundAccount = value;
	}

}


void CStrategyLimitupBuy::SetParam(CString value)
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

	//m_strModelName = m_strStrategyName;
	//m_sModuleId = m_strStrategyName;

}

//"交易合约;数量;价格;交易方向;开平方向;买一买二比;换手率;委托信息;账户;";
CString CStrategyLimitupBuy::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%.4f;%d;%d;%.3f;%.3f;%s;%s;",m_strInstrument,m_nCount,0.0,0,0,m_rate,m_dChangeRate,m_sEntrustInfo,m_fundAccount);

	return str;
}


CString CStrategyLimitupBuy::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupBuy::GetStatusInfo(void)
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

		openrice.Format("%.4f",m_fOpenPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "买开";

		openrice.Format("%.4f",m_fOpenPrice);

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "卖开";

		openrice.Format("%.4f",m_fOpenPrice);

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

	if( m_bs == 0 )
	{
		
	}
	else if( m_bs == 1 )
	{
		
	}
	

	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%.4f;%s;%.4f;%.4f(%s);%s;%s;",m_strStrategyName,m_strInstrument,codename,
		m_nCount,status,"0","0","0",0.0,modelstatus,0.0,0.0,"-",m_fundAccount,m_sStrategyId);

	//result = "网格交易策略TF1512做多;TF1512;TF1512;10;买开;100;0;空仓;0;0;041249,;";
	return result;

}

void CStrategyLimitupBuy::BeginRun()
{
	
	//下单
	if(strcmp(m_strInstrument,"-6666")==0)
	{
		m_strType = 1;
	}
	//监控
	else if(strcmp(m_strInstrument,"-9999")==0)
	{
		m_strType = 2;
	}
	//回购
	else if(strcmp(m_sEntrustInfo,"-1")==0 )
	{
		m_strType = 3;
	}
	//其他
	else
	{
		m_strType = -1;
	}


	m_tWorkThread = thread(bind(&CStrategyLimitupBuy::process,this));

}


void CStrategyLimitupBuy::order()
{

	//解析委托信息串,格式:code,count,price|code,count,price|

	if(m_sEntrustInfo=="")
		return;
	
	CStringArray list;

	CFunctionPub pub;

	CString code,count,price;

	pub.ParseParam(m_sEntrustInfo,list,'|');

	for(int i = 0;i<list.GetSize();i++)
	{
		CString str = list[i];
		CStringArray orderinfo;

		pub.ParseParam(str,orderinfo,',');

		code = orderinfo[0];

		count = orderinfo[1];

		price = orderinfo[2]; 

		if(code=="" || count=="" || price=="")
			continue;

		int orderref = OpenBuy(code.GetBuffer(0),atoi(count),atof(price),m_fundAccount.GetBuffer(0));

		COrderInfo *order = new COrderInfo();
		strcpy(order->szInstrument,code.GetBuffer(0));
		order->ref = orderref;
		order->ordercount = atoi(count);
		order->tradecount = 0;
		order->orderprice = atof(price);
		order->tradeprice = 0;
		order->bs = '0';
		order->kp = '0';
		m_orderinfo[orderref] = *order;
		delete order;

		std::this_thread::sleep_for(std::chrono::milliseconds(2*1000));
		
	}

}

void CStrategyLimitupBuy::process()
{

	if(data==NULL)
	{
		AddLog("帐号没找到...");
		return;

	}
	while(!m_Exit)
	{
		if(m_strType==1)
		{
			if(isOrderTime())
			{
				if(!m_done)
				{
					//下单
					order();  
					m_done = true;
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

					//查委托
					(*data).isReady = false;
					QueryOrder();      
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

					//查询持仓并监控
					/*(*data).isReady = false;
					QueryPosition();
					std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));*/

				}
			}

			if(isWithDrawTime())
			{
				BussInessDeal(0);

				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));
		}
		//监控挂单
		else if(m_strType==2)
		{
			if(!m_done)
			{
				m_done = true;
				//查委托
				(*data).isReady = false;
				QueryOrder();      
				std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

				//查询持仓并监控
				/*(*data).isReady = false;
				QueryPosition();
				std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));*/

			}

			if(isWithDrawTime())
			{
				BussInessDeal(0);

				return;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10*1000));

		}

		//下午回购
		else if(m_strType==3)
		{
			BussInessDeal(0);

			return;

		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));
		}

		//增加定时查询，避免恒泰长时间不操作导致服务器断开连接
		{
			//AddLog("隔500s,定时查询...");
			//QueryFund();
			//std::this_thread::sleep_for(std::chrono::milliseconds(500*1000));

		}
	}


}

bool CStrategyLimitupBuy::isOrderTime()
{

	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if(h==8)
	{
		ret =  true;
	}
	else if(h==9 && m<12)
	{
		ret =  true;
	}

	//return ret;

	return true;

}

bool CStrategyLimitupBuy::isWithDrawTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if( (h==14) && (0<m) && (m<10) )
	{
		ret =  true;
	}


	return ret;
	//return true;

}


bool CStrategyLimitupBuy::isDoNothingTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;


	if(h==9 && m<=35)
	{
		ret =  true;
	}

	return ret;

}

bool CStrategyLimitupBuy::isCloeTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

    if(h==14 && m>=55)
	{
		ret =  true;
	}

	return ret;

}

bool CStrategyLimitupBuy::isCollectiveBidTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

    if(h==9 && m>=20 && m<30)
	{
		ret =  true;
	}

	return ret;
}


//处理回报
void CStrategyLimitupBuy::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{


}


void CStrategyLimitupBuy::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}


void CStrategyLimitupBuy::withDraw()
{
	m_entrustMutex.lock();

	std::vector<CEntrustInfo>::iterator it;
	for(it=(*data).m_sEntrustInfo.begin();it!=(*data).m_sEntrustInfo.end();it++)
	{
		int entrustno = it->entrustno;

		CString log;
		log.Format("回购前撤单,然后删除,委托号:%d...",entrustno);
		AddLog(log.GetBuffer(0));
		
		OrderAction(entrustno);

		//(*data).m_sEntrustInfo.erase(it);

		std::this_thread::sleep_for(std::chrono::milliseconds(1*1000));
	}

	(*data).m_sEntrustInfo.clear();

	m_entrustMutex.unlock();

}

void CStrategyLimitupBuy::BussInessDeal(int type)
{
	//查询未成交委托
	(*data).isReady = false;
	QueryOrder();
	std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

	//撤单
	withDraw();
	std::this_thread::sleep_for(std::chrono::milliseconds(5*1000));

	//查询资金
	QueryFund();
	std::this_thread::sleep_for(std::chrono::milliseconds(3*1000));

	//可以回购
	m_huigou = true;

	return;
}
