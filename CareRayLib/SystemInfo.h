#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// CR_SystemInfo
		public ref class SystemInfo
		{
			unsigned int _RawImageWidth = 0;
			unsigned int _RawImageHeight = 0;
			unsigned int _FrameHeaderLength = 0;

			String^ _HardwareVersion = String::Empty;
			String^ _SerialNumber = String::Empty;
			String^ _SoftwareVersion = String::Empty;
			String^ _FirmwareVersion = String::Empty;
			String^ _DetectorMachineID = String::Empty;
			String^ _DetectorDescription = String::Empty;

		internal:
			// CR_SystemInfo
			SystemInfo(CR_SystemInfo *systeminfo)
			{
				if (NULL == systeminfo)
				{
					throw gcnew System::ArgumentNullException("systeminfo");
				}

				_RawImageWidth = systeminfo->nRawImageWidth;
				_RawImageHeight = systeminfo->nRawImageHeight;
				_FrameHeaderLength = systeminfo->nFrmHeaderLen;

				_HardwareVersion = gcnew String(systeminfo->szHardwareVersion);
				_SerialNumber = gcnew String(systeminfo->szSerialNumber);
				_SoftwareVersion = gcnew String(systeminfo->szSoftwareVersion);
				_FirmwareVersion = gcnew String(systeminfo->szFirmwareVersion);
				_DetectorMachineID = gcnew String(systeminfo->szDetrMachineID);
				_DetectorDescription = gcnew String(systeminfo->szDetrDesc);
			}

		public:

			// Raw Image Width
			property unsigned int RawImageWidth
			{
				unsigned int get() { return _RawImageWidth; }
			}

			// Raw Image Height
			property unsigned int RawImageHeight
			{
				unsigned int get() { return _RawImageHeight; }
			}

			// Frame Header Length
			property unsigned int FrameHeaderLength
			{
				unsigned int get() { return _FrameHeaderLength; }
			}

			property String^ HardwareVersion
			{
				String^ get() { return _HardwareVersion; }
			}

			property String^ SerialNumber
			{
				String^ get() { return _SerialNumber; }
			}

			property String^ SoftwareVersion
			{
				String^ get() { return _SoftwareVersion; }
			}

			property String^ FirmwareVersion
			{
				String^ get() { return _FirmwareVersion; }
			}

			property String^ DetectorMachineID
			{
				String^ get() { return _DetectorMachineID; }
			}

			property String^ DetectorDescription
			{
				String^ get() { return _DetectorDescription; }
			}

			~SystemInfo() {}

			!SystemInfo() {}
		};
	}
}