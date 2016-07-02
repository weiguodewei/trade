#ifndef TOOL_H
#define TOOL_H

#ifdef WIN32
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <Iphlpapi.h>
#include <process.h>
#endif

#ifdef LINUX 
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <pwd.h>

#define  Sleep(x) usleep((x)*1000)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <assert.h>

using namespace std;

extern "C" typedef void (*TDFUNC)(void*);

#pragma warning (disable:4996)
#pragma warning (disable:4100)

char *hs_strncpy(char *dest,const char *src, size_t size);
char *strtok_t(char *instr, char *delimit, char **saveptr);
bool isArbit(const char* contract);
void GetSQProductID(char* pid, const char* InstrumentID);
string NewClientName(const char* srvtype, const char* mac);
bool GetIpAddressByUrl(char* ip, const char* inurl);
bool GetLocalMACIP(char* macAddress, char* Ip,const char* desturl);
void RetDataRelease(void* pRetData);
void mkdirFullPath(char* path);
timeval CurrentTimeTag();
void system_pause(void);
void AddDate(int &val, int nday, bool IsTradingDay = false);



/////////////////////////////////////////////////CDate///////////////////////////////////////////////
class CDate
{
public:
	CDate();
	CDate(int year, int month, int day);
	CDate(int date);
	int GetDate();
	int GetTime();
	void GetTimeStr(char* time, char splitchar);
	int  GetMilliseconds() { return m_Milliseconds; }
	int  GetTimeStamp();
private:
	int m_iyear;
	int m_imonth;
	int m_iday;
	int m_ihour;
	int m_iMinutes;
	int m_iSeconds;
	int m_Milliseconds;
};

/////////////////////////////////////////////////ManualLock///////////////////////////////////////////////
#ifdef WIN32

class ManualLock
{
	CRITICAL_SECTION cs;
public:
	ManualLock()
	{
		InitializeCriticalSection(&cs);
	}
	inline void Lock()
	{
		EnterCriticalSection(&cs);
	}
	inline void UnLock()
	{
		LeaveCriticalSection(&cs);
	}
	~ManualLock()
	{
		DeleteCriticalSection(&cs);
	}
};
#endif

#ifdef LINUX
class ManualLock
{
	pthread_mutex_t lock;
public:
	ManualLock()
	{
		pthread_mutex_init(&lock, NULL);
	}
	void Lock()
	{
		pthread_mutex_lock(&lock);
	}
	void UnLock()
	{
		pthread_mutex_unlock(&lock);
	}
	~ManualLock()
	{
		pthread_mutex_destroy(&lock);
	}
};
#endif

////////////////////////////////////////////////OperateSystem////////////////////////////////////////////////
class OperateSystem
{
public:
	static int  getCurrentProcessId();
	static void getComputerName(char* username, int buflen);
};

////////////////////////////////////////////////CThreadSafeValue////////////////////////////////////////////////
class CThreadSafeValue
{
	long volatile value;
#ifdef LINUX
	pthread_mutex_t mutex;
#endif
public:
	CThreadSafeValue(long val)
	{
		value = val;
#ifdef LINUX
		pthread_mutex_init(&mutex, NULL);
#endif
	};

	long GetValue()
	{
		return value;
	};

	long Increase()
	{
#ifdef WIN32
		return InterlockedIncrement(&value);
#endif
#ifdef LINUX
		long val = 0;
		pthread_mutex_lock(&mutex);
		val = ++value;
		pthread_mutex_unlock(&mutex);
		return val;
#endif
	};

	long Decrease()
	{
#ifdef WIN32
		return InterlockedDecrement(&value);
#endif
#ifdef LINUX
		long val = 0;
		pthread_mutex_lock(&mutex);
		val = --value;
		pthread_mutex_unlock(&mutex);
		return val;
#endif
	};
};

/////////////////////////////////////////////////CEventEx///////////////////////////////////////////////
class CEventEx
{
#ifdef WIN32
	HANDLE hEvent;
#endif
#ifdef LINUX
	pthread_cond_t ptcond;
	pthread_mutex_t mutex;
	bool bSignaled;
#endif
public:
	enum EventWaitState{ EVENT_SIGNALED, EVENT_TIMEOUT, EVENT_ERROR};
	CEventEx();
	~CEventEx();
	void Notify();
	void Reset();
	CEventEx::EventWaitState Wait(long timeout);
};

/////////////////////////////////////////////////CThread///////////////////////////////////////////////
#ifdef WIN32
class CThread
{
	HANDLE hThread;
	TDFUNC tdfunc;
	void*  lpvoid;
	bool   isrunning;
	static unsigned int _stdcall Win32TdFunc(void *lpvoid);
public:
	CThread() { isrunning = false; }
	void Start(TDFUNC lpfunc, int stacksize, void* lpvoid);
	void Join();
	bool IsRunning(){ return isrunning; }
};
#endif
#ifdef LINUX
class CThread
{
private:
	pthread_t threadid;
	TDFUNC tdfunc;
	void *lpvoid;
	bool isrunning;
	static void* LinuxTdFunc(void* lpvoid);
public:
	void Start(TDFUNC tdfunc, int stacksize, void* lpvoid);
	void Join();
	bool IsRunning() { return isrunning; }

};
#endif

/////////////////////////////////////////////////CMessageQueue///////////////////////////////////////////////
template<typename T> class  CMessageQueue
{
private:
	CEventEx evt;
	ManualLock lock;
	std::queue<T> lst;

	T TryPop()
	{
		T ret = NULL;
		lock.Lock();
		if( lst.size() <= 0)
		{
			ret = NULL;
		}
		else
		{
			ret = lst.front();
			lst.pop();
		}
		lock.UnLock();
		return ret;
	}

public:
	void Push(T item)
	{
		bool bNotify = false;
		lock.Lock();
		lst.push(item);
		if( lst.size() == 1 )
		{
			bNotify = true;
		}
		lock.UnLock();
		if( bNotify == true)
		{
			evt.Notify();
		}
	}

	T Pop(int timeout)
	{
		T ret = TryPop();
		if( ret == NULL && timeout != 0)
		{
			evt.Wait(timeout);
			ret = TryPop();
		}
		return ret;
	}
};

/////////////////////////////////////////////////CLogInfo///////////////////////////////////////////////
enum LOGINFO_LEVEL{ 
	LOGINFO_Save,
	LOGINFO_Event,
	LOGINFO_Warning, 
	LOGINFO_Error
};

struct LogInfoField
{
	LOGINFO_LEVEL      level;
	int                ndate;
	char               sztime[16];
	char               szMsg[1024];
};

class CLogInfo
{
public:
	CLogInfo(const char* szFileName)
	{
		logLock.Lock();
		if(szFileName != NULL && strcmp(szFileName, "") != 0)
		{
			mkdirFullPath("./Log");
			m_fp = fopen(szFileName, "ab");
		}
		logLock.UnLock();

		bLogInfoStop = false;
		trd.Start(DealLog,0,this);
	}

	virtual ~CLogInfo()
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

	void AddLog(const char* pszMsg, LOGINFO_LEVEL level = LOGINFO_Event);
	void WriteLog(const char * szMsg);
private:
	volatile bool           bLogInfoStop;
	CThread                 trd;
	CMessageQueue<LogInfoField*> msgPush;
	ManualLock              logLock;
	FILE *					m_fp;

	static void DealLog(void* lpvoid);
};




#endif
