#include "stdafx.h"
#include "FunctionPub.h"
#include <minwinbase.h>


CFunctionPub::CFunctionPub(void)
{
}


CFunctionPub::~CFunctionPub(void)
{
}


void CFunctionPub::ParseParam(CString param, CStringArray & list,char ch)
{
	int nBegin = 0;

	int nEnd = param.Find(ch);

	if( nEnd == -1 )
		list.Add(param);

	while( nEnd >= 0) 
	{
		list.Add( param.Mid(nBegin,nEnd-nBegin) );

		nBegin = nEnd + 1;

		nEnd = param.Find(ch,nBegin);
	}
}


void CFunctionPub::Copy(char * pt,int len,CString &str)
{
	strcpy_s(pt,len,(char *) str.GetBuffer(str.GetLength())  );
}

CString CFunctionPub::GetModuleDir()
{
	char sModulePath[MAX_PATH];
	GetModuleFileName(NULL,sModulePath,MAX_PATH);
	CString strPath(sModulePath);
	int nt = strPath.ReverseFind('\\');
	strPath = strPath.Left(nt);
	return strPath;
}



//szDate:YYYYMMDD
//ret:0,suc; -1,fail
int CFunctionPub::String2Time(const char* szDate, time_t &tmDate)
{
	struct tm dstDate;
	if(szDate && strlen(szDate) >= 8)
	{
		char szYear[8] = {0};
		char szMonth[8] = {0};
		char szDay[8] = {0};
		ZeroMemory((void*)&dstDate, sizeof(dstDate));
		
		strncpy(szYear, szDate, 4);
		dstDate.tm_year = atoi(szYear) - 1900;
		strncpy(szMonth, szDate + 4, 2);
		dstDate.tm_mon = atoi(szMonth) - 1;
		strncpy(szDay, szDate + 6, 2);
		dstDate.tm_mday = atoi(szDay);
		//20160428 14:25:03
		if (strlen(szDate) == 17)
		{
			CString strTime(szDate);
			strTime = strTime.Right(8);

			int nt = strTime.Find(':');
			int nt2 = strTime.ReverseFind(':');
			char szHour[8] = {0};
			char szMin[8] = {0};
			char szSec[8] = {0};
			if (2 == nt && 5 == nt2)
			{
				strcpy(szHour, strTime.Left(2));
				strcpy(szMin, strTime.Mid(nt + 1, 2));
				strcpy(szSec, strTime.Right(2));

				dstDate.tm_hour = atoi(szHour);
				dstDate.tm_min = atoi(szMin);
				dstDate.tm_sec = atoi(szSec);
			}
			
			

		}
		else if (strlen(szDate) == 15)//20160428 142503
		{
			char szHour[8] = {0};
			char szMin[8] = {0};
			char szSec[8] = {0};
			CString strTime(szDate);
			strTime = strTime.Right(6);
			strcpy(szHour, strTime.Left(2));
			strcpy(szMin, strTime.Mid(2, 2));
			strcpy(szSec, strTime.Right(2));

			dstDate.tm_hour = atoi(szHour);
			dstDate.tm_min = atoi(szMin);
			dstDate.tm_sec = atoi(szSec);
		}

		
		tmDate = mktime(&dstDate);

		return 0;
	}

	return -1;
}

int CFunctionPub::String2Time(const char* szDate, CTime &tTime)
{
	time_t tm;
	int iRet = String2Time(szDate, tm);
	tTime = CTime(tm);
	return iRet;
}

CString CFunctionPub::GetFullDateString(const char* sTime)
{
	CString strFullTime;
	if (sTime && strlen(sTime) > 8)
	{
		return (char*)sTime;
	}
	
	CTime tmNow = CTime::GetCurrentTime();

	strFullTime.Format("%s %s", tmNow.Format("%Y%m%d"), sTime);

	return strFullTime;
}

void CMediaAux::PlayWavSound(HS_SOUND_TYPE eType)
{
	CString sWavPath = CFunctionPub::GetModuleDir();
	
	switch (eType)
	{
	case E_HS_SOUND_MSG:
		sWavPath += "\\sound\\msg.wav";
		break;
	case E_HS_SOUND_SIGNAL:
		sWavPath += "\\sound\\match.wav";
		break;
	case E_HS_SOUND_TRADE_RTN:
		sWavPath += "\\sound\\notify.wav";
		break;
	case E_HS_SOUND_TRADE_CANCEL:
		sWavPath += "\\sound\\type.wav";
		break;
	default:
		sWavPath += "\\sound\\msg.wav";
		break;
	}

	PlaySound(sWavPath, NULL, SND_FILENAME|SND_ASYNC ); 
}