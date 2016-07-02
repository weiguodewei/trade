#ifndef _TOOL_H
#define _TOOL_H 1

#include<string>
using namespace std;

class CBaseTradeApi;
class CStrategyBase;
class CAccData;

class Tool
{
public:
	Tool();
	~Tool();

public:
	static CString TransBS(char bs,CBaseTradeApi* t);

	static CString TransKP(char kp,CBaseTradeApi* t);

	static CString TransStatus(char status,CBaseTradeApi* t);

	static int TransExchId(char * exchid);


	static bool isWeiBao(char status,CBaseTradeApi* t);    //未报

	static bool isDaiBao(char status,CBaseTradeApi* t);    //待报

	static bool isYiBao(char status,CBaseTradeApi* t);     //已报

	static bool isBuChe(char status,CBaseTradeApi* t);     //部撤

	static bool isYiChe(char status,CBaseTradeApi* t);     //已撤

	static bool isBuCheng(char status,CBaseTradeApi* t);   //部成

	static bool isYiCheng(char status,CBaseTradeApi* t);   //已成

	static bool isFeiDan(char status,CBaseTradeApi* t);    //废单


	static string GetProgramDir();

	static CString getTimeStr();

	static string GetModelDir();

	static string getCodePrefix(string code);

	static int getDayofWeek();

	//解析socket传过来的xml格式字符串
	static char* getRootAttribute(int type,string s);

	static int getSessionid(string s);

	static char* getErrorInfo(string s);

	static int getEntrustNo(string s);


	//构造请求包
	static string TransEntrustInfo(char * instrument, int count, char direction, char offsetflag, double price,int orderref,char *account);

	static string TransWithDraw(int entrustno,char *account);

	static string TransSubInfo(int type,char *account);

	static string TransQueryInfo(int type,char *account);

	//解析应答包
	static int TransRspEntrust(string s);

	static void TransRspWithdraw(string s,int &entrustno,int &withdraw,char *errorinfo);

	static void TransRspSub(string s,char *code,char *account,char *subtype,char *datatype,int *entrustno,int *bs,double *price,char *tradetime,int *vol);

	static void TransRspQuery(string s,char*account,CAccData*info);


private:

};


#endif

