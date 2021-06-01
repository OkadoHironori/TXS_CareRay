#include "stdafx.h"

#include "CareRayLib.h"

#pragma comment(lib, "CRInterface")

using namespace System::Runtime::InteropServices;

using namespace CareRay;

// 単独である前提で検出器のIndexを取得する
void Xs::CareRayLib::CRLib::GetDetectorNum()
{
	// 検出器の取得
	int num = 0;
	CR_DetrIdxAndIPAddr indexes[1];
	
	int ret = CR_GetDetectorIndexAndIPAddress(indexes, &num);

	if (0 != ret)
	{
		OnError(ret);
	}

	if(num > 0)
		detectorIndex = indexes[0].nIdx;
}


inline Xs::CareRayLib::SystemInfo ^ Xs::CareRayLib::CRLib::GetSystemInfomation()
{
	// 各種情報の取得
	CR_SystemInfo systemInfo;

	int ret = CR_GetSystemInformation(detectorIndex, &systemInfo);

	if (0 != ret)
	{
		OnError(ret);
	}

	return gcnew Xs::CareRayLib::SystemInfo(&systemInfo);
}

inline Xs::CareRayLib::DetectorStatus^ Xs::CareRayLib::CRLib::GetDetectorStatus()
{
	//検出器ステータスの取得
	CR_DetrStatus detrStatus;

	int ret = CR_GetDetrStatus(detectorIndex, &detrStatus);

	if (0 != ret)
	{
		OnError(ret);
	}

	return gcnew Xs::CareRayLib::DetectorStatus(&detrStatus);
}

inline void Xs::CareRayLib::CRLib::GetAppModes()
{
	CR_ModeInfo modeInfos[16];

	int num = 0;

	int ret = CR_GetApplicationMode(detectorIndex, modeInfos, &num);

	if (0 != ret)
	{
		OnError(ret);
	}

	_ModeInfos = gcnew cli::array<Xs::CareRayLib::ModeInfo^>(num);

	for (int i = 0; i < num; ++i)
	{
		_ModeInfos[i] = gcnew Xs::CareRayLib::ModeInfo(&modeInfos[i]);
	}
}

void Xs::CareRayLib::CRLib::GetConnectionStatus()
{
	//
	CR_ConnectionStatus connectionStatus;

	int ret = CR_GetConnectionStatus(detectorIndex, &connectionStatus);

	// 検出器との接続状態
	connectionStatus.isDetrConnected;

	// サーバーとの接続状態
	connectionStatus.isServerConnected;

	throw gcnew System::NotImplementedException();
}

void Xs::CareRayLib::CRLib::SetCalibrationPathroot(String^ path)
{
	if (nullptr == path || String::IsNullOrWhiteSpace(path))
	{
		throw gcnew NotImplementedException();
	}

	IntPtr p = Marshal::StringToHGlobalAnsi(path);
	try
	{
		const char* CalibrationPathroot = (const char*)p.ToPointer();

		CR_SetConfigItemValue(CR_DIR_TO_SAVE_CAL_FILES, CalibrationPathroot);
	}
	finally
	{
		Marshal::FreeHGlobal(p);
	}
}

inline int Xs::CareRayLib::CRLib::GetReturnCode(int code)
{
	return code / 100000;
}

inline String^ Xs::CareRayLib::CRLib::GetMessage(int code) 
{
	const int length = 256;
	
	char buf[length];

	CR_GetLastIntlMsg(buf, length, code);

	return gcnew String(buf);
}

unsigned int Xs::CareRayLib::CRLib::GetCorrectOption(ProcChain chain)
{
	unsigned int correctOption = 0;

	if (chain.HasFlag(ProcChain::SanityCheck))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_SANITYCHECK;

	if (chain.HasFlag(ProcChain::Offset))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_DARKCORR;

	if (chain.HasFlag(ProcChain::Gain))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_GAINCORR;

	if (chain.HasFlag(ProcChain::Defect))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_DEFECTCORR;

	if (chain.HasFlag(ProcChain::Lag))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_LAGCORR;

	if (chain.HasFlag(ProcChain::Cropping))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_IMGCROPPING;

	if (chain.HasFlag(ProcChain::RtPixel))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_RTPIXELCORR;

	if (chain.HasFlag(ProcChain::Denoise))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_DENOISING;

	if (chain.HasFlag(ProcChain::Gauss))
		correctOption |= CR_ProcChainOpt::CR_PROCCHAIN_GAUSS;

	return correctOption;
}

inline void Xs::CareRayLib::CRLib::OnError(int errorCode)
{
	String^ msg = GetMessage(errorCode);

	if (GetReturnCode(errorCode) <= CR_WARN)
		Console::WriteLine(msg);
	else
		throw gcnew Exception(msg);
}

void Xs::CareRayLib::CRLib::OnDetectorDisconnected(void)
{
	//C++/Cliはコンパイラが自動生成してくれるのでnullチェック不要（できない）。
	DetectorDisconnected(this, EventArgs::Empty);
}

void Xs::CareRayLib::CRLib::OnDetectorReconnected(void)
{
	DetectorReconnected(this, EventArgs::Empty);
}

void Xs::CareRayLib::CRLib::OnServerDisconnected(void)
{
	ServerDisconnected(this, EventArgs::Empty);
}

void Xs::CareRayLib::CRLib::OnServerReconnected(void)
{
	ServerReconnected(this, EventArgs::Empty);
}

void Xs::CareRayLib::CRLib::OnQueueBlocked(void)
{
	QueueBlocked(this, EventArgs::Empty);
}

Xs::CareRayLib::CalibrationInfo^ Xs::CareRayLib::CRLib::Calibration::get()
{
	MutexLockGuard lockGuard(mutex); //ブロックを抜けると自動でアンロック

	return gcnew CalibrationInfo(callback->calibrationInfo);
}

Xs::CareRayLib::AcquisitionStatInfo^ Xs::CareRayLib::CRLib::AcquisitionStat::get()
{
	return gcnew AcquisitionStatInfo(callback->acquisitionStatInfo);
}


Xs::CareRayLib::CRLib::CRLib()
{
	detectorIndex = 0;
	
	currentKey = 0;

	mutex = new Mutex();

	callback = new CR_Callback(mutex, this);
}

Xs::CareRayLib::CRLib::~CRLib() 
{
	// Dispose

	// メモリ破棄
	if (NULL != buffer) 
	{
		free(buffer);
		buffer = NULL;
	}

	if (NULL != mutex)
	{
		free(mutex);
		mutex = NULL;
	}
}

Xs::CareRayLib::CRLib::!CRLib()
{
}

// 初期設定
void Xs::CareRayLib::CRLib::Init(String^ rootpath, int bufferCount)
{
	//TODO:複数回呼ばれた場合の処理

	// Index取得
	GetDetectorNum();

	// 校正ファイルの出力先ディレクトリの設定
	SetCalibrationPathroot(rootpath);

	// 校正データフォルダ末尾にKey値を追加する
	CR_SetConfigItemValue("CalibFileNameSuffix", "1");

	// コールバック設定
	CR_RegisterEventCallback(detectorIndex, callback);

	// システム情報の取得
	SystemInfo = GetSystemInfomation();

	if(nullptr == SystemInfo)
		throw gcnew System::NotImplementedException();

	callback->SetHeaderLength(SystemInfo->FrameHeaderLength);

	// メモリ確保（画像サイズが最大）
	int frameSize = SystemInfo->RawImageWidth * SystemInfo->RawImageHeight * sizeof(WORD) + SystemInfo->FrameHeaderLength;

	buffer = (char*)malloc(frameSize * bufferCount);

	if (NULL == buffer)
		throw gcnew System::NotImplementedException();

	bufferLength = frameSize * bufferCount;
}

// 接続
void Xs::CareRayLib::CRLib::Connect()
{
	// 接続
	{
		int ret = CR_Connect(detectorIndex);

		// 失敗
		if (0 != ret)
		{
			OnError(ret);
		}
	}

	// 検出器のリセット
	{
		// 再起動なしリセット
		int ret = CR_ResetDetector(detectorIndex, false);

		// 失敗
		if (0 != ret)
		{
			OnError(ret);
		}
	}

	// AppMode取得
	GetAppModes();

	if (nullptr == ModeInfos)
		throw gcnew System::NotImplementedException();
}

// 接続解除
void Xs::CareRayLib::CRLib::Disconnect()
{
	// 接続解除
	int ret = CR_Disconnect(detectorIndex);

	// 失敗
	if (0 != ret)
	{
		OnError(ret);
	}

	// 後処理
}

// 撮影開始
void Xs::CareRayLib::CRLib::Acquire()
{
	// 校正切替？
	{
		int ret = CR_LoadReference(detectorIndex, currentKey);

		if (0 != ret)
		{
			OnError(ret);
		}
	}

	//撮影開始
	{
		int ret = CR_StartAcquisition(detectorIndex, currentKey, buffer, bufferLength);

		if (0 != ret)
		{
			OnError(ret);
		}
	}
}

// 撮影開始（校正あり）
void Xs::CareRayLib::CRLib::AcquireWithCorrect()
{
	// 校正切替？
	{
		int ret = CR_LoadReference(detectorIndex, currentKey);

		if (0 != ret)
		{
			OnError(ret);
		}
	}

	//撮影開始
	{
		unsigned int correctOption = GetCorrectOption(ProcChain::All);

		int ret = CR_StartAcquisitionWithCorrOpt(detectorIndex, currentKey, buffer, bufferLength, correctOption);

		if (0 != ret)
		{
			OnError(ret);
		}
	}
}

// 撮影停止
void Xs::CareRayLib::CRLib::Abort()
{
	//停止
	int ret = CR_StopAcquisition(detectorIndex);

	if (0 != ret)
	{
		OnError(ret);
	}
}

void Xs::CareRayLib::CRLib::AcquireOffset()
{
	AcquireOffset(false);
}

// オフセットデータ取得
void Xs::CareRayLib::CRLib::AcquireOffset(bool updateDefectMap)
{
	int ret = CR_StartDarkCalibration(detectorIndex, currentKey, true, updateDefectMap);

	if (0 != ret)
	{
		OnError(ret);
	}
	
	//QueryCalibrationStatus -> 監視
}

// ゲインデータ取得
void Xs::CareRayLib::CRLib::AcquireGain()
{
	int ret = CR_StartGainCalibration(detectorIndex, currentKey);

	if (0 != ret)
	{
		OnError(ret);
	}

	//QueryCalibrationStatus -> 監視
}

// キャリブレーション停止
void Xs::CareRayLib::CRLib::AbortCalibration()
{
	int ret = CR_StopCalibration(detectorIndex);

	if (0 != ret)
	{
		OnError(ret);
	}
}

// 校正データの再読み込み // Keyの切り替え
void Xs::CareRayLib::CRLib::Reload(int key)
{
	int ret = CR_LoadReference(detectorIndex, key);

	if (0 != ret)
	{
		OnError(ret);
	}

	currentKey = key;

	UpdateRefStatusInfo();
}

//キャリブレーションステータスの更新
void Xs::CareRayLib::CRLib::UpdateRefStatusInfo()
{
	// 校正ステータスの取得
	CR_RefStatusInfo statusInfo;

	int ret = CR_QueryReferenceStatus(detectorIndex, currentKey, &statusInfo);

	if (0 != ret)
	{
		OnError(ret);
	}

	_RefStatus = gcnew RefStatusInfo(&statusInfo);
}

// フレーム取得
void Xs::CareRayLib::CRLib::GetFrame(cli::array<WORD>^ buf)
{
	GetFrame(buf, nullptr);
}

void Xs::CareRayLib::CRLib::GetFrame(cli::array<WORD>^ buf, cli::array<BYTE>^ headerInfo) 
{
	int currentFrameIndex = 0;
	int frameSize = 0;
	{
		MutexLockGuard lockGuard(mutex);

		currentFrameIndex = callback->NewFrameIndex;

		frameSize = callback->FrameLength;
	}

	int headerSize = SystemInfo->FrameHeaderLength;

	char* src = buffer + currentFrameIndex * frameSize;

	if (nullptr != headerInfo) 
	{
		// ヘッダー情報の取得
		pin_ptr<BYTE> pHeader = &headerInfo[0];
		try
		{
			memcpy_s(pHeader, headerSize, src, headerSize);
		}
		finally
		{
			pHeader = nullptr;
		}
	}

	int dstSize = buf->Length * sizeof(WORD);

	src += headerSize;

	int srcSize = frameSize - headerSize;

	int minSize = Math::Min(srcSize, dstSize);

	pin_ptr<WORD> pBuffer = &buf[0];
	try
	{
		memcpy_s(pBuffer, minSize, src, minSize);
	}
	finally
	{
		pBuffer = nullptr;
	}
}

Xs::CareRayLib::FrameHeader^ Xs::CareRayLib::CRLib::GetFrameAndHeader(cli::array<WORD>^ buf)
{
	int currentFrameIndex = 0;
	int frameSize = 0;
	{
		MutexLockGuard lockGuard(mutex);

		currentFrameIndex = callback->NewFrameIndex;

		frameSize = callback->FrameLength;
	}

	int headerSize = SystemInfo->FrameHeaderLength;

	char* src = buffer + currentFrameIndex * frameSize;

	// ヘッダー情報の取得
	CR_FrameHeader header;
	memcpy(&header, src, sizeof(header));
	FrameHeader^ headerInfo = gcnew FrameHeader(&header);

	int dstSize = buf->Length * sizeof(WORD);

	src += headerSize;

	int srcSize = frameSize - headerSize;

	int minSize = Math::Min(srcSize, dstSize);

	pin_ptr<WORD> pBuffer = &buf[0];
	try
	{
		memcpy_s(pBuffer, minSize, src, minSize);
	}
	finally
	{
		pBuffer = nullptr;
	}

	return headerInfo;
}

// 撮影待機
bool Xs::CareRayLib::CRLib::Wait(DWORD dwMilliseconds)
{
	DWORD ret = callback->WaitNewFrame(dwMilliseconds);

	if (ret == WAIT_OBJECT_0)
	{
		//OK
		return true;
	}
	else if (ret == WAIT_TIMEOUT)
	{
		//タイムアウト
		return false;
	}
	else
	{
		//エラー
		throw gcnew System::NotImplementedException();
	}
}

bool Xs::CareRayLib::CRLib::WaitCalibration(DWORD dwMilliseconds)
{
	DWORD ret = callback->WaitCalibrationFinished(dwMilliseconds);

	if (ret == WAIT_OBJECT_0)
	{
		//OK
		return true;
	}
	else if (ret == WAIT_TIMEOUT)
	{
		//タイムアウト
		return false;
	}
	else
	{
		//エラー
		throw gcnew System::NotImplementedException();
	}
}

// FPD設定の登録
void Xs::CareRayLib::CRLib::RegisterMode(RegisterationInfo^ registerationInfo)
{
	if (nullptr == registerationInfo)
	{
		throw gcnew NullReferenceException();
	}

	//優先するパラメーターのみ使用する。使用しないパラメーターは０にする。
	float fps = (registerationInfo->Priority == FpsPriority::ExposureTime) ? 0 : registerationInfo->FrameRate;
	float exp = (registerationInfo->Priority == FpsPriority::ExposureTime) ? registerationInfo->ExposureTime : 0;

	int ret = CR_RegisterApplicationMode(detectorIndex,
		registerationInfo->AppModeKey,
		registerationInfo->AppModeID,
		&fps,
		&exp,
		(CR_TrigTypeOpt)registerationInfo->TriggerType,
		(CR_GainLevelOpt)registerationInfo->GainID);

	if (0 != ret)
	{
		OnError(ret);
	}
	else
	{
		// 実際に適用されるフレームレート、露光時間を取得する
		registerationInfo->FrameRate = fps;
		registerationInfo->ExposureTime = exp;
	}
}

// 撮影した画像データの取得
void Xs::CareRayLib::CRLib::Initialize()
{
	// ライブラリの初期化。CR_Server.exeの起動？
	int ret = CR_InitializeLibrary();

	if (0 != ret)
	{
		OnError(ret);
	}
}

void Xs::CareRayLib::CRLib::Deinitialize()
{
	// ライブラリの非初期化。CR_Server.exeの終了？
	int ret = CR_DeinitializeLibrary();

	if (0 != ret)
	{
		OnError(ret);
	}
}
