/**
* \file               CCareRayAPIDemo.h
* \date             2017/11/07
* \version         5.2.2
* \author          Shuai Li
* \brief             CareRay API demo header file
*  Copyright 2017 CARERAY LTD. All Rights Reserved. 
*/

#ifndef __CARERAY_API_DEMO_H__
#define __CARERAY_API_DEMO_H__
#include "stdafx.h"
#include <iostream>
#include <tuple>
#include <algorithm>
#include "CallbackImp.h"
#include "IntlMsgAliasID.h"
#include <map>
#include <set>

#if (defined WIN32 || defined _WIN32 || defined _W64 || defined WINCE)
#include <Windows.h>
#else
int _kbhit();
char _getch();
#endif

using namespace std;

#define RAD_HEADER_SIZE 65536
inline int GetReturnCode(int nCode) { return nCode / 100000;}

/** 
* \class CCareRayAPIDemo
* \brief methods  and variables
*/
class CCareRayAPIDemo
{
public:

	CCareRayAPIDemo();
	~CCareRayAPIDemo();

	//<<ModeID, FrameRate, ExpTime, TrigType, GainLevel>, IsFluoroMode, RefStatus>
	typedef tuple<tuple<int, float, float, int, int>, bool, string> TUPLEOFAPPMODEKEY;

	/**
	* @brief initialize detector (connect)
	*/
	int Initialize();

	/**
	* @brief disconnect detector
	*/
	int DeInitialize();

	int InitializeLibrary();

	int DeInitializeLibrary();

	/**
	* @brief select application mode and set to detector
	*/
	int RegisterAppModes();
	
	/**
	* @brief perform offset calibration
	*/
	int GetConnectionStatus();

	/**
	* @brief calibration detector, include offset and gain calibration by interrupt way
	*/
/*	void CalibrationUsingInterrupts();*/

	/**
	* @brief fluoroscopy images acquisition process
	*/
	int Acquisition();

	int AcqAllDetrImgs();
	
	//int ObtainSpotFilm();

	int AcquireRadImage();

	int AcquireDarkImage();

	void PrintDetrInfo(int nDetrIdx);

	void PrintModeList(int nDetrIdx);

	bool IsRadiographicModeRegistered();

	int GetDaecParams();
	int SetDaecParams();

	void SetCurrDetrIdx(int nDetrIdx)
	{
		m_nCurrentDetrIndex = nDetrIdx;
		//m_pCallback = m_mapCallbackPtrs[nDetrIdx];
	}
	
	int GetCurrDetrIdx()
	{
		return m_nCurrentDetrIndex;
	}

	void GetDetrIdxs(vector<int> *pDetrIdxs)
	{
		for(map<int, string>::iterator iter = m_mapDetrIdxnIPAddr.begin(); iter != m_mapDetrIdxnIPAddr.end(); ++iter)
		{
			pDetrIdxs->push_back(iter->first);
		}
	}

	void PrintDetrIpIdx()
	{
		for(map<int, string>::iterator iter = m_mapDetrIdxnIPAddr.begin(); iter != m_mapDetrIdxnIPAddr.end(); ++iter)
		{
			cout << "Detector index " << iter->first << ", IP " << iter->second << endl;
		}
	}

	template <typename T>
	void checkInput(T & tInput)
	{
		while (cin.fail())
		{
			cin.clear();
			cin.ignore();
			cout << "Invalid input, enter again" << endl << "Your input >> ";
			cin >> tInput;
		}
	};

private:
	struct ModeParams 
	{
		int nModeID;
		int nTrigType;
		int nGainID;
		float fExpTime;
		int nSpotFilmNum;
		float fFrameRate;
	};

private:

/*	bool hIsRadandSyncSupported(CR_ModeInfo* szModeInfo, int nModeNum, int& nModeIdx, int nSyncType = -1);*/
		/**
	* @brief perform offset calibration
	*/
	int DoOffsetCalibration(int nAppModeKey);

	/**
	* @brief perform gain calibration
	*/
	int DoGainCalibration(pair<int, bool> pairCalType);

	/**
	* @brief calibration detector, include offset and gain calibration by polling way
	*/
	int Calibration(vector<pair<int, bool>> vecAppModeKeys);

	int ModifyAppMode(bool bAddNewMode);

	void DisplayAppModesInfo(map<int, TUPLEOFAPPMODEKEY>* mapAppModes, bool bDisplayFluoro, bool bDisplayRad);

	string GetReferenceFilesStatus(int nRefStatus);

	void FreeVariants();

	/*	static int CalibrationThread(CCallbackImp* pCallbackImp);*/
	//static int GetReturnCode(int nCode) { return nCode / 100000; };
	//CCallbackImp* m_pCallback;
	//vector<ModeInfo> m_vecModeInfo;
	//CR_ModeInfo m_oModeInfos[16];
	//int m_nModeNum;
	//unsigned short* m_pFrmBuf;
	int m_nCurrentDetrIndex; // current Detector Index used, this property is reserved until there are more than one detectors.

	map<int, unsigned short*> m_mapFrmBuf;
	map<int, int> m_mapModeNum;
	map<int, string> m_mapDetrIdxnIPAddr;
	map<int, CCallbackImp*> m_mapCallbackPtrs;
	map<int, CR_ModeInfo> m_mapActiveFluoroModes;
	map<int, CR_ModeInfo*> m_mapModeInfos;
	map<int, CR_SystemInfo> m_mapSystemInfos;
	map<int, map<int, TUPLEOFAPPMODEKEY>> m_mapRegisteredAppModes;
	//<detector id, <app mode key, is fluoro mode>>
	map<int, vector<pair<int, bool>>> m_mapNeedToCalibrateAppModes;
};

#endif