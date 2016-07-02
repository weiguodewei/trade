
/******************************************************************************/
/*类名：CDBF
/*作用：提供操作dbf数据库文件的一些简单函数 
/******************************************************************************/

#include "dbf.h"

//构造函数
CDBF::CDBF()
{
	dbfBuffer = new unsigned char[4 * 1024 * 1024];
}

CDBF::~CDBF()
{
	if(dbfBuffer) delete[] dbfBuffer;
	dbfBuffer = NULL;
}

//初始化数据库
bool CDBF::InitDbf(const char* filename)
{
	fp = fopen(filename, "rb");
	char errMsg[1024];
	if(!fp)
	{
		sprintf(errMsg, "打开文件[%s]失败!", filename);
		return false;
	}

	int len = fread(dbfBuffer, 1, 4 * 1024 * 1024, fp);  
	dbfBuffer[len] = '\0';
	memcpy((void*)&dbfFileHeader, dbfBuffer, sizeof(DBFFILEHEADER));
	fclose(fp);
	return true;
}
//获取字段个数
int CDBF::GetFieldCount()
{
	return(dbfFileHeader.dfHeaderLength / 32 - 1);
}
//获取任一字段名
unsigned char * CDBF::GetFieldName(int FieldNum)
{
	if (FieldNum < 1) return NULL;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldName);
}

//获取任一字段长度
int CDBF::GetFieldLength(int FieldNum)
{
	if (FieldNum < 1) return -1;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldLength);
}

//获取任一字段类型
unsigned char CDBF::GetFieldType(int FieldNum)
{
	if (FieldNum < 1) return '#';
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diFieldType);
}
//获取任一数值型字段小数点位数
int CDBF::GetDecimal(int FieldNum)
{
	if (FieldNum < 1) return -1;
	unsigned char * p = &(dbfBuffer[FieldNum * 32]);
	//		fp.Seek(FieldNum * 32,SEEK_SET);
	memcpy(&dbfInfoHeader, p, sizeof(dbfInfoHeader));
	//		fp.Read((void*)&dbfInfoHeader,sizeof(DBFINFOHEADER));
	return(dbfInfoHeader.diDecimal);
}
//获取记录个数
long CDBF::GetRecordCount()
{
	return(dbfFileHeader.dfRecordCount);
}
//获取记录长度
int CDBF::GetRecordLength()
{
	return(dbfFileHeader.dfRecordLength);
}

//获取任一记录
int CDBF::GetRecord(int RecordNum,unsigned char *pBuf)
{
	if (RecordNum < 1) return 0;
	
	//头长度+头结束标志长度+(RecordNum-1) *(记录长度+记录删除符号标志长度[1])
	long lPos=dbfFileHeader.dfHeaderLength + 1 + (RecordNum-1) * (dbfFileHeader.dfRecordLength);
	unsigned char * p = &(dbfBuffer[lPos]);
	//		fp.Seek(lPos,SEEK_SET);
	memcpy(pBuf, p, dbfFileHeader.dfRecordLength);
	//		fp.Read((void*)pBuf,GetRecordLength());
	
	return 1;
}

unsigned long	CDBF::GetFieldColumSize(int fieldnum)
{
	if (fieldnum < 1) return 0;
	return GetFieldLength(fieldnum) * GetRecordCount();
}

