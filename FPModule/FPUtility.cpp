/********************************
* FileName: FPUtility.cpp
* FileFunc: FP工程的工具实现
* Author: SQK
* Date: 2013-06-12
* Descript: 包含了常用的工具API和工具类的实现
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPUtility.h"

//检查目录
BOOL WINAPI IsFolderExist(tstring whichPath)
{
	WIN32_FIND_DATA ffd;
	BOOL exist = FALSE;
	TCHAR path[MAX_PATH] = { 0 };
	size_t len = whichPath.length();
	HANDLE hFind = NULL;
	if (_T('\\') == whichPath[len - 1])
		_tcsncpy(path, whichPath.c_str(), len - 1);
	else
		_tcsncpy(path, whichPath.c_str(), len);
	hFind = FindFirstFile(path, &ffd);
	if ((hFind != INVALID_HANDLE_VALUE) && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		exist = TRUE;
	}
	FindClose(hFind);
	return exist;
}

//获得一个路径字符串
LPTSTR WINAPI NewPathString(const tstring &str, const int size)
{
	LPTSTR tmp;
	int len = str.length();
	if (0 == len || size<16 || len>size)
	{
		return NULL;
	}
	tmp = new TCHAR[size];
	ZeroMemory(tmp, size*sizeof(TCHAR));
	_tcscpy(tmp, str.c_str());
	return tmp;
}

//得到最上层窗口
HWND WINAPI GetRootWindow(HWND hWnd)
{
	HWND hTemp;
	if (NULL == hWnd)
	{
		return NULL;
	}
	hTemp = GetParent(hWnd);
	while (hTemp)
	{
		hWnd = hTemp;
		hTemp = GetParent(hTemp);
	}
	return hWnd;
}


MemoryBuffer::MemoryBuffer(void)
{
}

MemoryBuffer::~MemoryBuffer(void)
{
	this->Clear();
}

MemoryBuffer::MemoryBuffer(int tiLength)
{
	vBuffer.reserve(tiLength);
}

MemoryBuffer::MemoryBuffer(const MemoryBuffer &other)
{
	*this = other;
}

MemoryBuffer::MemoryBuffer(const BYTE *tpBytes, int tiLength)
{
	this->CopyFrom(tpBytes, tiLength);
}

void MemoryBuffer::Truncate()
{
	vector<BYTE>(vBuffer).swap(vBuffer);
}

void MemoryBuffer::Clear()
{
	vector<BYTE>().swap(vBuffer);
}

const BYTE * MemoryBuffer::c_Bytes() const
{
	if (vBuffer.size() == 0) return NULL;
	return &vBuffer[0];
}

BYTE * MemoryBuffer::CopyOut(int &tiLength) const
{
	if (vBuffer.size() == 0) return NULL;
	tiLength = vBuffer.size();
	BYTE *pBytes = new BYTE[tiLength];
	memcpy(pBytes, &vBuffer[0], tiLength);
	return pBytes;
}

void MemoryBuffer::CopyTo(const BYTE ** tppBytes, int &tiLength) const
{
	if (tppBytes == NULL || *tppBytes == NULL || vBuffer.size() == 0) return;
	tiLength = vBuffer.size();
	memcpy(tppBytes, &vBuffer[0], tiLength);
}

void MemoryBuffer::CopyFrom(const BYTE * tpBytes, int tiLength)
{
	if (tpBytes == NULL || tiLength == 0) return;
	this->Clear();
	vBuffer.resize(tiLength, 0);
	memcpy(&vBuffer[0], tpBytes, tiLength);
}

void MemoryBuffer::Append(const BYTE & tByte)
{
	vBuffer.push_back(tByte);
}

void MemoryBuffer::Append(const BYTE & tByte, int tiLength)
{
	if (tiLength == 0) return;
	vBuffer.resize(vBuffer.size() + tiLength, tByte);
}

void MemoryBuffer::Append(const BYTE * tpBytes, int tiLength)
{
	if (tpBytes == NULL || tiLength == 0) return;
	vBuffer.resize(vBuffer.size() + tiLength, 0);
	memcpy(&vBuffer[0] + vBuffer.size() - tiLength, tpBytes, tiLength);
}

MemoryBuffer & MemoryBuffer::operator = (const MemoryBuffer &other)
{
	this->Clear();
	if (other.GetBuffer().size() > 0)
	{
		vBuffer.insert(vBuffer.begin(), other.GetBuffer().begin(), other.GetBuffer().end());
	}
	return *this;
}

MemoryBuffer & MemoryBuffer::operator += (const MemoryBuffer &other)
{
	if (other.GetBuffer().size() > 0)
	{
		vBuffer.insert(vBuffer.end(), other.GetBuffer().begin(), other.GetBuffer().end());
	}
	return *this;
}
