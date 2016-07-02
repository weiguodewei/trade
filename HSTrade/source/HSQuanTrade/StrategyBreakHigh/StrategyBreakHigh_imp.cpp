#include "stdafx.h"
#include "StrategyBreakHigh_imp.h"
#include "..\HSQuanTrade\typedefine.h"
#include "..\HSQuanTrade\FunctionPub.h"
#include <time.h>
#include "..\HSQuanTrade\tool.h"
#include "tinyxml.h"


CStrategyBreakHigh::CStrategyBreakHigh()
{

	m_strStrategyName = "新高突破买入";
	
	m_nStrategyStatus = STRATEGY_STATUS_INIT;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //是否跟单,默认写个-1

	m_timeOut = 0;                     //超时时间

	m_jiacha = 0;					       //价差

	m_nCount = 0;

	m_Exit = false;

	m_bRun = false;


////////本次参数
	m_iCycle = 30;

	m_iPriceDay = 20;

	m_dFluctuateRange = 0.3;

	m_dAbsoluteStop = 0.08;

	m_dMoveStop = 0.1;

	m_preHignPrice = 0;

	m_dTradePrice = 0.0;

	m_iTradeCount = 0;

	m_bDaPan = true;

	m_dStartFund = 1000000;
	
	strcpy(m_sCurDate,"-9999999");
	strcpy(m_sCurCode,"-6666");
	
	m_strInstrument = "";
								
	m_iInType = 0;

	m_dPrePriceS = 0;

	//m_xiaopan = 0;	

	m_fyingkuiPosition = 0;

	m_junxian = 1;	

	m_strParamList = "交易合约;日线周期;新高天数;震荡幅度;止损率;回调率;止盈率;初始权益;同时开仓数;手续费率;是否复利;是否下单;是否参考20日均线;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("日线周期","30"));

	m_params.insert(pair<string, CString>("新高天数","20"));

	m_params.insert(pair<string, CString>("震荡幅度","0.3"));

	m_params.insert(pair<string, CString>("止损率","0.08"));

	m_params.insert(pair<string, CString>("回调率","0.1"));

	m_params.insert(pair<string, CString>("初始权益","1000000"));

	m_params.insert(pair<string, CString>("同时开仓数","10"));

	m_params.insert(pair<string, CString>("账户",""));

	m_params.insert(pair<string, CString>("是否追单","-1"));

	m_params.insert(pair<string, CString>("超时时间","0"));

	m_params.insert(pair<string, CString>("价差","0"));

	m_params.insert(pair<string, CString>("手续费率","0.001"));

	m_params.insert(pair<string, CString>("是否复利","0"));
	
	m_params.insert(pair<string, CString>("止盈率","0.2"));

	m_params.insert(pair<string, CString>("是否参考20日均线","1"));

	m_params.insert(pair<string, CString>("是否下单", "0"));


	m_pPolicy = CFundManager::GetPolicyInstance(FUND_POLICY_STOCK_AVG);
}

void CStrategyBreakHigh::CreateStatisticObject()
{
	m_pStatistic = new CStockTradeStatistic();
}

CStrategyBreakHigh::~CStrategyBreakHigh()
{
	m_bRun = false;

	////test
	//m_vHold.push_back("601288");
	//m_mapHighPrice["601288"] = 10;
	//OpenBuy("601288", 100, 2.9, m_fundAccount.GetBuffer(0));
	//this->SetAvgPrice("601288",2.9);
	//this->SetStockCount("601288",100);
	//OpenBuy("600000", 100, 2.9, m_fundAccount.GetBuffer(0));
	//this->SetAvgPrice("600000",4.9);
	//this->SetStockCount("600000",200);
	////


	//saveModel();

}



void CStrategyBreakHigh::Tick(CTickData * pt)
{
	if(!m_bRun)
		return;

	char msg[256];

	if (!IsRightTickTime(pt->szTime))
	{
		return;
	}
	
	/*
	SYSTEMTIME st;
	GetLocalTime(&st);

	int m_ihour = st.wHour;
	int m_iMinutes = st.wMinute;
	int m_iSeconds = st.wSecond;
	int m_Milliseconds = st.wMilliseconds;
			
	char test[128];
	sprintf(test,"22222222   合约 %s  更新点  %s  更新时间%s  处理时间%d:%d:%d:%d",pt->szInstrument,pt->szTime,pt->time,m_ihour,m_iMinutes,m_iSeconds,m_Milliseconds);
	WRITE_LOG(1,test);*/


	if(strcmp("600570",pt->szInstrument)==0)
	{
		AddLog("600570不买...");
		return;
	}

	//涨停不买
	if(pt->UpperLimitPrice == pt->lastprice)
	{
		//sprintf(msg,"合约:%s 涨停不买...",pt->szInstrument);
		//AddLog(msg);
		return;
	}


	//strcpy(m_strInstrument.GetBuffer(0),pt->szInstrument);
	m_strInstrument = pt->szInstrument;

	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument);

	if (dbActiveCaptital > STOCK_TOTAL_MAX) //流通股本大于10亿股或者流通市值大于1000亿的票排除。
	{
		return;
	}

	if (dbActiveCaptital < STOCK_TOTAL_MIN) //流通股本大于10亿股或者流通市值大于1000亿的票排除。
	{
		return;
	}


	//第二天
	if(strcmp(m_sCurDate,pt->szDate) !=0)
		m_mapIN[pt->szInstrument] = 0;
		
	strcpy(m_sCurDate,pt->szDate);

	int iSignalStatus = GetSignalStatus(pt->szInstrument);

	int iCount = GetStockCount(pt->szInstrument);

	double last = pt->lastprice;

	//这里还是要取lastprice，而不是highestprice
	double high = pt->lastprice;

	double buy1 = pt->bidprice[0];

	double sale1 = pt->askprice[0];

	//买入过后才开始记录最高价
	if( (high > GetHgPrice(pt->szInstrument)) && (TRADE_STATUS_BK_TRADE == iSignalStatus))
	{
		SetHgPrice(pt->szInstrument,high);
	}

	//已经成交过，买信号,就跳过
	if( (iCount > 0) && (TRADE_STATUS_INIT == iSignalStatus) )
		return;


	if (TRADE_STATUS_INIT == iSignalStatus)
	{

		/*
		double dbMa20 = 0;

		CTime tmLastTradeDate;
		if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
		{
			m_bDaPan = false;
			if(m_junxian)
			{
				//AddLog("上证指数历史日线数据不存在", TRADE_MSG_TYPE_LOG_PRV);
				return;
			}
		}
		CString strLastTradeDate = tmLastTradeDate.Format("%Y%m%d");


		if (!m_pKManager->CalcMa20("999999", strLastTradeDate, dbMa20))
		{
			m_bDaPan = false;
			if(m_junxian)
				return;
		}

		CKStrdata oIndexData;
		bool bSuc = m_pKManager->getDayKData("999999", strLastTradeDate, oIndexData);

		if (!bSuc || oIndexData.priceS <= dbMa20)
		{		
			m_bDaPan = false;

			if(m_junxian)
			{
				//sprintf_s(msg,256,"大盘 %.2f 处于20日均线 %.2f 下，不买...",oIndexData.priceS,dbMa20);
				//AddLog(msg);
				return;
			}
		}

		*/

		/*
		按道理这里应该取消订阅该合约
		已经检测出不满足震荡区间
		*/
		if(m_mapIN[pt->szInstrument] == -1)
			return;

		//买入信号 还没校验过震荡 才获取数据
		//第一个tick过来，也会校验一次
		if(m_mapIN[pt->szInstrument] == 0)
		{
			GetPreNDaySumKData(pt->szDate, &m_v30KData, m_iCycle);
			CheckZhenFu(pt->szInstrument,m_sCurDate);
		}


		//如果已经存在30天
		//if(m_v30KData.size() == m_iCycle)
		{
			if(m_mapIN[pt->szInstrument]==1)
			{
				if(last > (GetPreHgPrice(pt->szInstrument)*1.01) )
				{
					char temp[32] = {0};
					strcpy(temp,pt->szDate);
					strcat(temp," ");
					strcat(temp,pt->szTime);
					strcpy(m_szTickTime, temp);

					//满足条件,买开,对价下单
					bool ret = OrderBuy(pt->szInstrument,sale1,m_dStartFund);

					if(ret)
					{
						//-2表示当天买入无法卖出
						m_mapIN[pt->szInstrument] = -2;

						SetHgPrice(pt->szInstrument,0);

						sprintf(msg,"合约: %s 买入 当前日期 %s  最新价 %.3f  买一价 %.3f  卖一价 %.3f  前最高价 %.3f  当前持仓数量 %d",pt->szInstrument,m_sCurDate,last,buy1,sale1,GetPreHgPrice(pt->szInstrument),GetPositionCount());

						AddLog(msg);

						//保存持仓信息
						//m_vHold.push_back(pt->szInstrument);
					}

				}
			
			}

		}


	}
	//卖出
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		double dbAvgPrice = GetAvgPrice(pt->szInstrument);
		double highprice = GetHgPrice(pt->szInstrument);

		//绝对止损、移动止损
		if( (last < dbAvgPrice * (1 - m_dAbsoluteStop)) || (last < highprice * (1- m_dMoveStop)) )
		{
			if( (iCount > 0) && (TRADE_STATUS_BK_TRADE == iSignalStatus) && (m_mapIN[pt->szInstrument]==-3) )
			{
				return;
			}

			sprintf(msg,"合约: %s 卖出 当前日期 %s  最新价 %.3f  最高价 %.3f  买一价 %.3f  卖一价 %.3f  成本价 %.3f  成交价1 %.3f  成交价2 %.3f",pt->szInstrument,m_sCurDate,last,highprice,buy1,sale1,dbAvgPrice,(1- m_dMoveStop)*highprice,(1 - m_dAbsoluteStop)*dbAvgPrice );

			AddLog(msg);

			//有持仓,隔天,卖出信号
			if( (iCount > 0) && (TRADE_STATUS_BK_TRADE == iSignalStatus) && (m_mapIN[pt->szInstrument]==-2) )
			{
				sprintf(msg,"合约:%s 日期:%s,当天买入不能卖出！",pt->szInstrument,pt->szDate);
				AddLog(msg);
				m_mapIN[pt->szInstrument] = -3;
				return;
			}

			CloseSale(pt->szInstrument, iCount, buy1, m_fundAccount.GetBuffer(0),'1');

			m_fundAccount.ReleaseBuffer();

			m_mapIN[pt->szInstrument] = 0;

			SetHgPrice(pt->szInstrument,0);

			SetStockCount(pt->szInstrument, 0);
			
			SetAvgPrice(pt->szInstrument, 0.0);

			sprintf(msg,"当前持仓量 %d",GetPositionCount());

			AddLog(msg);

			//删除
			/*vector<string> ::iterator it = m_vHold.begin();
			while(it!= m_vHold.end())
			{
				if( strcmp(pt->szInstrument,(*it).c_str()) ==0)
				{
					m_vHold.erase(it);
					break;
				}
				it++;
			}*/
		
		}

		//最高价绝对止盈
		if( last > dbAvgPrice * (1 + m_dGet)  )
		{

			if( (iCount > 0) && (TRADE_STATUS_BK_TRADE == iSignalStatus) && (m_mapIN[pt->szInstrument]==-3) )
			{
				return;
			}

			sprintf_s(msg,256,"合约 %s 当前日期 %s 止盈  最新价 %.3f  买一价 %.3f  卖一价格 %.3f  成本价 %.3f",pt->szInstrument,m_sCurDate,last,buy1,sale1,dbAvgPrice);
			
			AddLog(msg);

			//有持仓,隔天,卖出信号
			if( (iCount > 0) && (TRADE_STATUS_BK_TRADE == iSignalStatus) && (m_mapIN[pt->szInstrument]==-2) )
			{
				sprintf(msg,"合约:%s 日期:%s,当天买入不能卖出！",pt->szInstrument,pt->szDate);
				AddLog(msg);
				m_mapIN[pt->szInstrument] = -3;
				return;
			}
			
			CloseSale(pt->szInstrument, iCount, buy1, m_fundAccount.GetBuffer(0),'1');
			m_fundAccount.ReleaseBuffer();

			

			SetStockCount(pt->szInstrument, 0);

			SetPreHgPrice(pt->szInstrument,0);

			m_mapIN[pt->szInstrument] = 0;
			
			SetAvgPrice(pt->szInstrument, 0.0);

			sprintf(msg,"当前持仓量 %d",GetPositionCount());

			AddLog(msg);

			//删除
			/*vector<string> ::iterator it = m_vHold.begin();
			while(it!= m_vHold.end())
			{
				if( strcmp(pt->szInstrument,(*it).c_str()) ==0)
				{
					m_vHold.erase(it);
					break;
				}
				it++;
			}*/


		}

	}


}


void CStrategyBreakHigh::SetParam(CString paramname,CString value)
{
	//"交易合约;日线周期;震荡幅度;绝对止损;移动止损;账户;";

	m_params[(LPCTSTR)paramname] = value;

	//还需要将对应的值更新
	if(paramname=="交易合约")
	{
		m_strInstrument = value;
	}
	else if(paramname=="日线周期")
	{
		m_iCycle = atoi(value);
	}
	else if(paramname=="新高天数")
	{
		m_iPriceDay = atoi(value);
	}
	else if(paramname=="震荡幅度")
	{
		double temp = m_dFluctuateRange;
		m_dFluctuateRange = atof(value);

		//20160516  策略运行起来需要修改震荡幅度
		if(!m_mapIN.empty())
		{
			//震荡幅度变大
			if(m_dFluctuateRange > temp )
			{
				ReSubCode();
			}

			map<string,int> ::iterator it = m_mapIN.begin();
			for(;it != m_mapIN.end(); it++)
			{
				//震荡幅度变小
				if(m_dFluctuateRange < temp )
				{
					if(it->second == 1)
						it->second = 0;
				}

			}
		}
	}
	else if(paramname=="止损率")
	{
		m_dAbsoluteStop = atof(value);
	}
	else if(paramname=="回调率")
	{
		m_dMoveStop = atof(value);
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
	else if (paramname == "是否复利")
	{
		
		m_pPolicy->SetParams("是否复利", value);
	}
	else if (paramname == "入场模式")
	{
		
		m_iInType = atoi(value);
	}
	else if (paramname == "止盈率")
	{
		
		m_dGet = atof(value);
	}
	else if (paramname == "是否参考20日均线")
	{
		
		m_junxian = atoi(value);
	}
	else if(paramname == "是否下单")
	{
		m_bRealTrade = (0 == atoi(value) ? false : true);
	}
	else if (paramname == "是否追单")
	{
		
		m_follow = value;
	}
	else if (paramname == "超时时间")
	{
		
		m_timeOut = atoi(value);
	}
	else if (paramname == "价差")
	{
		
		m_jiacha = atof(value);
	}

}

void CStrategyBreakHigh::SetParam(CString value)
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

	/*m_strModelName = m_strStrategyName;*/
}

CString CStrategyBreakHigh::GetParamValue()
{
	//"交易合约;日线周期;震荡幅度;绝对止损;移动止损;账户;";

	CString str;

	CString str_fund = m_params["初始权益"];
	double d_fund = atof(str_fund);

	CString str_count = m_params["同时开仓数"];
	int i_count = atoi(str_count);

	CString str_fuli = m_params["是否复利"];
	int i_fuli = atoi(str_fuli);

	CString str_trade = m_params["是否下单"];
	int i_trade = atoi(str_trade);

	CString str_dapan = m_params["是否参考20日均线"];
	int i_dapan = atoi(str_dapan);

	str.Format("%s;%d;%d;%.3f;%.3f;%.3f;%.3f;%.3f;%d;%.3f;%d;%d;%d;%s;",m_strInstrument,m_iCycle,m_iPriceDay,m_dFluctuateRange,m_dAbsoluteStop,m_dMoveStop,m_dGet,d_fund,i_count,m_dbCostRate,i_fuli,i_trade,i_dapan,m_fundAccount);

	return str;
}

CString CStrategyBreakHigh::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyBreakHigh::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	double yinkuirate;

	double huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "空仓";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "买开";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "卖开";

	}


	if( m_bRun == false )
		modelstatus = "暂停";
	else
		modelstatus = "运行";


	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getName(m_strInstrument.GetBuffer(0));

	m_strInstrument.ReleaseBuffer();

	if(m_lastPrice < 0.001)
	{
		yinkuirate = 0.0;
	}
	else
	{
		yinkuirate = (m_lastPrice - m_dTradePrice) / m_dTradePrice;
	}
	


	if(m_hignPrice < 0.001)
	{
		huitiaorate = 0.00;
	}
	else
	{
		huitiaorate =( m_hignPrice - m_lastPrice) / m_hignPrice;
	}


	result.Format("%s;%s;%s;%d;%s;%.3f;%.4f(%.2f);%.4f;%s;%.4f;%.4f(%.2f);%s;%s;",m_strStrategyName,m_strInstrument,codename,
		m_nCount,status,m_dTradePrice,m_fyingkuiPosition,yinkuirate,0.0,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,/*m_strModelName*/GetStrategyId());

	return result;
}

void CStrategyBreakHigh::BeginRun()
{

}

void CStrategyBreakHigh::Pause()
{

}

void CStrategyBreakHigh::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{

}


void CStrategyBreakHigh::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}

void CStrategyBreakHigh::reload()
{
	TiXmlDocument dom("xxxx.xml");
	if (!dom.LoadFile())
	{
	  cout<<"打开失败!"<<endl;
	  cout<<"错误的原因:"<<dom.ErrorDesc()<<endl;
	  return;
	}

	TiXmlElement *root=dom.FirstChildElement();
	if(root == NULL)
		return;

   for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
   {
	   //知道id相同的那个
	   //if(strcmp(elem->Attribute("id"),GetStrategyId() )==0)
	   if(strcmp(elem->Attribute("account"),"10000013" )==0)
	   {
		   //然后遍历每个合约的数据
		   for(TiXmlElement* item = elem->FirstChildElement(); item != NULL; item = item->NextSiblingElement())
		   {
			   string code = item->Attribute("code");		
			   TiXmlNode* text = item->FirstChild();
			   char *sprice  = (char*)text->ToText()->Value();
			   //初始化
			   m_vHold.push_back(code);			   
			   SetHgPrice(code.c_str(),atof(sprice));
           }
			

	   }

   }

    dom.Clear();


}


void CStrategyBreakHigh::reload(CString strParam)
{
	CStringArray arInfo;
	CFunctionPub::ParseParam(strParam, arInfo, '#');

	for (int i = 0; i < arInfo.GetSize(); i++)
	{
		CStringArray arPos;
		CFunctionPub::ParseParam(arInfo[i], arPos, ':');
		if (arPos.GetSize() >= 2)
		{
			CString strCode = arPos[0];
			CString p = arPos[1];
			double dprice = atof(arPos[1]);
			SetHgPrice(strCode.GetBuffer(0),dprice);
		
		}
	}

}


void CStrategyBreakHigh::saveModel()
{  
    TiXmlDocument * xmlDocs = new TiXmlDocument();  
	TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0", "UTF-8", "yes");
    xmlDocs->LinkEndChild(xmlDec);  
    TiXmlElement * root = new TiXmlElement("root");  
    xmlDocs->LinkEndChild(root);  
    

	TiXmlElement *strategy = new TiXmlElement("strategy_case"); 
	strategy->SetAttribute("id", GetStrategyId());  
	strategy->SetAttribute("account", m_fundAccount); 
	strategy->SetAttribute("strname", m_strStrategyName);
    root->LinkEndChild(strategy); 

	char sprice[10];

	vector<string> ::iterator it = m_vHold.begin();
	for(;it!=m_vHold.end();it++)
	{
		char *code = (char *)(*it).c_str();
		double price = GetHgPrice(code);
		sprintf(sprice,"%.3f",price);

		TiXmlElement * hp = new TiXmlElement("highprice");  
		hp->SetAttribute("code",code);
		TiXmlText * p = new TiXmlText(sprice);  
		hp->LinkEndChild(p);  
		strategy->LinkEndChild(hp);  
 
	}
	
   xmlDocs->SaveFile("xxxx.xml");  
    
}


int CStrategyBreakHigh::GetPreNDaySumKData(const char* szCurDate,vector<CKStrdata>*vk,int iDay)
{
	
	int cap = m_v30KData.capacity();
	int size = m_v30KData.size();
	m_v30KData.clear();

	int iCount=-1;
	if (m_pKManager)
	{
		iCount = m_pKManager->getNDaySumKData(m_strInstrument.GetBuffer(), szCurDate, vk, iDay);
		
	}

	return iCount;
}

bool CStrategyBreakHigh::GetPreNDayKData(const char* szCurDate, CKStrdata &oKData, int iDay)
{
	ZeroMemory(&oKData, sizeof(oKData));
	if (m_pKManager)
	{
		m_pKManager->getNDayKData(m_strInstrument.GetBuffer(), szCurDate, oKData, iDay);
		return true;
	}

	return false;
}

bool CStrategyBreakHigh::CheckZhenFu(char *code,char *curDate)
{
	bool ret = true;
	double highprice = 0;

	//if(strcmp("600732",code)==0 || strcmp("000033",code)==0 || strcmp("600710",code)==0)
	//	int i =0;

	//初始化值
	SetPreHgPrice(code,9999);
	m_mapIN[code] = -1;

	if(m_v30KData.size() != m_iCycle)
	{
		ret = false;
		return ret;
	}


	char msg[128];

	double high30 = 0;
	double low30 = 9999;

	/*
	for(int i=0; i < m_v30KData.size(); i++)
	{
		sprintf(msg,"check--------日期:%s,合约:%s,高:%.3f,开:%.3f,低:%.3f,收:%.3f",m_v30KData[i].curDate,m_strInstrument.GetBuffer(0),m_v30KData[i].priceH,m_v30KData[i].priceK,m_v30KData[i].priceL,m_v30KData[i].priceS);
		AddLog(msg);
	}

	sprintf(msg,"列表个数:%d",m_v30KData.size());
	AddLog(msg);
	*/
	
	for(int i= 0;i< m_v30KData.size(); i++)
	{
		double high = m_v30KData[i].priceH;
		double low = m_v30KData[i].priceL;

		//取周期内最高价最低价
		if(high > high30)
			high30 = high;

		if(low < low30)
			low30 = low;

		char  now[9];
		strcpy(now,m_v30KData[i].curDate);

		//这里取新高天数
		if( (high > highprice) && ( i >= ( m_v30KData.size() - m_iPriceDay) ) )
			highprice = high;

		int d1 = atoi(now);
		int d2 = atoi(curDate);

		if(d1 > d2)
		{
			ret = false;
			sprintf(msg,"~~~~~~~~~~~k线日期(%d),日线日期(%d),不符合要求",d2,d1);
			AddLog(msg);
			break;
		}
		
		//改为除以最低价
		if( (high30 - low30)/low30 > m_dFluctuateRange )
		{
			ret = false;
			sprintf(msg,"合约:%s 日期:%s,最高价:%.3f,最低价:%.3f,不满足区间:%.3f",code,m_v30KData[i].curDate,high30,low30,m_dFluctuateRange);
			AddLog(msg);
			m_mapIN[code] = -1;

			break;
		}
	}

	if(ret)
	{
		SetPreHgPrice(code,highprice);
		m_mapIN[code] = 1;
		int k = m_v30KData.size()-1;
		m_dPrePriceS = m_v30KData[k].priceS;   //记录前一天的收盘价

		/*sprintf(msg,"~~~~~~符合震荡区间条件,最高价:%.3f",m_preHignPrice);
		AddLog(msg);*/
	}


	return ret;
}


bool CStrategyBreakHigh::OrderBuy(char *code,double dPrice,double dStartFund)
{
	bool ret = false;

	//if(strcmp("000886",code) != 0)
	//	return false;

	char sMsg[256];
	double dbFund = 0.0;
	if (!ApplyForFund(dbFund))
	{
		//sprintf_s(sMsg,256,"合约 %s 出信号,但资金不够,当前开仓数: %d", code, GetPositionCount());
		//AddLog(sMsg);
		return false;
	}

	int iCount = ((int)(dbFund / dPrice) / 100 ) * 100;
	if(iCount >= 100)
	{
		SetStockCount(code, iCount);
		sprintf_s(sMsg,256,"合约:%s,可用资金:%.2f,可用数量%d", m_strInstrument.GetBuffer(0),dbFund,iCount);
		m_strInstrument.ReleaseBuffer();
		AddLog(sMsg);
		OpenBuy(code, iCount, dPrice, m_fundAccount.GetBuffer(0));
		//OpenBuy(code, iCount, dPrice-0.02, m_fundAccount.GetBuffer(0));
		m_fundAccount.ReleaseBuffer();
		SetAvgPrice(code, dPrice);

		ret = true;

	}
	return ret;

}

int CStrategyBreakHigh::CloseAll(char* szInstrument, double dbPrice)
{ 
	char msg[256];

	int iSignalStatus = GetSignalStatus(szInstrument);

	if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		CloseSale(szInstrument, GetStockCount(szInstrument), dbPrice, m_fundAccount.GetBuffer(0),'1');

		m_mapIN[m_strInstrument.GetBuffer(0)] = 0;

		m_fundAccount.ReleaseBuffer();

		m_strInstrument.ReleaseBuffer();

		sprintf(msg,"xxxxxxx最后一个tick数据,清仓,然后初始化参数");

		AddLog(msg);
	}
	
	return 0;
}


void CStrategyBreakHigh::BackTest(CKStrdata *pk, const char* sCode)
{

	char msg[256];


	double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)sCode);

	if (dbActiveCaptital > STOCK_TOTAL_MAX) //流通股本大于10亿股或者流通市值大于1000亿的票排除。
	{
		//if(m_xiaopan)
			return;
	}

	if (dbActiveCaptital < STOCK_TOTAL_MIN) //流通股本大于10亿股或者流通市值大于1000亿的票排除。
	{
		//if(m_xiaopan)
			return;
	}



	//合约或者日期变了
	// m_mapIN=-2  可以归零了  表示可以平仓了
	if( (strcmp(m_sCurDate,pk->curDate) !=0) || (strcmp(m_sCurCode,sCode)!=0) )
	{
		m_v30KData.clear();
		m_mapIN[m_sCurCode] = 0;
	}


	strcpy(m_sCurCode,sCode);

	strcpy(m_sCurDate,pk->curDate);

	//m_mapIN[sCode] = 0;

	//GetPreNDaySumKData(pk->curDate, &m_v30KData, m_iCycle);

		
	int iSignalStatus = GetSignalStatus(sCode);

	int iCount = GetStockCount(sCode);

	double high = pk->priceH;


	//买信号才去数据
	if(TRADE_STATUS_INIT == iSignalStatus)
	{
		GetPreNDaySumKData(pk->curDate, &m_v30KData, m_iCycle);
	}

	//买入过后才开始记录最高价
	if( (high > GetHgPrice(sCode)) && (TRADE_STATUS_BK_TRADE == iSignalStatus))
	{
		SetHgPrice(sCode,high);
	}

	//有持仓,隔天,卖出信号
	if( (iCount > 0) && (TRADE_STATUS_BK_TRADE == iSignalStatus) && m_mapIN[sCode]==-2 )
		return;

	//已经成交过，买信号,就跳过
	if( (iCount > 0) && (TRADE_STATUS_INIT == iSignalStatus) )
		return;


	if (TRADE_STATUS_INIT == iSignalStatus)
	{
		//如果已经存在30天
		if(m_v30KData.size() == m_iCycle)
		{

			double dbMa20 = 0;

			if (!m_pKManager->CalcMa20("999999", pk->curDate, dbMa20))
				return;


			CKStrdata oIndexData;
			bool bSuc = m_pKManager->getDayKData("999999", pk->curDate, oIndexData);

			if (!bSuc || oIndexData.priceS <= dbMa20)
			{

				sprintf_s(msg,256,"当前日期:%s,合约:%s,大盘处于20日均线下，20日均线:%.3f,收盘价:%.3f...",pk->curDate,m_strInstrument.GetBuffer(0),dbMa20,oIndexData.priceS);
				AddLog(msg);
				m_strInstrument.ReleaseBuffer();
				return;

			}


			if( CheckZhenFu(m_sCurCode,m_sCurDate) )
			{
				//最高价大于前最高价
				if( pk->priceH > GetPreHgPrice(sCode))
				{
					double limit = m_dPrePriceS*1.1;
					//double limit = m_dPrePriceS*0.99;
					double price = 0.0;

					if(m_iInType == 1)
					{
						if( (pk->priceH >= limit) && (pk->priceK != limit))
						{
							price = limit ;
							//price = GetPreHgPrice(sCode) * 1.01;
						}
						else
							return;
						
					}
					else if(m_iInType == 0)
					{
						price = GetPreHgPrice(sCode) * 1.01;
					}

					//满足条件,买开
					OrderBuy((char*)sCode,price,m_dStartFund);

					SetHgPrice(sCode,pk->priceH);

					sprintf(msg,"买买买买买,当前日期:%s,成本价:%.3f,前最高价:%.3f",m_sCurDate,price,GetPreHgPrice(sCode));

					AddLog(msg);

					m_mapIN[sCode] = -2;

					
				}

				
			}
			else
			{
				sprintf(msg,"合约:%s,日期:%s,已检测不在震荡区间,跳过",m_strInstrument.GetBuffer(0),pk->curDate);
				m_strInstrument.ReleaseBuffer();
				AddLog(msg);

				return;
			}

		}


	}
	//卖出
	else if (TRADE_STATUS_BK_TRADE == iSignalStatus)
	{
		//成本价
		double dbAvgPrice = GetAvgPrice(sCode);

		double highprice = GetHgPrice(sCode);

		double low = pk->priceL;

		double spirce = pk->priceS;

		double saleprice = 0;

		//最低价绝对止损
		if( dbAvgPrice * (1 - m_dAbsoluteStop) > low)
		{
			saleprice = dbAvgPrice * (1 - m_dAbsoluteStop);

			sprintf_s(msg,256,"当前日期:%s,最低价绝对止损,卖出价:%.3f,成本价:%.3f,最低价:%.3f",pk->curDate,saleprice,dbAvgPrice,low);
			CloseSale((char*)sCode, iCount, saleprice, m_fundAccount.GetBuffer(0),'1');
			AddLog(msg);
			m_fundAccount.ReleaseBuffer();

			SetStockCount(sCode, 0);

			m_mapIN[sCode] = 0;
			
			if(m_bBackTest || false == m_bRun)
			{
				SetAvgPrice(sCode, 0.0);
			}

			return;

		}

		//最高价绝对止盈
		if( high > dbAvgPrice * (1 + m_dGet)  )
		{
			saleprice = dbAvgPrice * (1 + m_dGet);

			sprintf_s(msg,256,"当前日期:%s,最高价止盈,卖出价:%.3f,成本价:%.3f,最高价:%.3f",pk->curDate,saleprice,dbAvgPrice,high);
			CloseSale((char*)sCode, iCount, saleprice, m_fundAccount.GetBuffer(0),'1');
			AddLog(msg);
			m_fundAccount.ReleaseBuffer();

			SetStockCount(sCode, 0);

			m_mapIN[sCode] = 0;
			
			if(m_bBackTest || false == m_bRun)
			{
				SetAvgPrice(sCode, 0.0);
			}

			return;
		}

		//最高价-收盘价
		double rate = (GetHgPrice(sCode) - spirce) / GetHgPrice(sCode);
		if( rate > m_dMoveStop)
		{
			saleprice = GetHgPrice(sCode) * (1 - m_dMoveStop);
			sprintf_s(msg,256,"当前日期:%s,移动止损,卖出价:%.3f,移动止损最高价:%.3f,收盘价:%.3f",pk->curDate,saleprice,GetHgPrice(sCode),spirce);
			CloseSale((char*)sCode, iCount, saleprice, m_fundAccount.GetBuffer(0),'1');
			m_fundAccount.ReleaseBuffer();
			AddLog(msg);

			SetStockCount(sCode, 0);

			m_mapIN[sCode] = 0;
			
			if(m_bBackTest || false == m_bRun)
			{
				SetAvgPrice(sCode, 0.0);
			}

			return;
		}


	}


}


void CStrategyBreakHigh::OnSubcode()
{
	vector<string> arCodes;
	char sMsg[256];

	m_mapPreHighPrice.clear();
	m_mapIN.clear();

	//step1: 持仓
	std::map<string, int> arCodeSubs;
	std::map<string, int>::const_iterator itSignal = m_mapSignalStatus.cbegin();
	while (itSignal != m_mapSignalStatus.cend())
	{
		if (TRADE_STATUS_BK_TRADE == itSignal->second || TRADE_STATUS_SK_TRADE == itSignal->second)
		{
			char sSubcode[256];

			sprintf(sSubcode, "subcode#%s;STOCK;UFX;#",  itSignal->first.c_str());
			sprintf(sMsg, "持仓订阅 %s 行情",  itSignal->first.c_str());
			AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
			::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);	

			//持仓平掉过后可能继续开仓 需要校验
			FilterSub((char*)itSignal->first.c_str());

			arCodeSubs[itSignal->first] = itSignal->second;
		}
		itSignal++;
	}

	//大盘20日均线
	if(!DaPan())
		return;
	

	//step2: 开仓
	if (m_pKManager->GetCodeList(arCodes) > 0)
	{
		vector<string>::const_iterator itCode = arCodes.cbegin();

		while (itCode != arCodes.cend())
		{
			if (arCodeSubs.end() != arCodeSubs.find(*itCode))
			{
				itCode++;
				continue;
			}
			double dbActiveCaptital = m_pStrategyCodeInfo->getActiveCapital((char*)itCode->c_str());
			if ( (dbActiveCaptital > STOCK_TOTAL_MAX) || (dbActiveCaptital < STOCK_TOTAL_MIN)) //流通股本大于20亿股或者流通市值大于1000亿的票排除。
			{
				itCode++;
				continue;
			}
			if(strcmp("600570",(char*)itCode->c_str()) == 0)
			{
				itCode++;
				continue;
			}

			//过滤行情
			int iSignalStatus = GetSignalStatus(itCode->c_str());
			int iCount = GetStockCount(itCode->c_str());
			if((iSignalStatus == TRADE_STATUS_INIT) && (iCount == 0))
			{
				if (!FilterSub((char*)itCode->c_str()) )
				{
					char temp[64];
					sprintf(temp,"行情过滤 %s",(char*)itCode->c_str());
					AddLog(temp,TRADE_MSG_TYPE_OTHER);

					itCode++;
					continue;
				}
			}


			//订阅行情		
			{				
				char sSubcode[256];

				sprintf(sSubcode, "subcode#%s;STOCK;UFX;#", itCode->c_str());			
				::SendMessage(m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
				//sprintf(sMsg, "已订阅 %s 行情", itCode->c_str());
				//AddLog(sMsg);

				arCodeSubs[*itCode] = 0;
			}
			
			itCode++;
		}

		sprintf(sMsg, "本次订阅共遍历股票%d个, 其中成功订阅股票%d个", arCodes.size(), arCodeSubs.size());
		AddLog(sMsg, TRADE_MSG_TYPE_LOG_PRV);
	}

	m_pKManager->FreeData();

	//reload();

	////test
	/*strcpy(m_szTickTime, "20160419");
	m_vHold.push_back("601288");
	m_mapHighPrice["601288"] = 10;
	OpenBuy("601288", 100, 2.9, m_fundAccount.GetBuffer(0));
	this->SetAvgPrice("601288",2.9);
	this->SetStockCount("601288",100);
	OpenBuy("600000", 100, 2.9, m_fundAccount.GetBuffer(0));
	this->SetAvgPrice("600000",1.9);
	this->SetStockCount("600000",200);*/
	////
	
}



bool CStrategyBreakHigh::FilterSub(char *code)
{

	char m_szDate[16];
	CTime tmNow = CTime::GetCurrentTime();
	CString strTime = tmNow.Format("%Y%m%d");
	strcpy(m_szDate, strTime);

	m_v30KData.clear();

	int iCount=-1;

	if (m_pKManager)
	{
		iCount = m_pKManager->getNDaySumKData(code, m_szDate, &m_v30KData, m_iCycle);
		
	}
	else
	{
		return false;
	}

	return  CheckZhenFu(code,m_szDate);
	
}


bool CStrategyBreakHigh::DaPan()
{
	char msg[256];
	double dbMa20 = 0;

	bool ret = true;

	//不考虑20均线
	if(!m_junxian)
		return ret;

	CTime tmLastTradeDate;
	if (!m_pKManager->GetLastKDataDate("999999", tmLastTradeDate))
	{

		AddLog("上证指数历史日线数据不存在", TRADE_MSG_TYPE_LOG_PRV);
		ret = false;

	}
	CString strLastTradeDate = tmLastTradeDate.Format("%Y%m%d");


	if (!m_pKManager->CalcMa20("999999", strLastTradeDate, dbMa20))
	{

		ret = false;
	}

	CKStrdata oIndexData;
	bool bSuc = m_pKManager->getDayKData("999999", strLastTradeDate, oIndexData);

	if (!bSuc || oIndexData.priceS <= dbMa20)
	{		
		sprintf_s(msg,256,"大盘 %.2f 处于20日均线 %.2f 下，不买...",oIndexData.priceS,dbMa20);
		AddLog(msg);
		ret = false;
	}

	return ret;

}


void CStrategyBreakHigh::SetPreHgPrice(const char *code,double price)
{
	m_mapPreHighPrice[code] = price;
}


double CStrategyBreakHigh::GetPreHgPrice(const char *code)
{
	double ret = 9999.0;

	if(m_mapPreHighPrice.find(code) != m_mapPreHighPrice.end())
	{
		ret = m_mapPreHighPrice[code];
	}

	return ret;

}


void CStrategyBreakHigh::ReSubCode()
{
	m_pThdSubcode = new CEasyThread();

	m_pThdSubcode->Start(Task_Subcode,0,this);

}

void CStrategyBreakHigh::Task_Subcode(void *lpData)
{
	CStrategyBreakHigh *pStrategy = (CStrategyBreakHigh*)lpData;

	//pStrategy->m_mapPreHighPrice.clear();
	//pStrategy->m_mapIN.clear();

	map<string,int> ::iterator it = pStrategy->m_mapIN.begin();

	for(;it != pStrategy->m_mapIN.end(); it++)
	{
		string code = it->first;

		if(it->second == -1)
		{
			if (!( pStrategy->FilterSub((char*)code.c_str()) ) )
			{
				char temp[64];
				sprintf(temp,"行情过滤 %s",(char*)code.c_str());
				pStrategy->AddLog(temp,TRADE_MSG_TYPE_OTHER);

				continue;
			}

			//订阅行情			
			char sSubcode[256];
			sprintf(sSubcode, "subcode#%s;STOCK;UFX;#", code.c_str());			
			::SendMessage(pStrategy->m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)sSubcode);
		}
	}

	//pStrategy->m_pKManager->FreeData();

	//pStrategy->m_pThdSubcode = NULL;

}