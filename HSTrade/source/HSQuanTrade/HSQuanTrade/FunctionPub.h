#pragma once
#include <mmsystem.h>

#pragma comment(lib, "WINMM.LIB")

typedef enum HS_SOUND_TYPE
{
	E_HS_SOUND_MSG,
	E_HS_SOUND_SIGNAL,
	E_HS_SOUND_TRADE_RTN,
	E_HS_SOUND_TRADE_CANCEL,
};

class CFunctionPub
{
public:
	CFunctionPub(void);

	~CFunctionPub(void);

	static void ParseParam(CString param, CStringArray & list,char ch);

	static void Copy(char * pt,int len,CString &str);

	static CString GetModuleDir();


	static int String2Time(const char* szDate, time_t &tmDate);
	static int String2Time(const char* szDate, CTime &tTime);


	static CString GetFullDateString(const char* sTime);



};


class CMediaAux
{
public:
	static void PlayWavSound(HS_SOUND_TYPE eType = E_HS_SOUND_MSG);
};