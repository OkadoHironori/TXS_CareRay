#include "stdafx.h"

#include "CR_Callback.h"

#pragma comment(lib, "CRInterface")

using namespace CareRay;

CR_Callback::CR_Callback(Mutex* _mutex, Xs::CareRayLib::CRLib^ crlib) : ICallback()
{
	hEventNewFrame = CreateEvent(NULL, FALSE, FALSE, NULL);

	hEventCalibratoinFinished = CreateEvent(NULL, FALSE, FALSE, NULL);

	calibrationInfo = new CR_CalibrationInfo();

	temperature = new CR_Temperature();

	acquisitionStatInfo = new CR_AcquisitionStatInfo();

	mutex = _mutex;

	Parent = crlib;
}

CR_Callback::~CR_Callback()
{
	mutex = NULL;

	free(acquisitionStatInfo);

	free(temperature);

	free(calibrationInfo);

	CloseHandle(hEventCalibratoinFinished);

	CloseHandle(hEventNewFrame);
}


//override
void CR_Callback::Process(int nEventID, CR_Event* pEventParam)
{
	LastEvent = nEventID;

	switch (nEventID)
	{
	case CR_EVT_NEW_FRAME:
		{
			// フレーム取得
			{
				MutexLockGuard lockGuard(mutex);

				if (NULL == pEventParam) return; //エラー

				NewFrameIndex = *(int*)pEventParam->pData;

				FrameLength = (pEventParam->nWidth * pEventParam->nHeight * 2) + FrameHeaderLength;
			}

			// 同期処理
			if (NULL != hEventNewFrame)
			{
				SetEvent(hEventNewFrame);
			}
		}
		break;

	case CR_EVT_CALIBRATION_IN_PROGRESS:
		{
			CR_CalibrationInfo* calInfo_inprogress = (CR_CalibrationInfo*)pEventParam->pData;

			//current Num / total Num / frame mean
			if (NULL != calibrationInfo)
			{
				MutexLockGuard lockGuard(mutex); //ブロックを抜けると自動でアンロック

				memcpy(calibrationInfo, calInfo_inprogress, sizeof(CR_CalibrationInfo));
			}
		}
		//進捗確認
		break;

	case CR_EVT_CALIBRATION_FINISHED:
		//calibration info
		{
			CR_CalibrationInfo* calInfo_finished = (CR_CalibrationInfo*)pEventParam->pData;

			if (NULL != calibrationInfo)
			{
				MutexLockGuard lockGuard(mutex); //ブロックを抜けると自動でアンロック

				memcpy(calibrationInfo, calInfo_finished, sizeof(CR_CalibrationInfo));
			}

			//完了通知
			if (NULL != hEventCalibratoinFinished)
			{
				SetEvent(hEventCalibratoinFinished);
			}
		}
		break;

	case CR_EVT_TEMPERATURE_INFO:
		{
			CR_Temperature* temperatureInfo = (CR_Temperature*)pEventParam->pData;

			if (NULL != temperature)
			{
				memcpy(temperature, temperatureInfo, sizeof(CR_Temperature));
			}
		}
		break;

	case CR_EVT_ACQ_STAT_INFO:
		{
			// statistic of current acquisition
			CR_AcquisitionStatInfo* acqStatInfo = (CR_AcquisitionStatInfo*)pEventParam->pData;

			if (NULL != acquisitionStatInfo)
			{
				memcpy(acquisitionStatInfo, acqStatInfo, sizeof(CR_AcquisitionStatInfo));
			}
		}
		break;

	case CR_EVT_DETR_DISCONNECTED:
		if(static_cast<Xs::CareRayLib::CRLib^>(Parent) != nullptr)
			Parent->OnDetectorDisconnected();
		break;

	case CR_EVT_SERVER_DISCONNECTED:
		if (static_cast<Xs::CareRayLib::CRLib^>(Parent) != nullptr)
			Parent->OnServerDisconnected();
		break;

	case CR_EVT_DETR_RECONNECTED:
		if (static_cast<Xs::CareRayLib::CRLib^>(Parent) != nullptr)
			Parent->OnDetectorReconnected();
		break;

	case CR_EVT_SERVER_RECONNECTED:
		if (static_cast<Xs::CareRayLib::CRLib^>(Parent) != nullptr)
			Parent->OnServerReconnected();
		break;

	case CR_EVT_IMAGE_QUEUE_BLOCKED:
		if (static_cast<Xs::CareRayLib::CRLib^>(Parent) != nullptr)
			Parent->OnQueueBlocked();
		break;

	default:
		break;
	}
}

void CR_Callback::SetHeaderLength(int headerLength)
{
	FrameHeaderLength = headerLength;
}

void CR_Callback::Reset()
{
	if (NULL != hEventNewFrame)
	{
		ResetEvent(hEventNewFrame);
	}

	if (NULL != hEventCalibratoinFinished)
	{
		ResetEvent(hEventCalibratoinFinished);
	}
}

DWORD CR_Callback::WaitNewFrame(DWORD dwMilliseconds)
{
	if (NULL == hEventNewFrame)
		return -1;
	
	DWORD ret = WaitForSingleObject(hEventNewFrame, dwMilliseconds);

	if (0 == ret)
	{
		ResetEvent(hEventNewFrame);
	}
	
	return ret;
}

DWORD CR_Callback::WaitCalibrationFinished(DWORD dwMilliseconds)
{
	if (NULL == hEventCalibratoinFinished)
		return -1;

	DWORD ret = WaitForSingleObject(hEventCalibratoinFinished, dwMilliseconds);

	if (0 == ret)
	{
		ResetEvent(hEventCalibratoinFinished);
	}

	return ret; 
}
