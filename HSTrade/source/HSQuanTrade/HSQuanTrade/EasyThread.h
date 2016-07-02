#pragma once


extern "C" typedef void (*TDFUNC)(void*);

/////////////////////////////////////////////////CThread///////////////////////////////////////////////
#ifdef WIN32
class CEasyThread
{
	HANDLE hThread;
	TDFUNC tdfunc;
	void*  lpvoid;
	bool   isrunning;
	static unsigned int _stdcall Win32TdFunc(void *lpvoid);
public:
	CEasyThread() { isrunning = false; }
	void Start(TDFUNC lpfunc, int stacksize, void* lpvoid);
	void Join();
	bool IsRunning(){ return isrunning; }

	void Terminate();//É÷ÓÃ
};
#endif
#ifdef LINUX
class CEasyThread
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
