//
// ���Դ�ļ�������FP������ͼ��ģ������з���ʵ��
//

#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"
#include "FPFunction.h"

//================================
//��ʼ��ͼ��ģ��
//


FP_MODULE_API BOOL WINAPI InitGraphic(const IGameEnv &IEnv)
{
	LPCTSTR s = IEnv.GetRootPath();
	return TRUE;
}
