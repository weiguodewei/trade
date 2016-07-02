#pragma once
#include <map>
#include <string>
using namespace std;

#define WM_MODEL_RESULT_MSG    WM_USER+1004
#define WM_MODEL_RESULT_EQUITY_MSG WM_USER+1005
struct CTradeRecord
{
	char day[9];
	char time1[19];
	char time2[19];
	int  timepass;
	double kprice;
	double pprice;
	int    direction;
	double yingkui;
	double quanyi;
	double shouxufei;
	int count;
	double maxquanyi;
	double huiche;
	CTradeRecord()
	{
		memset(this,0,sizeof(CTradeRecord));
	};
	void Cal(double jiazhi,double feilv)
	{
		if( direction == 0 )
		{
			yingkui = ( pprice - kprice ) * jiazhi * count;			
		}
		else if( direction == 1 )
		{
			yingkui = (  kprice-pprice  ) * jiazhi * count;
		}
		shouxufei = kprice * jiazhi * feilv  + pprice * jiazhi * feilv;
	}
};

struct CTradeStaYear
{
	int year;
	double shouyi;
	double maxhuiche;
	double sum_kuisun;
	double sum_yingli;
	double avg_yinli;
	char   szMaxHuicheTime[32];
	int    count;
};

struct CTradeRecord2
{
	char heyue[16];
	char day[9];
	char time1[19]; 
	double price;
	int count;
	int direction;
	int openorclose;
	double huiche;

	CTradeRecord2()
	{
		memset(this,0,sizeof(CTradeRecord2));
	}
};

struct CStockEquity
{
	double m_dbTotalEquity;
	double m_dbStockEquity;
};


class CTradeStatistic2
{
public:
	CTradeStatistic2(void);
	~CTradeStatistic2(void);
	int AddOpreate(char * day, char * time, double price , int count , int direction , int openorclose,char * heyue=NULL);
	int SetMsgWnd(HWND hwnd);
	int init();
	void SetBeginQuanyi(double iBeginQuanyi)
	{
		m_beginquanyi = iBeginQuanyi;
	}
	int virtual Statistic(CString day,bool show);
	void SetEveryDayEquity(const char* szDate, CStockEquity oEquity);
	void SetStartTime(CTime tmBegin);
	double m_jiazhi;
	double m_feilv;
protected:
	void StatisticEquity();
	CString CalculateElapseTime();
	CTime m_tmBegin;	
	
public:
	CPtrArray m_arListTrade;
	HWND m_hMsgWnd;
	double m_sum_shouxufei;
	double m_sum_yk;
	double m_beginquanyi;//初始权益
	double m_sum_max;//最大权益
	double m_quanyiMax;//最大权益
	int m_nTradeCount;
	double m_shenglv;
	double m_HuicheMax;
	double m_avgYingli2;
	double m_avgYingli;
	double m_avgKuisun;

	std::map<string,CStockEquity> m_mapEquityEachDay;



};

class CStockTradeStatistic : public CTradeStatistic2
{
public:
	int virtual Statistic(CString day,bool show);

};