#include "StdAfx.h"
#include "CinfoLogger.h"

using namespace std;


// 类实例静态指针（初始为空）
CinfoLogger* CinfoLogger::_instance = NULL;


CinfoLogger::CinfoLogger(void)
{	
	// 启动信息
	m_strWriteStrInfo = "";

	// 日志文件名称
	m_strLogFileName ="strlog/"+GetCurTimeToString(YMD) + ".log";

	// 创建日志线程
	m_bRun = true;
	//m_hThread = boost::thread( boost::bind( &CinfoLogger::LogProcStart, this ));
	m_hThread = thread( bind( &CinfoLogger::LogProcStart, this ));
}



void  CinfoLogger::LogProcStart()
{
	int nCount = 1;
	clock_t preSaveTime = clock();
	do 
	{
		Sleep(250);

		if ( (clock() - preSaveTime) > TIME_TO_WRITE_LOG )
		{
			preSaveTime = clock();
			WriteLogToFile();	// 每隔五秒写一次日志
		}

	} while (m_bRun);

	if (!m_strWriteStrInfo.empty())
		WriteLogToFile();	// 退出循环后如果缓存非空，则将缓存写入日志 再退出线程

	std::cout << LogInfo(LOG_LEVEL_SUB_1, "[- Info Logger] 正在退出日志线程！") << std::endl;
}



void CinfoLogger::WriteLog(std::string strLog)
{
	if (m_logMutex.try_lock())
	{
		if ( !strLog.empty() )
		{
			m_strWriteStrInfo += GetCurTimeToString(LOG)+"   " + strLog + ("\r\n");
		
			//m_strWriteStrInfo += GetCurTimeToString(LOG)+"   ";
			//m_strWriteStrInfo += strLog;
			//m_strWriteStrInfo += ("\r\n");

			//m_strWriteStrInfo += _T("\r\n");
		
		}
	m_logMutex.unlock();
	}
	if ( m_strWriteStrInfo.length() > MAX_STR_BUF_LEN )
	{			
		WriteLogToFile();	// 达到一定量时才写入到文件中
	}
}



void CinfoLogger::WriteLogToFile()
{
	if (m_logMutex.try_lock())
	//m_logMutex.lock()
	//if (1)
	{
		try
		{
			// 以 Append 方式打开日志文件
			ofstream osWriter( m_strLogFileName.c_str(),  ios::out | ios_base::app );

			if (osWriter.is_open())
			{
				// 写入日志缓存
				osWriter << m_strWriteStrInfo.c_str();

				// 清空日志缓存
				m_strWriteStrInfo = "";

				// 关闭文件写入器
				osWriter.close();
			}
		}
		catch (std::exception e)
		{
			cout << LogInfo(LOG_LEVEL_FORMAT, "写入文件失败：%s", e.what()) << endl;
		}

		//文件是否过大需要新生成文件
		isNewFile();

		m_logMutex.unlock();
	}
}



CinfoLogger* CinfoLogger::instance()
{
	// 若静态指针为空，则创建类实例，并获取指针
	if (_instance == NULL)
	{
		_instance = new CinfoLogger();
	}
	// 返回静态指针
	return _instance;
}



string CinfoLogger::LogInfo(int level,char *fmt, ...)
{
	// 格式化输入的字符串 fmt 到输出字符串 out
	char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, fmt, body);    
	va_end(body);    

	// 将日志信息转换为 string 字符串，存入缓存变量中
	string strTemp(out);	              

	//日志级别需要大于设定级别 或者 小于 0，则只返回字符串，不作记录
	if(level > CinfoLogger::instance()->LogLevel() || level < 0)
	{
		return strTemp;
	}

	// 在输出窗口显示日志信息
	OutputDebugStringA(out); 
	OutputDebugStringA("\n");

	// 将信息存入日志缓存中
	WriteLog( strTemp );

	return strTemp;
}




string CinfoLogger::LogInfo(int level, string info)
{
	//日志级别需要小于或等于设定级别，否则不记录
	if(level > CinfoLogger::instance()->LogLevel())
	{
		return info;
	}

	if (level >= 0)
	{
		// 在输出窗口显示日志信息
		OutputDebugStringA(info.c_str()); 
		// 将信息存入日志缓存中
		WriteLog( info );
	}

	return info;
}



 
string CinfoLogger::GetCurTimeToString(TIME_STRING_MODE mode /* = FOR_TEXT */)
{
	int strLen = 20;
	string retStr = "0";

	if (mode == FOR_TEXT)
	{
		char *pszCurrTime1 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime1)
		{
			memset(pszCurrTime1, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime1, strLen , "%Y-%m-%d %H:%M:%S", localtime(&now));
			//strftime(pszCurrTime1, strLen , "%Y-%m-%d %H:%M:%S", localtime_s(&now));
			retStr = pszCurrTime1;
			free(pszCurrTime1);
			pszCurrTime1 = NULL;
		}
		
		

		

		
	} 	
	else if (mode == FOR_FILE_NAME)
	{
		char *pszCurrTime2 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime2)
		{
			memset(pszCurrTime2, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime2, strLen , "%Y_%m_%d_%H_%M_%S", localtime(&now));

			retStr = pszCurrTime2;

			free(pszCurrTime2);
		}
		
	}
	else if(mode == YMD)
	{
		char *pszCurrTime3 = (char*)malloc(sizeof(char)*strLen);
		

		if (pszCurrTime3)
		{
			memset(pszCurrTime3, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime3, strLen , "%Y%m%d", localtime(&now));
			retStr = pszCurrTime3;

			free(pszCurrTime3);
			pszCurrTime3 = NULL;
		}

	}
	else if(mode == LOG)
	{
		//char *pszCurrTime4 = (char*)malloc(sizeof(char)*strLen);
		//memset(pszCurrTime4, 0, sizeof(char)*strLen);
		//time_t now;
		//time(&now);
		//strftime(pszCurrTime4, strLen , "%H:%M:%S", localtime(&now));

		//retStr = pszCurrTime4;

		SYSTEMTIME  time;
		GetLocalTime(&time);
		char* strTime =(char*)malloc(sizeof(char)*strLen);;
		if (strTime)
		{
			sprintf(strTime, "%02d:%02d:%02d:%03d", time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
			retStr =  strTime;
			free(strTime);
			strTime = NULL;
		}
		

	}
	else if(mode == NEWFILE)
	{
		char *pszCurrTime4 = (char*)malloc(sizeof(char)*strLen);
		if (pszCurrTime4)
		{
			memset(pszCurrTime4, 0, sizeof(char)*strLen);
			time_t now;
			time(&now);
			strftime(pszCurrTime4, strLen , "%Y%m%d%H%M%S", localtime(&now));

			retStr = pszCurrTime4;
			free(pszCurrTime4);
			pszCurrTime4 = NULL;
		}
	}

 	return retStr;	
}


int CinfoLogger:: LogLevel()
{
	return 3;

}

void CinfoLogger::isNewFile()
{

	  string sFileName= m_strLogFileName ;
	  ifstream in(sFileName.c_str());   
	  in.seekg(0,ios::end);   
	  long size = in.tellg();   
	  in.close();   
	  cout<< "文件大小为："<<size<<" 字节"<<endl; 

	  if(size > 10 * 1024 * 1024)
	  //if(size > 10 * 1024 )
	  {
		m_strLogFileName = "strlog/"+GetCurTimeToString(NEWFILE) + ".log";
	  }
  
}
