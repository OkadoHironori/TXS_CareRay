#pragma once

#include <Windows.h>

#include "stdafx.h"

class CR_Callback;

using namespace System;

#include "CR_Callback.h"
#include "ConnectionStatus.h"
#include "DetectorStatus.h"
#include "DetectorTemperature.h"
#include "SystemInfo.h"
#include "ModeInfo.h"
#include "RegisterationInfo.h"
#include "RefStatusInfo.h"
#include "CalibrationInfo.h"
#include "AcquisitionStatInfo.h"
#include "Mutex.h"
#include "FrameHeader.h"

namespace Xs
{
	namespace CareRayLib
	{
		[FlagsAttribute]
		public enum class ProcChain
		{
			None = 0x00,

			SanityCheck = 0x01,
			
			Offset = 0x02,

			Gain = 0x04,

			Defect = 0x08,

			Lag = 0x10,

			Cropping = 0x20,

			RtPixel = 0x40,

			Denoise = 0x80,

			Gauss = 0x100,

			All = 0xFFF,
		};

		// FPD制御クラス
		public ref class CRLib
		{
			// TODO: このクラスのメソッドをここに追加します。

		private:
			// 検出器INDEX ≒ ID
			int detectorIndex;

			// アプリケーションモード
			int currentKey;

			//同期用 イベント
			CR_Callback *callback;

			// 内部バッファ
			char* buffer;

			int bufferLength = 0;

			// システム情報
			SystemInfo^ SystemInfo;

			// モード情報
			cli::array<Xs::CareRayLib::ModeInfo^>^ _ModeInfos;

			// キャリブレーションデータのステータス
			RefStatusInfo^ _RefStatus;

			Mutex* mutex;

			void GetDetectorNum();

			Xs::CareRayLib::SystemInfo^ GetSystemInfomation();

			Xs::CareRayLib::DetectorStatus^ GetDetectorStatus();

			void GetAppModes();

			void GetConnectionStatus();

			// キャリブレーションファイルを保存するディレクトリのルートパスを設定する
			void SetCalibrationPathroot(String^ path);

			
			static int GetReturnCode(int code);

			static String^ GetMessage(int code);

			static unsigned int GetCorrectOption(ProcChain chain);

			static inline void OnError(int errorCode);

		internal:

			void OnDetectorDisconnected(void);

			void OnDetectorReconnected(void);

			void OnServerDisconnected(void);

			void OnServerReconnected(void);

			void OnQueueBlocked(void);

		public:

			property cli::array<Xs::CareRayLib::ModeInfo^>^ ModeInfos
			{
				cli::array<Xs::CareRayLib::ModeInfo^>^  get() { return _ModeInfos; }
			}

			// キャリブレーション中の進捗情報
			property CalibrationInfo^ Calibration
			{
				CalibrationInfo^ get();
			}

			// 撮影中の統計情報
			property AcquisitionStatInfo^ AcquisitionStat
			{
				AcquisitionStatInfo^ get();
			}

			event EventHandler^ DetectorDisconnected;
			
			event EventHandler^ DetectorReconnected;
			
			event EventHandler^ ServerDisconnected;
			
			event EventHandler^ ServerReconnected;
			
			event EventHandler^ QueueBlocked;

			CRLib();

			~CRLib();

			!CRLib();

			// 初期化
			void Init(String^ rootpath, int bufferCount);

			// 接続
			void Connect();

			// 接続解除
			void Disconnect();

			// 画像取得
			void Acquire();
			
			void AcquireWithCorrect();

			// 撮影停止
			void Abort();

			// オフセット取得
			void AcquireOffset();

			void AcquireOffset(bool updateDefectMap);

			// ゲイン取得
			void AcquireGain();

			// キャリブレーション停止
			void AbortCalibration();

			// 校正データの再読み込み // Keyの切り替え
			void Reload(int key);

			// キャリブレーションステータスの更新
			void UpdateRefStatusInfo();

			// フレーム取得
			void GetFrame(cli::array<WORD>^ buf);

			void GetFrame(cli::array<WORD>^ buf, cli::array<BYTE>^ headerInfo);

			// 暫定
			FrameHeader^ GetFrameAndHeader(cli::array<WORD>^ buf);

			// 撮影待機
			bool Wait(DWORD dwMilliseconds);

			// キャリブレーション完了待機
			bool WaitCalibration(DWORD dwMilliseconds);

			// FPD設定の登録
			void RegisterMode(RegisterationInfo^ registerationInfo);

			// キャリブレーションデータのステータス
			RefStatusInfo^ GetRefStatusInfo() { return gcnew RefStatusInfo(_RefStatus); }

			// ライブラリの初期化
			static void Initialize();

			// ライブラリの終了
			static void Deinitialize();
		};
	}
}
