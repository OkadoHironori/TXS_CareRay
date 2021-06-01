#pragma once

#include <Windows.h>

#include "stdafx.h"
#include "Mutex.h"

#include "CareRayLib.h"

#include <msclr/gcroot.h>

using namespace std;

namespace Xs
{
	namespace CareRayLib
	{
		ref class CRLib;
	}
}

// �R�[���o�b�N�p�C���i�[�N���X
public class CR_Callback : public ICallback
{
	// �t���[���擾
	HANDLE hEventNewFrame;

	// �L�����u���[�V�������H
	HANDLE hEventCalibratoinFinished;

	int FrameHeaderLength = 256;

	msclr::gcroot<Xs::CareRayLib::CRLib^> Parent;

public:

	int LastEvent;

	int FrameLength;

	int NewFrameIndex;

	CR_CalibrationInfo* calibrationInfo;

	CR_Temperature* temperature;

	CR_AcquisitionStatInfo* acquisitionStatInfo;

	Mutex* mutex;

	CR_Callback(Mutex* _mutex, Xs::CareRayLib::CRLib^ crlib);

	~CR_Callback();
	
	//override
	virtual void Process(int nEventID, CR_Event* pEventParam);

	void SetHeaderLength(int headerLength);

	void Reset();

	DWORD WaitNewFrame(DWORD dwMilliseconds);

	DWORD WaitCalibrationFinished(DWORD dwMilliseconds);
};
