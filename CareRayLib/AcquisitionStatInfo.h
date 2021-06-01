#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// B‰e‚Ì“Œvî•ñ
		public ref class AcquisitionStatInfo
		{
			int _TotalFrameNum;

			int _LostFrameNum;

			float _StatFrameRate;

			float _TransmissionSpeed;

			long long _Duration;

		internal:

			AcquisitionStatInfo(CR_AcquisitionStatInfo* acquisitionStatInfo)
			{
				if (NULL == acquisitionStatInfo)
				{
					throw gcnew NullReferenceException();
				}

				_TotalFrameNum = acquisitionStatInfo->nTotalFrameNum;

				_LostFrameNum = acquisitionStatInfo->nLostFrameNum;

				_StatFrameRate = acquisitionStatInfo->fStatFrameRate;

				_TransmissionSpeed = acquisitionStatInfo->fTransmissionSpeed;

				_Duration = acquisitionStatInfo->nAcqDuration;
			}

		public:

			// ‘B‰e–‡”
			property int TotalFrameNum
			{
				int get() { return _TotalFrameNum; }
			}

			// æ‚è‚±‚Ú‚µ–‡”
			property int LostFrameNum
			{
				int get() { return _LostFrameNum; }
			}

			// ƒtƒŒ[ƒ€ƒŒ[ƒg
			property float FrameRate
			{
				float get() { return _StatFrameRate; }
			}

			// “]‘—‘¬“x
			property float TransmissionSpeed
			{
				float get() { return _TransmissionSpeed; }
			}

			// Œp‘±ŠúŠÔ [ms]
			property long long Duration
			{
				long long get() { return _Duration; }
			}

			~AcquisitionStatInfo() {}

			!AcquisitionStatInfo() {}
		};
	}
}