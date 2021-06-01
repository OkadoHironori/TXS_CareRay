#include "stdafx.h"

#include "CareRayLib.h"

#pragma comment(lib, "CRInterface")

using namespace System::Runtime::InteropServices;

using namespace CareRay;

// �P�Ƃł���O��Ō��o���Index���擾����
void Xs::CareRayLib::CRLib::GetDetectorNum()
{
	// ���o��̎擾
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
	// �e����̎擾
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
	//���o��X�e�[�^�X�̎擾
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

	// ���o��Ƃ̐ڑ����
	connectionStatus.isDetrConnected;

	// �T�[�o�[�Ƃ̐ڑ����
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
	//C++/Cli�̓R���p�C���������������Ă����̂�null�`�F�b�N�s�v�i�ł��Ȃ��j�B
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
	MutexLockGuard lockGuard(mutex); //�u���b�N�𔲂���Ǝ����ŃA�����b�N

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

	// �������j��
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

// �����ݒ�
void Xs::CareRayLib::CRLib::Init(String^ rootpath, int bufferCount)
{
	//TODO:������Ă΂ꂽ�ꍇ�̏���

	// Index�擾
	GetDetectorNum();

	// �Z���t�@�C���̏o�͐�f�B���N�g���̐ݒ�
	SetCalibrationPathroot(rootpath);

	// �Z���f�[�^�t�H���_������Key�l��ǉ�����
	CR_SetConfigItemValue("CalibFileNameSuffix", "1");

	// �R�[���o�b�N�ݒ�
	CR_RegisterEventCallback(detectorIndex, callback);

	// �V�X�e�����̎擾
	SystemInfo = GetSystemInfomation();

	if(nullptr == SystemInfo)
		throw gcnew System::NotImplementedException();

	callback->SetHeaderLength(SystemInfo->FrameHeaderLength);

	// �������m�ہi�摜�T�C�Y���ő�j
	int frameSize = SystemInfo->RawImageWidth * SystemInfo->RawImageHeight * sizeof(WORD) + SystemInfo->FrameHeaderLength;

	buffer = (char*)malloc(frameSize * bufferCount);

	if (NULL == buffer)
		throw gcnew System::NotImplementedException();

	bufferLength = frameSize * bufferCount;
}

// �ڑ�
void Xs::CareRayLib::CRLib::Connect()
{
	// �ڑ�
	{
		int ret = CR_Connect(detectorIndex);

		// ���s
		if (0 != ret)
		{
			OnError(ret);
		}
	}

	// ���o��̃��Z�b�g
	{
		// �ċN���Ȃ����Z�b�g
		int ret = CR_ResetDetector(detectorIndex, false);

		// ���s
		if (0 != ret)
		{
			OnError(ret);
		}
	}

	// AppMode�擾
	GetAppModes();

	if (nullptr == ModeInfos)
		throw gcnew System::NotImplementedException();
}

// �ڑ�����
void Xs::CareRayLib::CRLib::Disconnect()
{
	// �ڑ�����
	int ret = CR_Disconnect(detectorIndex);

	// ���s
	if (0 != ret)
	{
		OnError(ret);
	}

	// �㏈��
}

// �B�e�J�n
void Xs::CareRayLib::CRLib::Acquire()
{
	// �Z���ؑցH
	{
		int ret = CR_LoadReference(detectorIndex, currentKey);

		if (0 != ret)
		{
			OnError(ret);
		}
	}

	//�B�e�J�n
	{
		int ret = CR_StartAcquisition(detectorIndex, currentKey, buffer, bufferLength);

		if (0 != ret)
		{
			OnError(ret);
		}
	}
}

// �B�e�J�n�i�Z������j
void Xs::CareRayLib::CRLib::AcquireWithCorrect()
{
	// �Z���ؑցH
	{
		int ret = CR_LoadReference(detectorIndex, currentKey);

		if (0 != ret)
		{
			OnError(ret);
		}
	}

	//�B�e�J�n
	{
		unsigned int correctOption = GetCorrectOption(ProcChain::All);

		int ret = CR_StartAcquisitionWithCorrOpt(detectorIndex, currentKey, buffer, bufferLength, correctOption);

		if (0 != ret)
		{
			OnError(ret);
		}
	}
}

// �B�e��~
void Xs::CareRayLib::CRLib::Abort()
{
	//��~
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

// �I�t�Z�b�g�f�[�^�擾
void Xs::CareRayLib::CRLib::AcquireOffset(bool updateDefectMap)
{
	int ret = CR_StartDarkCalibration(detectorIndex, currentKey, true, updateDefectMap);

	if (0 != ret)
	{
		OnError(ret);
	}
	
	//QueryCalibrationStatus -> �Ď�
}

// �Q�C���f�[�^�擾
void Xs::CareRayLib::CRLib::AcquireGain()
{
	int ret = CR_StartGainCalibration(detectorIndex, currentKey);

	if (0 != ret)
	{
		OnError(ret);
	}

	//QueryCalibrationStatus -> �Ď�
}

// �L�����u���[�V������~
void Xs::CareRayLib::CRLib::AbortCalibration()
{
	int ret = CR_StopCalibration(detectorIndex);

	if (0 != ret)
	{
		OnError(ret);
	}
}

// �Z���f�[�^�̍ēǂݍ��� // Key�̐؂�ւ�
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

//�L�����u���[�V�����X�e�[�^�X�̍X�V
void Xs::CareRayLib::CRLib::UpdateRefStatusInfo()
{
	// �Z���X�e�[�^�X�̎擾
	CR_RefStatusInfo statusInfo;

	int ret = CR_QueryReferenceStatus(detectorIndex, currentKey, &statusInfo);

	if (0 != ret)
	{
		OnError(ret);
	}

	_RefStatus = gcnew RefStatusInfo(&statusInfo);
}

// �t���[���擾
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
		// �w�b�_�[���̎擾
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

	// �w�b�_�[���̎擾
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

// �B�e�ҋ@
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
		//�^�C���A�E�g
		return false;
	}
	else
	{
		//�G���[
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
		//�^�C���A�E�g
		return false;
	}
	else
	{
		//�G���[
		throw gcnew System::NotImplementedException();
	}
}

// FPD�ݒ�̓o�^
void Xs::CareRayLib::CRLib::RegisterMode(RegisterationInfo^ registerationInfo)
{
	if (nullptr == registerationInfo)
	{
		throw gcnew NullReferenceException();
	}

	//�D�悷��p�����[�^�[�̂ݎg�p����B�g�p���Ȃ��p�����[�^�[�͂O�ɂ���B
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
		// ���ۂɓK�p�����t���[�����[�g�A�I�����Ԃ��擾����
		registerationInfo->FrameRate = fps;
		registerationInfo->ExposureTime = exp;
	}
}

// �B�e�����摜�f�[�^�̎擾
void Xs::CareRayLib::CRLib::Initialize()
{
	// ���C�u�����̏������BCR_Server.exe�̋N���H
	int ret = CR_InitializeLibrary();

	if (0 != ret)
	{
		OnError(ret);
	}
}

void Xs::CareRayLib::CRLib::Deinitialize()
{
	// ���C�u�����̔񏉊����BCR_Server.exe�̏I���H
	int ret = CR_DeinitializeLibrary();

	if (0 != ret)
	{
		OnError(ret);
	}
}
