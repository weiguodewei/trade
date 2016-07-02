#include "stdafx.h"
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

bool GetIpAddressByUrl(char* ip, const char* inurl)
{
	const int allocsize =  strlen(inurl) + 1;
	char* name = (char*)malloc(allocsize);
	memset(name, 0, allocsize);
	const char* url = strchr(inurl,':');
	struct in_addr addr;

	if( url <= 0)
	{
		strcpy(name, url);
	}
	if( url[1] == '/' || url[1] == '\\') 
	{//tcp:\\... or http:\\...
		url += 3;
		const char* end = strchr(url,':');
		if( end <= 0)
		{
			strcpy(name, url);
		}
		else
		{
			hs_strncpy(name, url, end - url);
		}
	}
	else
	{
		hs_strncpy(name, inurl,url - inurl);
	}
	struct hostent *remoteHost;
	if (isalpha(name[0])) 
	{
		remoteHost = gethostbyname(name);
		if( remoteHost == NULL)
			return false;
		addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
		sprintf(ip, "%s",inet_ntoa(addr));
	} 
	else 
	{
		addr.s_addr = inet_addr(name);
		if (addr.s_addr == INADDR_NONE) 
		{
			return false;
		}
		else
		{
			//remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
			sprintf(ip, "%s",inet_ntoa(addr));
		}
	}
	return true;
}

bool GetLocalMACIP(char* macAddress, char* Ip,const char* desturl)
{
	char szDestIp[64];
	if( GetIpAddressByUrl(szDestIp, desturl) == false)
	{
		strcpy(macAddress, "Unknown");
		strcpy(Ip,"Unknown");
		return false;
	}
#ifdef WIN32
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
	bool bfind = false;
	//GetBestInterface取得最佳路由IP的index,szDestIp一般为登陆服务器的IP
	DWORD dwBestIndex = 0;
	DWORD dwRet = GetBestInterface(inet_addr(szDestIp), &dwBestIndex);
	if(NO_ERROR != dwRet)
	{
		dwBestIndex = -1;
	}

	//通过GetAdaptersInfo获取对应index的MAC地址
	ULONG ulBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)MALLOC(sizeof(IP_ADAPTER_INFO));
	PIP_ADAPTER_INFO pAdapter = NULL;//记录最匹配的MAC地址
	if(NULL != pAdapterInfo)
	{
		//先测试下需要的内存大小
		if (GetAdaptersInfo(pAdapterInfo, &ulBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			FREE(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulBufLen);
			if (pAdapterInfo == NULL)
			{
				return false;
			}
		}

		//OK,重新获取信息
		if ((dwRet = GetAdaptersInfo(pAdapterInfo, &ulBufLen)) == NO_ERROR) 
		{
			IP_ADAPTER_INFO* lpTemp = pAdapterInfo;

			//遍历一下,找对应的index
			while (lpTemp)
			{
				if((dwBestIndex == lpTemp->Index) && (lpTemp->AddressLength>0))
				{
					pAdapter = lpTemp;
					break;
				}
				//下一个Adapter
				lpTemp = lpTemp->Next;
			}

			//如果符合条件有一个都没找着,就默认一个.注意MAC不能为空
			if(NULL == pAdapter)
			{
				lpTemp = pAdapterInfo;
				//找一个MAC不为0的就ok了
				while (lpTemp)
				{
					if(lpTemp->AddressLength>0)
					{
						pAdapter = lpTemp;
						break;
					}
					lpTemp = lpTemp->Next;
				}
			}

			//理论上有网卡不会一个MAC都没有的,不过还是检查一下了
			if(NULL != pAdapter)
			{
				//格式化MAC地址
				for (unsigned int i = 0; i < pAdapter->AddressLength; i++) 
				{
					//无格式
					sprintf(macAddress + i * 2,"%02X",(int) pAdapter->Address[i]);
				}
				sprintf(Ip,"%s", pAdapter->IpAddressList.IpAddress.String);
				bfind = true;
			}
			else
			{
				sprintf(macAddress,"Unknown");
				bfind = false;
			}
			FREE(pAdapterInfo);
		}
	}
	return bfind == true;
#endif

#ifdef LINUX

#define SDKDebug(x) (std::cout<<x)
	register int fd, intrface = 0;
	ifreq buf[16];
	struct ifconf ifc;

	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) 
	{
		ifc.ifc_len = sizeof buf;
		ifc.ifc_buf = (caddr_t) buf;

		if(!ioctl (fd, SIOCGIFCONF, (char *) &ifc))
		{
			intrface = ifc.ifc_len / sizeof (struct ifreq);
			while (intrface-- > 0)
			{
				SDKDebug("net device "<<buf[intrface].ifr_name);

				//get flags
				if (!(ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface])))
				{
					if (buf[intrface].ifr_flags & IFF_PROMISC) 
					{
						SDKDebug("the interface is PROMISC");
					}
				}

				//Jugde whether the net card status is up
				if (buf[intrface].ifr_flags & IFF_UP) 
				{
					//get ip
					if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
					{
						sprintf(Ip,"%s", inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
						SDKDebug("IP address is:"<<Ip);
					}

					//get mac
					if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
					{		
						//是否为空
						unsigned char result = 0xFF;
						for (int t=0;t<6;t++)
						{
							result += ((unsigned char)buf[intrface].ifr_hwaddr.sa_data[t]);
						}
						if(result != 0)
						{
							sprintf(macAddress,"%02X%02X%02X%02X%02X%02x",
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
								(unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);
							SDKDebug("Hwadrr is:"<<macAddress);
							break;
						}
						else
						{
							SDKDebug("Hwadrr is empty"<<std::endl);
						}
					}	

				}
			}
		}
	}
	close(fd);
	return true;
#endif   
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
	for(unsigned int i=0; dupstr[i]; i++)
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

