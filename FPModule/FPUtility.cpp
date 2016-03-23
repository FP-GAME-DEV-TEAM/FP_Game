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
	TCHAR path[MAX_PATH] = {0};
	size_t len = whichPath.length();
	HANDLE hFind = NULL;
	if (_T('\\') == whichPath[len - 1])
		_tcsncpy(path, whichPath.c_str(), len - 1);
	else
		_tcsncpy(path, whichPath.c_str(), len);
	hFind = FindFirstFile(path, &ffd);
	if((hFind != INVALID_HANDLE_VALUE) && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
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
	if(0==len || size<16 || len>size)
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
	if(NULL == hWnd)
	{
		return NULL;
	}
	hTemp = GetParent(hWnd);
	while(hTemp)
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

MemoryBuffer::MemoryBuffer(const MemoryBuffer &other)
{
	*this = other;
}

MemoryBuffer::MemoryBuffer(const BYTE *tpBytes, int tiLength)
{
	this->CopyFrom(tpBytes, tiLength);
}

void MemoryBuffer::Clear()
{
	vector<BYTE>().swap(this->m_vctBuffer);
}


const BYTE * MemoryBuffer::c_Bytes() const
{
	if (this->IsEmpty()) return NULL;
	return &m_vctBuffer[0];
}

BYTE * MemoryBuffer::CopyOut(int &tiLength) const
{
	tiLength = this->GetLength();
	if (this->IsEmpty()) return NULL;
	BYTE *pBytes = new BYTE[tiLength];
	memcpy(pBytes, &m_vctBuffer[0], tiLength);
	return pBytes;
}

void MemoryBuffer::CopyFrom(const BYTE * tpBytes, int tiLength)
{
	this->Clear();
	if (tpBytes == NULL || tiLength == 0) return;
	m_vctBuffer.resize(tiLength, 0);
	memcpy(&m_vctBuffer[0], tpBytes, tiLength);

}

void MemoryBuffer::Append(const BYTE * tpBytes, int tiLength)
{
	if (tpBytes == NULL || tiLength == 0) return;
	m_vctBuffer.resize(this->GetLength() + tiLength, 0);
	memcpy(&m_vctBuffer[0] + this->GetLength() - tiLength, tpBytes, tiLength);
}

void MemoryBuffer::Append(const BYTE & tByte)
{
	m_vctBuffer.push_back(tByte);

}

void MemoryBuffer::Insert(int tiStartIndex, const BYTE * tpBytes, int tiLength)
{
	if (tpBytes == NULL || tiLength == 0) return;
	int iBufferSize = this->GetLength();
	if (tiStartIndex > iBufferSize) return;
	if (tiStartIndex == iBufferSize)
	{
		this->Append(tpBytes, tiLength);
	}
	else if ((tiStartIndex + tiLength) < iBufferSize)
	{
		memcpy(&m_vctBuffer[0] + tiStartIndex, tpBytes, tiLength);
	}
	else
	{
		m_vctBuffer.resize(tiStartIndex + tiLength, 0);
		memcpy(&m_vctBuffer[0] + tiStartIndex, tpBytes, tiLength);
	}

}

void MemoryBuffer::CopyTo(const BYTE ** tppBytes, int &tiLength)const
{
	if (tppBytes == NULL || *tppBytes == NULL || this->IsEmpty()) return;
	tiLength = this->GetLength();
	memcpy(tppBytes, &m_vctBuffer[0], tiLength);

}

MemoryBuffer & MemoryBuffer::operator = (const MemoryBuffer &other)
{
	this->Clear();
	if (!other.IsEmpty())
	{
		m_vctBuffer.insert(m_vctBuffer.begin(), other.GetBuffer().begin(), other.GetBuffer().end());
	}
	return *this;
}

MemoryBuffer & MemoryBuffer::operator += (const MemoryBuffer &other)
{
	if (!other.IsEmpty())
	{
		m_vctBuffer.insert(m_vctBuffer.end(), other.GetBuffer().begin(), other.GetBuffer().end());
	}
	return *this;
}

MemoryStream::MemoryStream(void)
{
	m_pBuffer = new MemoryBuffer();
	m_iOffset = 0;
}

MemoryStream::MemoryStream(MemoryBuffer *tpbuffer)
{
	m_pBuffer = tpbuffer;
	m_iOffset = 0;
}

MemoryStream * MemoryStream::GetInstance()
{
	MemoryStream *pStream = new MemoryStream(new MemoryBuffer);
	return pStream;
}

MemoryStream::~MemoryStream(void)
{
	if (m_pBuffer != NULL)
		delete m_pBuffer;
	m_pBuffer = NULL;
}

void MemoryStream::Write(LPCWSTR szStr)
{
	if (szStr == NULL)
	{
		return;
	}
	int iLength = (int)wcslen(szStr) * sizeof(wchar_t);
	Write(iLength);
	if (iLength != 0)
	{
		Write((const BYTE*)szStr, iLength);
	}
}

void MemoryStream::Write(LPCSTR szStr)
{
	if (szStr == NULL)
	{
		return;
	}
	int iLength = (int)strlen(szStr) * sizeof(char);
	Write(iLength);
	if (iLength != 0)
	{
		Write((const BYTE*)szStr, iLength);
	}
}

void MemoryStream::Write(const string &szStr)
{
	Write(szStr.c_str());
}

void MemoryStream::Write(const vector<string> &szStrVct)
{
	int iCnt = (int)szStrVct.size();
	Write(iCnt);
	for (int i = 0; i < (int)szStrVct.size(); i++)
	{
		Write(szStrVct.at(i));
	}
}

void MemoryStream::Write(const wstring &szStr)
{
	Write(szStr.c_str());
}

void MemoryStream::Write(const vector<wstring> &szStrVct)
{
	int iCnt = (int)szStrVct.size();
	Write(iCnt);
	for (int i = 0; i < (int)szStrVct.size(); i++)
	{
		Write(szStrVct.at(i));
	}
}

void MemoryStream::Write(const MemoryBuffer &field)
{
	Write(field.GetLength());
	Write(field.c_Bytes(), field.GetLength());
}

void MemoryStream::Write(const BYTE * tpBytes, const int &tiLength)
{
	m_pBuffer->Insert(m_iOffset, tpBytes, tiLength);
	m_iOffset += tiLength;
}

void MemoryStream::Read(BYTE * tpBytes, const int tiLength)
{
	if (tpBytes == NULL) return;
	if (m_iOffset + tiLength > m_pBuffer->GetLength()) return;
	memcpy((tpBytes), m_pBuffer->c_Bytes() + m_iOffset, tiLength);
	m_iOffset += tiLength;
}

void MemoryStream::Read(string &tszStr)
{
	int iBytesCnt = 0;
	Read(iBytesCnt);
	if (iBytesCnt >= 0)
	{
		Read(tszStr, iBytesCnt);
	}
}

void MemoryStream::Read(vector<string> &szStrVct)
{
	int iCnt = 0;
	Read(iCnt);
	if (iCnt >= 0)
	{
		for (int i = 0; i < iCnt; i++)
		{
			string temp;
			Read(temp);
			szStrVct.push_back(temp);
		}
	}
}

void MemoryStream::Read(string &tszStr, const int tiNumberofcharacter)
{
	char *pszTemp = new char[tiNumberofcharacter + 1];
	memset(pszTemp, 0, (tiNumberofcharacter + 1) * sizeof(char));
	Read((BYTE*)pszTemp, (sizeof(char))*tiNumberofcharacter);
	tszStr = pszTemp;
	delete[]pszTemp;
}

void MemoryStream::Read(wstring &tszStr)
{
	int iBytesCnt = 0;
	Read(iBytesCnt);
	if (iBytesCnt >= 0)
	{
		Read(tszStr, iBytesCnt / sizeof(wchar_t));
	}
}

void MemoryStream::Read(vector<wstring> &szStrVct)
{
	int iCnt = 0;
	Read(iCnt);
	if (iCnt >= 0)
	{
		for (int i = 0; i < iCnt; i++)
		{
			wstring temp;
			Read(temp);
			szStrVct.push_back(temp);
		}
	}
}

void MemoryStream::Read(wstring &tszStr, const int tiNumberofcharacter)
{
	wchar_t *pszTemp = new wchar_t[tiNumberofcharacter + 1];
	memset(pszTemp, 0, (tiNumberofcharacter + 1) * sizeof(wchar_t));
	Read((BYTE*)pszTemp, (sizeof(wchar_t))*tiNumberofcharacter);
	tszStr = pszTemp;
	delete[]pszTemp;
}

void MemoryStream::Read(MemoryBuffer &field)
{
	int iBytesCnt = 0;
	Read(iBytesCnt);
	BYTE *pBytes = new BYTE[iBytesCnt];
	memset(pBytes, 0, iBytesCnt);
	Read(pBytes, iBytesCnt);
	field.Append(pBytes, iBytesCnt);
}

void MemoryStream::Seek(int tioffset, SeekOrigin loc)
{
	switch (loc)
	{
	case soBegin:
		m_iOffset = tioffset;
		break;
	case soCurrent:
		m_iOffset += tioffset;
		break;
	case soEnd:
		m_iOffset = m_pBuffer->GetLength();
		break;
	}
}
