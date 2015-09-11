/********************************
* FileName: FPDataType.h
* FileFunc: FP��Ϸ��Դ���Ͷ���ģ��
* Author: SQK
* Date: 2013-07-26
* Descript: ����FP����������������������͵Ķ���
********************************/


#pragma once


//================================
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


//================================
// Error Data Types
//

//��Դ��������
typedef enum tagErrorRes
{
	ERROR_RES_Unknown = 0, //δ֪����
	ERROR_RES_MissingPath = 1, //·���쳣
	ERROR_RES_MissingFile = 2, //�ļ���ʧ
	ERROR_RES_DuplicatedFile = 3, //�������ļ�������
	ERROR_RES_VersionMismatch = 4, //�汾ƥ�䲻ͨ��
	ERROR_RES_FilesSumMismatch = 5, //�ļ�������ƥ��
	ERROR_RES_DateTimeMismatch = 6, //�޸�����ƥ�䲻ͨ��
	ERROR_RES_DigitalMismatch = 7, //�ļ�У��ʧ��
} ErrorRes;

//��Դ������Ϣ
typedef struct tagResError
{
	ErrorRes error; //��Դ��������
	LPTSTR func; //������
	tagResError *next; //��һ������
} ResError, *PResError;

//IO��������
typedef enum tagErrorIO
{
	ERROR_IO_Unknown = 0, //δ֪����
	ERROR_IO_EOF = 1, //�����ļ�β
	ERROR_IO_Limited = 2, //Ȩ�����ƴ���
	ERROR_IO_Timeout = 3, //�첽��ʱ
} ErrorIO;

//IO������Ϣ
typedef struct tagIOError
{
	ErrorIO error; //IO��������
	LPTSTR func; //������
	tagIOError *next; //��һ������
} IOError, *PIOError;


//��Ϸ��Դ����������
class GameEnv : public IGameEnv
{
private:
	static GameEnv *pEnv; //��������
	PGameRes pGameRes; //��Ϸ������Ϣ
	PGraphicLink pGraphic; //Ӱ����Դ����

	GameEnv(PGameRes pRes); //˽�й��췽��
	~GameEnv(); //��������
	static PGameRes WINAPI OpenResFiles(const tstring whichPath); //�ж�Ŀ¼�Ƿ�Ϊ��Ϸ��Դ�ĸ�Ŀ¼
	static HRESULT WINAPI ValidateFile(HANDLE hFile); //У��ָ���ļ�

public:
	static HRESULT WINAPI InitEnv(const tstring whichPath); //��ʼ������
	static GameEnv* WINAPI GetEnv(); //�õ���Դ�������ʵ��(*)
	static void WINAPI ReleaseEnv(); //������Ϸȫ����Դ����

	LPCTSTR GetRootPath() const; //�õ���Ϸ��Ŀ¼
	LPCTSTR GetBinPath() const; //�õ���Ϸ����Ŀ¼
	LPCTSTR GetDataPath() const; //�õ��û�����Ŀ¼
	LPCTSTR GetMapPath() const; //�õ���ͼĿ¼

	BinLib &GetBinLib() const; //�õ������ƿ�����
	PalLib &GetPaletteLib() const; //�õ���ɫ�������
	SndLib &GetSoundLib() const; //�õ���Ч������
	BgmLib &GetBGMLib() const; //�õ��������ֿ�����

	BOOL LoadPalette(PalLib& pal); //���ص�ɫ��
};

