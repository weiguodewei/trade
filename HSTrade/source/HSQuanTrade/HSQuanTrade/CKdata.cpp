#include "stdafx.h"
#include "CKdata.h"
#include<fstream>
#include<iostream>
#include "FunctionPub.h"
//#include "HundsunH5sdkApi.h"
//#include "h5sdkSpi_pilot.h"


/*
CKdata
*/
CKdata::CKdata()
{

}

int CKdata::ReadDayKData()
{
	//read k data file
	char sModulePath[MAX_PATH];
	GetModuleFileName(NULL,sModulePath,MAX_PATH);
	CString strPath(sModulePath);
	int nt = strPath.ReverseFind('\\');
	strPath = strPath.Left(nt);
	strPath += "\\1day\\";
	CString strFile;
	CString sCode = code;
	if ('6' == sCode.GetAt(0) || '9' == sCode.GetAt(0))   //这里把上证指数定义为合约999999
	{
		strFile.Format("%sSH%s.txt", strPath, sCode);
	}
	else
	{
		strFile.Format("%sSZ%s.txt", strPath, sCode);
	}



	CStdioFile file;

	if(!file.Open(strFile,CFile::modeRead))
		return -1;

	CString strKinfo;
	CString strCode =_T(" ");
	CString strName =_T(" ");
	CString strDate =_T(" ");
	CString strK =_T(" ");
	CString strH =_T(" ");
	CString strL =_T(" ");
	CString strS =_T(" ");
	CString strVol = _T(" ");

	file.ReadString(strKinfo);
	AfxExtractSubString (strCode,(LPCTSTR)strKinfo,0,' ');
	AfxExtractSubString (strName,(LPCTSTR)strKinfo,1,' ');

	file.ReadString(strKinfo);

	int iKSize = 0;
	while(file.ReadString(strKinfo))
	{

		AfxExtractSubString (strDate,(LPCTSTR)strKinfo,0,'\t');
		AfxExtractSubString (strK,(LPCTSTR)strKinfo,1,'\t');
		AfxExtractSubString (strH,(LPCTSTR)strKinfo,2,'\t');
		AfxExtractSubString (strL,(LPCTSTR)strKinfo,3,'\t');
		AfxExtractSubString (strS,(LPCTSTR)strKinfo,4,'\t');
		AfxExtractSubString (strVol,(LPCTSTR)strKinfo,5,'\t');

		
		if (strKinfo.GetLength() > 0 && strKinfo.GetAt(0) >= 48 &&  strKinfo.GetAt(0) <= 57)//过滤掉非正常数据，如文件末尾的'数据来源'
		{
			CKStrdata * sc = new CKStrdata();        //存入结构体
			sc->type = 1;
			//strcpy(sc->code,strCode.GetBuffer(0));
			//strcpy(sc->name,strName.GetBuffer(0));
			strcpy(sc->curDate,strDate.GetBuffer(0));

			sc->priceH = atof(strH);
			sc->priceL = atof(strL);
			sc->priceK = atof(strK);
			sc->priceS = atof(strS);
			sc->vol = atof(strVol);
			m_kDay.push_back(*sc);

			delete sc;
		}
		iKSize++;
		
	}
	file.Close();
	return iKSize;
}
bool CKdata::getDayKData(const char* szCurDate, CKStrdata &oKdata)
{
	
	if (0 == m_kDay.size())
	{
		ReadDayKData();
	}
	char szDate[9] = {0};
	strncpy(szDate, szCurDate, 8);
	bool bFound = false;
	std::vector<CKStrdata>::const_iterator itK = m_kDay.cbegin();
	while (itK != m_kDay.cend())
	{
		if (0 == strcmp(itK->curDate, szDate))
		{
			oKdata = *itK;
			bFound = true;
			break;
		}
		itK++;
		
	}
	

	return bFound;
}

//iDays, -n ~ n
bool CKdata::getNDayKData(const char* szCurDate, CKStrdata &oKdata, int iDays)
{
	CKStrdata oCurKData;
	if (0 == m_kDay.size())
	{
		ReadDayKData();
	}

	char szDate[9] = {0};
	strncpy(szDate, szCurDate, 8);
	bool bFound = false;

	unsigned int iPos;
	for (unsigned int i = 0; i < m_kDay.size(); i++)
	{
		std::vector<CKStrdata>::reference rfK = m_kDay.at(i);
		if (0 == strcmp(rfK.curDate, szDate))
		{
			iPos = i + iDays;
			if ( iPos >= 0 && iPos < m_kDay.size())
			{
				oKdata = m_kDay.at(iPos);
				bFound = true;
			}

			break;
		}
		
	}

	return bFound;
}


//获取前N天日线
int CKdata::getNDaySumKData(const char* szCurDate,std::vector<CKStrdata>* pvKData,int iDays)
{
	bool bFind = false;

	CKStrdata oCurKData;
	if (0 == m_kDay.size())
	{
		ReadDayKData();
	}

	char szDate[9] = {0};
	strncpy(szDate, szCurDate, 8);
	int iCount = -1;

	CKStrdata oKdata;
	std::vector<CKStrdata>::iterator itk;
	int i = 0;
	
	for( ; i < m_kDay.size(); i++)
	{
		std::vector<CKStrdata>::reference rfK = m_kDay.at(i);

		if (0 == strcmp(rfK.curDate, szDate))
		{
			i = i - 1;
			bFind = true;
			break;
		}	

	}

	if(!bFind)
	{
		i = i - 1;
	}

	if(i < iDays && i >= 0)
	{
		for(int k=0; k <= i; k++)
			pvKData->push_back(m_kDay[k]);
	}

	/*else if( i >= iDays)*/

	if( i >= iDays)
	{
		for(int k = i - iDays + 1 ; k <= i; k++)
			pvKData->push_back(m_kDay[k]);
	}	

	iCount = pvKData->size();

	return iCount;
}



bool CKdata::CalcMa20(const char* szTime, double& dbMa20)
{
	if (m_hsMa20.end() != m_hsMa20.find(szTime))
	{
		dbMa20 = m_hsMa20[szTime];
		return true;
	}
	else
	{
		CKStrdata oPreKData2;
		if (!getNDayKData(szTime, oPreKData2, -20))
		{
			return false;
		}


		for (int i = 20; i > 0; i--)
		{
			getNDayKData(szTime, oPreKData2, -i);
			dbMa20 += oPreKData2.priceS;
		}
		dbMa20 = dbMa20 / 20;

		m_hsMa20.clear();

		m_hsMa20[szTime] = dbMa20;


		
		return true;
	}


}

bool CKdata::GetLastKDataDate(CTime &tmLast)
{
	if (0 == m_kDay.size())
	{
		ReadDayKData();
	}
	
	if (0 == m_kDay.size() )
	{
		return false;
	}
	CKStrdata oData = m_kDay.at(m_kDay.size() - 1);
	if (0 == CFunctionPub::String2Time(oData.curDate, tmLast))
	{
		return true;
	}
	return false;
}


//szDate1 < szDate2
int CKdata::CalDaysBetween2DayK(const char* szDate1, const char* szDate2)//从历史日K线数据计算两根K线间隔交易日数
{
	int iDays = 0;

	if (0 == m_kDay.size())
	{
		ReadDayKData();
	}
	if (0 == m_kDay.size())
	{
		return 0;
	}
	bool bDate1 = false;
	bool bDate2 = false;
	unsigned int i;
	for( i = 0; i < m_kDay.size(); i++)
	{
		std::vector<CKStrdata>::reference rfK = m_kDay.at(i);

		if (0 == strcmp(rfK.curDate, szDate1))
		{	
			bDate1 = true;
			break;
		}	

	}
	unsigned int j;
	for ( j = i; j < m_kDay.size(); j++)
	{
		std::vector<CKStrdata>::reference rfK = m_kDay.at(j);

		if (0 == strcmp(rfK.curDate, szDate2))
		{	
			bDate2 = true;
			break;
		}
	}
	if (bDate1 && bDate2)
	{
		iDays = j - i;
	}

	return iDays;
}
/*
CKdataManager
*/
CKdataManager::CKdataManager()
{

}

CKdataManager::~CKdataManager()
{

}

bool CKdataManager::getMinKData(char *sCode, std::vector<CKStrdata>** pvKData)
{

	if (m_kData.end() != m_kData.find(sCode))
	{
		*pvKData = &m_kData[sCode]->m_kMin1;
		return true;
	}
	else
	{
		char sModulePath[MAX_PATH];
		GetModuleFileName(NULL,sModulePath,MAX_PATH);
		CString strPath(sModulePath);
		int nt = strPath.ReverseFind('\\');
		strPath = strPath.Left(nt);
		strPath += "\\1min\\";
		CString strCode = sCode;
		CString strFile;
		if ('6' == strCode.GetAt(0))
		{
			strFile.Format("%sSH#%s.txt", strPath, strCode);
		}
		else
		{
			strFile.Format("%sSZ#%s.txt", strPath, strCode);
		}
		ReadKdata(sCode, strFile.GetBuffer(), 1);
		strFile.ReleaseBuffer();
		if (m_kData.end() != m_kData.find(sCode))
		{
			*pvKData = &m_kData[sCode]->m_kMin1;
			return true;
		}

	}

	return false;
}
 bool CKdataManager::getDayKData(const char* sCode, const char* szDate, CKStrdata &kData)
{
	string code(sCode);
	if (m_kData.end() != m_kData.find(code))
	{
		return m_kData[code]->getDayKData(szDate, kData);
	}
	else
	{
		CKdata* pData = new CKdata();
		strcpy(pData->code, sCode);
		m_kData[sCode] = pData;
		
		return pData->getDayKData(szDate, kData);

	}

	return false;
	
}


 bool CKdataManager::getNDayKData(const char* sCode, const char* szCurDate, CKStrdata &oKdata, int iDays)
 {
	 string code(sCode);

	 if (m_kData.end() != m_kData.find(code))
	 {
		 return m_kData[code]->getNDayKData(szCurDate, oKdata, iDays);
	 }
	 else
	 {
		 CKdata* pData = new CKdata();
		 strcpy(pData->code, sCode);
		 m_kData[sCode] = pData;

		 return pData->getNDayKData(szCurDate, oKdata, iDays);

	 }

	 return false;
 }

 int CKdataManager::getNDaySumKData(const char* sCode,const char* szCurDate,std::vector<CKStrdata>* pvKData,int iDays)
 {
	 int iRet = -1; 
	 string code(sCode);
	 if (m_kData.end() != m_kData.find(code))
	 {
		 return m_kData[code]->getNDaySumKData(szCurDate, pvKData, iDays);
	 }
	 else
	 {
		 CKdata* pData = new CKdata();
		 strcpy(pData->code, sCode);
		 m_kData[sCode] = pData;

		 return pData->getNDaySumKData(szCurDate, pvKData, iDays);

	 }
	 return iRet;
 }


void CKdataManager::ReadFile(int iType)
{

	string StrPath;

	string exePath = GetDir();

	if(iType==0)
	{
		StrPath = exePath+"\\1min\\*.txt";
	}
	else if(iType==1)
	{
		StrPath = exePath+"\\1min\\*.txt";
	}
	else
	{
		StrPath = exePath+"\\1min\\*.txt";
	}


	CFileFind find;

	int nt = find.FindFile(StrPath.c_str());

	while(nt)
	{
		nt = find.FindNextFileA();

		if( find.IsDirectory() || find.IsDots() )
		{
			
		}
		else
		{
			CString filename = find.GetFileName();
			int nt = filename.ReverseFind('#');
			CString sCode = filename.Mid(nt + 1, filename.GetLength() - nt - 1 - 4);//SZ#000001.txt
			CString name;

			name.Format("%s\\1min\\%s",exePath.c_str(),filename.GetBuffer(0));

			ReadKdata(sCode.GetBuffer() ,name.GetBuffer(0),iType);
			sCode.ReleaseBuffer();
	
		}

	}
}


void CKdataManager::ReadKdata(char *sCode,char *sPath,int iType)
{

	string code(sCode);

	CKdata *ck;

	if (m_kData.end() != m_kData.find(code))
	{
		ck = m_kData[code];
	}
	else
	{
		ck = new CKdata();
	}

	CStdioFile file;

	if(!file.Open(sPath,CFile::modeRead))
		return;

	CString strKinfo;
	CString strCode =_T(" ");
	CString strName =_T(" ");
	CString strDate =_T(" ");
	CString strTime =_T(",");
	CString strK =_T(" ");
	CString strH =_T(" ");
	CString strL =_T(" ");
	CString strS =_T(" ");
	CString strVol =_T(" ");

	//第一行读取合约信息
	file.ReadString(strKinfo);
	AfxExtractSubString (strCode,(LPCTSTR)strKinfo,0,' ');
	AfxExtractSubString (strName,(LPCTSTR)strKinfo,1,' ');

	//第二行跳过字段名字
	file.ReadString(strKinfo);

	while(file.ReadString(strKinfo))
	{
		//日线
		if(iType==0)
		{
			AfxExtractSubString (strDate,(LPCTSTR)strKinfo,0,'\t');
			strTime = "";
			AfxExtractSubString (strK,(LPCTSTR)strKinfo,1,'\t');
			AfxExtractSubString (strH,(LPCTSTR)strKinfo,2,'\t');
			AfxExtractSubString (strL,(LPCTSTR)strKinfo,3,'\t');
			AfxExtractSubString (strS,(LPCTSTR)strKinfo,4,'\t');
			AfxExtractSubString (strVol,(LPCTSTR)strKinfo,5,'\t');
		}
		//1min线
		else if(iType==1)
		{
			AfxExtractSubString (strDate,(LPCTSTR)strKinfo,0,'\t');
			AfxExtractSubString (strTime,(LPCTSTR)strKinfo,1,'\t');
			AfxExtractSubString (strK,(LPCTSTR)strKinfo,2,'\t');
			AfxExtractSubString (strH,(LPCTSTR)strKinfo,3,'\t');
			AfxExtractSubString (strL,(LPCTSTR)strKinfo,4,'\t');
			AfxExtractSubString (strS,(LPCTSTR)strKinfo,5,'\t');
			AfxExtractSubString (strVol,(LPCTSTR)strKinfo,6,'\t');
		}

		if (strKinfo.GetLength() > 0 && strKinfo.GetAt(0) >= 48 &&  strKinfo.GetAt(0) <= 57)//过滤掉非正常数据，如文件末尾的'数据来源'
		{
			CKStrdata sc;// = new CKStrdata();        //存入结构体
			sc.type = iType;
		//	strcpy(sc.code,strCode.GetBuffer(0));
		//	strcpy(sc.name,strName.GetBuffer(0));
			strcpy(sc.curDate,strDate.GetBuffer(0));
			strcpy(sc.curTime,strTime.GetBuffer(0));
			sc.priceH = atof(strH);
			sc.priceL = atof(strL);
			sc.priceK = atof(strK);
			sc.priceS = atof(strS);
			sc.vol = atof(strVol);
			if (1 == iType)
			{
				ck->m_kMin1.push_back(sc);
			}
			else if (0 == iType)
			{
				ck->m_kDay.push_back(sc);
			}

			strcpy(ck->code,strCode.GetBuffer(0));

		}
		
	
	}
	file.Close();

	//string sCode(code);
	m_kData[sCode] = ck;

}


string CKdataManager::GetDir()
{
	char exeFullPath[MAX_PATH];

	string StrPath="";

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);

	StrPath = (string)exeFullPath;

	int pos = StrPath.find_last_of("\\",StrPath.length());

	return StrPath.substr(0,pos);
}

void CKdataManager::UnLoadData(const char* szCode)
{
	if (m_kData.end() != m_kData.find(szCode))
	{
		CKdata* pData = m_kData[szCode];
		if (pData)
		{
			//1min k data
			if (pData->m_kMin1.size() > 0)
			{
				pData->m_kMin1.clear();
				{
					std::vector<CKStrdata> vTemp;

					pData->m_kMin1.swap(vTemp);
				}
			}
			
			if (pData->m_kDay.size() > 0)
			{
				//day k data
				pData->m_kDay.clear();
				{
					std::vector<CKStrdata> vTemp2;
					pData->m_kDay.swap(vTemp2);

				}
			}
			if (pData->m_hsMa20.size() > 0)
			{
				pData->m_hsMa20.clear();
			}
			

			m_kData.erase(szCode);
			delete pData;
			pData = NULL;

			

			
		}

		
		
	}
}


void CKdataManager::FreeData()
{
	std::map<string, CKdata*> ::iterator it = m_kData.begin();

	for(;it!=m_kData.end();it++)
	{
		CKdata* temp = (CKdata*)it->second;
		delete temp;
		it->second = NULL;
	}

	m_kData.clear();

}

bool CKdataManager::CalcMa20(const char *sCode, const char* szTime, double& dbMa20)
{
	CKdata *ck;

	if (m_kData.end() != m_kData.find(sCode))
	{
		ck = m_kData[sCode];
		return ck->CalcMa20(szTime, dbMa20);
	}
	else
	{
		CString strPath = CFunctionPub::GetModuleDir();
		strPath += "\\1day\\";
		CString strFilePath;
		CString strCode = sCode;
		if (0 == strCode.Compare("999999"))
		{
			strFilePath.Format("%sSH%s.txt", strPath, strCode);
		}
		else if ('6' == strCode.GetAt(0))
		{
			strFilePath.Format("%sSH%s.txt", strPath, strCode);
		}
		else
		{
			strFilePath.Format("%sSZ%s.txt", strPath, strCode);
		}
		ReadKdata((char*)sCode, strFilePath.GetBuffer(), 0);
		if (m_kData.end() != m_kData.find(sCode))
		{
			ck = m_kData[sCode];
			return ck->CalcMa20(szTime, dbMa20);
		}
	}
	return false;
}

bool CKdataManager::GetLastKDataDate(const char* szCode, CTime &tmLast)
{
	CKdata *ck;
	if (m_kData.end() != m_kData.find(szCode))
	{
		ck = m_kData[szCode];
		return ck->GetLastKDataDate(tmLast);
	}
	else
	{
		CString strPath = CFunctionPub::GetModuleDir();
		strPath += "\\1day\\";
		CString strFilePath;
		CString strCode = szCode;
		if ('6' == strCode.GetAt(0) || '9' == strCode.GetAt(0))
		{
			strFilePath.Format("%sSH%s.txt", strPath, strCode);
		}
		else
		{
			strFilePath.Format("%sSZ%s.txt", strPath, strCode);
		}
		ReadKdata((char*)szCode, strFilePath.GetBuffer(), 0);
		if (m_kData.end() != m_kData.find(szCode))
		{
			ck = m_kData[szCode];
			return ck->GetLastKDataDate(tmLast);
		}
	}
	
	return false;
}


int CKdataManager::LoadStockList()
{

	FILE *oFile = fopen("./stock_list.txt", "r");


	if (NULL == oFile)
	{
		return 0;
	}
	fseek( oFile, 0, SEEK_END );
	int iSize = ftell(oFile);
	char *p = new char[iSize + 1];
	fseek(oFile, 0, SEEK_SET);
	int iRead = fread(p, sizeof(char), iSize, oFile);
	int iPos = 0;
	int iStart = 0;
	while (iPos < iSize)
	{
		if (*(p + iPos) == '\n')
		{
			char * szLine = (char*)malloc(7);
			memset(szLine, 0, 7);
			strncpy(szLine, p + iStart, 6);
			m_arCodes.push_back(szLine);
			iStart = iPos + 1;
		}
		iPos++;
	}

	fclose(oFile);


	return m_arCodes.size();
}

void CKdataManager::Init()
{
	m_bSupplementToday = false;
	if (0 == LoadStockList())
	{
		char msg[256];
		sprintf_s(msg,256,"未找到股票代码文件stock_list.txt");
		::SendMessage(m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)msg);
		return;
	}
	//m_thdAux.Start(Task_Aux, 0, this);

}
int CKdataManager::CalDaysBetween2DayK(const char* szCode, const char* szDate1, const char* szDate2)
{
	CKdata *pKData;
	if (m_kData.end() != m_kData.find(szCode))
	{
		pKData = m_kData[szCode];
		return pKData->CalDaysBetween2DayK(szDate1, szDate2);
	}
	else
	{
		CString strPath = CFunctionPub::GetModuleDir();
		strPath += "\\1day\\";
		CString strFilePath;
		CString strCode = szCode;
		if (0 == strCode.Compare("999999"))
		{
			strFilePath.Format("%sSH%s.txt", strPath, strCode);
		}
		else if ('6' == strCode.GetAt(0))
		{
			strFilePath.Format("%sSH%s.txt", strPath, strCode);
		}
		else
		{
			strFilePath.Format("%sSZ%s.txt", strPath, strCode);
		}
		ReadKdata((char*)szCode, strFilePath.GetBuffer(), 0);
		if (m_kData.end() != m_kData.find(szCode))
		{
			pKData = m_kData[szCode];
			return pKData->CalDaysBetween2DayK(szDate1, szDate2);
		}
	}
	return 0;
}
int CKdataManager::GetCodeList(vector<string>& arCodes)
{
	int iSize = 0;
	iSize = m_arCodes.size();
	if (iSize > 0)
	{
		std::copy(m_arCodes.begin(), m_arCodes.end(), std::back_inserter(arCodes));
	}

	return iSize;
}
void CKdataManager::Task_Aux(void *lpData)
{
	for (;;)
	{
		
		CKdataManager *pManager = (CKdataManager*)lpData;
		if (pManager)
		{
			
//#ifdef _DEBUG
//			pManager->m_bSupplementToday = false;
//			if (!pManager->m_thdSupplement.IsRunning() && !pManager->m_bSupplementToday)
//			{
//				pManager->m_thdSupplement.Start(Task_SupplementDayKData, 0, pManager);
//			}
//
//#else
//
#if 0
			CTime tmNow = CTime::GetCurrentTime();
			pManager->m_mtxSupplement.Lock();
			if (9 == tmNow.GetHour())
			{
				pManager->m_bSupplementToday = false;
			}
			pManager->m_mtxSupplement.Unlock();
			

			if (tmNow.GetHour() >= 15 && tmNow.GetMinute() >= 5 && !pManager->m_bSupplementToday)
			{
				if (!pManager->m_thdSupplement.IsRunning())
				{

					pManager->m_thdSupplement.Start(Task_SupplementDayKData, 0, pManager);
				}
			}
#endif

		}

		Sleep(10000);
	}
	
	
}

void CKdataManager::Task_SupplementDayKData(void *lpData)
{
#if 0
	char HqTypeCode[255];
	char ProdCode[255];

	int CandlePeriod;
	char HSExchangeType[255];
	char msg[256];
	CKdataManager *pManager = (CKdataManager*)lpData;
	if (pManager)
	{
		pManager->m_mtxSupplement.Lock();
		pManager->m_bSupplementToday = true;
		pManager->m_mtxSupplement.Unlock();

		CHundsunH5sdkApi* lpUserApi = CHundsunH5sdkApi::CreateHundsunH5sdkApi();
		CH5sdkSpi_Pilot* lpUserSpi = new CH5sdkSpi_Pilot(lpUserApi);
		lpUserApi->RegisterSpi(lpUserSpi);
		lpUserApi->RegisterFront("112.124.211.5:9999");
		lpUserApi->Init();

		//ShowList();
		lpUserSpi->WaitLoginCompleate();

		vector<string>::const_iterator itCode = pManager->m_arCodes.cbegin();
		while (itCode != pManager->m_arCodes.cend())
		{
			bool bStart = false;
			for(;;)
			{
				if(lpUserSpi->GetQueryStatus())
				{
					Sleep(100);
					continue;
				}
				else if (bStart)
				{
					break;
				}

				CTime tmLast;

				if (pManager->GetLastKDataDate(itCode->c_str(), tmLast))
				{
					if('6' == itCode->at(0))
					{
						strcpy(HqTypeCode, "XSHG");

					}
					else
					{
						strcpy(HqTypeCode, "XSHE");
					}



					strcpy(ProdCode, itCode->c_str());

					CandlePeriod = 6; //日K线-->6

					
					CTimeSpan tmSpan(1,0,0,0);
					tmLast += tmSpan;
					
					CString strTime = tmLast.Format("%Y%m%d");

					
					sprintf_s(msg,256,"HqTypeCode = %s, ProdCode = %s, CandlePeriod =%d, QueryDate = %s",
						HqTypeCode,
						ProdCode,
						CandlePeriod,
						strTime);
					::SendMessage(pManager->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)msg);

					lpUserSpi->SetHqTypeCode(HqTypeCode);
					lpUserSpi->SetProdCode(ProdCode);
					lpUserSpi->SetCandlePeriod(CandlePeriod);
					lpUserSpi->SetQueryDate(strTime);

					hs_strncpy(HSExchangeType, GetHSExchagneType(HqTypeCode), sizeof(HSExchangeType));

					


					lpUserSpi->ReqH5sdkCandleByRange();
					lpUserSpi->SetQueryStatus(true);
					bStart = true;
				}

				continue;
			}
			itCode++;
		}
		

	}

#endif
}
	