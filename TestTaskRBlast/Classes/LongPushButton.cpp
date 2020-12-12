#include "LongPushButton.h"

LongPushButton::LongPushButton()
: Button()
, _pushedLongColor(cocos2d::Color4F::GRAY)
, _timeout(0)
{

}

LongPushButton::~LongPushButton()
{

}

void LongPushButton::setTimeout(float timeout)
{
    _timeout = timeout;
}

void LongPushButton::setStateColors(const cocos2d::Color4F& idleColor, const cocos2d::Color4F& pushedColor, const cocos2d::Color4F& pushedLongColor, const cocos2d::Color4F& dragoutColor)
{
    _idleColor = idleColor;
    _pushedColor = pushedColor;
    _pushedLongColor = pushedLongColor;
    _dragoutColor = dragoutColor;

    updateRects();
}

void LongPushButton::initEvents()
{
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (!this->getBoundingBox().containsPoint(touch->getLocation()))
        {
            return false;
        }

        if (RectApplyAffineTransform(getExpandZoneRect(), getNodeToParentAffineTransform()).containsPoint(touch->getLocation()))
        {
            setState(STATE_PUSHED);
            updateLongPush();
        }

        return true;
    };
    listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (_state == STATE_IDLE)
        {
            return;
        }

        if (! RectApplyAffineTransform(getSafeZoneRect(), getNodeToParentAffineTransform()).containsPoint(touch->getLocation()))
        {
            setState(STATE_IDLE);
            return;
        }

        if (RectApplyAffineTransform(getExpandZoneRect(), getNodeToParentAffineTransform()).containsPoint(touch->getLocation()))
        {
            if (_state == STATE_DRAGOUT)
            {
                setState(STATE_PUSHED);
                updateLongPush();
            }
        }
        else
        {
            setState(STATE_DRAGOUT);
        }
    };
    listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (_state == STATE_PUSHED_LONG)
        {
            cocos2d::EventCustom pressEvent("LongPushButtonPress");
            _eventDispatcher->dispatchEvent(&pressEvent);
            _callback();
        }

        setState(STATE_IDLE);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void LongPushButton::setState(int state)
{
    if (_state == state)
    {
        return;
    }
    _state = state;
    stopActionByTag(0);
    updateRects();
}

void LongPushButton::updateLongPush()
{   
    auto delayAction = cocos2d::DelayTime::create(_timeout);
    auto longPushAction = cocos2d::CallFunc::create([&]() {
        setState(STATE_PUSHED_LONG);
    });
    auto sequence = cocos2d::Sequence::create(delayAction, longPushAction, NULL);
    auto action = runAction(sequence);
    stopActionByTag(0);
    action->setTag(0);
}

void LongPushButton::updateRects()
{
    _rectsNode->clear();
    cocos2d::Color4F buttonColor;
    switch (_state) {
    case STATE_IDLE:
        buttonColor = _idleColor;
        break;
    case STATE_PUSHED:
        buttonColor = _pushedColor;
        break;
    case STATE_DRAGOUT:
        buttonColor = _dragoutColor;
        break;
    case STATE_PUSHED_LONG:
        buttonColor = _pushedLongColor;
        break;
    default:
        buttonColor = cocos2d::Color4F::GRAY;
        cocos2d::log("Unexpected button state!");
    }
    _rectsNode->drawSolidRect(cocos2d::Vec2(0, 0), cocos2d::Vec2(_contentSize.width, _contentSize.height), buttonColor);

    if (_isDebugMode)
    {
        _rectsNode->drawRect(getExpandZoneRect().origin, cocos2d::Vec2(getExpandZoneRect().getMaxX(), getExpandZoneRect().getMaxY()), cocos2d::Color4F::BLUE);
        _rectsNode->drawRect(getSafeZoneRect().origin, cocos2d::Vec2(getSafeZoneRect().getMaxX(), getSafeZoneRect().getMaxY()), cocos2d::Color4F::GRAY);
    }
}

LongPushButton* LongPushButton::create()
{
    LongPushButton* ret = new (std::nothrow) LongPushButton();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}