#pragma once
#include <windows.h>
#include <string>
#include <vector>

//Sends MIDI directly to a real Windows MIDI output device, bypassing the host's
//own MIDI-out routing entirely. Used for GenMDM output in the VST build, since
//VST2 hosts (Ableton in particular) route a plugin's generated MIDI through a
//single, fixed-channel track output, which strips the per-channel addressing
//GenMDM depends on for its 6 FM parts.
class WinMidiOutput
{
public:
	WinMidiOutput();
	~WinMidiOutput();

	static std::vector<std::string> enumerateDeviceNames();

	bool open(const std::string& deviceName);
	void close();
	bool isOpen() const { return _handle != nullptr; }
	const std::string& getOpenDeviceName() const { return _openDeviceName; }

	void sendShortMessage(unsigned char status, unsigned char data1, unsigned char data2);

private:
	HMIDIOUT _handle;
	std::string _openDeviceName;
};
