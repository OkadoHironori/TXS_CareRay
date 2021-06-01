#pragma once

#include <Windows.h>

#include "stdafx.h"

// CareRay画像フレームヘッダー 256byte
public struct CR_FrameHeader
{
	// length of header
	unsigned short HeaderLength;

	// flag of spot frame
	unsigned char IsSpotFrame;

	unsigned char HeaderReserved[5];

	// current frame ID
	unsigned int FrameID;

	// seconds of FPGA clock
	unsigned int RealAcqSec;

	// absolute fetch time. unit: ms
	unsigned long long AbsAcqTime;

	// bytes of a block in image
	unsigned int BlockLength;

	// average temperature
	float AverageTemperature;

	// maximum temperature
	float MaxTemperature;

	// vcmp
	float VCmp;

	// vbias
	float VBias;

	// gain value
	unsigned char GainValue;

	// ID of application mode
	unsigned char ModeID;

	unsigned char DetectorSanityCheck;

	unsigned char FirmwareReserved[1];

	// absolute sending time at detector. unit: ms
	unsigned long long DetectroSendTime;

	unsigned char DetectorReserved[115];

	// index of connected detectors in Server
	unsigned char DetectorIndex;

	// pre-processing executed in Server
	unsigned int ImagePreproc;

	// absolute receiving time at Server. unit: ms
	unsigned long long ServerReceiveTime;

	// absolute sending time at detector. unit: ms
	unsigned long long ServerSendTime;

	// absolute receiving time at API. unit: ms
	unsigned long long APIReceiveTime;

	// absolute receiving time at APP/DROC. unit: ms
	unsigned long long APPReceiveTime;

	unsigned char ServerReserved[44];

	unsigned int CRC32;
};