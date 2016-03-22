/********************************
* FileName: FPUtility.h
* FileFunc: FP工程的工具定义
* Author: SQK
* Date: 2016-03-23
* Descript: 包含一些实用的函数和类
********************************/


#pragma once

#include <vector>

using namespace std;

class MemoryBuffer
{
protected:
	vector<BYTE> m_vctBuffer;

public:
	MemoryBuffer(void);
	MemoryBuffer(const MemoryBuffer &other);
	MemoryBuffer(const BYTE *tpBytes, int tiLength);
	virtual ~MemoryBuffer(void);

	//得到内部指针——const
	const BYTE * c_Bytes() const;
	//从内部拷贝数据到数据中
	BYTE * CopyOut(int &tiLength) const;
	//确保tppBytes指向的数据足够大
	void CopyTo(const BYTE ** tppBytes, int &tiLenth) const;
	//从外部数据拷贝数据
	void CopyFrom(const BYTE * tpBytes, int tiLength);
	//从外部数据拷贝数据,添加
	void Append(const BYTE * tpBytes, int tiLength);
	void Append(const BYTE & tByte);
	//从外部数据拷贝数据,插入
	void Insert(int tiStartIndex, const BYTE * tpBytes, int tiLength);

	MemoryBuffer & operator = (const MemoryBuffer &other);
	MemoryBuffer & operator += (const MemoryBuffer &other);
	const vector<BYTE> &GetBuffer() const { return m_vctBuffer; }
	void Clear();
	int GetLength() const { return (int)m_vctBuffer.size(); }
	bool IsEmpty() const { return m_vctBuffer.size() == 0; }
};


#define _Base_Struct_Write_Func(_Struct_Type) 	void Write(const _Struct_Type &field) \
{ \
	Write((const BYTE*)&field, sizeof(_Struct_Type)); \
} \

#define _Base_Struct_Read_Func(_Struct_Type)    void Read( _Struct_Type &field) \
{ \
	Read(((BYTE*)&field), sizeof(_Struct_Type)); \
} \

#define _Struct_Vector_Write_Func(_Struct_Type) void Write(vector<_Struct_Type> &vcts)\
{ \
	Write((int)vcts.size()); \
for (std::vector<_Struct_Type>::iterator iter = vcts.begin(); iter != vcts.end(); ++iter) \
{ \
	Write(*iter); \
} \
} \

#define _Struct_Vector_Read_Func(_Struct_Type) void Read(vector<_Struct_Type> &vcts)\
{ \
	int iCnt = 0; \
	Read(iCnt); \
if (iCnt <= 0) return; \
	_Struct_Type _Item; \
for (int i = 0; i < iCnt; i++) \
{ \
	Read(_Item); \
	vcts.push_back(_Item); \
} \
} \

#define _Base_Struct_Read_Write_Func(_Struct_Type)  _Base_Struct_Write_Func(_Struct_Type)\
	_Base_Struct_Read_Func(_Struct_Type) \
	_Struct_Vector_Write_Func(_Struct_Type) \
	_Struct_Vector_Read_Func(_Struct_Type) \


class MemoryStream
{
protected:
	MemoryBuffer *m_pBuffer;
	int m_iOffset;

public:
	// Summary:
	//     Provides the fields that represent reference points in streams for seeking.
	enum SeekOrigin
	{
		// Summary:
		//     Specifies the beginning of a stream.
		soBegin = 0,
		//
		// Summary:
		//     Specifies the current position within a stream.
		soCurrent = 1,
		//
		// Summary:
		//     Specifies the end of a stream.
		soEnd = 2,
	};

public:
	MemoryStream(MemoryBuffer *tpbuffer);
	MemoryStream();
	virtual ~MemoryStream(void);

	static MemoryStream * GetInstance(void);

public:
	//基本结构
	_Base_Struct_Read_Write_Func(int)
		_Base_Struct_Read_Write_Func(unsigned int)
		_Base_Struct_Read_Write_Func(short)
		_Base_Struct_Read_Write_Func(unsigned short)
		_Base_Struct_Read_Write_Func(__int64)
		_Base_Struct_Read_Write_Func(unsigned __int64)
		_Base_Struct_Read_Write_Func(char)
		_Base_Struct_Read_Write_Func(unsigned char)
		_Base_Struct_Read_Write_Func(float)
		_Base_Struct_Read_Write_Func(double)
		_Base_Struct_Read_Write_Func(SYSTEMTIME)
		;

	//字符串
	void Write(LPCWSTR szStr);
	void Write(LPCSTR szStr);
	void Write(const string &szStr);
	void Write(const wstring &szStr);
	void Write(const vector<string> &szStrVct);
	void Write(const vector<wstring> &szStrVct);
	void Read(string &tszStr);
	void Read(wstring &tszStr);
	void Read(string &tszStr, const int tiNumberofcharacter);
	void Read(wstring &tszStr, const int tiNumberofcharacter);
	void Read(vector<string> &szStrVct);
	void Read(vector<wstring> &szStrVct);
	//扩展类型
	void Write(const MemoryBuffer &tbuffer);
	void Read(MemoryBuffer &tbuffer);
	//字节流
	void Write(const BYTE * tpBytes, const int &tiLength);
	void Read(BYTE * tpBytes, const int tiLength);

public:
	//Stream操作
	void Seek(int tioffset, SeekOrigin loc);
	int GetOffset()const{ return this->m_iOffset; };
	int GetLength()const{ return this->m_pBuffer->GetLength(); };
};
