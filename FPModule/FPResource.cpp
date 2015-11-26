/********************************
* FileName: FPResource.cpp
* FileFunc: FP��Ϸ��Դ����ʵ��ģ��
* Author: SQK
* Date: 2013-06-12
* Descript: ��Ҫ������Ϸȫ����Դ��׼���������Լ��
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"
#include "FPFunction.h"

//ͼ������ݻ���
static GraphicInfo *gListGraphic; //ͼ��Ŀ¼����
static AnimeInfo *gListAnime; //����Ŀ¼����
//static HashMap_Dict gDictGraphic; //ͼ��Ŀ¼�ֵ�
//static HashMap_Dict gDictAnime; //����Ŀ¼�ֵ�
//static TreeMap_Cache gCacheGraphic; //ͼ�����ݻ���
//static TreeMap_Cache gCacheAnime; //�������ݻ���
static DWORD gPaletteDefault[FP_STORE_PAL_DEFAULT]; //�̶���ɫ�������
static DWORD gPaletteSelected[FP_FILE_COUNT_PAL][FP_STORE_PAL_SELECTED]; //�ɱ��ɫ�������

BOOL GameEnv::LoadPalette(PalLib& pal)
{
	DWORD n,count;
	tstring filePath = pal.palPath;
	tstring tmpPath;
	HANDLE hFile;
	LPBYTE buffer;

	//�̶���ɫ�������
	n = 0;
	gPaletteDefault[n++] = 0x00000000;
	gPaletteDefault[n++] = 0x00800000;
	gPaletteDefault[n++] = 0x00008000;
	gPaletteDefault[n++] = 0x00808000;
	gPaletteDefault[n++] = 0x00000080;
	gPaletteDefault[n++] = 0x00800080;
	gPaletteDefault[n++] = 0x00008080;
	gPaletteDefault[n++] = 0x00808080;
	gPaletteDefault[n++] = 0x00c0dcc0;
	gPaletteDefault[n++] = 0x00a6caf0;
	gPaletteDefault[n++] = 0x00de0000;
	gPaletteDefault[n++] = 0x00ff5f00;
	gPaletteDefault[n++] = 0x00ffffa0;
	gPaletteDefault[n++] = 0x00005fd2;
	gPaletteDefault[n++] = 0x0050d2ff;
	gPaletteDefault[n++] = 0x0028e128;
	gPaletteDefault[n++] = 0x00f5c396;
	gPaletteDefault[n++] = 0x001ea05f;
	gPaletteDefault[n++] = 0x00c37d46;
	gPaletteDefault[n++] = 0x009b551e;
	gPaletteDefault[n++] = 0x00464137;
	gPaletteDefault[n++] = 0x0028231e;
	gPaletteDefault[n++] = 0x00fffbf0;
	gPaletteDefault[n++] = 0x003a6ea5;
	gPaletteDefault[n++] = 0x00808080;
	gPaletteDefault[n++] = 0x00ff0000;
	gPaletteDefault[n++] = 0x0000ff00;
	gPaletteDefault[n++] = 0x00ffff00;
	gPaletteDefault[n++] = 0x000000ff;
	gPaletteDefault[n++] = 0x00ff80ff;
	gPaletteDefault[n++] = 0x0000ffff;
	gPaletteDefault[n++] = 0x00ffffff;

	//�ɱ��ɫ�������
	buffer = new BYTE[FP_FILE_SIZE_PAL];
	for(n=0; n<FP_FILE_COUNT_PAL; n++)
	{
		// to do
	}
	
	delete buffer;
	return TRUE;
}
