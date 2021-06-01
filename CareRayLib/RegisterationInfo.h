#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// �t���[�����[�g�����肷���ŗD�悷��p�����[�^�[
		public enum class FpsPriority
		{
			// �t���[�����[�g�ݒ�D��
			FrameRate = 0, 
			
			// �I�����Ԑݒ�D��
			ExposureTime = 1,
		};

		// �g���K�[�ݒ�
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

		// �Q�C�����x���ݒ�
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

		// �o�^���
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

			// TODO:�ʂɂ��邱�Ƃ���������
			// �o�^���Key�ƂȂ�l
			property int AppModeKey
			{
				int get() { return _AppModeKey; }
				void set(int value) { _AppModeKey = value; }
			}

			// �K�p����AppMode��ID
			property int AppModeID
			{
				int get() { return _AppModeID; }
				void set(int value) { _AppModeID = value; }
			}

			// �t���[�����[�g[Hz]
			property float FrameRate
			{
				float get() { return _FrameRate; }
				void set(float value) { _FrameRate = value; }
			}

			// �I������[msec]
			property float ExposureTime
			{
				float get() { return _ExposureTime; }
				void set(float value) { _ExposureTime = value; }
			}

			// �D�悷��p�����[�^�[
			property FpsPriority Priority
			{
				FpsPriority get() { return _Priority; }
				void set(FpsPriority value) { _Priority = value; }
			}

			// �g���K�[�̎��
			property TriggerTypes TriggerType
			{
				TriggerTypes get() { return _TriggerType; }
				void set(TriggerTypes value) { _TriggerType = value; }
			}

			// �Q�C�����x��
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