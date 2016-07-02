#include "public.h"

char *hs_strncpy(char *dest,const char *src, size_t size)
{
    if( dest == NULL)
        return 0;
    if( src == NULL)
    {
        dest[0] = 0;
        return 0;
    }
    char *ret = strncpy(dest, src, size - 1);
    dest[size-1] = 0;
    return ret;
}

char *strtok_t(char *instr, char *delimit, char **saveptr)
{
    char *sstr = instr;
    if( sstr == NULL)
    {
        sstr = *saveptr;
        if( sstr == NULL )
            return NULL;
    }
    unsigned char dmap[32];
    memset(dmap, 0, sizeof(dmap));
    while(*delimit)
    {
        unsigned char c = *delimit;
        dmap[c>>3] |= (1<<(c&7));
        delimit++;
    }
    char *estr = sstr; 
    while(*estr)
    {
        unsigned char c = *estr;
        if( dmap[c>>3] & (1<<(c&7)) )
        {
            goto FOUND;
        }
        estr++;
    }
FOUND:
    if( *estr != '\0' )
    {
        *estr = '\0';
        *saveptr = estr + 1;
    }
    else
    {
        *saveptr = NULL;
    }
    return sstr;
}

//UTF-8到GB2312的转换
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) 
		delete[] wstr;
	return str;
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) 
		delete[] wstr;
	return str;
}

bool isArbit(const char* contract)
{
	int i = 0;
	for(i=0; contract[i] != 0; i++)
	{
		if( contract[i] == '&')
			return true;
	}
	return false;
}

void GetSQProductID(char* pid, const char* InstrumentID)
{
	if( isArbit(InstrumentID) )
	{
		//SP SPC SPD IPS
		if( memcmp(InstrumentID, "SP ", 3) == 0 )
		{
			strcpy(pid, "SP");
		}
		else
		{
			memcpy(pid, InstrumentID, 3);
			pid[3]  = 0;
		}
	}
	else
	{
		memcpy(pid, InstrumentID, 2);
		if( InstrumentID[1] >= '0' && InstrumentID[1] <= '9')
		{
			pid[1] = 0;
		}
		else
		{
			pid[2] = 0;
		}
	}
}

std::string NewClientName(const char* srvtype, const char* mac)
{
	char buf[128];
	sprintf(buf,"%s%s%d",srvtype,mac,OperateSystem::getCurrentProcessId());
	return buf;
}

void RetDataRelease(void* pRetData)
{
	if(pRetData != NULL)
	{
		delete[] pRetData;
		pRetData = NULL;
	}
}

void mkdirFullPath(char* path)
{
	char* dupstr = strdup(path);
	for(int i=0; dupstr[i]; i++)
	{
		if(dupstr[i] == '\\' || dupstr[i] == '/' )
		{
			char c = dupstr[i];
#ifdef WIN32
			mkdir(dupstr);
#endif
#ifdef LINUX
			mkdir(dupstr,S_IRWXU | S_IRWXG);
#endif
			dupstr[i] = c;
		}
	}
#ifdef WIN32
	mkdir(dupstr);
#endif
#ifdef LINUX
	mkdir(dupstr,S_IRWXU | S_IRWXG);
#endif
}

timeval CurrentTimeTag()
{
#ifdef WIN32
	LARGE_INTEGER liFreq, liCounter;
	LONG64 ln64Freq, ln64Counter;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liCounter);
	ln64Freq = *((LONG64 *)&liFreq);
	ln64Counter = *((LONG64 *)&liCounter);
	LONG64 ln64Remainder = ln64Counter % ln64Freq;
	LONG64 ln64Sec = ln64Counter / ln64Freq;
	LONG64 ln64uSec = ln64Remainder * 1000 * 1000 / ln64Freq;
	timeval tv;
	tv.tv_sec = ln64Sec;
	tv.tv_usec = ln64uSec;
	return tv;
#endif
#ifdef LINUX
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv;
#endif
}

void system_pause(void)
{ 
#ifdef WIN32
	system("pause");
#endif
#ifdef LINUX
	puts("Press any key to continue..."); 
	system("stty raw"); 
	getchar();
	system("stty cooked");
#endif
}

void AddDate(int &val, int nday, bool IsTradingDay)
{
	if(!IsTradingDay)
	{
		SYSTEMTIME st = {};
		FILETIME ft;
		st.wYear = val/10000;
		st.wMonth = (val/100)%100;
		st.wDay	= val%100;
		SystemTimeToFileTime(&st,&ft);
		*(__int64 *)(&ft)=*(__int64 *)(&ft) + nday * (24*3600*1000i64)*10000i64;
		FileTimeToSystemTime(&ft,&st);
		val = st.wYear * 10000 + st.wMonth * 100 + st.wDay;
	}
	else
	{

	}
}

////////////////////////////////////////////////CDate////////////////////////////////////////////////

CDate::CDate()
{
#ifdef WIN32
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_iyear = st.wYear;
	m_imonth = st.wMonth;
	m_iday = st.wDay;
	m_ihour = st.wHour;
	m_iMinutes = st.wMinute;
	m_iSeconds = st.wSecond;
	m_Milliseconds = st.wMilliseconds;
#endif

#ifdef LINUX
	time_t  now;
	struct tm *tmlocal;
	time(&now);
	tmlocal = localtime(&now);
	m_iyear = tmlocal->tm_year + 1900;
	m_imonth = tmlocal->tm_mon + 1;
	m_iday = tmlocal->tm_mday;
	m_ihour = tmlocal->tm_hour;
	m_iMinutes = tmlocal->tm_min;
	m_iSeconds = tmlocal->tm_sec;
	m_Milliseconds = 0;
#endif 
}

CDate::CDate(int year, int month, int day)
{
	m_iyear =year;
	m_imonth = month;
	m_iday = day;
}

CDate::CDate(int date)
{
	m_iyear =	date/10000;
	m_imonth= (date/100)%100;
	m_iday	=	date%100;
}

int CDate::GetDate()
{
	return m_iyear*10000 + m_imonth*100 + m_iday;
}

int CDate::GetTime()
{
	return m_ihour*10000 + m_iMinutes * 100 + m_iSeconds;
}

void CDate::GetTimeStr(char* time, char splitchar)
{	
	sprintf(time,"%02d%c%02d%c%02d.%03d", m_ihour,splitchar,m_iMinutes,splitchar,m_iSeconds,m_Milliseconds);
}

int CDate::GetTimeStamp()
{
	return m_Milliseconds + 1000 * ( m_iSeconds + 60 * ( m_iMinutes + 60 * m_ihour ) );
}

////////////////////////////////////////////////OperateSystem////////////////////////////////////////////////
int OperateSystem::getCurrentProcessId()
{
#ifdef WIN32
	return GetCurrentProcessId();
#endif 

#ifdef LINUX
	return getpid();
#endif 
}
//调用者保证大小合适
void OperateSystem::getComputerName(char* compName, int buflen)
{
#ifdef WIN32
	DWORD dbuflen = buflen;
	if(!GetComputerNameA(compName,&dbuflen))
	{
		hs_strncpy(compName,"UnknowCompName",-1);
	}
#endif
#ifdef LINUX
	gethostname(compName,buflen-1); 
#endif
}

////////////////////////////////////////////////CEventEx////////////////////////////////////////////////
CEventEx::CEventEx()
{
#ifdef WIN32
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
#ifdef LINUX
	bSignaled = false;
	pthread_cond_init(&ptcond, NULL);
	pthread_mutex_init(&mutex, NULL);
#endif
}

void CEventEx::Notify()
{
#ifdef WIN32
	SetEvent(hEvent);
#endif
#ifdef LINUX
	pthread_mutex_lock(&mutex);
	bSignaled = true;
	pthread_cond_signal(&ptcond);
	pthread_mutex_unlock(&mutex);
#endif 
}

CEventEx::EventWaitState CEventEx::Wait(long timeout)
{
#ifdef WIN32
	long ret = WaitForSingleObject(hEvent, timeout);
	if( ret == WAIT_OBJECT_0)
		return EVENT_SIGNALED;
	if( ret == WAIT_TIMEOUT )
		return EVENT_TIMEOUT;
	return EVENT_ERROR;
#endif 
#ifdef LINUX
	int ret = 0;
	if( timeout < 0 )
	{
		pthread_mutex_lock(&mutex);
		if( bSignaled == false )
		{
			ret = pthread_cond_wait(&ptcond, &mutex);
		}
		if( bSignaled == true)
		{
			ret = 0;
			bSignaled = false;
		}
		pthread_mutex_unlock(&mutex);
		if( ret == 0 )
			return EVENT_SIGNALED;
		return EVENT_ERROR;
	}
	else
	{	
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout / 1000;
		ts.tv_nsec += (timeout % 1000) * 1000000;

		pthread_mutex_lock(&mutex);
		if( bSignaled == false )
		{
			ret = pthread_cond_timedwait(&ptcond, &mutex, &ts); 
		}
		if( bSignaled == true)
		{
			ret = 0;
			bSignaled = false;
		}
		pthread_mutex_unlock(&mutex);

		if( ret == 0 )
			return EVENT_SIGNALED;
		if( ret == ETIMEDOUT)
			return EVENT_TIMEOUT;
		return EVENT_ERROR;
	}
#endif
}

void CEventEx::Reset()
{
#ifdef WIN32
	ResetEvent(hEvent);
#endif
#ifdef LINUX
	pthread_mutex_lock(&mutex);
	bSignaled = false;
	pthread_mutex_unlock(&mutex);
#endif
}

CEventEx::~CEventEx()
{
#ifdef WIN32
	CloseHandle((HANDLE)hEvent);
#endif
#ifdef LINUX
	pthread_cond_destroy(&ptcond);
	pthread_mutex_destroy(&mutex);
#endif
}

/////////////////////////////////////////////////CThread///////////////////////////////////////////////
#ifdef WIN32
unsigned int _stdcall CThread::Win32TdFunc(void *lpvoid)
{
	CThread *lpThread = (CThread*)lpvoid;
	lpThread->isrunning = true;
	lpThread->tdfunc(lpThread->lpvoid);
	lpThread->isrunning = false;
	return 0;
}

void CThread::Start(TDFUNC lpfunc, int stacksize, void* lpvoid)
{
	this->tdfunc = lpfunc;
	this->lpvoid = lpvoid;
	hThread = (HANDLE)_beginthreadex(NULL,stacksize,Win32TdFunc,(void*)this,0,NULL);
}

void CThread::Join()
{
	if( isrunning == true)
	{
		WaitForSingleObject(hThread, -1);
	}
	CloseHandle(hThread);
}

#endif

#ifdef LINUX

void* CThread::LinuxTdFunc(void* lpvoid)
{
	CThread *lpThread = (CThread*)lpvoid;
	lpThread->isrunning = true;
	lpThread->tdfunc(lpThread->lpvoid);
	lpThread->isrunning = false;
	return NULL;
}

void CThread::Start(TDFUNC tdfunc,int stacksize, void* lpvoid)
{	
	this->tdfunc = tdfunc;
	this->lpvoid = lpvoid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if( stacksize < 1024 * 1024 )
		pthread_attr_setstacksize(&attr, 1024 * 1024);
	else 
		pthread_attr_setstacksize(&attr, stacksize);
	pthread_create(&threadid, &attr, LinuxTdFunc, (void *)this);
	pthread_attr_destroy(&attr);
}

void CThread::Join()
{
	void *res;
	pthread_join(threadid, &res);
	free(res);
}

#endif

/////////////////////////////////////////////////CLogInfo///////////////////////////////////////////////
void CLogInfo::AddLog(const char* pszMsg, LOGINFO_LEVEL level)
{
	logLock.Lock();
	CDate now;
	LogInfoField* lpLog = new LogInfoField();
	lpLog->level = level;
	lpLog->ndate = now.GetDate();
	now.GetTimeStr(lpLog->sztime, ':');
	hs_strncpy(lpLog->szMsg, pszMsg, sizeof(lpLog->szMsg));
	msgPush.Push(lpLog);
	logLock.UnLock();
}

void CLogInfo::WriteLog(const char * szMsg)
{
	logLock.Lock();
	fwrite(szMsg, strlen(szMsg), 1, m_fp);
	fwrite("\r\n", 2, 1, m_fp);
	fflush(m_fp);
	logLock.UnLock();
}

void CLogInfo::DealLog(void* lpvoid)
{
	CLogInfo *helper = (CLogInfo*)lpvoid;
	LogInfoField* lpLog;
	char szMsg[1024];
	while(helper->bLogInfoStop == false)
	{
		lpLog = helper->msgPush.Pop(1000);

		if(lpLog != NULL)
		{
			switch(lpLog->level)
			{
			case LOGINFO_Save:
				{
					sprintf(szMsg, "%s\0", lpLog->szMsg);
					helper->WriteLog(szMsg);
				}
				break;
			case LOGINFO_Event:
				{
					sprintf(szMsg, "[%d %s][  Event] %s\0", lpLog->ndate, lpLog->sztime, lpLog->szMsg);
					helper->WriteLog(szMsg);

					cout<<szMsg<<endl; 
				}
				break;
			case LOGINFO_Warning:
				{
					sprintf(szMsg, "[%d %s][Warning] %s\0", lpLog->ndate, lpLog->sztime, lpLog->szMsg);
					helper->WriteLog(szMsg);

					cout<<szMsg<<endl; 
				}
				break;
			case LOGINFO_Error:
				{
					sprintf(szMsg, "[%d %s][  Error] %s\0", lpLog->ndate, lpLog->sztime, lpLog->szMsg);
					helper->WriteLog(szMsg);

					cout<<szMsg<<endl; 
				}
				break;
			default:
				break;
			}

			delete lpLog;
		}
	}
}

