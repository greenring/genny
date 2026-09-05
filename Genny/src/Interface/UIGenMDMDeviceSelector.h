#pragma once
#include "UIInstrument.h"
#include "UIImage.h"
#include "UICheckBoxNum.h"
#include "Genny2612.h"
#include <string>
#include <vector>

class UIInstrumentsPanel;

//Lets the user pick which real Windows MIDI output device GenMDM sends directly to,
//bypassing the VST host's own MIDI routing (see WinMidiOutput / GennyVST::setGenMDMDevice).
class UIGenMDMDeviceSelector : public CControl, public IControlListener, public GennyInterfaceObject
{
public:
	UIGenMDMDeviceSelector(CPoint point, GennyInterfaceObject* owner);
	~UIGenMDMDeviceSelector(void);
	virtual bool attached (CView* parent);

	void setSelectedIndex(int index);
	void updateDisplay(int index);
	virtual void valueChanged (CControl* control);
	virtual void draw (CDrawContext* pContext);
	void reconnect();
	void setVisible(bool visible);

	virtual bool onWheel (const CPoint& where, const CMouseWheelAxis& axis, const float& distance, const CButtonState& buttons);

	CLASS_METHODS(UIGenMDMDeviceSelector, CControl)

private:
	void refreshDeviceList();
	void applySelection();

	CTextLabel* _deviceLabel;
	CKickButton* _upButton;
	CKickButton* _downButton;
	GennyInterfaceObject* _owner;
	CPoint _position;

	std::vector<std::string> _deviceNames;
	int _selectedIndex; //0 = none/"-", 1..N = _deviceNames[index-1]
};
