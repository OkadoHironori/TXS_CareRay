#pragma once

#include "stdafx.h"

#include "DetectorTemperature.h"

namespace Xs
{
	namespace CareRayLib
	{
		// CR_DetrStatus
		public ref class DetectorStatus
		{
			DetectorTemperature^ _Temperature;
		internal:
			DetectorStatus(CR_DetrStatus *detrStatus)
			{
				_Temperature = gcnew DetectorTemperature(&detrStatus->oDetrTemperature);
			}
		public:
			property DetectorTemperature^ Temperature
			{
				DetectorTemperature^ get() { return _Temperature; }
			}
		};
	}
}