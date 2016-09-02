/********************************
* FileName: FPDataType.h
* FileFunc: FP��Ϸ��Դ���Ͷ���ģ��
* Author: SQK
* Date: 2013-07-26
* Descript: ����FP����������������������͵Ķ���
********************************/


#pragma once

#include "FPModule.h"


//���·�����ļ����궨��
#define FP_PATH_BIN _T("\\bin") //��Դ�ļ����·��
#define FP_PATH_PAL _T("\\pal") //��ɫ���ļ����·��
#define FP_PATH_SND _T("\\sound") //��Ч�ļ����·��
#define FP_PATH_BGM _T("\\bgm") //���������ļ����·��

#define FP_PATH_DAT _T("\\data") //�����ļ����·��
#define FP_PATH_SET _T("\\setting") //�����ļ����·��
#define FP_PATH_LOG _T("\\log") //��־�ļ����·��
#define FP_PATH_EML _T("\\email") //�ʼ��ļ����·��

#define FP_PATH_MAP _T("\\map") //��ͼ�ļ����·��
#define FP_PATH_STC _T("\\static") //�̶���ͼ�������·��
#define FP_PATH_RDM _T("\\random") //�����ͼ�������·��

#define FP_FILE_GAME_CONFIG _T("Config.dat") //���������ļ���
#define FP_FILE_GRAPHIC_INFO _T("GraphicInfo_") //ͼ���ֵ��ļ���
#define FP_FILE_GRAPHIC_DATA _T("GraphicData_") //ͼ�������ļ���
#define FP_FILE_ANIME_INFO _T("AnimeInfo_") //�����ֵ��ļ���
#define FP_FILE_ANIME_DATA _T("AnimeData_") //���������ļ���

//�ļ���׺�궨��
#define FP_FILE_SUFFIX_BIN _T(".bin") //ͼ����Դ�ļ���׺
#define FP_FILE_SUFFIX_DAT _T(".dat") //���������ļ���׺
#define FP_FILE_SUFFIX_PAL _T(".fgp") //��ɫ���ļ���׺
#define FP_FILE_SUFFIX_LOG _T(".log") //��־�ļ���׺
#define FP_FILE_SUFFIX_MAP _T(".map") //��ͼ�ļ���׺
#define FP_FILE_SUFFIX_MP3 _T(".mp3") //���������ļ���׺
#define FP_FILE_SUFFIX_WAV _T(".wav") //��Ч�ļ���׺

//�ļ���Ӧ��HANDLE����
#define FP_HANDLE_GRAPHIC_DATA 0x0000 //ͼ�����ݾ�����
#define FP_HANDLE_GRAPHIC_INFO 0x0001 //ͼ���ֵ������
#define FP_HANDLE_ANIME_DATA 0x0002 //�������ݾ�����
#define FP_HANDLE_ANIME_INFO 0x0003 //�����ֵ������


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
// CallbackFunc: UINT (CALLBACK *func)(LPVOID)
// If maxSize>0, the inner union specify 'end' to indicate that the length of the data requested is unknown,
// So we use this 'end' to be a terminator of the stream and 'size' will be the final data length,
// Else the union specify 'size' to indicate that the length of the data requested is already specified.
// When IO is complete, using PostMessage() to notify main window to do extra process.
// Sample: PostMessage(HWND, IOType, IDList, )
//

typedef struct tagIOItem
{
	LONG id;
	DWORD maxSize;
	DWORD offset;
	union
	{
		DWORD size;
		TCHAR end;
	};
	LPVOID pData;
} IOItem, *PIOItem;

typedef struct tagIOList
{
	BOOL isCompleted;
	UINT uIOType;
	HANDLE hEvent;
	LONG nCount;
	PIOItem pItemList;

	static tagIOList * WINAPI CreateIOList(UINT type, HANDLE event, LONG count);
	PIOItem SetIOListItem(LONG index, LONG id, DWORD maxSize, DWORD offset, DWORD sizeEnd, LPVOID lpData);
	virtual ~tagIOList();

private:
	tagIOList(){}
	tagIOList(const tagIOList &other){}
} IOList, *PIOList;



//=====================================
// Thread Data Types
// Definition of FP thread routine
//

typedef std::pair<LONG, LPVOID> KeyValue, *PKeyValue;
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

typedef std::map<LONG, PFPImage> ImageMap;
typedef std::map<LONG, PFPAction> ActionMap;
typedef std::vector<GraphicInfo> ImageList;
typedef std::vector<AnimeInfo> ActionList;
typedef std::set<LONG> BinReqSet;

//��Ϸͼ����
class GameGraphics : public IGameGraphics
{
private:
	static GameGraphics *pInstance; //��������

	HANDLE hGraphicInfo; //ͼ����������
	HANDLE hGraphicData; //ͼ�����ݶ���
	HANDLE hAnimeInfo; //������������
	HANDLE hAnimeData; //�������ݶ���
	PALETTEENTRY mPalette[FP_STORE_PAL_COUNT]; //��ǰ��ɫ��
	PALETTEENTRY mPaletteDefault[FP_STORE_PAL_DEFAULT]; //�̶���ɫ�������
	PALETTEENTRY mPaletteOptional[FP_FILE_COUNT_PAL][FP_STORE_PAL_OPTIONAL]; //�ɱ��ɫ�������
	LONG mPaletteIndex; //��ǰȫ�ֵ�ɫ������

	GameGraphics(){}; //���캯��
	GameGraphics(const GameGraphics &other){}; //�������캯��

protected:
	ImageList mGraphicList; //ͼ����������
	ActionList mAnimeList; //������������
	ImageMap mGraphicCache; //ͼ�����ݻ���
	ActionMap mAnimeCache; //�������ݻ���

	BinReqSet mImageReqQueue; //ͼ��IO�������
	BinReqSet mActionReqQueue; //����IO�������

	virtual ~GameGraphics(void); //��������
	static UINT CALLBACK GraphicsIOComplete(LPVOID pParam); //ͼ��IO��ɽӿ�

public:
	static HRESULT WINAPI Create(const PBinLib pBin);
	static VOID WINAPI Destroy();

	HANDLE GetFileHandle(const UINT type) const; //�õ�����ļ����
	HRESULT InitPalette(const PPalLib pPal); //���ص�ɫ��
	HRESULT InitGraphicInfo(); //����ͼ�������ļ�
	HRESULT InitAnimeInfo(); //���ض��������ļ�
	HRESULT GetGraphicInfo(const LONG id, const GraphicInfo **pInfo); //����ͼ�������ļ�
	HRESULT GetAnimeInfo(const LONG id, const AnimeInfo **pInfo); //����ͼ�������ļ�

	// DLL interface functions

	LONG GetCurrentPaletteIndex(); //��õ�ǰ��ɫ��������
	HRESULT ChangePalette(const LONG id, const PALETTEENTRY **pData); //������ɫ��
	HRESULT GetImage(const LONG id, const FPImage **pData); //ͨ��ID�õ�ͼƬ
	HRESULT GetAction(const LONG id, const FPAction **pData); //ͨ��ID�õ�����
	HRESULT LoopIORequest(const DWORD dwTick); //��ѯ����Graphics���IO����
	HRESULT IODataBack(const UINT type, const LONG count, const LPVOID data); //ͼ�����ݷ��ؽӿ�
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
