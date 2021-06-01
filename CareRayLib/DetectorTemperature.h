#pragma once

#include "stdafx.h"

namespace Xs
{
	namespace CareRayLib
	{
		// CR_Temperature
		public ref class DetectorTemperature
		{
			float _Current;

			float _Maximum;

			float _Average;

			bool _OverHot;

		internal:
			DetectorTemperature(CR_Temperature *temperature)
			{
				_Current = temperature->currentTemperature;
				_Maximum = temperature->maxTemperature;
				_Average = temperature->aveTemperature;
				_OverHot = (bool)temperature->overhot_flag;
			}

		public:
			property float Current
			{
				float get() { return _Current; }
			}

			property float Maximum
			{
				float get() { return _Maximum; }
			}

			property float Average
			{
				float get() { return _Average; }
			}

			property bool OverHot
			{
				bool get() { return _OverHot; }
			}

		};
	}
}