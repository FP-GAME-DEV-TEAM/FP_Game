/********************************
* FileName: FPModule.h
* FileFunc: dllģ�鶨�����
* Author: SQK
* Date: 2013-06-01
* Descript: ����FP����ȫ�����˽��ģ�鶨��
********************************/


#pragma once


/* This macro is for dll debugging */
#ifdef FP_MODULE_COMPILE
#define FP_MODULE_API __declspec(dllexport)
#else
#define FP_MODULE_API __declspec(dllimport)
#endif

/* This macro is for FP project debugging */
#ifdef FP_PROJECT_DEBUG
#include <stdarg.h>
#define FP_DEBUG_MSG(format, ...) _tprintf(format, ##__VA_ARGS__)
#else
#define FP_DEBUG_MSG(format, ...)
#endif

/* tstring for different char-set encodings */
#if (defined UNICODE) || (defined _UNICODE)
#define tstring std::wstring
#else
#define tstring std::string
#endif

/* These macros below can convert a macro to its varible name. */
#define _S(x) __S(x)
#define __S(x) _T(#x)


//-----------------------------------------------------------------------------
// Misc Functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct);}


//-----------------------------------------------------------------------------
// Game Message Defines
//-----------------------------------------------------------------------------

// Start at 0x0400, AKA WM_USER. end with 0x7fff.
// FP project own message base value
#define FPMSG_BASE						(WM_USER + 0x0100)

// Thread message
#define FPMSG_THREAD_START				(FPMSG_BASE + 0x0100)
#define FPMSG_THREAD_STOP				(FPMSG_BASE + 0x0101)

#define FPMSG_IO_BIN_REQ				(FPMSG_BASE + 0x0110)
#define FPMSG_IO_BIN_RSP				(FPMSG_BASE + 0x0111)
#define FPMSG_IO_BIN_PAL				(FPMSG_BASE + 0x0112)

#define FPMSG_IO_SND_REQ				(FPMSG_BASE + 0x0120)
#define FPMSG_IO_SND_RSP				(FPMSG_BASE + 0x0121)
#define FPMSG_IO_BGM_ON					(FPMSG_BASE + 0x0122)
#define FPMSG_IO_BGM_OFF				(FPMSG_BASE + 0x0123)

#define FPMSG_END						(WM_USER + 0x0c00 - 1)


//=====================================
// Game Constant Defines
//

//��Դ�����궨��
#define FP_FILE_NAME_SIZE 32
#define FP_FILE_COUNT_BGM 43
#define FP_FILE_COUNT_PAL 18
#define FP_FILE_COUNT_SND 266
#define FP_FILE_SIZE_PAL 672

#define FP_STORE_PAL_COUNT 256
#define FP_STORE_PAL_DEFAULT 32
#define FP_STORE_PAL_SELECTED 224

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

#define FP_FILE_GRAPHIC_INFO _T("GraphicInfo_") //ͼ���ֵ��ļ���
#define FP_FILE_GRAPHIC_DATA _T("GraphicData_") //ͼ�������ļ���
#define FP_FILE_ANIME_INFO _T("AnimeInfo_") //�����ֵ��ļ���
#define FP_FILE_ANIME_DATA _T("AnimeData_") //���������ļ���

//��ɫ���ļ��궨��
#define FP_PALETTE_DAY		palet_00
#define FP_PALETTE_DUSK		palet_01
#define FP_PALETTE_NIGHT	palet_02
#define FP_PALETTE_DAWN		palet_03
#define FP_PALETTE_04		palet_04
#define FP_PALETTE_05		palet_05
#define FP_PALETTE_06		palet_06
#define FP_PALETTE_07		palet_07
#define FP_PALETTE_08		palet_08
#define FP_PALETTE_09		palet_09
#define FP_PALETTE_10		palet_10
#define FP_PALETTE_11		palet_11
#define FP_PALETTE_12		palet_12
#define FP_PALETTE_13		palet_13
#define FP_PALETTE_14		palet_14
#define FP_PALETTE_15		palet_15
#define FP_PALETTE_BLACK	palet_16
#define FP_PALETTE_WHITE	palet_17
//��ɫ��ö��
typedef enum tagPalette
{
	palet_00 = 0,
	palet_01 = 1,
	palet_02 = 2,
	palet_03 = 3,
	palet_04 = 4,
	palet_05 = 5,
	palet_06 = 6,
	palet_07 = 7,
	palet_08 = 8,
	palet_09 = 9,
	palet_10 = 10,
	palet_11 = 11,
	palet_12 = 12,
	palet_13 = 13,
	palet_14 = 14,
	palet_15 = 15,
	palet_16 = 16,
	palet_17 = 17,
} Palette;

//�ļ���׺�궨��
#define FP_FILE_SUFFIX_BIN _T(".bin") //ͼ����Դ�ļ���׺
#define FP_FILE_SUFFIX_DAT _T(".dat") //���������ļ���׺
#define FP_FILE_SUFFIX_PAL _T(".pal") //��ɫ���ļ���׺
#define FP_FILE_SUFFIX_LOG _T(".log") //��־�ļ���׺
#define FP_FILE_SUFFIX_MAP _T(".map") //��ͼ�ļ���׺
#define FP_FILE_SUFFIX_MP3 _T(".mp3") //���������ļ���׺
#define FP_FILE_SUFFIX_WAV _T(".wav") //��Ч�ļ���׺


//=====================================
// Resource Data Types
//

//�������ֿ�
typedef struct tagBgmLib
{
	LPCTSTR bgmPath; //��������Ŀ¼����б�ܽ���
	DWORD sum; //ʵ�������ļ�����
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} BgmLib, *PBgmLib;

//��Ϸ��Ч��
typedef struct tagSndLib
{
	LPCTSTR sndPath; //��Ϸ��ЧĿ¼����б�ܽ���
	DWORD sum; //ʵ����Ч�ļ�����
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} SndLib, *PSndLib;

//��ɫ���
typedef struct tagPalLib
{
	LPCTSTR palPath; //��ɫ��Ŀ¼����б�ܽ���
	DWORD sum; //��ɫ������
	LPCTSTR *fileList; //�ļ��б�
	LPDWORD crc32; //����У��
} PalLib, *PPalLib;

//��������Դ��
typedef struct tagBinLib
{
	LPCTSTR binPath; //����������Ŀ¼����б�ܽ���
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
	LPCTSTR dataPath; //��������Ŀ¼����б�ܽ���
	LPCTSTR userID; //����ָʾ��ǰ�û�
	PLocalData userData; //ָ��ǰ�û��ı�������
} DataSet, *PDataSet;

//��ͼ��Դ��
typedef struct tagMapPack
{
	LPCTSTR mapPath; //��ͼĿ¼����б�ܽ���
	//������ͼ
	//˽�е�ͼ
	//�����ͼ
} MapPack, *PMapPack;

//��Ϸ��Դ�ṹ
typedef struct tagGameRes
{
	LPCTSTR rootPath; //��Ϸ��Ŀ¼����б�ܽ���
	WORD mainVersion; //��Ϸ���汾
	WORD subVersion; //��Ϸ�Ӱ汾
	PBinLib pBinLib; //��������Դ
	PDataSet pDataSet; //����������Դ
	PMapPack pMapPack; //��ͼ��Դ
} GameRes, *PGameRes;


//Ӱ�����ӿ�
typedef struct tagGraphicLink
{
	HANDLE hGraphicInfo; //ͼ��Ŀ¼����
	HANDLE hGraphicData; //ͼ�����ݶ���
	HANDLE hAnimeInfo; //����Ŀ¼����
	HANDLE hAnimeData; //�������ݶ���
	LPBYTE table_Selected; //��ѡ��ɫ��
	LPBYTE table_Default; //�̶���ɫ��
} GraphicLink, *PGraphicLink;


//=====================================
// Game Environment Interface
//

class FP_MODULE_API IGameEnv
{
public:
	virtual LPCTSTR GetRootPath() const = 0; //�õ���Ϸ��Ŀ¼
	virtual LPCTSTR GetBinPath() const = 0; //�õ���Ϸ����Ŀ¼
	virtual LPCTSTR GetDataPath() const = 0; //�õ��û�����Ŀ¼
	virtual LPCTSTR GetMapPath() const = 0; //�õ���ͼĿ¼
	virtual BinLib &GetBinLib() const = 0; //�õ������ƿ�����
	virtual PalLib &GetPaletteLib() const = 0; //�õ���ɫ�������
	virtual SndLib &GetSoundLib() const = 0; //�õ���Ч������
	virtual BgmLib &GetBGMLib() const = 0; //�õ��������ֿ�����
	virtual BOOL LoadPalette(PalLib& pal) = 0; //���ص�ɫ��
};


//=====================================
// Utility API
//

// tstringת��д
inline tstring WINAPI StrToUpper(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),toupper);
	return tmp;
}
// tstringתСд
inline tstring WINAPI StrToLower(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	return tmp;
}

LPTSTR WINAPI NewPath(const tstring &str, const int size); //���ò��ɸ��ĵ��ַ������õ��û��Զ����ȵ��ַ���
BOOL WINAPI IsFolderExist(tstring whichPath); //�ж�Ŀ¼�Ƿ���ڣ������ڷ���FALSE
HWND WINAPI GetRootWindow(HWND hWnd); //��������ؼ������������ϲ㸸���ڵľ��
