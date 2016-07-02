#ifndef _DBF_H_
#define _DBF_H_

#include "public.h"

//DBF文件数据读取类
///////////////////////////////////////////////////////////////////////////////////////
//DBF文件的总体格式
//	a.数据记录部分,紧随字段结构描述部分,以ASCII码方式顺序存放,结束标志为1AH(即文件结束标志)
//		每条记录的首字节为删除标志,20H表示有效,2AH表示已被删除.
//		字段值间无分隔符,记录亦无结束标记.

//	b.字段结构描述部分,位于文件首部,以二进制方式存放,结束标志为0DH.
//		长度为32*(数据库的字段个数+1(即库整体描述))+1(0DH结束标志)


//文件头长度=文件描述头长度+字段描述长度（32）*字段个数

//DBF 文件头结构//即库整体描述表,共32个字节
typedef struct _DBFFILEHEADER
{
	unsigned char	dfMark;						//0x03h 或 0x80h(开头字节，有MEMO字段)		0
	unsigned char	dfYear,dfMmonth,dfDay;		//依次为年月日,二进制						1-3
	unsigned long	dfRecordCount;				//总记录个数,低位字节在前					4-7
	unsigned short	dfHeaderLength;				//文件头长度=第9字节值*256+第8字节值		8-9
	unsigned short	dfRecordLength;				//记录长度=第11字节值*256+第10字节值		10-11
	unsigned char	dfReserved[20];				//保留（略去更细节的描述）					12-31
}DBFFILEHEADER;

//DBF 字段结构//即字段描述表,32个字节/字段
typedef struct _DBFINFOHEADER
{
	unsigned char	diFieldName[10];			//字段名称,ASCII码			0-9
	unsigned char	diReserved1;				//保留字节（0）				10
	unsigned char	diFieldType;				//字段类型,CDNL等ASCII码	11
	unsigned long	diOffset;					//本字段在首记录中的位置	12-15
	unsigned char	diFieldLength;				//字段长度<=256				16
	unsigned char	diDecimal;					//小数点的位数				17
	unsigned char	diReserved[14];				//保留（略去更细节的描述）  18-31
}DBFINFOHEADER;

typedef DBFFILEHEADER *		LPDBFFILEHEADER;
typedef DBFINFOHEADER *		LPDBFINFOHEADER;

/*
field identifier table(字段表)
ascii description
c character
d date, format yyyymmdd
f floating point
g general - foxpro addition
l logical, t:t,f:f,y:y,n:n,?-not initialized
m memo (stored as 10 digits representing the dbt block number)
n numeric
p picture - foxpro addition
*/

class CDBF
{
public:
	CDBF();
	~CDBF();
	bool            InitDbf(const char* filename);				//填充FILEHEADER和INFOHEADER
	int				GetFieldCount();
	int				GetFieldLength(int FieldNum);
	int				GetDecimal(int FieldNum);
	unsigned char * GetFieldName(int FieldNum);
	unsigned char	GetFieldType(int FieldNum);
	long			GetRecordCount();
	int				GetRecordLength();
	int				GetRecord(int RecordNum,unsigned char *pBuf);
	unsigned long	GetFieldColumSize(int fieldnum);

private:
	FILE*           fp;
	unsigned char * dbfBuffer;		
	DBFFILEHEADER	dbfFileHeader;		//文件整体结构
	DBFINFOHEADER	dbfInfoHeader;		//首字段描述
		
protected:

};


#endif //_DBF_H_
