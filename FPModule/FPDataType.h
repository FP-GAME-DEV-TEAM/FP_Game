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
} GraphicInfo;

//ͼ������ͷ�ṹ(16BYTE)
typedef struct tagGraphicData
{
	BYTE mod[2]; //ħ�����̶�Ϊ��RD������Ϊreal data
	BYTE flag; //�汾��ţ������ж�ѹ����ʽ
	BYTE check; //У��λ����;δ֪
	LONG width; //ͼƬ�Ŀ��
	LONG height; //ͼƬ�ĸ߶�
	DWORD size; //ͼƬ���ݴ�С���������ͷ��
} GraphicData;

//����Ŀ¼�ṹ(16BYTE)
typedef struct tagAnimeInfo
{
	LONG id; //������Ψһ��ʶID
	DWORD addr; //�����������ļ��е�λ��
	DWORD size; //һ�����������Ĵ�С
	WORD count; //һ���������������Ķ�����
	WORD check; //�汾У��λ����;δ֪
} AnimeInfo;

//�������ݵ�ͷ�ṹ(12BYTE)
typedef struct tagAnimeHead
{
	WORD facing; //�����ĳ���0-7
	WORD action; //���������ͣ���Щ�����������ж������У�
	DWORD duration; //�������ѭ�������ʱ�䣨��λΪ���룩
	DWORD frames; //�������������ͼƬ֡��
} AnimeHead;

//�������ݵ�Ԫ(10BYTE)
typedef struct tagAnimeData
{
	LONG index; //ͼƬ֡��Ӧ��ͼ������ID
	BYTE reserve[6]; //����λ����������Ϸ�߼��ж������ֲ����й�
} AnimeData;



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



//=====================================
// Interface Implements Data Types
//

//��Ϸͼ����
class GameGraphics : public IGameGraphics
{
private:
	HANDLE hGraphicInfo; //ͼ��Ŀ¼����
	HANDLE hGraphicData; //ͼ�����ݶ���
	HANDLE hAnimeInfo; //����Ŀ¼����
	HANDLE hAnimeData; //�������ݶ���
	PALETTEENTRY pPalette[FP_STORE_PAL_COUNT]; //��ǰ��ɫ��

public:
	GameGraphics(PBinLib pBin); //���캯��
	~GameGraphics(); //��������

	LPCVOID GetImageById(int id) const; //ͨ��ID�õ�ͼƬ
	LPCVOID GetAnimeById(int id) const; //ͨ��ID�õ�����
	HRESULT SwitchPalette(int id) const; //������ɫ��
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
	LPCTSTR GetTextById(int id) const; //ͨ��ID�õ��ı�
	LPCTSTR GetUserLogById(int id) const; //ͨ��ID�õ��û���־
};


//��Ϸ��ͼ��
class GameMaps : public IGameMaps
{
public:
	LPCVOID GetLocalMapById(int id) const; //ͨ��ID�õ����ص�ͼ
	LPCVOID GetRemoteMapById(int id) const; //ͨ��ID�õ�Զ�̵�ͼ
};


//��Ϸ������
class GameNetwork : public IGameNetwork
{

};


//��Ϸ��Դ������
class GameEnv : public IGameEnv
{
private:
	static GameEnv *pEnv; //��������
	PGameRes pGameRes; //��Ϸ������Ϣ
	GameGraphics *pGraphics; //��Ϸͼ����Դ
	GameAudio *pAudio; //��Ϸ������Դ
	GameText *pText; //��Ϸ�ı���Դ
	GameMaps *pMaps; //��Ϸ��ͼ��Դ
	GameNetwork *pNetwork; //��Ϸ������Դ

	GameEnv(const PGameRes pRes); //˽�й��췽��
	~GameEnv(); //��������

	static PGameRes WINAPI OpenResFiles(const tstring whichPath); //�ж�Ŀ¼�Ƿ�Ϊ��Ϸ��Դ�ĸ�Ŀ¼
	static HRESULT WINAPI ValidateFile(HANDLE hFile); //У��ָ���ļ�

protected:
	const BinLib &GetBinLib() const; //�õ������ƿ�����
	const PalLib &GetPaletteLib() const; //�õ���ɫ�������
	const SndLib &GetSoundLib() const; //�õ���Ч������
	const BgmLib &GetBGMLib() const; //�õ��������ֿ�����

	BOOL LoadPalette(PalLib& pal); //���ص�ɫ��

public:
	static HRESULT WINAPI OpenEnv(const tstring whichPath); //����ȫ����Դ���ò��򿪶���
	static void WINAPI CloseEnv(); //ж��ȫ����Դ���ò��رն���

	LPCTSTR GetRootPath() const; //�õ���Ϸ��Ŀ¼
	LPCTSTR GetBinPath() const; //�õ���Ϸ����Ŀ¼
	LPCTSTR GetDataPath() const; //�õ��û�����Ŀ¼
	LPCTSTR GetMapPath() const; //�õ���ͼĿ¼
};

