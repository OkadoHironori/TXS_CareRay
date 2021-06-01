#pragma once

#include "stdafx.h"

using namespace System;

namespace Xs
{
	namespace CareRayLib
	{
		// CR_ModeInfo
		public ref class ModeInfo
		{
			int _ModeID;

			int _ImageWidth;
			int _ImageHeight;

			int _BinX;
			int _BinY;

			int _PixelDepth;

			float _MaxFrameRate;
			float _MaxExposureTime;

			cli::array<int>^ _GainLevels;
			int _DefaultGainLevel;

			cli::array<int>^ _TriggerTypes;
			int _DefaultTriggerType;

			int _RoiX;
			int _RoiY;
			int _CutoffX;
			int _CutoffY;

			String^ _Description;

		internal:

			ModeInfo(CR_ModeInfo *modeInfo)
			{
				if (NULL == modeInfo)
				{
					throw gcnew System::ArgumentNullException("modeInfo");
				}

				_ModeID = modeInfo->nModeID;

				_ImageWidth = modeInfo->nImageWidth;
				_ImageHeight = modeInfo->nImageHeight;

				_PixelDepth = modeInfo->nPixelDepth;

				//Binning
				_BinX = modeInfo->nBinX;
				_BinY = modeInfo->nBinY;

				_MaxFrameRate = modeInfo->fMaxFrmRate;

				_MaxExposureTime = modeInfo->fMaxExpTime;

				//ゲイン設定
				_GainLevels = gcnew cli::array<int>(modeInfo->nGainLevelNum);
				for (int i = 0; i < _GainLevels->Length; ++i) 
				{
					_GainLevels[i] = modeInfo->nGainLevels[i];
				}
				_DefaultGainLevel = modeInfo->nDefaultGainLevel;

				//トリガー設定
				_TriggerTypes = gcnew cli::array<int>(modeInfo->nTrigTypeNum);
				for (int i = 0; i < _TriggerTypes->Length; ++i)
				{
					_TriggerTypes[i] = modeInfo->nTrigTypes[i];
				}

				_DefaultTriggerType = modeInfo->nDefaultTrigType;

				_RoiX = modeInfo->nRoiX;
				_RoiY = modeInfo->nRoiY;

				_CutoffX = modeInfo->nCutoffX;
				_CutoffY = modeInfo->nCutoffY;

				_Description = gcnew String(modeInfo->szDesc);
			}
		public:

			// ID
			property int ModeID
			{
				int get() { return _ModeID; }
			}

			// 画像幅
			property int ImageWidth
			{
				int get() { return _ImageWidth; }
			}

			// 画像高さ
			property int ImageHeight
			{
				int get() { return _ImageHeight; }
			}

			// 画像サイズ
			property System::Drawing::Size^ ImageSize
			{
				System::Drawing::Size^ get() { return gcnew System::Drawing::Size(_ImageWidth, _ImageHeight); }
			}

			// ビット深度
			property int PixelDepth
			{
				int get() { return _PixelDepth; }
			}

			// ビニング X方向
			property int BinX
			{
				int get() { return _BinX; }
			}

			// ビニング Y方向
			property int BinY
			{
				int get() { return _BinY; }
			}

			// 最大フレームレート [Hz]
			property float MaxFrameRate
			{
				float get() { return _MaxFrameRate; }
			}

			// 最大露光時間[ms]
			property float MaxExposureTime
			{
				float get() { return _MaxExposureTime; }
			}

			//ゲイン
			property cli::array<int>^ GainLevels
			{
				cli::array<int>^ get() { return _GainLevels; }
			}

			//デフォルトゲイン
			property int DefaultGainLevel
			{
				int get() { return _DefaultGainLevel; }
			}

			//トリガー
			property cli::array<int>^ TriggerTypes
			{
				cli::array<int>^ get() { return _TriggerTypes; }
			}

			//デフォルトトリガー 
			property int DefaultTriggerType
			{
				int get() { return _DefaultTriggerType; }
			}

			// ROI X
			property int RoiX
			{
				int get() { return _RoiX; }
			}

			// ROI Y
			property int RoiY
			{
				int get() { return _RoiY; }
			}

			// ROI 原点
			property System::Drawing::Point^ RoiOrigin
			{
				System::Drawing::Point^ get() { return gcnew System::Drawing::Point(_RoiX, _RoiY); }
			}

			// CutOff X
			property int CutoffX
			{
				int get() { return _CutoffX; }
			}

			// CutOff Y
			property int CutoffY
			{
				int get() { return _CutoffY; }
			}

			// Cutoff Size
			property System::Drawing::Size^ Cutoff
			{
				System::Drawing::Size^ get() { return gcnew System::Drawing::Size(_CutoffX, _CutoffY); }
			}

			// 説明
			property String^ Description
			{
				String^ get() { return _Description; }
			}

			ModeInfo() 
			{

			}

			~ModeInfo() 
			{
				_GainLevels = nullptr;
				_TriggerTypes = nullptr;
			}

			!ModeInfo() 
			{
			}
		};
	}
}