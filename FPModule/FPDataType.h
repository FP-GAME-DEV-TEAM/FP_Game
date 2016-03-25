/********************************
* FileName: FPDataType.h
* FileFunc: FP��Ϸ��Դ���Ͷ���ģ��
* Author: SQK
* Date: 2013-07-26
* Descript: ����FP����������������������͵Ķ���
********************************/


#pragma once

#include "FPModule.h"

//=====================================
// Graphics Data Types
//

//ͼ��Ŀ¼�ṹ(40BYTE)
typedef struct tagGraphicInfo
{
	LONG id; //ͼ���Ψһ��ʶID
	DWORD addr; //ͼ���������ļ��е�λ��
	DWORD size; //ͼ�����ݵ�ʵ�ʴ�С
	LONG offsetX; //ͼ����ʾʱ��X��ƫ��
	LONG offsetY; //ͼ����ʾʱ��Y��ƫ��
	LONG width; //ͼ��Ŀ��
	LONG height; //ͼ��ĸ߶�
	BYTE east; //ͼ���򶫷���ռ��
	BYTE south; //ͼ�����Ϸ���ռ��
	BYTE path; //���ڵ�ͼ��0��ʾ�ϰ��1��ʾ��������ȥ
	BYTE reserve[5]; //����λ������δ֪
	LONG index; //���ڷ���ĵ�ͼ���
} GraphicInfo, *PGraphicInfo;

//ͼ������ͷ�ṹ(16BYTE)
typedef struct tagGraphicData
{
	BYTE mod[2]; //ħ�����̶�Ϊ��RD������Ϊreal data
	BYTE flag; //�汾��ţ������ж�ѹ����ʽ
	BYTE check; //У��λ����;δ֪
	LONG width; //ͼƬ�Ŀ��
	LONG height; //ͼƬ�ĸ߶�
	DWORD size; //ͼƬ���ݴ�С���������ͷ��
} GraphicData, *PGraphicData;

//����Ŀ¼�ṹ(16BYTE)
typedef struct tagAnimeInfo
{
	LONG id; //������Ψһ��ʶID
	DWORD addr; //�����������ļ��е�λ��
	DWORD size; //һ�����������Ĵ�С
	WORD count; //һ���������������Ķ�����
	WORD check; //�汾У��λ����;δ֪
} AnimeInfo, *PAnimeInfo;

//�������ݵ�ͷ�ṹ(12BYTE)
typedef struct tagAnimeHead
{
	WORD facing; //�����ĳ���0-7
	WORD action; //���������ͣ���Щ�����������ж������У�
	DWORD duration; //�������ѭ�������ʱ�䣨��λΪ���룩
	DWORD frames; //�������������ͼƬ֡��
} AnimeHead, *PAnimeHead;

//�������ݵ�Ԫ(10BYTE)
typedef struct tagAnimeData
{
	LONG index; //ͼƬ֡��Ӧ��ͼ������ID
	BYTE reserve[6]; //����λ����������Ϸ�߼��ж������ֲ����й�
} AnimeData, *PAnimeData;



//=====================================
// Resource Data Types
//

//�������ֿ�
typedef struct tagBgmLib
{
	LPCTSTR bgmPath; //��������Ŀ¼
	DWORD sum; //ʵ�������ļ�����
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} BgmLib, *PBgmLib;

//��Ϸ��Ч��
typedef struct tagSndLib
{
	LPCTSTR sndPath; //��Ϸ��ЧĿ¼
	DWORD sum; //ʵ����Ч�ļ�����
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} SndLib, *PSndLib;

//��ɫ���
typedef struct tagPalLib
{
	LPCTSTR palPath; //��ɫ��Ŀ¼
	DWORD sum; //��ɫ������
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} PalLib, *PPalLib;

//��������Դ��
typedef struct tagBinLib
{
	LPCTSTR binPath; //����������Ŀ¼
	DWORD verGraphic; //ͼ����Դ�汾��
	DWORD verAnime; //������Դ�汾��
	LPCTSTR sGraphicInfo; //ͼ��Ŀ¼�ļ�
	LPCTSTR sGraphicData; //ͼ�������ļ�
	LPCTSTR sAnimeInfo; //����Ŀ¼�ļ�
	LPCTSTR sAnimeData; //���������ļ�
	PPalLib pLibPal; //��ɫ����Ϣ
	PBgmLib pLibBgm; //����������Ϣ
	PSndLib pLibSnd; //��Ϸ��Ч��Ϣ
} BinLib, *PBinLib;


//�û�����
typedef struct tagLocalData
{
	LPCTSTR userID; //����ָʾ��ǰ�û�
} LocalData, *PLocalData;

//����������Դ��
typedef struct tagDataSet
{
	LPCTSTR dataPath; //��������Ŀ¼
	LPCTSTR userID; //����ָʾ��ǰ�û�
	PLocalData userData; //ָ��ǰ�û��ı�������
} DataSet, *PDataSet;

//��ͼ��Դ��
typedef struct tagMapPack
{
	LPCTSTR mapPath; //��ͼĿ¼
	//������ͼ
	//˽�е�ͼ
	//�����ͼ
} MapPack, *PMapPack;

//��Ϸ��Դ�ṹ
typedef struct tagGameRes
{
	LPCTSTR rootPath; //��Ϸ��Ŀ¼
	WORD mainVersion; //��Ϸ���汾
	WORD subVersion; //��Ϸ�Ӱ汾
	PBinLib pBinLib; //��������Դ
	PDataSet pDataSet; //����������Դ
	PMapPack pMapPack; //��ͼ��Դ
} GameRes, *PGameRes;



//=====================================
// IO Data Types
// Sample: PostThreadMessage(ThreadId, IOType, IOReqList, CallbackFunc)
// CallbackFunc: void (*func)(void)
//

typedef struct tagIOItem
{
	BOOL isCompleted;
	DWORD offset;
	union
	{
		DWORD size;
		TCHAR end;
	};
	DWORD maxSize;
	LPVOID pData;
} IOItem, *PIOItem;

typedef struct tagIOList
{
	LONG count;
	PIOItem pList;
} IOList, *PIOList;



//=====================================
// Thread Data Types
// Definition of FP thread routine
//

typedef UINT(CALLBACK *FP_THREAD_ROUTINE)(LPVOID);

typedef struct tagThreadInfo
{
	HANDLE hThread;
	UINT uThreadId;
	BOOL bRunning;
	FP_THREAD_ROUTINE lpCallback;
} ThreadInfo, *PThreadInfo;



//=====================================
// Interface Implements Data Types
//

//��Ϸͼ����
class GameGraphics : public IGameGraphics
{
private:
	static GameGraphics *pInstance; //��������

	HANDLE hGraphicInfo; //ͼ��Ŀ¼����
	HANDLE hGraphicData; //ͼ�����ݶ���
	HANDLE hAnimeInfo; //����Ŀ¼����
	HANDLE hAnimeData; //�������ݶ���
	PALETTEENTRY mPalette[FP_STORE_PAL_COUNT]; //��ǰ��ɫ��
	PALETTEENTRY mPaletteDefault[FP_STORE_PAL_DEFAULT]; //�̶���ɫ�������
	PALETTEENTRY mPaletteOptional[FP_FILE_COUNT_PAL][FP_STORE_PAL_OPTIONAL]; //�ɱ��ɫ�������

protected:
	std::map<LONG, LPBYTE> mGraphicCache; //ͼ������ݻ���
	std::map<LONG, LPBYTE> mAnimeCache; //ͼ������ݻ���
	std::vector<GraphicInfo> mGraphicsList; //ͼ��Ŀ¼����
	std::vector<AnimeInfo> mAnimeList; //����Ŀ¼����

	GameGraphics(); //���캯��
	virtual ~GameGraphics(void); //��������

public:
	static HRESULT WINAPI Create(const PBinLib pBin);
	static VOID WINAPI Destroy();

	HRESULT InitPalette(const PPalLib pPal); //���ص�ɫ��

	HANDLE GetFileHandle(const UINT type) const;
	HRESULT GetImageById(LONG id, LPVOID pData); //ͨ��ID�õ�ͼƬ
	HRESULT GetAnimeById(LONG id, LPVOID pData); //ͨ��ID�õ�����
	HRESULT SwitchPalette(LONG id); //������ɫ��
};


//��Ϸ������
class GameAudio : public IGameAudio
{
public:
	LPCVOID GetWavDataById(LPCTSTR name) const; //ͨ�����Ƶõ�.wav�ļ�����
	LPCVOID GetMp3DataById(LPCTSTR name) const; //ͨ�����Ƶõ�.mp3�ļ�����
};


//��Ϸ�ı���
class GameText : public IGameText
{
public:
	LPCTSTR GetTextById(LONG id) const; //ͨ��ID�õ��ı�
	LPCTSTR GetUserLogById(LONG id) const; //ͨ��ID�õ��û���־
};


//��Ϸ��ͼ��
class GameMaps : public IGameMaps
{
public:
	LPCVOID GetLocalMapById(LONG id) const; //ͨ��ID�õ����ص�ͼ
	LPCVOID GetRemoteMapById(LONG id) const; //ͨ��ID�õ�Զ�̵�ͼ
};


//��Ϸ������
class GameNetwork : public IGameNetwork
{

};


//��Ϸ��Դ������
class GameEnv : public IGameEnv
{
private:
	static GameEnv *pInstance; //������Ϸ��Դ����

	PGameRes pGameRes; //��Ϸ������Ϣ

	GameEnv(const PGameRes pRes); //˽�й��췽��
	virtual ~GameEnv(void); //��������

protected:
	static PGameRes WINAPI OpenResFiles(const tstring whichPath); //�ж�Ŀ¼�Ƿ�Ϊ��Ϸ��Դ�ĸ�Ŀ¼
	static HRESULT WINAPI ValidateFile(HANDLE hFile); //У��ָ���ļ�

public:
	static HRESULT WINAPI OpenEnv(const tstring whichPath); //����ȫ����Դ���ò��򿪶���
	static VOID WINAPI CloseEnv(); //ж��ȫ����Դ���ò��رն���

	const PBinLib GetBinLib() const; //�õ������ƿ�����
	const PPalLib &GetPaletteLib() const; //�õ���ɫ�������
	const PSndLib &GetSoundLib() const; //�õ���Ч������
	const PBgmLib &GetBGMLib() const; //�õ��������ֿ�����

public:
	LPCTSTR GetRootPath() const; //�õ���Ϸ��Ŀ¼
	LPCTSTR GetBinPath() const; //�õ���Ϸ����Ŀ¼
	LPCTSTR GetDataPath() const; //�õ��û�����Ŀ¼
	LPCTSTR GetMapPath() const; //�õ���ͼĿ¼
};


extern GameEnv *mainEnv;
extern GameGraphics *mainGraphics;

extern ThreadInfo mainIOThread;
