#pragma once
#include "CRZUSB.h"

using namespace System;

namespace RzReciver {
	public ref class ClassRzReciver
	{
		public:
		int countRZMessage;

		static bool RzUsb_ConnectDeviceC()
		{
			return RzUsb_ConnectDevice();
		}

	public:
		static bool RzUsb_IsConnectToDeviceC()
		{
			return RzUsb_IsConnectToDevice();
		}

	public:
		static bool RzUsb_DisconnectDeviceC()
		{
			return RzUsb_DisconnectDevice();
		}

	public:
		static bool RzUsb_InitDeviceExC()
		{
			return MainInitDevi();
		}

	public:
		static void RzUsb_SetReadTimeoutC(int k)
		{
			RzUsb_SetReadTimeout(k);
		}

	public :
		static void RzUsb_SetWriteTimeoutC(int k)
		{
			RzUsb_SetWriteTimeout(k);
		}

	public :
		static bool RzUsb_RunReceiveRZC()
		{
			return RzUsb_RunReceiveRZ();
		}

	public :
		static bool RzUsb_StopReceiveRZC()
		{
			return RzUsb_StopReceiveRZ();
		}
	public :
		Collections::Generic::List<int>^ RzUsb_ReceiveRZStreamExC()
		{
			Collections::Generic::List<int> ^ListMessage = gcnew Collections::Generic::List<int>();
			if (MainRzUsb_ReceiveRZStreamEx())
			{
				
				countRZMessage = _sRZStreamEx.CountRZMessage;
				for (int i=0; i < _sRZStreamEx.CountRZMessage;i++)
				{
					ListMessage->Add(_sRZStreamEx.RZ_DATA[i].Address | (_sRZStreamEx.RZ_DATA[i].Low << 8) | (_sRZStreamEx.RZ_DATA[i].High << 16) | (_sRZStreamEx.RZ_DATA[i].Additional << 24));
				}
			}
			return ListMessage;
		}
	};
}
