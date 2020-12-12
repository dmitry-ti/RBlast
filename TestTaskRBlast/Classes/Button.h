#pragma once

#include "cocos2d.h"

typedef std::function<void()> ccButtonCallback;

class Button : public cocos2d::Node
{
public:
	virtual bool init();
	virtual void setContentSize(const cocos2d::Size& contentSize);
	void setStateColors(const cocos2d::Color4F& idleColor, const cocos2d::Color4F& pushedColor, const cocos2d::Color4F& dragoutColor);
	void setExpandZone(float topMargin, float rightMargin, float bottomMargin, float leftMargin);
	void setSafeZone(float topMargin, float rightMargin, float bottomMargin, float leftMargin);
	void setCallback(const ccButtonCallback& callback);
	void setDebugMode(bool value);
	virtual cocos2d::Rect getBoundingBox() const;

	Button();
	virtual ~Button();

public:
	static Button* create();

protected:
	virtual void initEvents();
	virtual void setState(int state);
	virtual void updateRects();
	cocos2d::Rect getExpandZoneRect() const;
	cocos2d::Rect getSafeZoneRect() const;

protected:
	cocos2d::Color4F _idleColor;
	cocos2d::Color4F _pushedColor;
	cocos2d::Color4F _dragoutColor;
	cocos2d::DrawNode *_rectsNode;
	float _safeZoneLeftMargin;
	float _safeZoneRightMargin;
	float _safeZoneTopMargin;
	float _safeZoneBottomMargin;
	float _expandZoneLeftMargin;
	float _expandZoneRightMargin;
	float _expandZoneTopMargin;
	float _expandZoneBottomMargin;
	int _state;
	ccButtonCallback _callback;
	bool _isDebugMode;

protected:
	static const int STATE_IDLE = 0;
	static const int STATE_PUSHED = 1;
	static const int STATE_DRAGOUT = 2;
};

