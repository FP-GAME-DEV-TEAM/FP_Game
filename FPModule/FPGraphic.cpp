//
// 这个源文件包含了FP工程中图像模块的所有方法实现
//

#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"
#include "FPFunction.h"

//================================
//初始化图形模块
//


FP_MODULE_API BOOL WINAPI InitGraphic(const IGameEnv &IEnv)
{
	LPCTSTR s = IEnv.GetRootPath();
	return TRUE;
}
