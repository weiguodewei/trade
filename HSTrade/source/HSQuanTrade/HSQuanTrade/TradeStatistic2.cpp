#include "stdafx.h"
#include "TradeStatistic2.h"
#include <map>
#include <string>

using namespace std;

CTradeStatistic2::CTradeStatistic2(void)
{
	/*m_jiazhi = 300;
	m_feilv = 0.0000275;*/
	m_jiazhi = 1;
	m_feilv = 0.001;
	m_beginquanyi = 200000;
	m_sum_max=0;
	m_quanyiMax=0;	
}


CTradeStatistic2::~CTradeStatistic2(void)
{
}

int CTradeStatistic2::AddOpreate(char * day, char * time, double price , int count , int direction , int openorclose,char * heyue)
{
	CTradeRecord2 * pt= new CTradeRecord2;
	m_arListTrade.Add(pt);

	if( day)
		strcpy_s(pt->day,day);

	if(heyue)
		strcpy_s(pt->heyue,heyue);

	strcpy_s(pt->time1,time);
	pt->price = price;
	pt->count = count;
	pt->direction = direction;
	pt->openorclose = openorclose;

	return 0;
}

int CTradeStatistic2::SetMsgWnd(HWND hwnd)
{
	m_hMsgWnd = hwnd; 
	return 0;
}

int CTradeStatistic2::init()
{	
	//m_beginquanyi = 200000;
	m_sum_max=0;
	m_quanyiMax=0;
		
	for(int i=0;i<m_arListTrade.GetSize();i++)
	{
		CTradeRecord * pt = (CTradeRecord *)m_arListTrade[i];
		delete pt;
	}
	m_arListTrade.RemoveAll();
	m_mapEquityEachDay.clear();
	return 0;
}
void CTradeStatistic2::StatisticEquity()
{
	CStringArray arEquity;
	arEquity.Add("日期,总权益,股票权益");
	CString strLine;
	std::map<string,CStockEquity>::const_iterator itEquity = m_mapEquityEachDay.cbegin();
	while (itEquity != m_mapEquityEachDay.cend())
	{
		strLine.Format("%s, %.2f, %.2f",itEquity->first.c_str(), itEquity->second.m_dbTotalEquity, itEquity->second.m_dbStockEquity);
		arEquity.Add(strLine);

		itEquity++;
	}

	::SendMessage(m_hMsgWnd,WM_MODEL_RESULT_MSG,1,(long)&arEquity);
}
int CTradeStatistic2::Statistic(CString day,bool show)
{
	CStringArray list;
	list.Add("序号,合约,日期,开仓时,数量,价格,方向,开平,盈亏,手续费,权益,");
	CStringArray arDirection;
	arDirection.Add("买");
	arDirection.Add("卖");
	CStringArray arOpenClose;
	arOpenClose.Add("开");
	arOpenClose.Add("平");

	
	CStringArray arTempPrice;
	CStringArray arTempCount;
	
	double maxquanyi=0;
	double maxhuiche=0;
	//CString strMaxHuicheTime;
	CString str;

	int yearbegin=0;
	CTradeStaYear * pStaYear=0;
	double beginQuanyi=0;
	CPtrArray arYear;
	double sum_yk=0;

	int countyingli = 0;
	int countkuisun=0;
	double allyingli=0;
	double allkuisun=0;
	double HuicheMax=0;
	CString HuicheTime;
	double HuicheMaxAll=0;
	CString HuicheTimeAll;

	m_nTradeCount = m_arListTrade.GetSize();

	for(int i=0;i<m_arListTrade.GetSize();i++)
	{
		double yingkui=0;
		double shouxufei= 0;
		CTradeRecord2 * pt = (CTradeRecord2 *)m_arListTrade[i];

		CString time(pt->day);
		int year = atoi(time.Mid(0,4));

		shouxufei = pt->price * pt->count * m_jiazhi * m_feilv;
		CString row;

		if( pt->openorclose == 0 )
		{
			m_beginquanyi = m_beginquanyi - shouxufei;
			row.Format("%d,%s,%s,%s,%d,%.2f,%s,%s,%.2f,%.2f,%.2f,",i,pt->heyue, pt->day,pt->time1,pt->count,pt->price,arDirection[pt->direction],arOpenClose[pt->openorclose],yingkui,shouxufei,m_beginquanyi);
			list.Add(row);
			
			str.Format("%.2f",pt->price);
			arTempPrice.Add(str);

			str.Format("%d",pt->count);
			arTempCount.Add(str);
		}
		else
		{
			yingkui = 0;
			for(int j=0;j<arTempPrice.GetSize();j++)
			{
				if( pt->direction ==0 )//BP
				{
					yingkui += atoi(arTempCount[j]) * (atof(arTempPrice[j])-pt->price) *m_jiazhi ;
				}
				else
				{
					yingkui += atoi(arTempCount[j]) * ( pt->price -atof(arTempPrice[j])  ) *m_jiazhi ;
				}
			}
			arTempPrice.RemoveAll();
			arTempCount.RemoveAll();

			m_beginquanyi += yingkui - shouxufei;

			if( m_beginquanyi > maxquanyi )
				maxquanyi = m_beginquanyi;

			pt->huiche =  maxquanyi - m_beginquanyi ;

			if( pt->huiche > HuicheMaxAll )
			{
				HuicheMaxAll = pt->huiche;
				HuicheTimeAll.Format("%s %s",pt->day, pt->time1);
			}

			//统计平均盈利和亏损
			if( yingkui > 0 )
			{
				allyingli += yingkui ;
				countyingli++;
			}
			else
			{
				allkuisun += yingkui;
				countkuisun++;
			}


			row.Format("%d,%s,%s,%s,%d,%.2f,%s,%s,%.2f,%.2f,%.2f,",i,pt->heyue,pt->day,pt->time1,pt->count,pt->price,arDirection[pt->direction],arOpenClose[pt->openorclose],yingkui,shouxufei,m_beginquanyi);
			list.Add(row);

			

			sum_yk += yingkui- shouxufei * 2;
			if( sum_yk>m_sum_max)
			{
				m_sum_max = sum_yk;			
			}
			pt->huiche = m_sum_max-sum_yk;
			if( pt->huiche > HuicheMax )
			{
				HuicheMax = pt->huiche;
				HuicheTime.Format("%s %s",pt->day,pt->time1);
			}
		}

		//分年统计
		if(yearbegin==0  )
		{			
			pStaYear = new CTradeStaYear;
			arYear.Add(pStaYear);
			yearbegin = year;
			pStaYear->year = yearbegin;

			beginQuanyi = m_beginquanyi;
		}
		else if (pStaYear && year != pStaYear->year)
		{
			//对前一年进行统计
			pStaYear->shouyi = m_beginquanyi-beginQuanyi;
			pStaYear->maxhuiche = HuicheMax;
			strcpy_s(pStaYear->szMaxHuicheTime,HuicheTime);
			
			sum_yk = 0;
			m_sum_max = 0;
			HuicheMax = 0;

			pStaYear = new CTradeStaYear;
			arYear.Add(pStaYear);
			yearbegin = year;
			pStaYear->year = yearbegin;

			beginQuanyi = m_beginquanyi;
		}	

	}

	//对前一年进行统计
	if( pStaYear)
	{
		pStaYear->shouyi = m_beginquanyi-beginQuanyi;
		pStaYear->maxhuiche = HuicheMax;
		strcpy_s(pStaYear->szMaxHuicheTime,HuicheTime);
	}


	double avgYingli = allyingli /countyingli;
	double avgKuisun = allkuisun/countkuisun;
	double shenglv = 1.0* countyingli /(countyingli+countkuisun);
	double avgYingli2 = (allyingli+allkuisun)/(countyingli+countkuisun);


	CString row;
	row = ",,,,,,";
	list.Add(row);
	row = "测试统计,,,,,,";
	list.Add(row);
	row.Format("最大回撤,%.2f,",HuicheMaxAll);
	list.Add(row);
	row.Format("最大回撤时间,%s,",HuicheTimeAll);
	list.Add(row);
	row.Format("平均盈利,%.2f,",avgYingli);
	list.Add(row);
	row.Format("平均亏损,%.2f,",avgKuisun);
	list.Add(row);
	row.Format("胜率,%.2f,",shenglv);
	list.Add(row);
	row.Format("每手平均盈利,%.2f,",avgYingli2);
	list.Add(row);

	m_shenglv = shenglv;
	m_HuicheMax = HuicheMaxAll;
	m_avgYingli2 = avgYingli2;
	m_avgYingli = avgYingli;
	m_avgKuisun = avgKuisun;

	list.Add(",,,,,,,,");
	list.Add("分年统计,");
	list.Add("年份,收益,最大回撤,回撤时间,交易次数,平均盈利,平均亏损,每手盈利,胜率,");
	for(int i=0;i<arYear.GetSize();i++)
	{
		CTradeStaYear * pYear = (CTradeStaYear *)arYear[i];
		CString row;
		row.Format("%d,%.2f,%.2f,%s,",pYear->year,pYear->shouyi,pYear->maxhuiche,pYear->szMaxHuicheTime);
		list.Add(row);
	}

	row.Format("耗时,%s", CalculateElapseTime());
	list.Add(row);

	list.InsertAt(0,day);
	if( show )
	{
		::SendMessage(m_hMsgWnd,WM_MODEL_RESULT_MSG,0,(long)&list);
		StatisticEquity();
	}
	return 0;
}

void CTradeStatistic2::SetStartTime(CTime tmBegin)
{
	m_tmBegin = tmBegin;
}

CString CTradeStatistic2::CalculateElapseTime()
{
	CString strElapsedTime;
	CTime tmNow = CTime::GetCurrentTime();
	CTimeSpan tElapsed = tmNow - m_tmBegin;
	strElapsedTime.Format("%d小时 %d分钟 %d秒", tElapsed.GetHours(), tElapsed.GetMinutes(), tElapsed.GetSeconds());
	return strElapsedTime;
}

void CTradeStatistic2::SetEveryDayEquity(const char* szDate, CStockEquity oEquity)
{
	m_mapEquityEachDay[szDate] = oEquity;
}

int CStockTradeStatistic::Statistic(CString day,bool show)
{
	CStringArray list;
	list.Add("序号,合约,日期,开仓时,数量,价格,方向,开平,盈亏,手续费,权益,");
	CStringArray arDirection;
	arDirection.Add("买");
	arDirection.Add("卖");
	CStringArray arOpenClose;
	arOpenClose.Add("开");
	arOpenClose.Add("平");


	CStringArray arTempPrice;
	CStringArray arTempCount;

	double maxquanyi=0;
	double maxhuiche=0;
	//CString strMaxHuicheTime;
	CString str;

	int yearbegin=0;
	CTradeStaYear * pStaYear=0;
	double beginQuanyi=0;
	CPtrArray arYear;
	double sum_yk=0;

	int countyingli = 0;
	int countkuisun=0;
	double allyingli=0;
	double allkuisun=0;
	double HuicheMax=0;
	CString HuicheTime;
	double HuicheMaxAll=0;
	CString HuicheTimeAll;

	m_nTradeCount = m_arListTrade.GetSize();

	std::map<string, CTradeRecord2*> mapOpenPostion;

	for(int i=0;i<m_arListTrade.GetSize();i++)
	{
		double yingkui=0;
		double shouxufei= 0;
		CTradeRecord2 * pt = (CTradeRecord2 *)m_arListTrade[i];

		CString time(pt->day);
		int year = atoi(time.Mid(0,4));

		shouxufei = pt->price * pt->count * m_jiazhi * m_feilv;
		CString row;

		if( pt->openorclose == 0 )
		{
			m_beginquanyi = m_beginquanyi - shouxufei;
			row.Format("%d,%s,%s,%s,%d,%.2f,%s,%s,%.2f,%.2f,%.2f,",i,pt->heyue, pt->day,pt->time1,pt->count,pt->price,arDirection[pt->direction],arOpenClose[pt->openorclose],yingkui,shouxufei,m_beginquanyi);
			list.Add(row);

			//str.Format("%.2f",pt->price);
			//arTempPrice.Add(str);

			//str.Format("%d",pt->count);
			//arTempCount.Add(str);
			
			mapOpenPostion[pt->heyue] = pt;
		}
		else
		{
			yingkui = 0;

			CTradeRecord2 * ptOpen = mapOpenPostion[pt->heyue];
			if (!ptOpen)
			{
				return -1;
			}
			yingkui += ptOpen->count * ( pt->price - ptOpen->price)  * m_jiazhi ;

			//
			//for(int j=0;j<arTempPrice.GetSize();j++)
			//{
			//	if( pt->direction ==0 )//BP
			//	{
			//		yingkui += atoi(arTempCount[j]) * (atof(arTempPrice[j])-pt->price) *m_jiazhi ;
			//	}
			//	else
			//	{
			//		yingkui += atoi(arTempCount[j]) * ( pt->price -atof(arTempPrice[j])  ) *m_jiazhi ;
			//	}
			//}
			//arTempPrice.RemoveAll();
			//arTempCount.RemoveAll();

			m_beginquanyi += yingkui - shouxufei;

			if( m_beginquanyi > maxquanyi )
				maxquanyi = m_beginquanyi;

			pt->huiche =  maxquanyi - m_beginquanyi ;

			if( pt->huiche > HuicheMaxAll )
			{
				HuicheMaxAll = pt->huiche;
				HuicheTimeAll.Format("%s %s",pt->day, pt->time1);
			}

			//统计平均盈利和亏损
			if( yingkui > 0 )
			{
				allyingli += yingkui ;
				countyingli++;
			}
			else
			{
				allkuisun += yingkui;
				countkuisun++;
			}


			row.Format("%d,%s,%s,%s,%d,%.2f,%s,%s,%.2f,%.2f,%.2f,",i,pt->heyue,pt->day,pt->time1,pt->count,pt->price,arDirection[pt->direction],arOpenClose[pt->openorclose],yingkui,shouxufei,m_beginquanyi);
			list.Add(row);



			sum_yk += yingkui- shouxufei * 2;
			if( sum_yk>m_sum_max)
			{
				m_sum_max = sum_yk;			
			}
			pt->huiche = m_sum_max-sum_yk;
			if( pt->huiche > HuicheMax )
			{
				HuicheMax = pt->huiche;
				HuicheTime.Format("%s %s",pt->day,pt->time1);
			}
		}

		//分年统计
		if(yearbegin==0  )
		{			
			pStaYear = new CTradeStaYear;
			arYear.Add(pStaYear);
			yearbegin = year;
			pStaYear->year = yearbegin;

			beginQuanyi = m_beginquanyi;
		}
		else if (pStaYear && year != pStaYear->year)
		{
			//对前一年进行统计
			pStaYear->shouyi = m_beginquanyi-beginQuanyi;
			pStaYear->maxhuiche = HuicheMax;
			strcpy_s(pStaYear->szMaxHuicheTime,HuicheTime);

			sum_yk = 0;
			m_sum_max = 0;
			HuicheMax = 0;

			pStaYear = new CTradeStaYear;
			arYear.Add(pStaYear);
			yearbegin = year;
			pStaYear->year = yearbegin;

			beginQuanyi = m_beginquanyi;
		}	

	}

	//对前一年进行统计
	if( pStaYear)
	{
		pStaYear->shouyi = m_beginquanyi-beginQuanyi;
		pStaYear->maxhuiche = HuicheMax;
		strcpy_s(pStaYear->szMaxHuicheTime,HuicheTime);
	}


	double avgYingli = allyingli /countyingli;
	double avgKuisun = allkuisun/countkuisun;
	double shenglv = 1.0* countyingli /(countyingli+countkuisun);
	double avgYingli2 = (allyingli+allkuisun)/(countyingli+countkuisun);


	CString row;
	row = ",,,,,,";
	list.Add(row);
	row = "测试统计,,,,,,";
	list.Add(row);
	row.Format("最大回撤,%.2f,",HuicheMaxAll);
	list.Add(row);
	row.Format("最大回撤时间,%s,",HuicheTimeAll);
	list.Add(row);
	row.Format("平均盈利,%.2f,",avgYingli);
	list.Add(row);
	row.Format("平均亏损,%.2f,",avgKuisun);
	list.Add(row);
	row.Format("胜率,%.2f,",shenglv);
	list.Add(row);
	row.Format("每手平均盈利,%.2f,",avgYingli2);
	list.Add(row);

	m_shenglv = shenglv;
	m_HuicheMax = HuicheMaxAll;
	m_avgYingli2 = avgYingli2;
	m_avgYingli = avgYingli;
	m_avgKuisun = avgKuisun;

	list.Add(",,,,,,,,");
	list.Add("分年统计,");
	list.Add("年份,收益,最大回撤,回撤时间,交易次数,平均盈利,平均亏损,每手盈利,胜率,");
	for(int i=0;i<arYear.GetSize();i++)
	{
		CTradeStaYear * pYear = (CTradeStaYear *)arYear[i];
		CString row;
		row.Format("%d,%.2f,%.2f,%s,",pYear->year,pYear->shouyi,pYear->maxhuiche,pYear->szMaxHuicheTime);
		list.Add(row);
	}

	row.Format("耗时,%s", CalculateElapseTime());
	list.Add(row);

	list.InsertAt(0,day);
	if( show )
	{
		::SendMessage(m_hMsgWnd,WM_MODEL_RESULT_MSG,0,(long)&list);
		StatisticEquity();
	}
	return 0;
}

