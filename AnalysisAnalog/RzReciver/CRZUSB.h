//---------------------------------------------------------------------------
#ifndef CRZUSBH
#define CRZUSBH
//---------------------------------------------------------------------------
#ifdef CRZUSB_EXPORTS
#define CRZUSB_API extern "C" __declspec(dllexport)
#else
#define CRZUSB_API extern "C" __declspec(dllimport)
#endif
//---------------------------------------------------------------------------
#define SIZE_MATR_ADDR_RECV			512
#define MAX_COUNT_RZ_DATA				128
#define MAX_COUNT_CHANNELS			16

#define FREQ_RZ_GET_DEVICE			0x00
#define FREQ_RZ_12and5kHz				0x01
#define FREQ_RZ_50kHZ						0x02
#define FREQ_RZ_100kHZ					0x03
#define FREQ_RZ_UNKNOWN					0xFF

#define FORMAT_RZ_DOP						0x00
#define FORMAT_RZ_OSN						0x01

#define CTRL_PARR_PARITY				0x00
#define CTRL_PARR_ODDNESS				0x01

#define IDDEV_2RZTOUSB					0x01

#define ENABLE_ADDR_REC					0x01
#define DISABLE_ADDR_REC				0x00
//---------------------------------------------------------------------------
//#pragma pack(show)
#pragma pack(push,1)
#include <minwindef.h>

//#pragma pack(show)

struct sInitDevice
{
	unsigned char ucFreqRZ;
	unsigned char ucFormatRZ;
	unsigned char ucCtrlParr;
	unsigned char ucAddrRecv;

	unsigned char pucMatrAddrRecv[SIZE_MATR_ADDR_RECV];
};

struct sInitDeviceEx
{
	struct sInitChannel
	{
		BYTE FreqRZ;
		BYTE FormatRZ;
		BYTE CtrlParr;
	} ParamChannel[MAX_COUNT_CHANNELS];
	
	BYTE AddrRecv;
	BYTE MatrAddrRecv[SIZE_MATR_ADDR_RECV];
};

struct sInfoDevice
{
	bool bCorrectInfo;		
	unsigned char	ucCodeErrorDevice;
	unsigned char	ucIDDevice;
	unsigned char	ucFreqRZ;
	unsigned char  ucCountChannels;
	unsigned short	usDensityRZ;
	unsigned short usPlantNum;
	unsigned short usNumPO;
	unsigned short usVersionPO;
};

struct sInfoDeviceEx
{
	bool bCorrectInfo;
	BYTE CodeErrorDevice;
	BYTE IDDevice;
	WORD NumPO;
	WORD VersionPO;
	WORD PlantNum;
	BYTE CountChannels;

	struct sInfoChannel
	{
		BYTE FreqRZ;
		WORD DensityRZ;
	} InfoChannel[MAX_COUNT_CHANNELS];
};

struct sRZStream
{
	unsigned char  CountRZMessage;
	unsigned short CountTrashRZMessage;
	unsigned short CountMissingRZMessage;
	bool IsMissingRZMessage;

	struct
	{
		unsigned char Address;
		unsigned char Low;
		unsigned char High;
		unsigned char Additional;
		unsigned char Channel;
	} RZ_DATA[MAX_COUNT_RZ_DATA];
	//Index RZ_DATA: [0 ... (CountRZMessage - 1)]
};

struct sRZStreamEx
{
	struct
	{
		union
		{
			struct
			{
				BYTE Low;
				BYTE High;
				BYTE Additional;	
				BYTE Address;						
			};
			DWORD RzMessage;
		};
	} RZ_DATA[MAX_COUNT_RZ_DATA];
	
	struct 
	{
		BYTE Channel_0 : 4;
		BYTE Channel_1 : 4;
	} CHANNELS[MAX_COUNT_RZ_DATA / 2];

	BYTE System00;

	BYTE CountRZMessage;

	union
	{
		struct
		{	
			BYTE CountTrashRZMessageL;
			BYTE CountTrashRZMessageH;
		};
		WORD CountTrashRZMessage;
	};

	BYTE IsMissingRZMessage;

	union
	{
		struct
		{	
			BYTE CountMissingRZMessageL;
			BYTE CountMissingRZMessageH;
		};
		WORD CountMissingRZMessage;
	};

	BYTE System07;
	BYTE System08;
	BYTE System09;
	BYTE System10;
	BYTE System11;
	BYTE System12;
	BYTE System13;
	BYTE System14;
	BYTE System15;

	inline BYTE GetChannelRZMessage (int IndexRZMessage)
	{
		return (IndexRZMessage & 0x01) ? CHANNELS[IndexRZMessage >> 1].Channel_1 : CHANNELS[IndexRZMessage >> 1].Channel_0;
	}
};

struct sRZTimingData
{
	BYTE Data[MAX_COUNT_RZ_DATA];
};

#pragma pack(pop)
//#pragma pack(show)
////---------------------------------------------------------------------------
 bool RzUsb_ConnectDevice (void);
 bool RzUsb_IsConnectToDevice (void);
 bool RzUsb_DisconnectDevice (void);
 bool RzUsb_InitDevice (sInitDevice*, sInfoDevice*);
 bool RzUsb_InitDeviceEx (sInitDeviceEx*, sInfoDeviceEx*);
 void RzUsb_SetReadTimeout (unsigned long);
 void RzUsb_SetWriteTimeout (unsigned long);
 bool RzUsb_RunReceiveRZ (void);
 bool RzUsb_StopReceiveRZ (void);
 bool RzUsb_ReceiveRZStream (sRZStream*);
 bool RzUsb_ReceiveRZStreamEx (sRZStreamEx*);
 bool RzUsb_IsReceiveRZStream (void);
//--------------------SUPPORT TIMING DATA------------------------------------
 bool RzUsb_IsSupportTimingData (void);
 bool RzUsb_GetTimingData (sRZTimingData*);
BOOL MainInitDevi();
BOOL MainRzUsb_ReceiveRZStreamEx();
extern int CountRZMessageC;
extern sRZStreamEx _sRZStreamEx;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
