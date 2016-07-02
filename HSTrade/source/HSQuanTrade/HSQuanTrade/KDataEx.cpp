#include "stdafx.h"
#include "KDataEx.h"
#include "FunctionPub.h"


CKDataEx::CKDataEx(void)
{
	m_bLastKMin = false;
}


CKDataEx::~CKDataEx(void)
{
}


int CKDataManagerEx::GetKDataListByDate(const char* szDate, CKDataOneDay** pvKData)
{
	int iRet = -1;
	m_mtxData.Lock();
	std::map<string, CKDataOneDay*>::iterator itData = m_mapData.find(szDate);
	if (m_mapData.end() != itData)
	{
		*pvKData = itData->second;
		m_mapData.erase(itData);
		iRet = 0;
	}
	m_mtxData.Unlock();

	return iRet;
}
int CKDataManagerEx::s_nWidth = 2;
void CKDataManagerEx::PrepareData(CStringArray &arCodes, CTime tmBegin, CTime tmEnd)
{
	m_tmBegin = tmBegin;
	m_tmEnd = tmEnd;

	if (m_arCodes.GetCount() > 0)
	{
		m_arCodes.RemoveAll();	
	}

	m_arCodes.Append(arCodes);

	CollectLastKData();

	m_thdProduce.Start(Task_Produce, 0, this);
	m_thdClear.Start(Task_Clear, 0, this);

	WaitForSingleObject(m_oDataReadyEvent, INFINITE);

#ifdef _DEBUG
	int iLen = m_mapData.size();
	TRACE("test data's len: %d", iLen);
#endif
}

void CKDataManagerEx::Task_Produce(void *lpData)
{
	CKDataManagerEx* pManager = (CKDataManagerEx*)lpData;
	if (pManager)
	{
		CTime tmKData = pManager->m_tmBegin;

		
		while (tmKData < pManager->m_tmEnd)
		{
			
#ifdef _DEBUG
			CString strDate = tmKData.Format("%Y%m%d");

			if(strDate.Compare("20160219") == 0 )
			{
				//pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
				int xx = 0;
				xx++;
				printf("%d",xx);

			}
#endif

			if (2 == pManager->LoadKMinData(tmKData))
			{
				Sleep(500);
				continue;
			}
			tmKData += CTimeSpan(1,0,0,0);
		}
	}
	
}
void CKDataManagerEx::Task_Clear(void *lpData)
{
	CKDataManagerEx* pManager = (CKDataManagerEx*)lpData;
	if (pManager)
	{
		for (;;)
		{
			pManager->OnClear();
			Sleep(2000);
		}
		
	}
	
}
void CKDataManagerEx::OnClear()
{
	m_mtxClear.Lock();
	std::vector<CKDataOneDay*>::iterator itRemove;
	for (itRemove = m_vKDataRemove.begin(); itRemove != m_vKDataRemove.end(); )
	{
		CKDataOneDay* pDayData = *itRemove;
		if (pDayData)
		{
			std::map<string, CKDataEx*>::iterator itKData;
			for (itKData = pDayData->m_mapKData.begin(); itKData != pDayData->m_mapKData.end();)
			{
				CKDataEx* pDataEx = itKData->second;
				if (pDataEx)
				{
					pDataEx->m_kMin1.clear();
					{
						std::vector<CKStrdata> vTemp;

						pDataEx->m_kMin1.swap(vTemp);
					}

					itKData = pDayData->m_mapKData.erase(itKData);
					delete pDataEx;
					pDataEx = NULL;
				}


			}

			itRemove = m_vKDataRemove.erase(itRemove);
			delete pDayData;
			pDayData = NULL;
		}
		
		
		
	}
	{
		std::vector<CKDataOneDay*> vTemp;
		m_vKDataRemove.swap(vTemp);
	}
	m_mtxClear.Unlock();
}

void CKDataManagerEx::Recycle(CKDataOneDay* pKData)
{
	m_mtxClear.Lock();
	m_vKDataRemove.push_back(pKData);
	m_mtxClear.Unlock();

}
LONGLONG CKDataManagerEx::ReadStringAcc(CString &strLine, LONGLONG &lAcc, int nWidth)
{
	lAcc += strLine.GetLength();
	lAcc += nWidth;//0d0a
	return lAcc;
}

LONGLONG CKDataManagerEx::ResumeFilePosition(CString &strLine, LONGLONG &lAcc)
{
	lAcc -= strLine.GetLength();
	return lAcc;
}


int CKDataManagerEx::ReadKData(CTime tBeginDate, const char* szCode, const char* szFilePath, CKDataOneDay* pDayData, int iDays)
{
	CString strBeginDate = tBeginDate.Format("%Y%m%d");


	CStdioFile file;

	if(!file.Open(szFilePath,CFile::modeRead))
		return -1;

	CKDataEx *pData = new CKDataEx();
	if (!pData)
	{
		return -1;
	}

	//generate nDays date string
	

	strcpy(pData->code, szCode);
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

	string sCode(szCode);
	LONGLONG lCursor = 0;

	file.Seek(0, CFile::end);
	LONGLONG lFileLength = file.GetLength();


	if (m_mapCursor.find(sCode) == m_mapCursor.end())
	{

		m_mapCursor[sCode] = 0;
		
		//判断\r\n结尾，或是\n结尾
		char szBuf[256];
		file.Seek(0, CFile::begin);
		file.Read(szBuf, 256);
		if (!strstr(szBuf, "\r\n"))
		//if ( strstr(szBuf, "\r\n") > 0)
		{
			s_nWidth = 2;
		}
		else
		{
			s_nWidth = 1;
		}
		

		file.Seek(0, CFile::begin);
		//第一行读取合约信息
		file.ReadString(strKinfo);
		ReadStringAcc(strKinfo, lCursor, s_nWidth);
		AfxExtractSubString (strCode,(LPCTSTR)strKinfo,0,' ');
		AfxExtractSubString (strName,(LPCTSTR)strKinfo,1,' ');

		//第二行跳过字段名字
		file.ReadString(strKinfo);
		
		ReadStringAcc(strKinfo, lCursor, s_nWidth);
		
	}
	else
	{
		
		lCursor = m_mapCursor[sCode];

		if (lCursor >= 0 && lCursor < lFileLength)
		{
			file.Seek(lCursor, CFile::begin);
		}
		
	}
	


	while(file.ReadString(strKinfo))
	{

		
		AfxExtractSubString (strDate,(LPCTSTR)strKinfo,0,'\t');

		if (0 != strDate.Compare(strBeginDate))
		{
			time_t tmLine;
			CFunctionPub::String2Time(strDate, tmLine);

			CTime tLine(tmLine);
			if (tLine < tBeginDate)
			{

				ReadStringAcc(strKinfo, lCursor, s_nWidth);
				continue;
			}
			else if (tLine > tBeginDate)
			{
		
				
				
				break;
			}
			
		}
		
		ReadStringAcc(strKinfo, lCursor, s_nWidth);
		AfxExtractSubString (strTime,(LPCTSTR)strKinfo,1,'\t');
		AfxExtractSubString (strK,(LPCTSTR)strKinfo,2,'\t');
		AfxExtractSubString (strH,(LPCTSTR)strKinfo,3,'\t');
		AfxExtractSubString (strL,(LPCTSTR)strKinfo,4,'\t');
		AfxExtractSubString (strS,(LPCTSTR)strKinfo,5,'\t');
		AfxExtractSubString (strVol,(LPCTSTR)strKinfo,6,'\t');


		if (strKinfo.GetLength() > 0 && strKinfo.GetAt(0) >= 48 &&  strKinfo.GetAt(0) <= 57)//过滤掉非正常数据，如文件末尾的'数据来源'
		{
			CKStrdata sc;        //存入结构体
			sc.type = 1;
			//strcpy(sc.code,szCode);
		//	strcpy(sc.name,strName.GetBuffer(0));
			strcpy(sc.curDate,strDate.GetBuffer(0));
			strcpy(sc.curTime,strTime.GetBuffer(0));
			sc.priceH = atof(strH);
			sc.priceL = atof(strL);
			sc.priceK = atof(strK);
			sc.priceS = atof(strS);
			sc.vol = atof(strVol);

			
			if (lCursor == lFileLength)
			{
				pData->m_bLastKMin = true;
			}
			
			pData->m_kMin1.push_back(sc);

		}


	}
	m_mapCursor[sCode] = lCursor;
	file.Close();

	if (pDayData->m_mapKData.end() == pDayData->m_mapKData.find(szCode))
	{
		pDayData->m_mapKData[szCode] = pData;
	}
	
	return 0;
}

int CKDataManagerEx::LoadKMinData(CTime tBeginDate, int iDays)
{
	int iDataSize = 0;
	m_mtxData.Lock();
	iDataSize = m_mapData.size();
	m_mtxData.Unlock();
	if (iDataSize > 2)
	{
		return 2;
	}

	CString strBeginDate = tBeginDate.Format("%Y%m%d");
	CKDataOneDay* pDayData = new CKDataOneDay();

	if (!pDayData)
	{
		return -1;
	}
	

	strcpy(pDayData->szDate, strBeginDate.GetBuffer());
	strBeginDate.ReleaseBuffer();

	CString strPath = CFunctionPub::GetModuleDir();
	strPath += "\\1min\\";
	
	for (int i = 0; i < m_arCodes.GetCount(); i++)
	{
		CString strCode = m_arCodes.GetAt(i);
		CString strFilePath;
		if (strCode.Find("#") >= 0)
		{
			strFilePath.Format("%s%s.txt", strPath, strCode);
			strCode = strCode.Right(6);
		}
		else
		{
			if ('6' == strCode.GetAt(0))
			{
				strFilePath.Format("%sSH#%s.txt", strPath, strCode);
			}
			else
			{
				strFilePath.Format("%sSZ#%s.txt", strPath, strCode);
			}
		}
				
		ReadKData(tBeginDate, strCode, strFilePath, pDayData, iDays);
	}

	
	m_mtxData.Lock();
	m_mapData[pDayData->szDate] = pDayData;
	m_mtxData.Unlock();


	m_oDataReadyEvent.SetEvent();

	
	return 0;
}

void CKDataManagerEx::CollectLastKData()
{
	CString strPath = CFunctionPub::GetModuleDir();
	strPath += "\\1min\\";

	for (int i = 0; i < m_arCodes.GetCount(); i++)
	{
		CString strCode = m_arCodes.GetAt(i);
		CString strFilePath;
		if (strCode.Find("#") >= 0)
		{
			strFilePath.Format("%s%s.txt", strPath, strCode);
			strCode = strCode.Right(6);
		}
		else
		{
			if ('6' == strCode.GetAt(0))
			{
				strFilePath.Format("%sSH#%s.txt", strPath, strCode);
			}
			else
			{
				strFilePath.Format("%sSZ#%s.txt", strPath, strCode);
			}
		}

		CStdioFile oFile;

		if(!oFile.Open(strFilePath,CFile::modeRead))
			return;
		oFile.Seek(0, CFile::end);
		LONGLONG lFileLength = oFile.GetLength();
		if (lFileLength < 200)
		{
			continue;
			oFile.Close();
		}
		oFile.Seek(-200, CFile::end);
		CString strLine, strLastLine;
		while(oFile.ReadString(strLine))
		{
			if (strLine.GetLength() > 15)
			{
				strLastLine = strLine;
			}
			
		}
		oFile.Close();
		
		if (strLastLine.GetLength() > 15)
		{
			CString strDate, strTime, strK, strH, strL, strS, strVol;
			AfxExtractSubString (strDate,(LPCTSTR)strLastLine,0,'\t');
			AfxExtractSubString (strTime,(LPCTSTR)strLastLine,1,'\t');
			AfxExtractSubString (strK,(LPCTSTR)strLastLine,2,'\t');
			AfxExtractSubString (strH,(LPCTSTR)strLastLine,3,'\t');
			AfxExtractSubString (strL,(LPCTSTR)strLastLine,4,'\t');
			AfxExtractSubString (strS,(LPCTSTR)strLastLine,5,'\t');
			AfxExtractSubString (strVol,(LPCTSTR)strLastLine,6,'\t');
			if (strLastLine.GetLength() > 0 && strLastLine.GetAt(0) >= 48 &&  strLastLine.GetAt(0) <= 57)//过滤掉非正常数据，如文件末尾的'数据来源'
			{
				CKStrdata sc;        //存入结构体
				sc.type = 1;
				strcpy(sc.curDate,strDate.GetBuffer(0));
				strcpy(sc.curTime,strTime.GetBuffer(0));
				sc.priceH = atof(strH);
				sc.priceL = atof(strL);
				sc.priceK = atof(strK);
				sc.priceS = atof(strS);
				sc.vol = atof(strVol);

				m_mapLastK[strCode.GetBuffer()] = sc;

			}
		}
		

	}
}

bool CKDataManagerEx::IsLastKData(const char* szCode, const char* szDate)
{
	if (m_mapLastK.end() != m_mapLastK.find(szCode))
	{
		CKStrdata oData = m_mapLastK[szCode];
		if (0 == strcmp(oData.curDate, szDate))
		{
			return true;
		}
	}
	return false;
}



void CKDataManagerEx::Dispose()
{
	m_mapCursor.clear();
}