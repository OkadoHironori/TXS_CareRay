#pragma once

#include <Windows.h>

#include "stdafx.h"

using namespace std;

//C++/CLI�Ŕr�����������Ȃ�WinAPI���g�������Ȃ��B
//std::mutex �Ȃǂ̓G���[�ɂȂ�B
//���̂��ߎ��O��std::mutex_lock_guard���C�N�ȃN���X���쐬����B
//�Q�l�Fhttps://qiita.com/hidetaka0/items/59002521130450093dbf

class Mutex
{
private:
	HANDLE g_hMutex;

public:
	Mutex()
	{
		//Mutex����
		g_hMutex = CreateMutex(NULL, FALSE, NULL);
	}

	~Mutex()
	{
		//�N���[�Y
		CloseHandle(g_hMutex);
	}

	//���b�N
	void Lock()
	{
		WaitForSingleObject(g_hMutex, INFINITE);
	}

	//�A�����b�N
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
		g_Mutex->Lock();//���b�N
	}

	~MutexLockGuard()
	{
		g_Mutex->Unlock();//�A�����b�N
	}
};