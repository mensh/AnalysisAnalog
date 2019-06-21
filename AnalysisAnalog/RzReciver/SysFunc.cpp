#include "stdafx.h"
#include "SysFunc.h"
#include <handleapi.h>
#include <fileapi.h>
//#include <winbase.h>
#include <minwindef.h>




#define STATEUN_INIT  0x10
#define STATEUN_RUN   0x20

#define DRVUN1_IOCTL_SET_STATE_INIT   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DRVUN1_IOCTL_SET_STATE_RUN    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define DRVUN1_IOCTL_GET_STATE        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS)

char *LIST_ERROR_CONNECTING[COUNT_KNOW_ERROR] = 
		{"",
		 "",
		 "Не найдена динамическая библиотека PDFGUID.DLL",
		 "Не найдена динамическая библиотека CRIOCTL.DLL",
		 "Не найдены в динимических библиотеках необходимые для работы функции",
		 "Устройство UN1 не обнаружено в системе",
		 "Не удаётся подключиться к устройству UN1",
		 "Устройство UN1 может испльзовать только одна копия программы",
		 "Не удаётся инициализировать функции считывания/записи из/в устройство",
		 "Неизвестная ошибка"};

GUID ClassGuid = DrvRZtoUSBDevice_CLASS_GUID;

HANDLE hDevice = INVALID_HANDLE_VALUE;

char DevicePath[SIZE_DEVICE_PATH] = {NULL};

DWORD IOCTL_READ_PIPE_1		= 0;
DWORD IOCTL_READ_PIPE_2		= 0;
DWORD IOCTL_READ_PIPE_3		= 0;
DWORD IOCTL_WRITE_PIPE_1	= 0;
DWORD IOCTL_WRITE_PIPE_2	= 0;
DWORD IOCTL_WRITE_PIPE_3	= 0;

//HINSTANCE dllInstancePDfGUID = NULL;
//HINSTANCE dllInstanceCRIOCTL = NULL;

int LAST_ERROR = DONT_ERROR;

unsigned long ReadTimeout	= 5;
unsigned long WriteTimeout = 5;

OVERLAPPED olRead;
OVERLAPPED olWrite;

SECURITY_ATTRIBUTES saRead;
SECURITY_ATTRIBUTES saWrite;

HANDLE ReadEvent		= NULL;
HANDLE WriteEvent		= NULL;

//-----------------------------------------------------------------------------

DWORD GetReadPipe1IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncReadPipe1, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

DWORD GetReadPipe2IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncReadPipe2, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

DWORD GetReadPipe3IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncReadPipe3, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

DWORD GetWritePipe1IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncWritePipe1, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

DWORD GetWritePipe2IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncWritePipe2, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

DWORD GetWritePipe3IOCTL(void)
{
	return CTL_CODE(FILE_DEVICE_UNKNOWN, FuncWritePipe3, 
						 METHOD_BUFFERED, FILE_ANY_ACCESS);
}
//-----------------------------------------------------------------------------

void ClearAllVar (void)
{
	IOCTL_READ_PIPE_1	= IOCTL_READ_PIPE_2 = IOCTL_READ_PIPE_3 = 
	IOCTL_WRITE_PIPE_1 = IOCTL_WRITE_PIPE_2 = IOCTL_WRITE_PIPE_3 = 0;

	HANDLE hDevice = INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ThrowLastErrorConnect (void)
{
	switch (LAST_ERROR)
	{
		case SUCCEFULY_CONNECTING:
			//throw(LIST_ERROR_CONNECTING[SUCCEFULY_CONNECTING]);
			break;

		case ERROR_DONT_FOUND_PDFGUID_DLL:
			throw(LIST_ERROR_CONNECTING[ERROR_DONT_FOUND_PDFGUID_DLL]);
			break;

		case ERROR_DONT_FOUND_CRIOCTL_DLL:
			throw(LIST_ERROR_CONNECTING[ERROR_DONT_FOUND_CRIOCTL_DLL]);
			break;

		case ERROR_DONT_FOUND_FUNCTIONS:
			throw(LIST_ERROR_CONNECTING[ERROR_DONT_FOUND_FUNCTIONS]);
			break;

		case ERROR_DONT_FOUND_DEVICE:
			throw(LIST_ERROR_CONNECTING[ERROR_DONT_FOUND_DEVICE]);
			break;

		case ERROR_CONNECTING_TO_DEVICE:
			throw(LIST_ERROR_CONNECTING[ERROR_CONNECTING_TO_DEVICE]);
			break;

		case ERROR_DONT_AGAIN_CONNECTING:
			throw(LIST_ERROR_CONNECTING[ERROR_DONT_AGAIN_CONNECTING]);
			break;

		case ERROR_CREATE_EVENT:
			throw(LIST_ERROR_CONNECTING[ERROR_CREATE_EVENT]);
			break;

		case DONT_ERROR:
			break;

		default:
			throw(LIST_ERROR_CONNECTING[UNKNOW_ERROR]);
	}
}
//---------------------------------------------------------------------------

int ConnectDevice (void)
{
	if (hDevice != INVALID_HANDLE_VALUE)
		return (LAST_ERROR = ERROR_DONT_AGAIN_CONNECTING);

	ClearAllVar();

	IOCTL_READ_PIPE_1		= GetReadPipe1IOCTL();
	IOCTL_READ_PIPE_2		= GetReadPipe2IOCTL();
	IOCTL_READ_PIPE_3		= GetReadPipe3IOCTL();
	IOCTL_WRITE_PIPE_1	= GetWritePipe1IOCTL();
	IOCTL_WRITE_PIPE_2	= GetWritePipe2IOCTL();
	IOCTL_WRITE_PIPE_3	= GetWritePipe3IOCTL();

	if(!GetDevicePathFromGUID(&ClassGuid, DevicePath))
	{
		ClearAllVar();
		return (LAST_ERROR = ERROR_DONT_FOUND_DEVICE);
	}

	hDevice = CreateFile(DevicePath,	GENERIC_READ | GENERIC_WRITE,
								0,	NULL,
								OPEN_EXISTING,	FILE_FLAG_OVERLAPPED, 
								NULL);
								
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		ClearAllVar();
		return (LAST_ERROR = ERROR_CONNECTING_TO_DEVICE);
	}

	saRead.nLength						= sizeof (SECURITY_ATTRIBUTES);
	saRead.lpSecurityDescriptor	= NULL;
	saRead.bInheritHandle			= false;

	ReadEvent = CreateEvent(&saRead, false, false, "EventForReadDevice");

	olRead.hEvent		= ReadEvent;
	olRead.Offset		= 0;
	olRead.OffsetHigh = 0;

	//-------------
	olRead.hEvent = 0;
	olRead.Internal = 0;
	olRead.InternalHigh = 0;
	olRead.Offset = 0;
	olRead.OffsetHigh = 0;
	olRead.Pointer = 0;
	//-------------

	saWrite.nLength					= sizeof (SECURITY_ATTRIBUTES);
	saWrite.lpSecurityDescriptor	= NULL;
	saWrite.bInheritHandle			= false;

	WriteEvent = CreateEvent(&saWrite, false, false, "EventForWriteDevice");

	olWrite.hEvent			= WriteEvent;
	olWrite.Offset			= 0;
	olWrite.OffsetHigh	= 0;

	//-------------
	olWrite.hEvent = 0;
	olWrite.Internal = 0;
	olWrite.InternalHigh = 0;
	olWrite.Offset = 0;
	olWrite.OffsetHigh = 0;
	olWrite.Pointer = 0;
	//-------------

	if (!ReadEvent || !WriteEvent) 
	{	
		ClearAllVar();

		CloseHandle (ReadEvent);
		CloseHandle (WriteEvent);

		ReadEvent	= NULL;
		WriteEvent	= NULL;

		return (LAST_ERROR = ERROR_CREATE_EVENT);
	}

	return (LAST_ERROR = SUCCEFULY_CONNECTING);
}
//---------------------------------------------------------------------------

bool DisconnectDevice (void)
{
	DevicePath[0] = NULL;

	if (hDevice != INVALID_HANDLE_VALUE)
  {
		CloseHandle(hDevice);
	  hDevice = INVALID_HANDLE_VALUE;

		ClearAllVar();
  }
	
	return true;
}
//---------------------------------------------------------------------------

bool IsConnectToDevice (void)
{
	return (hDevice != INVALID_HANDLE_VALUE) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxReadPipe1 (void)
{
DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_READ_PIPE_1,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_READ_PIPE_1) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxReadPipe2 (void)
{
	DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_READ_PIPE_2,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_READ_PIPE_2) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxReadPipe3 (void)
{
DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_READ_PIPE_3,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_READ_PIPE_3) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxWritePipe1 (void)
{
DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_WRITE_PIPE_1,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_WRITE_PIPE_1) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxWritePipe2 (void)
{
DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_WRITE_PIPE_2,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_WRITE_PIPE_2) ? true : false;
}
//---------------------------------------------------------------------------

bool MuxWritePipe3 (void)
{
DWORD ReturnCode = 0;

	if (hDevice == INVALID_HANDLE_VALUE) return false;

int Status = DeviceIoControl(hDevice, IOCTL_WRITE_PIPE_3,
										NULL, 0, NULL, 0, &ReturnCode, NULL);

	return (Status && ReturnCode == RETURN_WRITE_PIPE_3) ? true : false;
}
//---------------------------------------------------------------------------

unsigned long ReadFromDevice (char *buf, unsigned long n)
{
unsigned long nRead = 0;
BOOL Status = false;
	
	if (hDevice == INVALID_HANDLE_VALUE) return 0L;

	{
// 		char buff[256];
// 		wsprintf(buff, ">> Enter ReadFromDevice(*buf=%d; n=%d)", buf, n);
// 		OutputDebugString(buff);
	}

	Status = ReadFile(hDevice, buf, n, &nRead, &olRead);

	if (Status || GetLastError() != ERROR_IO_PENDING)
		return 0L;
	
	
	{
// 		char buff[256];
// 		wsprintf(buff, "ReadFile: nRead=%d; Status=%d", nRead, Status);
// 		OutputDebugString(buff);
	}

 	if (WaitForSingleObject(hDevice/*ReadEvent*/, ReadTimeout) == WAIT_TIMEOUT)
	{
// 		char buff[256];
// 		wsprintf(buff, "if (WaitForSingleObject(hDevice/*ReadEvent*/, ReadTimeout) == WAIT_TIMEOUT) -- [%d]", GetLastError());
// 		//::MessageBox(NULL, buff, "Error", MB_OK);
// 		OutputDebugString(buff);
 		return 0L;
	}

	
	if (!GetOverlappedResult(hDevice, &olRead, &nRead, false))
	{							
// 		char buff[256];
// 		wsprintf(buff, "if (!GetOverlappedResult(hDevice, &olRead, &nRead, false)) -- [%d]; nRead=%d", GetLastError(), nRead);
// 		//::MessageBox(NULL, buff, "Error", MB_OK);
// 		OutputDebugString(buff);
		CancelIo(hDevice);
		return 0L;
	}

	{
// 		char buff[256];
// 		wsprintf(buff, "<< Return ReadFromDevice: nRead=%d", nRead);
// 		OutputDebugString(buff);
	}

	return nRead;
}
//---------------------------------------------------------------------------

unsigned long WriteToDevice (char *buf, unsigned long n)
{
unsigned long nWritten = 0;
BOOL Status = false;

	if (hDevice == INVALID_HANDLE_VALUE) return 0L;

	Status = WriteFile(hDevice, buf, n, &nWritten, &olWrite);

	/*if (Status || GetLastError() != ERROR_IO_PENDING)
		return 0L;*/
	
 	if (WaitForSingleObject(hDevice/*WriteEvent*/, WriteTimeout) == WAIT_TIMEOUT)
	{
		//MessageBox(NULL, "if (WaitForSingleObject(hDevice/*WriteEvent*/, WriteTimeout) == WAIT_TIMEOUT)", "Error", MB_OK);
 		return 0L;
	}

	

	if (!GetOverlappedResult(hDevice, &olWrite, &nWritten, false))
	{							
		//MessageBox(NULL, "if (!GetOverlappedResult(hDevice, &olWrite, &nWritten, false))", "Error", MB_OK);
		CancelIo(hDevice);
		return 0L;
	}

	return nWritten;

/////////////////
	/*Status = WriteFileEx(hDevice, buf, n, &olWrite, CallBackFunc);

	if (!Status)
		return 0L;//throw ("Не удалось выполнить чтение с устройства");
	
	while(1);*/

	/*WaitForSingleObject(WriteEvent, WriteTimeout);

	if (!GetOverlappedResult(hDevice, &olWrite, &nWritten, false))
	{							
		CancelIo(hDevice);
		return 0L;//throw ("Не удалось выполнить чтение с устройства");   
	}*/

	//return nWritten;
}
//---------------------------------------------------------------------------

HANDLE GetHandleDevice (void)
{
	return hDevice;
}
//---------------------------------------------------------------------------

char* GetDevicePath (void)
{
	return DevicePath;
}
//---------------------------------------------------------------------------

void SetReadTimeout (unsigned long Timeout)
{
	ReadTimeout = Timeout;
}
//---------------------------------------------------------------------------

void SetWriteTimeout (unsigned long Timeout)
{
	WriteTimeout = Timeout;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*void WINAPI CallBackFunc (DWORD A, DWORD B, LPOVERLAPPED C)
{
	throw ("");
}*/

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass constructor
//
DEVINTF_INLINE CDeviceInterfaceClass::CDeviceInterfaceClass(
	GUID* pClassGuid, 
	PDWORD status
	) 
{
	DWORD flags = DIGCF_DEVICEINTERFACE | DIGCF_PRESENT ;
	m_hInfo = INVALID_HANDLE_VALUE;
	ZeroMemory(&m_Guid,sizeof(GUID));

	try
	{
		*status = ERROR_INVALID_PARAMETER;
		m_Guid = *pClassGuid;
		m_hInfo = SetupDiGetClassDevs(pClassGuid, NULL, NULL, flags);

		if ( m_hInfo == INVALID_HANDLE_VALUE )
			*status = GetLastError();
		else
			*status = ERROR_SUCCESS;

	}
	catch (...)
	{
		m_hInfo = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass destructor
//
DEVINTF_INLINE CDeviceInterfaceClass::~CDeviceInterfaceClass(void)
{
	if ( m_hInfo != INVALID_HANDLE_VALUE )
		SetupDiDestroyDeviceInfoList(m_hInfo);

	m_hInfo = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface constructor
//
DEVINTF_INLINE CDeviceInterface::CDeviceInterface(
	CDeviceInterfaceClass* pClassObject, 
	DWORD Index,
	PDWORD Error
	)
{
	m_Class = pClassObject;

	BOOL status;
	DWORD ReqLen;
	
	m_Detail = NULL;
	m_Data.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

	try
	{
		*Error = ERROR_INVALID_PARAMETER;

		status = SetupDiEnumDeviceInterfaces(
			m_Class->GetHandle(), 
			NULL, 
			m_Class->GetGuid(), 
			Index, 
			&m_Data
			);

		if ( !status )
		{
			*Error = GetLastError();
			return;
		}					  

		SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			NULL,
			0,
			&ReqLen,
			NULL 
			);

		*Error = GetLastError();

		if ( *Error != ERROR_INSUFFICIENT_BUFFER )
			return;

		m_Detail = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[ReqLen]);

		if ( !m_Detail )
		{
			*Error = ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		m_Detail->cbSize = sizeof (SP_INTERFACE_DEVICE_DETAIL_DATA);

		status = SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			m_Detail,
			ReqLen,
			&ReqLen,
			NULL
			);

		if ( !status )
		{
			*Error = GetLastError();
			delete m_Detail;
			m_Detail = NULL;
			return;
		}

		*Error = ERROR_SUCCESS;
	}
	catch (...)
	{
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface destructor
//
DEVINTF_INLINE CDeviceInterface::~CDeviceInterface(void)
{
	if (m_Detail)
	{
		delete m_Detail;
		m_Detail = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface::DevicePath
//
DEVINTF_INLINE TCHAR* CDeviceInterface::DevicePath(void)
{
	try
	{
		if ( m_Detail)
			return m_Detail->DevicePath;
		else
			return NULL;
	}
	catch (...)
	{
		return NULL;
	}

}

//-----------------------------------------------------------------------------

/*HANDLE OpenByInterface(
		GUID* pClassGuid,	// points to the GUID that identifies the interface class
		DWORD instance,		// specifies which instance of the enumerated devices to open
		PDWORD pError		// address of variable to receive error status
		)
{
	HANDLE hDev;
	CDeviceInterfaceClass DevClass(pClassGuid, pError);

	if (*pError != ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;

	CDeviceInterface DevInterface(&DevClass, instance, pError);

	if (*pError != ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;

	hDev = CreateFile(
		DevInterface.DevicePath(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (hDev == INVALID_HANDLE_VALUE)
		*pError = GetLastError();

	return hDev;
}*/
//-----------------------------------------------------------------------------

char* GetDevicePathFromGUID (GUID *pClassGuid, char *pDevicePath)
{
PDWORD pError = new DWORD(ERROR_SUCCESS);
DWORD instance = 0;

	//*pError = ERROR_SUCCESS;

	CDeviceInterfaceClass DevClass(pClassGuid, pError);

	if (*pError != ERROR_SUCCESS)
		return NULL;

	CDeviceInterface DevInterface(&DevClass, instance, pError);

	if (*pError != ERROR_SUCCESS)
		return NULL;

	if(!lstrcpy(pDevicePath, DevInterface.DevicePath()))
		return NULL;

	return pDevicePath;
}
//---------------------------------------------------------------------------
bool SetStateInit (void)
{
  DWORD ReturnCode = 0;

  if (hDevice == INVALID_HANDLE_VALUE) return false;

  int Status = DeviceIoControl(hDevice, DRVUN1_IOCTL_SET_STATE_INIT,
    NULL, 0, NULL, 0, &ReturnCode, NULL);

  return (Status && ReturnCode == 0) ? true : false;
}
//---------------------------------------------------------------------------

bool SetStateRun (void)
{
  DWORD ReturnCode = 0;

  if (hDevice == INVALID_HANDLE_VALUE) return false;

  int Status = DeviceIoControl(hDevice, DRVUN1_IOCTL_SET_STATE_RUN,
    NULL, 0, NULL, 0, &ReturnCode, NULL);

  return (Status && ReturnCode == 0) ? true : false;
}
//---------------------------------------------------------------------------

bool GetState (BYTE& state)
{
  DWORD ReturnCode = 0;
  BYTE BufferOUT[1] = {0x00};

  if (hDevice == INVALID_HANDLE_VALUE) return false;

  int Status = DeviceIoControl(hDevice, DRVUN1_IOCTL_GET_STATE,
    NULL, 0, BufferOUT, 1, &ReturnCode, NULL);

  if (!Status || ReturnCode != 1)
    return false;

  state = BufferOUT[0];

  return true;
}
//---------------------------------------------------------------------------

