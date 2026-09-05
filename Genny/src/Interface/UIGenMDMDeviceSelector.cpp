#include "UIGenMDMDeviceSelector.h"

//This control drives GennyVST's direct Windows MIDI output (see
//GennyVST::setGenMDMDevice/getGenMDMDeviceNames), which only exists in the VST
//build - the FL native plugin path already has real per-port MIDI routing and
//doesn't need it.
#if BUILD_VST

#include "UIBitmap.h"
#include "../resource.h"
#include "IndexBaron.h"
#include "GennyVST.h"
#include "UIImage.h"
#include "UIInstrumentsPanel.h"

const int kGenMDMDeviceLabelWidth = 110;

UIGenMDMDeviceSelector::UIGenMDMDeviceSelector(CPoint point, GennyInterfaceObject* owner):
	CControl(CRect(point.x, point.y, point.x + kGenMDMDeviceLabelWidth + 20, point.y + 30), (IControlListener*)owner),
	GennyInterfaceObject(owner),
	_owner(owner),
	_position(point),
	_selectedIndex(0)
{

}

UIGenMDMDeviceSelector::~UIGenMDMDeviceSelector(void)
{

}

bool UIGenMDMDeviceSelector::attached (CView* parent)
{
	bool returnValue = CControl::attached(parent);

	CFrame* frame = parent->getFrame();

	float xoff = kGenMDMDeviceLabelWidth;
	_upButton = new CKickButton(CRect(_position.x + xoff, _position.y, _position.x + xoff + 16, _position.y + 8), this, 9999999, 8, UIBitmap(PNG_LITTLEUPARROW));
	frame->addView(_upButton);

	_downButton = new CKickButton(CRect(_position.x + xoff, _position.y + 10, _position.x + xoff + 16, _position.y + 10 + 8), this, 99999999, 8, UIBitmap(PNG_LITTLEDOWNARROW));
	frame->addView(_downButton);

	_deviceLabel = new CTextLabel(CRect(_position.x - 4, _position.y - 8, _position.x + kGenMDMDeviceLabelWidth, _position.y - 4 + 25), "-");
	_deviceLabel->setFont(kNormalFontBig);
	_deviceLabel->setHoriAlign(kLeftText);
	_deviceLabel->getFont()->setStyle(kBoldFace);
	_deviceLabel->setFontColor(CColor(16, 20, 16, 255));
	frame->addView(_deviceLabel);
	_deviceLabel->setMouseableArea(CRect());
	_deviceLabel->setBackColor(CColor(0, 0, 0, 0));
	_deviceLabel->setFrameColor(CColor(0, 0, 0, 0));

	refreshDeviceList();
	reconnect();

	return returnValue;
}

void UIGenMDMDeviceSelector::refreshDeviceList()
{
	_deviceNames = _vst->getGenMDMDeviceNames();
}

void UIGenMDMDeviceSelector::updateDisplay(int index)
{
	int maxIndex = (int)_deviceNames.size();
	if (index < 0)
		index = 0;
	if (index > maxIndex)
		index = maxIndex;

	_selectedIndex = index;

	if (_selectedIndex == 0)
	{
		_deviceLabel->setText("-");
	}
	else
	{
		std::string name = _deviceNames[_selectedIndex - 1];
		if (name.length() > 14)
			name = name.substr(0, 14) + "...";

		_deviceLabel->setText(name.c_str());
	}

	_deviceLabel->invalid();
}

void UIGenMDMDeviceSelector::setSelectedIndex(int index)
{
	updateDisplay(index);
	applySelection();
}

void UIGenMDMDeviceSelector::applySelection()
{
	_vst->setGenMDMDevice(_selectedIndex == 0 ? "" : _deviceNames[_selectedIndex - 1]);
}

void UIGenMDMDeviceSelector::valueChanged (CControl* control)
{
	if(control->getTag() == 9999999 && control->getValue() > 0.5f)
	{
		setSelectedIndex(_selectedIndex + 1);
	}
	else if(control->getTag() == 99999999 && control->getValue() > 0.5f)
	{
		setSelectedIndex(_selectedIndex - 1);
	}
}

void UIGenMDMDeviceSelector::draw (CDrawContext* pContext)
{
	CControl::setDirty(false);
}

void UIGenMDMDeviceSelector::reconnect()
{
	refreshDeviceList();

	const std::string& current = _vst->genMDMDeviceName;
	int foundIndex = 0;

	for (size_t i = 0; i < _deviceNames.size(); i++)
	{
		if (_deviceNames[i] == current)
		{
			foundIndex = (int)i + 1;
			break;
		}
	}

	updateDisplay(foundIndex);
}

void UIGenMDMDeviceSelector::setVisible(bool visible)
{
	_deviceLabel->setVisible(visible);
	_upButton->setVisible(visible);
	_downButton->setVisible(visible);
	CControl::setVisible(visible);
}

bool UIGenMDMDeviceSelector::onWheel (const CPoint& where, const CMouseWheelAxis& axis, const float& distance, const CButtonState& buttons)
{
	if(distance > 0)
		setSelectedIndex(_selectedIndex + 1);
	else if(distance < 0)
		setSelectedIndex(_selectedIndex - 1);

	return true;
}

#endif
