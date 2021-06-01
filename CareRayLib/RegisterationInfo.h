#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// フレームレートを決定する上で優先するパラメーター
		public enum class FpsPriority
		{
			// フレームレート設定優先
			FrameRate = 0, 
			
			// 露光時間設定優先
			ExposureTime = 1,
		};

		// トリガー設定
		public enum class TriggerTypes
		{
			RadExtSync = (int) CR_RadExtSync,

			RadSoftSync = (int)CR_RadSoftSync,

			RadAutoSync = (int)CR_RadAutoSync,

			RadManualSync = (int)CR_RadManualSync,

			RadScanSync = (int)CR_RadScanSync,

			//RadAEDSync = (int)CR_RadAEDSync,

			FluReserved = (int)CR_FluReserved,

			FluExtSync = (int)CR_FluExtSync,

			FluIntSync = (int)CR_FluIntSync,
		};

		// ゲインレベル設定
		public enum class  GainLevels
		{
			Gain0 = (int)CR_G0,
			Gain1 = (int)CR_G1,
			Gain2 = (int)CR_G2,
			Gain3 = (int)CR_G3,
			Gain4 = (int)CR_G4,
			Gain5 = (int)CR_G5,
			Gain6 = (int)CR_G6,
			Gain7 = (int)CR_G7,
		};

		// 登録情報
		public ref class RegisterationInfo
		{
		private:
			int _AppModeKey;

			int _AppModeID;

			float _FrameRate;

			float _ExposureTime;

			FpsPriority _Priority;

			TriggerTypes _TriggerType;

			GainLevels _GainID;

		public:

			// TODO:別にすることも検討する
			// 登録上のKeyとなる値
			property int AppModeKey
			{
				int get() { return _AppModeKey; }
				void set(int value) { _AppModeKey = value; }
			}

			// 適用するAppModeのID
			property int AppModeID
			{
				int get() { return _AppModeID; }
				void set(int value) { _AppModeID = value; }
			}

			// フレームレート[Hz]
			property float FrameRate
			{
				float get() { return _FrameRate; }
				void set(float value) { _FrameRate = value; }
			}

			// 露光時間[msec]
			property float ExposureTime
			{
				float get() { return _ExposureTime; }
				void set(float value) { _ExposureTime = value; }
			}

			// 優先するパラメーター
			property FpsPriority Priority
			{
				FpsPriority get() { return _Priority; }
				void set(FpsPriority value) { _Priority = value; }
			}

			// トリガーの種類
			property TriggerTypes TriggerType
			{
				TriggerTypes get() { return _TriggerType; }
				void set(TriggerTypes value) { _TriggerType = value; }
			}

			// ゲインレベル
			property GainLevels GainID
			{
				GainLevels get() { return _GainID; }
				void set(GainLevels value) { _GainID = value; }
			}

			RegisterationInfo() {};

			~RegisterationInfo() {};

			!RegisterationInfo() {};
		};
	}
}