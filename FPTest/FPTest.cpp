// FPTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

using namespace std;

DWORD WINAPI ThreadNormal(LPVOID lpParam)
{
	int i = 0;
	while (i++ < 10)
		cout << "Normal Thread is running" << endl;
	return 0;
}

DWORD WINAPI ThreadAboveNormal(LPVOID lpParam)
{
	int i = 0;
	while (i++ < 10)
		cout << "Above Normal Thread is running" << endl;
	return 0;
}

int main()
{
	DWORD dwThreadID;
	HANDLE hThread[2];
	//Create a above Normal Thread
	hThread[0] = CreateThread(NULL, 0, ThreadAboveNormal, NULL, CREATE_SUSPENDED, &dwThreadID);
	//set priority is above normal
	SetThreadPriority(hThread[0], THREAD_PRIORITY_ABOVE_NORMAL);
	//resume thread
	ResumeThread(hThread[0]);
	//Create a Normal Thread
	hThread[1] = CreateThread(NULL, 0, ThreadNormal, NULL, 0, &dwThreadID);
	//wait two
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	//close thread 
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	system("pause");
	return 0;
}
