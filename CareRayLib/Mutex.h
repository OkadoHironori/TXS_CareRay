#pragma once

#include <Windows.h>

#include "stdafx.h"

using namespace std;

//C++/CLIで排他制御をするならWinAPIを使うしかない。
//std::mutex などはエラーになる。
//そのため自前でstd::mutex_lock_guardライクなクラスを作成する。
//参考：https://qiita.com/hidetaka0/items/59002521130450093dbf

class Mutex
{
private:
	HANDLE g_hMutex;

public:
	Mutex()
	{
		//Mutex生成
		g_hMutex = CreateMutex(NULL, FALSE, NULL);
	}

	~Mutex()
	{
		//クローズ
		CloseHandle(g_hMutex);
	}

	//ロック
	void Lock()
	{
		WaitForSingleObject(g_hMutex, INFINITE);
	}

	//アンロック
	void Unlock()
	{
		ReleaseMutex(g_hMutex);
	}
};

class MutexLockGuard
{
private:
	Mutex* g_Mutex;

public:
	MutexLockGuard(Mutex* mutex)
	{
		g_Mutex = mutex;
		g_Mutex->Lock();//ロック
	}

	~MutexLockGuard()
	{
		g_Mutex->Unlock();//アンロック
	}
};