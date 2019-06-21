#include "stdafx.h"

#include "RzReciver.h"

//---------------------------------------------------------------------------
#include "CRZUSB.h"
#include "SysFunc.h"
#include "strcFrcIRP.h"
#include <winnt.h>
#include <minwindef.h>
#include <debugapi.h>
//---------------------------------------------------------------------------

//#define DEBUG_RZUSB_DLL		

#ifdef DEBUG_RZUSB_DLL
	#define DELAY_DUBUG_RC_RZ_STREAM_EX			100
#endif

//#define DEBUG_IN_OUT_FILE

//---------------------------------------------------------------------------
#define SIZE_BUFFER_CMD							16
#define SIZE_BUFFER_CMD_EX					64
#define SIZE_BUFFER_MAIN						512
#define SIZE_MATR_ADDR_RECV					512
#define SIZE_DATA_RZ								592
#define SIZE_MATRIX_RZ_CHANNELS			64
#define SIZE_MATRIX_RZ_DATA					512
#define SIZE_MATRIX_RZ_TIME					128
#define SIZE_DATA_RZ_WITH_TIME_DATA	(SIZE_MATRIX_RZ_DATA + SIZE_MATRIX_RZ_CHANNELS + SIZE_MATRIX_RZ_TIME)


#define SIZE_RZ_MESSAGE								4
#define OFFSET_RZ_MESSAGE_ADDRESS			0
#define OFFSET_RZ_MESSAGE_LOW_DATA		1
#define OFFSET_RZ_MESSAGE_HIGH_DATA		2
#define OFFSET_RZ_MESSAGE_ADDITIONAL	3

#define OFFSET_MATRIX_RZ_CHANNELS		SIZE_MATRIX_RZ_DATA
#define OFFSET_ADD_DATA						(OFFSET_MATRIX_RZ_CHANNELS + SIZE_MATRIX_RZ_CHANNELS)

#define TIMEOUT_READ							500
#define TIMEOUT_WRITE							500

#define STATE_OK				0x00
#define STATE_ERROR			0x01

#define CMD_INIT_DEVICE	0x01
#define CMD_START_RZ		0x02
#define CMD_STOP_RZ			0xFE

#define BYTE_CMD				0x00
#define BYTE_BR					0x01
#define BYTE_FR					0x02
#define BYTE_CP					0x03
#define BYTE_EMAR				0x04

#define BYTE_IDDEV			0x01
#define BYTE_FREQ				0x02
#define BYTE_DENS0			0x03
#define BYTE_DENS1			0x04
#define BYTE_ZND0				0x05
#define BYTE_ZND1				0x06
#define BYTE_NPO0				0x07
#define BYTE_NPO1				0x08
#define BYTE_VPO0				0x09
#define BYTE_VPO1				0x0A
#define BYTE_CCH				0x0B
#define BYTE_ERR_D			0x0E
#define BYTE_STATE			0x0F

#define BYTE_CRZP				0x01
#define BYTE_TR0				0x02
#define BYTE_TR1				0x03
#define BYTE_PCERR			0x04
#define BYTE_PC0				0x05
#define BYTE_PC1				0x06

//---------------------------------------------------------------------------
int CountRZMessageC;
using namespace System;
using namespace System::Reflection;
unsigned long ulTimeOutRead  = 10000; //тайм-аут чтени€ 10с. 
unsigned long ulTimeOutWrite = 10000; //тайм-аут записи 10с. 
 
sInitDeviceEx _sInitDeviceEx;
sInfoDeviceEx _sInfoDeviceEx;
sRZStreamEx _sRZStreamEx;
BOOL MainInitDevi()
{
	RzUsb_SetReadTimeout(ulTimeOutRead); 
	RzUsb_SetWriteTimeout(ulTimeOutWrite); 
	_sInitDeviceEx.ParamChannel[0].FreqRZ = FREQ_RZ_100kHZ; //частота потока100к√ц
	_sInitDeviceEx.ParamChannel[0].FormatRZ = FORMAT_RZ_OSN; //основной формат приЄма
	_sInitDeviceEx.ParamChannel[0].CtrlParr = CTRL_PARR_PARITY; //контроль на чЄтность
	_sInitDeviceEx.AddrRecv  = DISABLE_ADDR_REC; 
	return RzUsb_InitDeviceEx(&_sInitDeviceEx, &_sInfoDeviceEx);
}

BOOL MainRzUsb_ReceiveRZStreamEx()
{
	return RzUsb_ReceiveRZStreamEx(&_sRZStreamEx);
}
//
//namespace StatusRZ
//{
//	public ref class RzReciverStatus
//	{
//	public:
//		int countRZMessage;
//	public:
//		
//
//	public:
//		 bool RzUsb_ConnectDeviceC()
//		{
//			return RzUsb_ConnectDevice();
//		}
//
//	public:
//		 bool RzUsb_IsConnectToDeviceC()
//		{
//			return RzUsb_IsConnectToDevice();
//		}
//
//	public:
//		bool RzUsb_DisconnectDeviceC()
//		{
//			return RzUsb_DisconnectDevice();
//		}
//
//	public:
//		bool RzUsb_InitDeviceExC()
//		{
//			return MainInitDevi();
//		}
//
//	public:
//		void RzUsb_SetReadTimeoutC(int k)
//		{
//			RzUsb_SetReadTimeout(k);
//		}
//
//	public :
//		void RzUsb_SetWriteTimeoutC(int k)
//		{
//			RzUsb_SetWriteTimeout(k);
//		}
//
//	public :
//		bool RzUsb_RunReceiveRZC()
//		{
//			return RzUsb_RunReceiveRZ();
//		}
//
//	public :
//		bool RzUsb_StopReceiveRZC()
//		{
//			return RzUsb_StopReceiveRZ();
//		}
//	public :
//		Collections::Generic::List<int>^ RzUsb_ReceiveRZStreamExC()
//		{
//			Collections::Generic::List<int> ^ListMessage = gcnew Collections::Generic::List<int>();
//			if (MainRzUsb_ReceiveRZStreamEx())
//			{
//				
//				countRZMessage = _sRZStreamEx.CountRZMessage;
//				for (int i=0;i<_sRZStreamEx.CountRZMessage;i++)
//				{
//					ListMessage->Add(_sRZStreamEx.RZ_DATA[i].Address | (_sRZStreamEx.RZ_DATA[i].Low << 8) |
//                                               (_sRZStreamEx.RZ_DATA[i].High << 16) |
//                                               (_sRZStreamEx.RZ_DATA[i].Additional << 24));
//				}
//			}
//			return ListMessage;
//		}
//	
//	};
//}

#pragma unmanaged
cFrcIRP IRP;

unsigned char* pucBufferMainData = NULL;
unsigned char* pucBufferRZData = NULL;
bool bSTOP_TRANSFER = false;
bool bIsReceiveRZStream = false;
bool bIsSupportTimingData = false;
sRZTimingData RZ_TIME_DATA;

#ifdef DEBUG_IN_OUT_FILE
	HANDLE hDebugRZUSB = INVALID_HANDLE_VALUE;
	unsigned long ulCountWriteBytes = 0;
#endif

//---------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

#ifdef DEBUG_IN_OUT_FILE
			hDebugRZUSB = CreateFile("DEBUG_RZUSB.TXT", 
											 GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
																	FILE_ATTRIBUTE_NORMAL, 0);
#endif

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:

#ifdef DEBUG_IN_OUT_FILE
			CloseHandle(hDebugRZUSB);
#endif

		break;
	}
	return TRUE;
}

//---------------------------------------------------------------------------
bool RzUsb_ConnectDevice(void)
{
#ifdef DEBUG_RZUSB_DLL
	return true;
#endif

	if (ConnectDevice() == SUCCEFULY_CONNECTING)
	{
		if (!IRP.SetSizeFrcIRP(SIZE_BUFFER_CMD)) return false;

		pucBufferMainData = new unsigned char[SIZE_BUFFER_MAIN];
		pucBufferRZData = new unsigned char[SIZE_DATA_RZ];

		if (!pucBufferMainData || !pucBufferRZData) return false;

		SetReadTimeout(TIMEOUT_READ);
		SetWriteTimeout(TIMEOUT_WRITE);

		bSTOP_TRANSFER = false;

		return true;
	}
	else
	{
		return false;
	}
}

//---------------------------------------------------------------------------
bool RzUsb_IsConnectToDevice(void)
{
#ifdef DEBUG_RZUSB_DLL
	return true;
#endif

	return IsConnectToDevice();
}

//---------------------------------------------------------------------------
bool RzUsb_DisconnectDevice(void)
{
	IRP.ClearMem();

	bSTOP_TRANSFER = false;

	return DisconnectDevice();
}

//---------------------------------------------------------------------------
void RzUsb_SetReadTimeout(unsigned long Value)
{
	SetReadTimeout(Value);
}

//---------------------------------------------------------------------------
void RzUsb_SetWriteTimeout(unsigned long Value)
{
	SetWriteTimeout(Value);
}

//---------------------------------------------------------------------------
bool RzUsb_InitDevice(IN sInitDevice* InitData, OUT sInfoDevice* InfoDevice)
{
	return false;

#ifdef DEBUG_RZUSB_DLL
	InfoDevice->ucIDDevice	= 0x01;
	InfoDevice->ucFreqRZ		= 100;
	InfoDevice->usDensityRZ = 0;
	InfoDevice->usPlantNum	= 0;
	InfoDevice->usNumPO		= 0;
	InfoDevice->usVersionPO = 0;
	InfoDevice->ucCountChannels = 2;
	InfoDevice->bCorrectInfo = true;
	return true;
#endif

	unsigned long nWritten = 0;
	unsigned long nReading = 0;
	char* pucTEMP_BUFFER = new char[SIZE_BUFFER_CMD + SIZE_MATR_ADDR_RECV];

	if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3()) return false;

	IRP.ClearFrcIRP();
	IRP[BYTE_CMD] = CMD_INIT_DEVICE;
	IRP[BYTE_BR] = InitData->ucFreqRZ;
	IRP[BYTE_FR] = InitData->ucFormatRZ;
	IRP[BYTE_CP] = InitData->ucCtrlParr;
	IRP[BYTE_EMAR] = InitData->ucAddrRecv;

	/*CopyMemory(pucTEMP_BUFFER, IRP.GetPtrFrcIRP(), SIZE_BUFFER_CMD);
	CopyMemory(pucTEMP_BUFFER + SIZE_BUFFER_CMD, InitData->pucMatrAddrRecv, SIZE_MATR_ADDR_RECV);

	nWritten = WriteToDevice(pucTEMP_BUFFER, SIZE_BUFFER_CMD + SIZE_MATR_ADDR_RECV);

	if (nWritten != SIZE_BUFFER_CMD + SIZE_MATR_ADDR_RECV)	return false;*/

	nWritten = WriteToDevice((char*)IRP.GetPtrFrcIRP(), IRP.GetSizeFrcIRP());

	if (nWritten != IRP.GetSizeFrcIRP()) return false;

	nWritten = WriteToDevice((char*)InitData->pucMatrAddrRecv, SIZE_MATR_ADDR_RECV);

	if (nWritten != SIZE_MATR_ADDR_RECV) return false;


	nReading = ReadFromDevice((char*)IRP.GetPtrFrcIRP(), IRP.GetSizeFrcIRP());

	if (nReading != IRP.GetSizeFrcIRP()) return false;

	InfoDevice->bCorrectInfo = false;

	InfoDevice->usDensityRZ = InfoDevice->usNumPO = InfoDevice->usPlantNum =
		InfoDevice->usVersionPO = InfoDevice->ucFreqRZ = InfoDevice->ucIDDevice =
		InfoDevice->ucCodeErrorDevice = InfoDevice->ucCountChannels = 0;

	if (IRP[BYTE_CMD] != CMD_INIT_DEVICE) return false;

	if (IRP[BYTE_STATE] == STATE_ERROR)
	{
		InfoDevice->ucCodeErrorDevice = IRP[BYTE_ERR_D];
		return false;
	}

	InfoDevice->ucIDDevice = IRP[BYTE_IDDEV];
	InfoDevice->ucFreqRZ = IRP[BYTE_FREQ];
	InfoDevice->usDensityRZ = (((unsigned short)IRP[BYTE_DENS1]) << 8) | ((unsigned short)IRP[BYTE_DENS0]);
	InfoDevice->usPlantNum = (((unsigned short)IRP[BYTE_ZND1]) << 8) | ((unsigned short)IRP[BYTE_ZND0]);
	InfoDevice->usNumPO = (((unsigned short)IRP[BYTE_NPO1]) << 8) | ((unsigned short)IRP[BYTE_NPO0]);
	InfoDevice->usVersionPO = (((unsigned short)IRP[BYTE_VPO1]) << 8) | ((unsigned short)IRP[BYTE_VPO0]);
	InfoDevice->ucCountChannels = IRP[BYTE_CCH];
	InfoDevice->bCorrectInfo = true;

	return true;
}

//---------------------------------------------------------------------------
bool RzUsb_StopReceiveRZ(void)
{
#ifdef DEBUG_RZUSB_DLL
	return true;
#endif

	bSTOP_TRANSFER = true;
	bIsReceiveRZStream = false;

	OutputDebugString("CRZUSB: >> Start RzUsb_StopReceiveRZ");

	if (RzUsb_RunReceiveRZ())
	{
		SetStateInit();
		bSTOP_TRANSFER = false;
		OutputDebugString("CRZUSB: >> Exit: if(RzUsb_RunReceiveRZ()): RzUsb_StopReceiveRZ");
		return true;
	}
	else
	{
		SetStateRun();
		bSTOP_TRANSFER = false;
		OutputDebugString("CRZUSB: >> Exit: if(RzUsb_RunReceiveRZ()):Else: RzUsb_StopReceiveRZ");
		return false;
	}
}

//---------------------------------------------------------------------------
bool RzUsb_RunReceiveRZ(void)
{
#ifdef DEBUG_RZUSB_DLL
	return true;
#endif

	unsigned long nWritten = 0;
	unsigned long nReading = 0;
	unsigned long ulCountReadStop = 10;

	bIsReceiveRZStream = false;

	if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3()) return false;

	IRP.ClearFrcIRP();
	IRP[BYTE_CMD] = (bSTOP_TRANSFER) ? CMD_STOP_RZ : CMD_START_RZ;

	nWritten = WriteToDevice((char*)IRP.GetPtrFrcIRP(), IRP.GetSizeFrcIRP());

	if (nWritten != IRP.GetSizeFrcIRP())
	{
		if (IRP[BYTE_CMD] == CMD_STOP_RZ)
			return true;
		else
			return false;
	}

	if (IRP[BYTE_CMD] == CMD_STOP_RZ)
	{
		do
		{
			if (!(ulCountReadStop--)) return false;

			if (RzUsb_IsSupportTimingData())
			{
				BYTE tmpBuff[SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD];

				nReading = ReadFromDevice((char*)tmpBuff, SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD);

				if (nReading != SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD) return false;

				CopyMemory(IRP.GetPtrFrcIRP(), (tmpBuff + SIZE_DATA_RZ_WITH_TIME_DATA), SIZE_BUFFER_CMD);
			}
			else
			{
				nReading = ReadFromDevice((char*)pucBufferRZData, SIZE_DATA_RZ);

				if (nReading != SIZE_DATA_RZ) return false;

				CopyMemory(IRP.GetPtrFrcIRP(), (pucBufferRZData + OFFSET_ADD_DATA), SIZE_BUFFER_CMD);
			}
		}
		while (IRP[BYTE_CMD] != CMD_STOP_RZ);

		return true;
	}

	bIsReceiveRZStream = true;
	SetStateRun();

	return true;
}

//---------------------------------------------------------------------------
bool RzUsb_ReceiveRZStream(sRZStream* RZ_DATA)
{
	return false;

#ifdef DEBUG_RZUSB_DLL
	RZ_DATA->CountRZMessage				= 10;
	RZ_DATA->CountTrashRZMessage		= 0;
	RZ_DATA->IsMissingRZMessage		= false; 
	RZ_DATA->CountMissingRZMessage	= 0;

	RZ_DATA->RZ_DATA[0].Address		= 0x00;
	RZ_DATA->RZ_DATA[0].Low				= 0x45;
	RZ_DATA->RZ_DATA[0].High			= 0x72;
	RZ_DATA->RZ_DATA[0].Additional	= 0x78;
	RZ_DATA->RZ_DATA[0].Channel		= 0;

	RZ_DATA->RZ_DATA[1].Address		= 0x02;
	RZ_DATA->RZ_DATA[1].Low				= 0x45;
	RZ_DATA->RZ_DATA[1].High			= 0x34;
	RZ_DATA->RZ_DATA[1].Additional	= 0x78;
	RZ_DATA->RZ_DATA[1].Channel		= 1;

	RZ_DATA->RZ_DATA[2].Address		= 0x04;
	RZ_DATA->RZ_DATA[2].Low				= 0x45;
	RZ_DATA->RZ_DATA[2].High			= 0x12;
	RZ_DATA->RZ_DATA[2].Additional	= 0x12;
	RZ_DATA->RZ_DATA[2].Channel		= 0;

	RZ_DATA->RZ_DATA[3].Address		= 0x05;
	RZ_DATA->RZ_DATA[3].Low				= 0x45;
	RZ_DATA->RZ_DATA[3].High			= 0x12;
	RZ_DATA->RZ_DATA[3].Additional	= 0x78;
	RZ_DATA->RZ_DATA[3].Channel		= 0;

	RZ_DATA->RZ_DATA[4].Address		= 0x06;
	RZ_DATA->RZ_DATA[4].Low				= 0x45;
	RZ_DATA->RZ_DATA[4].High			= 0x00;
	RZ_DATA->RZ_DATA[4].Additional	= 0x78;
	RZ_DATA->RZ_DATA[4].Channel		= 1;

	RZ_DATA->RZ_DATA[5].Address		= 0x08;
	RZ_DATA->RZ_DATA[5].Low				= 0xFF;
	RZ_DATA->RZ_DATA[5].High			= 0x12;
	RZ_DATA->RZ_DATA[5].Additional	= 0x78;
	RZ_DATA->RZ_DATA[5].Channel		= 0;

	RZ_DATA->RZ_DATA[6].Address		= 0x12;
	RZ_DATA->RZ_DATA[6].Low				= 0xCE;
	RZ_DATA->RZ_DATA[6].High			= 0x12;
	RZ_DATA->RZ_DATA[6].Additional	= 0x78;
	RZ_DATA->RZ_DATA[6].Channel		= 1;

	RZ_DATA->RZ_DATA[7].Address		= 0x0A;
	RZ_DATA->RZ_DATA[7].Low				= 0x48;
	RZ_DATA->RZ_DATA[7].High			= 0x99;
	RZ_DATA->RZ_DATA[7].Additional	= 0x78;
	RZ_DATA->RZ_DATA[7].Channel		= 0;

	RZ_DATA->RZ_DATA[8].Address		= 0x0B;
	RZ_DATA->RZ_DATA[8].Low				= 0x45;
	RZ_DATA->RZ_DATA[8].High			= 0x66;
	RZ_DATA->RZ_DATA[8].Additional	= 0x78;
	RZ_DATA->RZ_DATA[8].Channel		= 0;

	RZ_DATA->RZ_DATA[9].Address		= 0x03;
	RZ_DATA->RZ_DATA[9].Low				= 0x45;
	RZ_DATA->RZ_DATA[9].High			= 0x77;
	RZ_DATA->RZ_DATA[9].Additional	= 0x78;
	RZ_DATA->RZ_DATA[9].Channel		= 1;
	return true;
#endif

	unsigned long nReading = 0;
	unsigned long ulTempOffset = 0;

	bIsReceiveRZStream = false;

	nReading = ReadFromDevice((char*)pucBufferRZData, SIZE_DATA_RZ);

	if (nReading != SIZE_DATA_RZ) return false;

#ifdef DEBUG_IN_OUT_FILE
		WriteFile(hDebugRZUSB, pucBufferRZData, SIZE_MATRIX_RZ_DATA, &ulCountWriteBytes, NULL);		
#endif

	CopyMemory(IRP.GetPtrFrcIRP(), (pucBufferRZData + OFFSET_ADD_DATA), SIZE_BUFFER_CMD);

	if (IRP[BYTE_CMD] != CMD_START_RZ) return false;

	if (IRP[BYTE_STATE] == STATE_ERROR)
	{
		return false;
	}

	RZ_DATA->CountRZMessage = IRP[BYTE_CRZP];
	RZ_DATA->CountTrashRZMessage = (((unsigned short)IRP[BYTE_TR1]) << 8) | ((unsigned short)IRP[BYTE_TR0]);
	RZ_DATA->IsMissingRZMessage = (IRP[BYTE_PCERR]) ? true : false;
	RZ_DATA->CountMissingRZMessage = (((unsigned short)IRP[BYTE_PC1]) << 8) | ((unsigned short)IRP[BYTE_PC0]);

	for (unsigned char I = 0; I < RZ_DATA->CountRZMessage; I++)
	{
		ulTempOffset = SIZE_RZ_MESSAGE * I;

		RZ_DATA->RZ_DATA[I].Address = pucBufferRZData[ulTempOffset + OFFSET_RZ_MESSAGE_ADDRESS];
		RZ_DATA->RZ_DATA[I].Low = pucBufferRZData[ulTempOffset + OFFSET_RZ_MESSAGE_LOW_DATA];
		RZ_DATA->RZ_DATA[I].High = pucBufferRZData[ulTempOffset + OFFSET_RZ_MESSAGE_HIGH_DATA];
		RZ_DATA->RZ_DATA[I].Additional = pucBufferRZData[ulTempOffset + OFFSET_RZ_MESSAGE_ADDITIONAL];

		RZ_DATA->RZ_DATA[I].Channel = (I & 0x01)
			                              ? (((pucBufferRZData + OFFSET_MATRIX_RZ_CHANNELS)[I >> 1]) & 0xF0) >> 4
			                              : (((pucBufferRZData + OFFSET_MATRIX_RZ_CHANNELS)[I >> 1]) & 0x0F);
	}


	bIsReceiveRZStream = true;

	return true;
}

//---------------------------------------------------------------------------
bool RzUsb_IsReceiveRZStream(void)
{
	BYTE state;
	GetState(state);
	return state == STATEUN_RUN;
	//return bIsReceiveRZStream;
}

//---------------------------------------------------------------------------
bool RzUsb_ReceiveRZStreamEx(sRZStreamEx* RZ_DATA)
{
#ifdef DEBUG_RZUSB_DLL
	RZ_DATA->CountRZMessage				= 10;
	RZ_DATA->CountTrashRZMessage		= 0;
	RZ_DATA->IsMissingRZMessage		= false; 
	RZ_DATA->CountMissingRZMessage	= 0;

	RZ_DATA->RZ_DATA[0].Address		= 0x00;
	RZ_DATA->RZ_DATA[0].Low				= 0x45;
	RZ_DATA->RZ_DATA[0].High			= 0x72;
	RZ_DATA->RZ_DATA[0].Additional	= 0x78;

	RZ_DATA->RZ_DATA[1].Address		= 0x02;
	RZ_DATA->RZ_DATA[1].Low				= 0x45;
	RZ_DATA->RZ_DATA[1].High			= 0x34;
	RZ_DATA->RZ_DATA[1].Additional	= 0x78;

	RZ_DATA->RZ_DATA[2].Address		= 0x04;
	RZ_DATA->RZ_DATA[2].Low				= 0x45;
	RZ_DATA->RZ_DATA[2].High			= 0x12;
	RZ_DATA->RZ_DATA[2].Additional	= 0x12;

	RZ_DATA->RZ_DATA[3].Address		= 0x05;
	RZ_DATA->RZ_DATA[3].Low				= 0x45;
	RZ_DATA->RZ_DATA[3].High			= 0x12;
	RZ_DATA->RZ_DATA[3].Additional	= 0x78;

	RZ_DATA->RZ_DATA[4].Address		= 0x06;
	RZ_DATA->RZ_DATA[4].Low				= 0x45;
	RZ_DATA->RZ_DATA[4].High			= 0x00;
	RZ_DATA->RZ_DATA[4].Additional	= 0x78;

	RZ_DATA->RZ_DATA[5].Address		= 0x08;
	RZ_DATA->RZ_DATA[5].Low				= 0xFF;
	RZ_DATA->RZ_DATA[5].High			= 0x12;
	RZ_DATA->RZ_DATA[5].Additional	= 0x78;

	RZ_DATA->RZ_DATA[6].Address		= 0x12;
	RZ_DATA->RZ_DATA[6].Low				= 0xCE;
	RZ_DATA->RZ_DATA[6].High			= 0x12;
	RZ_DATA->RZ_DATA[6].Additional	= 0x78;

	RZ_DATA->RZ_DATA[7].Address		= 0x0A;
	RZ_DATA->RZ_DATA[7].Low				= 0x48;
	RZ_DATA->RZ_DATA[7].High			= 0x99;
	RZ_DATA->RZ_DATA[7].Additional	= 0x78;

	RZ_DATA->RZ_DATA[8].Address		= 0x0B;
	RZ_DATA->RZ_DATA[8].Low				= 0x45;
	RZ_DATA->RZ_DATA[8].High			= 0x66;
	RZ_DATA->RZ_DATA[8].Additional	= 0x78;

	RZ_DATA->RZ_DATA[9].Address		= 0x03;
	RZ_DATA->RZ_DATA[9].Low				= 0x45;
	RZ_DATA->RZ_DATA[9].High			= 0x77;
	RZ_DATA->RZ_DATA[9].Additional	= 0x78;

	ZeroMemory (RZ_DATA->CHANNELS, sizeof (RZ_DATA->CHANNELS));

	bIsReceiveRZStream = true;

	Sleep(DELAY_DUBUG_RC_RZ_STREAM_EX);

	return true;
#endif

	unsigned long nReading = 0;
	unsigned long ulTempOffset = 0;
	bool isNeedRestart = false;
	const unsigned int nStopCriticalError = 20;

	OutputDebugString("CRZUSB: >> Start RzUsb_ReceiveRZStreamEx");
	CountRZMessageC = RZ_DATA->CountRZMessage;
	if (RzUsb_IsSupportTimingData())
	{
		BYTE tmpBuff[SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD];

		bIsReceiveRZStream = false;

		nReading = ReadFromDevice((char*)tmpBuff, SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD);

		if (nReading != SIZE_DATA_RZ_WITH_TIME_DATA + SIZE_BUFFER_CMD) return false;

		RtlCopyMemory(RZ_DATA, tmpBuff, SIZE_MATRIX_RZ_DATA + SIZE_MATRIX_RZ_CHANNELS);
		RtlCopyMemory(((char*)RZ_DATA) + SIZE_MATRIX_RZ_DATA + SIZE_MATRIX_RZ_CHANNELS,
									tmpBuff + SIZE_MATRIX_RZ_DATA + SIZE_MATRIX_RZ_CHANNELS + SIZE_MATRIX_RZ_TIME,
									SIZE_BUFFER_CMD);
		RtlCopyMemory(&RZ_TIME_DATA, ((char*)RZ_DATA) + SIZE_MATRIX_RZ_DATA + SIZE_MATRIX_RZ_CHANNELS,
 SIZE_MATRIX_RZ_TIME);

		if (RZ_DATA->System00 != CMD_START_RZ) return false;

		if (RZ_DATA->System15 == STATE_ERROR)
		{
			return false;
		}

		bIsReceiveRZStream = true;
	}
	else
	{
		bIsReceiveRZStream = false;

		OutputDebugString("CRZUSB  >> start ReadFromDevice");
		nReading = ReadFromDevice((char*)RZ_DATA, SIZE_DATA_RZ);
		OutputDebugString("CRZUSB  << end ReadFromDevice");


		if (nReading != SIZE_DATA_RZ)
		{
			isNeedRestart = true;
			OutputDebugString("CRZUSB  >> if (nReading != SIZE_DATA_RZ)");
			//MessageBox(NULL, "nReading = ReadFromDevice((char*)RZ_DATA, SIZE_DATA_RZ);", "Error", MB_OK);
		}


#ifdef DEBUG_IN_OUT_FILE
			unsigned char *B = new unsigned char[SIZE_DATA_RZ];
			unsigned long ulCountWriteBytes;

			WriteFile(hDebugRZUSB, (char*)RZ_DATA, SIZE_DATA_RZ, &ulCountWriteBytes, NULL);		

			delete B;
#endif

		if (RZ_DATA->System00 != CMD_START_RZ)
		{
			//MessageBox(NULL, "if (RZ_DATA->System00 != CMD_START_RZ)", "Error", MB_OK);
			isNeedRestart = true;
			OutputDebugString("CRZUSB  >> if (RZ_DATA->System00 != CMD_START_RZ)");
			//return false;
		}

		if (RZ_DATA->System15 == STATE_ERROR)
		{
			//MessageBox(NULL, "if (RZ_DATA->System15 == STATE_ERROR)", "Error", MB_OK);
			isNeedRestart = true;
			OutputDebugString("CRZUSB  >> if (RZ_DATA->System15 == STATE_ERROR)");
			//return false;
		}

		if (isNeedRestart)
		{
			OutputDebugString("CRZUSB  >> if (isNeedRestart)");

			Sleep(200);
			if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3())
			{
				OutputDebugString(
					"CRZUSB  >> if (isNeedRestart): Sleep(200); if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3())");
				return false;
			}

			unsigned int err_count = 0;
			for (unsigned int err_stop = 0; err_stop < nStopCriticalError; ++err_stop)
			{
				if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3())
				{
					OutputDebugString(
						"CRZUSB  >> if (isNeedRestart): for; if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3())");
					return false;
				}

				bool err = false;
				if (!RzUsb_StopReceiveRZ())
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (!RzUsb_StopReceiveRZ())");
					err = true;
				}
				if (!RzUsb_RunReceiveRZ())
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (!RzUsb_RunReceiveRZ())");
					err = true;
				}
				if (ReadFromDevice((char*)RZ_DATA, SIZE_DATA_RZ) != SIZE_DATA_RZ)
				{
					OutputDebugString(
						"CRZUSB  >> if (isNeedRestart): if (ReadFromDevice((char*)RZ_DATA, SIZE_DATA_RZ) != SIZE_DATA_RZ)");
					err = true;
				}
				if (RZ_DATA->System00 != CMD_START_RZ)
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (RZ_DATA->System00 != CMD_START_RZ)	");
					err = true;
				}
				if (RZ_DATA->System15 == STATE_ERROR)
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (RZ_DATA->System15 == STATE_ERROR)");
					err = true;
				}

				if (err)
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (err)");
					err_count++;
				}
				else
				{
					OutputDebugString("CRZUSB  >> if (isNeedRestart): if (err) else");
					return true;
				}
			}

			if (err_count == nStopCriticalError)
			{
				OutputDebugString("CRZUSB  >> if (isNeedRestart): if (err_count == nStopCriticalError)");
				return false;
			}

			bIsReceiveRZStream = true;
			{
				OutputDebugString("CRZUSB  >> if (isNeedRestart): bIsReceiveRZStream = true;");
				return true;
			}
		}

		bIsReceiveRZStream = true;
	}

	OutputDebugString("CRZUSB: << Exit RzUsb_ReceiveRZStreamEx");
	return true;
}

//---------------------------------------------------------------------------
bool RzUsb_InitDeviceEx(sInitDeviceEx* InitData, sInfoDeviceEx* InfoDevice)
{
#ifdef DEBUG_RZUSB_DLL
	InfoDevice->bCorrectInfo = true;
	InfoDevice->IDDevice = 0x00;
	InfoDevice->CountChannels = 2;
	InfoDevice->NumPO = 0x00;
	InfoDevice->VersionPO = 0x00;
	InfoDevice->PlantNum = 0x00;

	bIsSupportTimingData = false;

	for (int i = 0; i < 16; ++i)
	{
		InfoDevice->InfoChannel[i].FreqRZ = 0x00;
		InfoDevice->InfoChannel[i].DensityRZ = 0x00;
	}

	return true;
#endif

	unsigned long nWritten = 0;
	unsigned long nReading = 0;
	BYTE* pucTEMP_BUFFER = new BYTE[SIZE_BUFFER_CMD_EX + SIZE_MATR_ADDR_RECV];

	if (!IsConnectToDevice() || !MuxWritePipe1() || !MuxReadPipe3()) return false;

	//  [4/29/2013 nn.nikolaev]

	BYTE stateDriver = 0x00;
	if (!GetState(stateDriver))
		return false;

	if (stateDriver == STATEUN_RUN)
	{
		if (!RzUsb_StopReceiveRZ())
			return false;

		if (!GetState(stateDriver))
			return false;

		if (stateDriver == STATEUN_RUN)
			return false;
	}


	//  [4/29/2013 nn.nikolaev]

	ZeroMemory(pucTEMP_BUFFER, SIZE_BUFFER_CMD_EX + SIZE_MATR_ADDR_RECV);

	pucTEMP_BUFFER[0] = CMD_INIT_DEVICE;
	pucTEMP_BUFFER[1] = InitData->AddrRecv;

	pucTEMP_BUFFER[16] = InitData->ParamChannel[0].FreqRZ;
	pucTEMP_BUFFER[17] = InitData->ParamChannel[0].FormatRZ;
	pucTEMP_BUFFER[18] = InitData->ParamChannel[0].CtrlParr;

	pucTEMP_BUFFER[19] = InitData->ParamChannel[1].FreqRZ;
	pucTEMP_BUFFER[20] = InitData->ParamChannel[1].FormatRZ;
	pucTEMP_BUFFER[21] = InitData->ParamChannel[1].CtrlParr;

	pucTEMP_BUFFER[22] = InitData->ParamChannel[2].FreqRZ;
	pucTEMP_BUFFER[23] = InitData->ParamChannel[2].FormatRZ;
	pucTEMP_BUFFER[24] = InitData->ParamChannel[2].CtrlParr;

	pucTEMP_BUFFER[25] = InitData->ParamChannel[3].FreqRZ;
	pucTEMP_BUFFER[26] = InitData->ParamChannel[3].FormatRZ;
	pucTEMP_BUFFER[27] = InitData->ParamChannel[3].CtrlParr;

	pucTEMP_BUFFER[28] = InitData->ParamChannel[4].FreqRZ;
	pucTEMP_BUFFER[29] = InitData->ParamChannel[4].FormatRZ;
	pucTEMP_BUFFER[30] = InitData->ParamChannel[4].CtrlParr;

	pucTEMP_BUFFER[31] = InitData->ParamChannel[5].FreqRZ;
	pucTEMP_BUFFER[32] = InitData->ParamChannel[5].FormatRZ;
	pucTEMP_BUFFER[33] = InitData->ParamChannel[5].CtrlParr;

	pucTEMP_BUFFER[34] = InitData->ParamChannel[6].FreqRZ;
	pucTEMP_BUFFER[35] = InitData->ParamChannel[6].FormatRZ;
	pucTEMP_BUFFER[36] = InitData->ParamChannel[6].CtrlParr;

	pucTEMP_BUFFER[37] = InitData->ParamChannel[7].FreqRZ;
	pucTEMP_BUFFER[38] = InitData->ParamChannel[7].FormatRZ;
	pucTEMP_BUFFER[39] = InitData->ParamChannel[7].CtrlParr;

	pucTEMP_BUFFER[40] = InitData->ParamChannel[8].FreqRZ;
	pucTEMP_BUFFER[41] = InitData->ParamChannel[8].FormatRZ;
	pucTEMP_BUFFER[42] = InitData->ParamChannel[8].CtrlParr;

	pucTEMP_BUFFER[43] = InitData->ParamChannel[9].FreqRZ;
	pucTEMP_BUFFER[44] = InitData->ParamChannel[9].FormatRZ;
	pucTEMP_BUFFER[45] = InitData->ParamChannel[9].CtrlParr;

	pucTEMP_BUFFER[46] = InitData->ParamChannel[10].FreqRZ;
	pucTEMP_BUFFER[47] = InitData->ParamChannel[10].FormatRZ;
	pucTEMP_BUFFER[48] = InitData->ParamChannel[10].CtrlParr;

	pucTEMP_BUFFER[49] = InitData->ParamChannel[11].FreqRZ;
	pucTEMP_BUFFER[50] = InitData->ParamChannel[11].FormatRZ;
	pucTEMP_BUFFER[51] = InitData->ParamChannel[11].CtrlParr;

	pucTEMP_BUFFER[52] = InitData->ParamChannel[12].FreqRZ;
	pucTEMP_BUFFER[53] = InitData->ParamChannel[12].FormatRZ;
	pucTEMP_BUFFER[54] = InitData->ParamChannel[12].CtrlParr;

	pucTEMP_BUFFER[55] = InitData->ParamChannel[13].FreqRZ;
	pucTEMP_BUFFER[56] = InitData->ParamChannel[13].FormatRZ;
	pucTEMP_BUFFER[57] = InitData->ParamChannel[13].CtrlParr;

	pucTEMP_BUFFER[58] = InitData->ParamChannel[14].FreqRZ;
	pucTEMP_BUFFER[59] = InitData->ParamChannel[14].FormatRZ;
	pucTEMP_BUFFER[60] = InitData->ParamChannel[14].CtrlParr;

	pucTEMP_BUFFER[61] = InitData->ParamChannel[15].FreqRZ;
	pucTEMP_BUFFER[62] = InitData->ParamChannel[15].FormatRZ;
	pucTEMP_BUFFER[63] = InitData->ParamChannel[15].CtrlParr;

	nWritten = WriteToDevice((char*)pucTEMP_BUFFER, SIZE_BUFFER_CMD_EX);
	if (nWritten != SIZE_BUFFER_CMD_EX) return false;

	nWritten = WriteToDevice((char*)InitData->MatrAddrRecv, SIZE_MATR_ADDR_RECV);
	if (nWritten != SIZE_MATR_ADDR_RECV) return false;


	nReading = ReadFromDevice((char*)pucTEMP_BUFFER, SIZE_BUFFER_CMD_EX);
	if (nReading != SIZE_BUFFER_CMD_EX) return false;

	/*nReading = ReadFromDevice((char*)pucTEMP_BUFFER, 65);
	if (nReading != 4)	return false;*/

	/*nReading = ReadFromDevice((char*)pucTEMP_BUFFER + 4, 60);
	if (nReading != 60)	return false;*/

	InfoDevice->bCorrectInfo = false;

	if (pucTEMP_BUFFER[BYTE_CMD] != CMD_INIT_DEVICE) return false;

	if (pucTEMP_BUFFER[BYTE_STATE] == STATE_ERROR)
	{
		InfoDevice->CodeErrorDevice = pucTEMP_BUFFER[BYTE_ERR_D];
		return false;
	}

	InfoDevice->bCorrectInfo = true;
	InfoDevice->IDDevice = pucTEMP_BUFFER[BYTE_IDDEV];
	InfoDevice->CountChannels = pucTEMP_BUFFER[8];
	InfoDevice->NumPO = (((unsigned short)pucTEMP_BUFFER[3]) << 8) | ((unsigned short)pucTEMP_BUFFER[2]);
	InfoDevice->VersionPO = (((unsigned short)pucTEMP_BUFFER[5]) << 8) | ((unsigned short)pucTEMP_BUFFER[4]);
	InfoDevice->PlantNum = (((unsigned short)pucTEMP_BUFFER[7]) << 8) | ((unsigned short)pucTEMP_BUFFER[6]);

	bIsSupportTimingData = (InfoDevice->VersionPO == 0x0200) ? true : false;

	InfoDevice->InfoChannel[0].FreqRZ = pucTEMP_BUFFER[16];
	InfoDevice->InfoChannel[0].DensityRZ = (((unsigned short)pucTEMP_BUFFER[18]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[17]);

	InfoDevice->InfoChannel[1].FreqRZ = pucTEMP_BUFFER[19];
	InfoDevice->InfoChannel[1].DensityRZ = (((unsigned short)pucTEMP_BUFFER[21]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[20]);

	InfoDevice->InfoChannel[2].FreqRZ = pucTEMP_BUFFER[22];
	InfoDevice->InfoChannel[2].DensityRZ = (((unsigned short)pucTEMP_BUFFER[24]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[23]);

	InfoDevice->InfoChannel[3].FreqRZ = pucTEMP_BUFFER[25];
	InfoDevice->InfoChannel[3].DensityRZ = (((unsigned short)pucTEMP_BUFFER[27]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[26]);

	InfoDevice->InfoChannel[4].FreqRZ = pucTEMP_BUFFER[28];
	InfoDevice->InfoChannel[4].DensityRZ = (((unsigned short)pucTEMP_BUFFER[30]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[29]);

	InfoDevice->InfoChannel[5].FreqRZ = pucTEMP_BUFFER[31];
	InfoDevice->InfoChannel[5].DensityRZ = (((unsigned short)pucTEMP_BUFFER[33]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[32]);

	InfoDevice->InfoChannel[6].FreqRZ = pucTEMP_BUFFER[34];
	InfoDevice->InfoChannel[6].DensityRZ = (((unsigned short)pucTEMP_BUFFER[36]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[35]);

	InfoDevice->InfoChannel[7].FreqRZ = pucTEMP_BUFFER[37];
	InfoDevice->InfoChannel[7].DensityRZ = (((unsigned short)pucTEMP_BUFFER[39]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[38]);

	InfoDevice->InfoChannel[8].FreqRZ = pucTEMP_BUFFER[40];
	InfoDevice->InfoChannel[8].DensityRZ = (((unsigned short)pucTEMP_BUFFER[42]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[41]);

	InfoDevice->InfoChannel[9].FreqRZ = pucTEMP_BUFFER[43];
	InfoDevice->InfoChannel[9].DensityRZ = (((unsigned short)pucTEMP_BUFFER[45]) << 8) | ((unsigned short)pucTEMP_BUFFER
		[44]);

	InfoDevice->InfoChannel[10].FreqRZ = pucTEMP_BUFFER[46];
	InfoDevice->InfoChannel[10].DensityRZ = (((unsigned short)pucTEMP_BUFFER[48]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[47]);

	InfoDevice->InfoChannel[11].FreqRZ = pucTEMP_BUFFER[49];
	InfoDevice->InfoChannel[11].DensityRZ = (((unsigned short)pucTEMP_BUFFER[51]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[50]);

	InfoDevice->InfoChannel[12].FreqRZ = pucTEMP_BUFFER[52];
	InfoDevice->InfoChannel[12].DensityRZ = (((unsigned short)pucTEMP_BUFFER[54]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[53]);

	InfoDevice->InfoChannel[13].FreqRZ = pucTEMP_BUFFER[55];
	InfoDevice->InfoChannel[13].DensityRZ = (((unsigned short)pucTEMP_BUFFER[57]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[56]);

	InfoDevice->InfoChannel[14].FreqRZ = pucTEMP_BUFFER[58];
	InfoDevice->InfoChannel[14].DensityRZ = (((unsigned short)pucTEMP_BUFFER[60]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[59]);

	InfoDevice->InfoChannel[15].FreqRZ = pucTEMP_BUFFER[61];
	InfoDevice->InfoChannel[15].DensityRZ = (((unsigned short)pucTEMP_BUFFER[63]) << 8) | ((unsigned short)
		pucTEMP_BUFFER[62]);

	return true;
}

//-----------------------------SUPPORT TIMING DATA---------------------------
//---------------------------------------------------------------------------

bool RzUsb_IsSupportTimingData(void)
{
	return bIsSupportTimingData;
	//return true;
}

//---------------------------------------------------------------------------

bool RzUsb_GetTimingData(sRZTimingData* tdata)
{
	RtlCopyMemory(tdata->Data, RZ_TIME_DATA.Data, MAX_COUNT_RZ_DATA);

	return true;
}

//---------------------------------------------------------------------------
//--------------------------------------------------------------------------- 
//--------------------------------------------------------------------------- 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
