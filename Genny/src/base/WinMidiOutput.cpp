#include "WinMidiOutput.h"

WinMidiOutput::WinMidiOutput() : _handle(nullptr)
{
}

WinMidiOutput::~WinMidiOutput()
{
	close();
}

std::vector<std::string> WinMidiOutput::enumerateDeviceNames()
{
	std::vector<std::string> names;

	UINT numDevices = midiOutGetNumDevs();
	for (UINT i = 0; i < numDevices; i++)
	{
		MIDIOUTCAPSA caps;
		if (midiOutGetDevCapsA(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR)
			names.push_back(std::string(caps.szPname));
	}

	return names;
}

bool WinMidiOutput::open(const std::string& deviceName)
{
	close();

	UINT numDevices = midiOutGetNumDevs();
	for (UINT i = 0; i < numDevices; i++)
	{
		MIDIOUTCAPSA caps;
		if (midiOutGetDevCapsA(i, &caps, sizeof(caps)) != MMSYSERR_NOERROR)
			continue;

		if (deviceName == caps.szPname)
		{
			if (midiOutOpen(&_handle, i, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR)
			{
				_openDeviceName = deviceName;
				return true;
			}

			_handle = nullptr;
			return false;
		}
	}

	return false;
}

void WinMidiOutput::close()
{
	if (_handle != nullptr)
	{
		midiOutClose(_handle);
		_handle = nullptr;
	}

	_openDeviceName.clear();
}

void WinMidiOutput::sendShortMessage(unsigned char status, unsigned char data1, unsigned char data2)
{
	if (_handle == nullptr)
		return;

	DWORD msg = (DWORD)status | ((DWORD)data1 << 8) | ((DWORD)data2 << 16);
	midiOutShortMsg(_handle, msg);
}
