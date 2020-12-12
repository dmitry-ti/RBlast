#pragma once

#include "Button.h"

class LongPushButton : public Button
{
public:
	void setStateColors(const cocos2d::Color4F& idleColor, const cocos2d::Color4F& pushedColor, const cocos2d::Color4F& pushedLongColor, const cocos2d::Color4F& dragoutColor);
	void setTimeout(float timeout);

	LongPushButton();
	virtual ~LongPushButton();

public:
	static LongPushButton* create();

protected:
	virtual void initEvents();
	virtual void setState(int state);
	void updateLongPush();
	virtual void updateRects();

protected:
	cocos2d::Color4F _pushedLongColor;
	float _timeout;

protected:
	static const int STATE_PUSHED_LONG = 3;
};

