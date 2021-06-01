#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// キャリブレーション情報
		public ref class CalibrationInfo
		{
			int _TotalFrameNum;

			int _CurrentFrameNum;

			int _CurrentFrameMeanValue;

			int _Result;

			int _ExpStatus;

			int _TargetGrayValue;

			String^ _ErrorMessage;

		internal:

			CalibrationInfo(CR_CalibrationInfo* calibrationInfo)
			{
				if (NULL == calibrationInfo)
				{
					throw gcnew NullReferenceException();
				}

				_TotalFrameNum = calibrationInfo->nTotalFrameNum;

				_CurrentFrameNum = calibrationInfo->nCurrentFrameNum;

				_CurrentFrameMeanValue = calibrationInfo->nCurrentFrameMeanValue;

				_Result = calibrationInfo->nResult;

				_ExpStatus = calibrationInfo->nExpStatus;

				_TargetGrayValue = calibrationInfo->nTargetGrayValue;

				_ErrorMessage = gcnew String(calibrationInfo->szErrorMsg);
			}

		public:

			property int TotalFrameNum
			{
				int get() { return _TotalFrameNum; }
			}

			property int CurrentFrameNum
			{
				int get() { return _CurrentFrameNum; }
			}

			property int CurrentFrameMeanValue
			{
				int get() { return _CurrentFrameMeanValue; }
			}

			property int Result
			{
				int get() { return _Result; }
			}

			property int ExpStatus
			{
				int get() { return _ExpStatus; }
			}

			property int TargetGrayValue
			{
				int get() { return _TargetGrayValue; }
			}

			property String^ ErrorMessage
			{
				String^ get() { return _ErrorMessage; }
			}

			~CalibrationInfo() {}

			!CalibrationInfo() {}
		};
	}
}