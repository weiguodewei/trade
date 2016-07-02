#include "stdafx.h"
#include "EasyThread.h"



/////////////////////////////////////////////////CThread///////////////////////////////////////////////
#ifdef WIN32
unsigned int _stdcall CEasyThread::Win32TdFunc(void *lpvoid)
{
	CEasyThread *lpThread = (CEasyThread*)lpvoid;
	lpThread->isrunning = true;
	lpThread->tdfunc(lpThread->lpvoid);
	lpThread->isrunning = false;
	return 0;
}

void CEasyThread::Start(TDFUNC lpfunc, int stacksize, void* lpvoid)
{
	this->tdfunc = lpfunc;
	this->lpvoid = lpvoid;
	hThread = (HANDLE)_beginthreadex(NULL,stacksize,Win32TdFunc,(void*)this,0,NULL);
}

void CEasyThread::Join()
{
	if( isrunning == true)
	{
		WaitForSingleObject(hThread, -1);
	}
	CloseHandle(hThread);
}

void CEasyThread::Terminate()
{
	TerminateThread(hThread, -1);
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);

}

#endif



#ifdef LINUX

void* CEasyThread::LinuxTdFunc(void* lpvoid)
{
	CThread *lpThread = (CThread*)lpvoid;
	lpThread->isrunning = true;
	lpThread->tdfunc(lpThread->lpvoid);
	lpThread->isrunning = false;
	return NULL;
}

void CEasyThread::Start(TDFUNC tdfunc,int stacksize, void* lpvoid)
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

void CEasyThread::Join()
{
	void *res;
	pthread_join(threadid, &res);
	free(res);
}

#endif