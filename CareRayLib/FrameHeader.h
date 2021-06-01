#pragma once

#include "stdafx.h"

#include "CR_FrameHeader.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// 画像フレームヘッダー 外部参照用に必要なパラメーターを定義する。
		public ref class FrameHeader
		{
		private:
			unsigned int _FrameID;

			unsigned int _RealAcqSec;

			unsigned long long _AbsAcqTime;

			float _AverageTemperature;

			unsigned long long _DetectroSendTime;

			unsigned long long _ServerSendTime;

			unsigned long long _ServerReceiveTime;

			unsigned long long _APIReceiveTime;
			
			unsigned long long _APPReceiveTime;

		internal:
			FrameHeader(CR_FrameHeader* header)
			{
				if (NULL == header) throw gcnew NullReferenceException();

				_FrameID = header->FrameID;

				_RealAcqSec = header->RealAcqSec;

				_AbsAcqTime = header->AbsAcqTime;

				_AverageTemperature = header->AverageTemperature;

				_ServerReceiveTime = header->ServerReceiveTime;

				_ServerSendTime = header->ServerSendTime;

				_APIReceiveTime = header->APIReceiveTime;

				_APPReceiveTime = header->APPReceiveTime;

				_DetectroSendTime = header->DetectroSendTime;

			};

		public:

			property unsigned int FrameID
			{
				unsigned int get() { return _FrameID; }
			}

			property unsigned int RealAcqSec 
			{
				unsigned int get() { return _RealAcqSec; }
			}

			property unsigned long long AbsAcqTime
			{
				unsigned long long  get() { return _AbsAcqTime; }
			}

			property float AverageTemperature 
			{
				float get() { return _AverageTemperature; }
			}

			property unsigned long long ServerReceiveTime
			{
				unsigned long long  get() { return _ServerReceiveTime; }
			}

			property unsigned long long ServerSendTime
			{
				unsigned long long  get() { return _ServerSendTime; }
			}
			
			property unsigned long long APIReceiveTime
			{
				unsigned long long  get() { return _APIReceiveTime; }
			}
			
			property unsigned long long APPReceiveTime
			{
				unsigned long long  get() { return _APPReceiveTime; }
			}

			property unsigned long long DetectroSendTime
			{
				unsigned long long  get() { return _DetectroSendTime; }
			}

			FrameHeader() {};

			FrameHeader(FrameHeader^ header) 
			{
				if (nullptr == header) throw gcnew NullReferenceException();

				_FrameID = header->FrameID;

				_RealAcqSec = header->RealAcqSec;

				_AbsAcqTime = header->AbsAcqTime;

				_AverageTemperature = header->AverageTemperature;

				_ServerReceiveTime = header->ServerReceiveTime;

				_ServerSendTime = header->ServerSendTime;

				_APIReceiveTime = header->APIReceiveTime;

				_APPReceiveTime = header->APPReceiveTime;

				_DetectroSendTime = header->DetectroSendTime;
			}

			~FrameHeader() {};

			!FrameHeader() {};
		};
	}
}