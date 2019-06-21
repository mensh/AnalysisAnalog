#ifndef SYSFUNCH
#define SYSFUNCH

#include <setupapi.h>
#pragma comment (lib, "setupapi.lib")

#include <tchar.h>

#define FuncReadPipe1	0x800
#define FuncReadPipe2	0x802
#define FuncReadPipe3	0x804
#define FuncWritePipe1	0x801
#define FuncWritePipe2	0x803
#define FuncWritePipe3	0x805

#define FILE_DEVICE_UNKNOWN	0x00000022
#define METHOD_BUFFERED                0
#define FILE_ANY_ACCESS                0

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

#define RETURN_READ_PIPE_1			0x60
#define RETURN_READ_PIPE_2			0x50
#define RETURN_READ_PIPE_3			0x40
#define RETURN_WRITE_PIPE_1		0x30
#define RETURN_WRITE_PIPE_2		0x20
#define RETURN_WRITE_PIPE_3		0x10

#define DONT_ERROR							0x00
#define SUCCEFULY_CONNECTING				0x01
#define ERROR_DONT_FOUND_PDFGUID_DLL	0x02
#define ERROR_DONT_FOUND_CRIOCTL_DLL	0x03
#define ERROR_DONT_FOUND_FUNCTIONS		0x04
#define ERROR_DONT_FOUND_DEVICE			0x05
#define ERROR_CONNECTING_TO_DEVICE		0x06
#define ERROR_DONT_AGAIN_CONNECTING		0x07
#define ERROR_CREATE_EVENT					0x08
#define UNKNOW_ERROR							0x09

#define COUNT_KNOW_ERROR					10

#define DrvRZtoUSBDevice_CLASS_GUID \
	{ 0xf621b886, 0x615f, 0x456c, { 0xa4, 0x31, 0xec, 0x33, 0xb9, 0xbf, 0xc4, 0xb6} }

#define SIZE_DEVICE_PATH	512

#define DEVINTF_INLINE inline

#define STATEUN_INIT  0x10
#define STATEUN_RUN   0x20

//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterfaceClass
//
class CDeviceInterfaceClass
{
public:
	CDeviceInterfaceClass(GUID* pClassGuid, PDWORD status);
	~CDeviceInterfaceClass(void);
	GUID* GetGuid(void)      { return &m_Guid; }
	HDEVINFO GetHandle(void) { return m_hInfo; }

protected:
	HDEVINFO		m_hInfo;
	GUID			m_Guid;
};

//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterface
//
class CDeviceInterface
{
public:
	DEVINTF_INLINE CDeviceInterface( CDeviceInterfaceClass* pClassObject, DWORD Index, PDWORD Error );
	DEVINTF_INLINE ~CDeviceInterface(void);
	DEVINTF_INLINE TCHAR* DevicePath(void);

protected:
	CDeviceInterfaceClass*				m_Class;
	SP_DEVICE_INTERFACE_DATA			m_Data;
	PSP_INTERFACE_DEVICE_DETAIL_DATA	m_Detail;
};


DWORD GetReadPipe1IOCTL(void);
DWORD GetReadPipe2IOCTL(void);
DWORD GetReadPipe3IOCTL(void);
DWORD GetWritePipe1IOCTL(void);
DWORD GetWritePipe2IOCTL(void);
DWORD GetWritePipe3IOCTL(void);

char* GetDevicePathFromGUID (GUID*, char*);

int ConnectDevice (void);
bool DisconnectDevice (void);
bool MuxReadPipe3 (void);
bool MuxWritePipe1 (void);
bool IsConnectToDevice (void);
void SetReadTimeout (unsigned long);
void SetWriteTimeout (unsigned long);
unsigned long WriteToDevice (char*, unsigned long);
unsigned long ReadFromDevice (char*, unsigned long);

bool SetStateInit (void);
bool SetStateRun (void);
bool GetState (BYTE& state);

#endif