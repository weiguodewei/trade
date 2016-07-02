#if !defined(MARKDATATRADERAPI_LOG_H)
#define MARKDATATRADERAPI_LOG_H

#include "public.h"

enum LOGINFO_TYPE{ 
	LOGINFO_Request,
	LOGINFO_Response, 
	LOGINFO_Return
};

struct CTraderApiLog
{
	LOGINFO_TYPE       logType;
	int                nFunction;
	int                nIssueType;
	int                nRequestID;
	int                ndate;
	char               sztime[16];
	void*              pBuffer;
	int                nLength;
	void*              pReturnBuf;
	int                nReturnLen;
};

///行情日志
class CMarkDataTraderApi_log
{
public:
	CMarkDataTraderApi_log(char* szFundAccount)
	{
		logLock.Lock();
		char szFileName[1024];
		CDate now;
		mkdirFullPath("./Debug");
		sprintf(szFileName,"./Debug/%s", szFundAccount);
		mkdirFullPath(szFileName);
		sprintf(szFileName,"./Debug/%s/MarkDataTraderLog_%d_%d.log", szFundAccount, now.GetDate(), now.GetTimeStamp());
		m_fp = fopen(szFileName, "ab");
		logLock.UnLock();

		bLogInfoStop = false;
		trd.Start(MarkDataTraderLog, 0, this);
	}

	virtual ~CMarkDataTraderApi_log()
	{
		logLock.Lock();
		fflush(m_fp);
		fclose(m_fp);
		logLock.UnLock();

		if(trd.IsRunning())
		{
			bLogInfoStop = true;
			trd.Join();
		}
	}

	void AddLog(LOGINFO_TYPE logType, int nFunction, int nIssueType, void* pBuffer, int nLength, void* pReturnBuf = NULL, int nReturnLen = 0)
	{
		logLock.Lock();
		CDate now;
		CTraderApiLog* lpLog = new CTraderApiLog();
		lpLog->logType = logType;
		lpLog->nFunction = nFunction;
		lpLog->nIssueType = nIssueType;
		lpLog->ndate = now.GetDate();
		now.GetTimeStr(lpLog->sztime, ':');
		lpLog->pBuffer = NULL;
		lpLog->pReturnBuf = NULL;
		if(pBuffer)
		{
			lpLog->pBuffer = malloc(nLength);
			memcpy(lpLog->pBuffer, pBuffer, nLength);
		}
		if(pReturnBuf)
		{
			lpLog->pReturnBuf = malloc(nReturnLen);
			memcpy(lpLog->pReturnBuf, pReturnBuf, nReturnLen);
		}
		msgPush.Push(lpLog);
		logLock.UnLock();
	}

	void WriteLog(const char * szMsg)
	{
		fwrite(szMsg, strlen(szMsg), 1, m_fp);
		fwrite("\r\n", 2, 1, m_fp);
		fflush(m_fp);
	}
private:
	volatile bool           bLogInfoStop;
	CThread                 trd;
	CMessageQueue<CTraderApiLog*> msgPush;
	ManualLock              logLock;
	FILE *					m_fp;

	static void MarkDataTraderLog(void* lpvoid);
};

#endif