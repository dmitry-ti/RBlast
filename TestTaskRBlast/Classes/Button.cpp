#include "Button.h"

Button::Button()
: _idleColor(cocos2d::Color4F::GRAY)
, _pushedColor(cocos2d::Color4F::GRAY)
, _dragoutColor(cocos2d::Color4F::GRAY)
, _rectsNode(nullptr)
, _safeZoneLeftMargin(0.0f)
, _safeZoneRightMargin(0.0f)
, _safeZoneTopMargin(0.0f)
, _safeZoneBottomMargin(0.0f)
, _expandZoneLeftMargin(0.0f)
, _expandZoneRightMargin(0.0f)
, _expandZoneTopMargin(0.0f)
, _expandZoneBottomMargin(0.0f)
, _state(STATE_IDLE)
, _callback(nullptr)
, _isDebugMode(false)
{

}

Button::~Button()
{

}

bool Button::init()
{
    _rectsNode = cocos2d::DrawNode::create();
	addChild(_rectsNode);
    updateRects();

    initEvents();
	
	return true;
}

void Button::initEvents()
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
        }

        return true;
    };
    listener->onTouchMoved = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (_state == STATE_IDLE)
        {
            return;
        }

        if (!RectApplyAffineTransform(getSafeZoneRect(), getNodeToParentAffineTransform()).containsPoint(touch->getLocation()))
        {
            setState(STATE_IDLE);
            return;
        }

        if (RectApplyAffineTransform(getExpandZoneRect(), getNodeToParentAffineTransform()).containsPoint(touch->getLocation()))
        {
            setState(STATE_PUSHED);
        }
        else
        {
            setState(STATE_DRAGOUT);
        }
    };
    listener->onTouchEnded = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (_state == STATE_PUSHED)
        {
            cocos2d::EventCustom pressEvent("ButtonPress");
            _eventDispatcher->dispatchEvent(&pressEvent);
            _callback();
        }

        setState(STATE_IDLE);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Button::setContentSize(const cocos2d::Size& size)
{
    Node::setContentSize(size);

    updateRects();
}

void Button::setStateColors(const cocos2d::Color4F& idleColor, const cocos2d::Color4F& pushedColor, const cocos2d::Color4F& dragoutColor)
{
    _idleColor = idleColor;
    _pushedColor = pushedColor;
    _dragoutColor = dragoutColor;

    updateRects();
}

void Button::setExpandZone(float topMargin, float rightMargin, float bottomMargin, float leftMargin)
{
    _expandZoneTopMargin = (topMargin >= 0) ? topMargin : 0;
    _expandZoneRightMargin = (rightMargin >= 0) ? rightMargin : 0;
    _expandZoneBottomMargin = (bottomMargin >= 0) ? bottomMargin : 0;
    _expandZoneLeftMargin = (leftMargin >= 0) ? leftMargin : 0;

    updateRects();
}

void Button::setSafeZone(float topMargin, float rightMargin, float bottomMargin, float leftMargin)
{
    _safeZoneTopMargin = (topMargin >= 0) ? topMargin : 0;
    _safeZoneRightMargin = (rightMargin >= 0) ? rightMargin : 0;
    _safeZoneBottomMargin = (bottomMargin >= 0) ? bottomMargin : 0;
    _safeZoneLeftMargin = (leftMargin >= 0) ? leftMargin : 0;

    updateRects();
}

void Button::setCallback(const ccButtonCallback& callback)
{
    _callback = callback;
}

void Button::setDebugMode(bool value)
{
    _isDebugMode = value;

    updateRects();
}

Button* Button::create()
{
    Button* ret = new (std::nothrow) Button();
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

cocos2d::Rect Button::getBoundingBox() const
{
    const cocos2d::Rect rect(
        -_safeZoneLeftMargin,
        -_safeZoneBottomMargin,
        _contentSize.width + _safeZoneLeftMargin + _safeZoneRightMargin,
        _contentSize.height + _safeZoneTopMargin + _safeZoneBottomMargin);
    return RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
}

void Button::setState(int state)
{
    if (_state == state)
    {
        return;
    }
    _state = state;
    updateRects();
}

void Button::updateRects()
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

cocos2d::Rect Button::getExpandZoneRect() const
{
    return cocos2d::Rect(
        -_expandZoneLeftMargin,
        -_expandZoneBottomMargin,
        _contentSize.width + _expandZoneLeftMargin + _expandZoneRightMargin,
        _contentSize.height + _expandZoneTopMargin + _expandZoneBottomMargin);
}

cocos2d::Rect Button::getSafeZoneRect() const
{
    return cocos2d::Rect(
        -_safeZoneLeftMargin,
        -_safeZoneBottomMargin,
        _contentSize.width + _safeZoneLeftMargin + _safeZoneRightMargin,
        _contentSize.height + _safeZoneTopMargin + _safeZoneBottomMargin);
}

