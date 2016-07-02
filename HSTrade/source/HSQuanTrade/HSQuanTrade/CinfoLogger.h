#ifndef _INFO_LOGGER_H_
#define _INFO_LOGGER_H_

#define		MAX_STR_BUF_LEN		30000	//日志缓存最大长度，超过则将日志缓存写入到本地文件
#define		TIME_TO_WRITE_LOG	5000	//定时时间（毫秒），超过则将日志缓存写入到本地文件

//using namespace std;
#pragma once

#include <stdio.h>
#include <tchar.h>

// TODO: 在此处引用程序需要的其他头文件
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include<string>
#include<wchar.h>
#include<thread>
#include<mutex>
#include <fstream>

#define	LOG_LEVEL_FORMAT	-1
#define	LOG_LEVEL_MAIN		0
#define	LOG_LEVEL_SUB_1		1
#define	LOG_LEVEL_SUB_2		2
#define	LOG_LEVEL_SUB_3		3


//-------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------
class CinfoLogger
{
public:

	static CinfoLogger* instance();

	std::string LogInfo(int level, char *fmt, ...);

	std::string LogInfo(int level, std::string info);
	
	void EnterFunction(std::string funcName)
	{
		LogInfo(1, "Enter function: " + funcName);
	}

	//重载1
	void EnterFunction(const char* funcName)
	{
		LogInfo(1, "Enter function: %s", funcName);
	}

	void LeaveFunction(std::string funcName)
	{
		LogInfo(1, "Leave function: " + funcName);
	}

	//重载1
	void LeaveFunction(const char* funcName)
	{
		LogInfo(1, "Leave function: %s", funcName);
	}

	enum TIME_STRING_MODE { 
		FOR_FILE_NAME,	// 按 文件名格式 获取时间字符串
		FOR_TEXT,		// 按 文本格式 获取时间字符串
		YMD,
		LOG,
		NEWFILE
	};

	std::string GetCurTimeToString(TIME_STRING_MODE mode = FOR_TEXT);


	void SetExit() { m_bRun = false; }

private:

	//-------------------------------------------------------------------------
	// 内部变量
	//-------------------------------------------------------------------------
	static CinfoLogger* _instance;	

	std::thread m_hThread;
	std::mutex m_logMutex;
	bool m_bRun;					// 线程循环使能标志

	std::string m_strWriteStrInfo;		// 日志文件缓存字符串
	std::string m_strLogFileName;		// 日志文件名，每次启动时按照系统时间自动生成

	//-------------------------------------------------------------------------
	// 内部函数
	//-------------------------------------------------------------------------
	

	CinfoLogger(void);

	void WriteLog(std::string strLog);

	void LogProcStart();

	void WriteLogToFile();

	int LogLevel();

	void isNewFile();

};

#endif