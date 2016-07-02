#pragma once
#include<vector>
#include "typedefine.h"
#include <map>
#include <string>
#include "public.h"
#include <afxmt.h>
using namespace std;

class CKDataEx
{
	friend class CKDataManagerEx;
public:
	
	CKDataEx(void);
	~CKDataEx(void);

	char code[31];
	std::vector<CKStrdata> m_kMin1;     //“ª∑÷÷”Kœﬂ

	bool IsLastKMin()
	{
		return m_bLastKMin;
	}
protected:
	bool m_bLastKMin;

	
};
 
class CKDataOneDay
{
public:
	char szDate[10];
	std::map<string, CKDataEx*> m_mapKData;

	
};



class CKDataManagerEx
{
public:
	void PrepareData(CStringArray &arCodes, CTime tmBegin, CTime tmEnd);	
	int GetKDataListByDate(const char* szDate, CKDataOneDay** pvKData);
	void Recycle(CKDataOneDay* pKData);

	bool IsLastKData(const char* szCode, const char* szDate);

	void Dispose();
	CTime m_tmBegin;
	CTime m_tmEnd;

protected:

	void CollectLastKData();
	std::map<string, CKStrdata> m_mapLastK;

	std::map<string, LONGLONG> m_mapCursor;
	CStringArray m_arCodes;
	std::map<string, CKDataOneDay*> m_mapData;
	std::vector<CKDataOneDay*> m_vKDataRemove;
	CThread m_thdProduce;
	CThread m_thdClear;
	CMutex m_mtxData;
	CMutex m_mtxClear;
	static void Task_Produce(void *lpData);
	static void Task_Clear(void *lpData);

	LONGLONG ReadStringAcc(CString &strLine, LONGLONG &lAcc, int nWidth);
	LONGLONG ResumeFilePosition(CString &strLine, LONGLONG &lAcc);
	void OnClear();

	CEvent m_oDataReadyEvent;

	int LoadKMinData(CTime tBeginDate, int iDays = 1);

	int ReadKData(CTime tBeginDate, const char* szCode, const char* szFilePath, CKDataOneDay* pDayData, int iDays = 1); 

	static int s_nWidth;
};
