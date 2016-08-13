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
	vector<BYTE> vBuffer;

public:
	MemoryBuffer(void);
	MemoryBuffer(int tiLength);
	MemoryBuffer(const MemoryBuffer &other);
	MemoryBuffer(const BYTE *tpBytes, int tiLength);
	virtual ~MemoryBuffer(void);

	const BYTE * c_Bytes() const;
	BYTE * CopyOut(int &tiLength) const;
	void CopyTo(const BYTE ** tppBytes, int &tiLenth) const;
	void CopyFrom(const BYTE * tpBytes, int tiLength);
	void Append(const BYTE & tByte);
	void Append(const BYTE & tByte, int tiLength);
	void Append(const BYTE * tpBytes, int tiLength);
	void Truncate();
	void Clear();

	MemoryBuffer & operator = (const MemoryBuffer &other);
	MemoryBuffer & operator += (const MemoryBuffer &other);
	const vector<BYTE> &GetBuffer() const { return vBuffer; }

};
