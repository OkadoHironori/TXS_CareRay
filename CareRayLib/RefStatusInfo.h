#pragma once

#include "stdafx.h"

namespace Xs
{
	namespace CareRayLib
	{
		public enum class RefStatus
		{
			// �t�@�C����������Ȃ�
			NotFound = (int)CR_REF_NOT_FOUND,

			// �L���ȃL�����u���[�V�����f�[�^
			Valid = (int)CR_REF_VALID,

			// �����؂�ԋ�
			AboutToExpire = (int)CR_REF_ABOUT_TO_EXPIRE,

			// �����ȃL�����u���[�V�����f�[�^
			Invalid = (int)CR_REF_INVALID,
		};

		// �L�����u���[�V�����f�[�^�̃X�e�[�^�X���
		public ref class RefStatusInfo
		{
			RefStatus _Dark;
			RefStatus _Gain;
			RefStatus _Defect;

		internal:

			RefStatusInfo(CR_RefStatusInfo* refStatusInfo)
			{
				_Dark = (RefStatus)refStatusInfo->nDarkStatus;
				_Gain = (RefStatus)refStatusInfo->nGainStatus;
				_Defect = (RefStatus)refStatusInfo->nDefectStatus;
			}

		public:

			property RefStatus Dark
			{
				RefStatus get() { return _Dark; }
			}

			property RefStatus Gain
			{
				RefStatus get() { return _Gain; }
			}

			property RefStatus Defect
			{
				RefStatus get() { return _Defect; }
			}

			RefStatusInfo(RefStatusInfo^ refStatusInfo) 
			{
				_Dark = refStatusInfo->Dark;

				_Gain = refStatusInfo->Gain;

				_Defect = refStatusInfo->Defect;
			}

			~RefStatusInfo() {}

			!RefStatusInfo() {}
		};
	}
}