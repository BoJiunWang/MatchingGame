//
//  MyPokerCard.cpp
//
//  Created by ivan.wang on 2019/4/19.
//

#include "MyPokerCard.hpp"

MyPokerCard::~MyPokerCard(){}

MyPokerCard::MyPokerCard(const int cardNum)
{
    if (!Node::init())
        return;
    
    _cardNum = cardNum;
    _isBackSide = true;
    _isPaired = false;
    _isRunAction = false;
    
    auto cardBackSprite = Sprite::create("res/MyPokerCard/card_back.png");
    _cardMenuItemSprite = MenuItemSprite::create(cardBackSprite, cardBackSprite);
    _cardMenuItemSprite->setScale(0.4f);
    
    _cardMenuItemSprite->setCallback(CC_CALLBACK_0(MyPokerCard::runFlip, this));
    Menu* cardMenu = (Menu::create(_cardMenuItemSprite, nullptr));
    cardMenu->setPosition(Vec2::ZERO);
    
    this->setContentSize(_cardMenuItemSprite->getBoundingBox().size);
    this->addChild(cardMenu);
}

MyPokerCard* MyPokerCard::create(const int cardNum)
{
    MyPokerCard *MyPokerCard = new (std::nothrow) class MyPokerCard(cardNum);
    
    if (MyPokerCard)
    {
        MyPokerCard->autorelease();
        return MyPokerCard;
    }
    
    CC_SAFE_DELETE(MyPokerCard);
    return nullptr;
}

const int MyPokerCard::getCardNum()
{
    return _cardNum;
}

const bool MyPokerCard::getIsBackSide()
{
    return _isBackSide;
}

void MyPokerCard::setIsBackSide(const bool isBackSide)
{
    _isBackSide = isBackSide;
}

const bool MyPokerCard::getIsPaired()
{
    return _isPaired;
}

void MyPokerCard::setIsPaired(const bool isPaired)
{
    _isPaired = isPaired;
}

void MyPokerCard::setCallBack(std::function<void (MyPokerCard *card)> callBack){
    _callBack = callBack;
}

void MyPokerCard::flipEffect()
{
    if(this->getIsBackSide())
    {
        auto cardFrontSprite = Sprite::create("res/MyPokerCard/c_b_1" + std::to_string(this->getCardNum()) + ".png");
        _cardMenuItemSprite->setNormalImage(cardFrontSprite);
        this->setIsBackSide(!this->getIsBackSide());
    }
    else
    {
        auto cardFrontSprite = Sprite::create("res/MyPokerCard/card_back.png");
        _cardMenuItemSprite->setNormalImage(cardFrontSprite);
        this->setIsBackSide(!this->getIsBackSide());
    }
}

void MyPokerCard::runFlip()
{
    if(false == _isPaired && false == _isRunAction)
    {
        auto backSpriteHide = ScaleTo::create(0.2f, 0.0f, 0.4f);
        auto frontSpriteShow = ScaleTo::create(0.2f, 0.4f, 0.4f);
        
        _isRunAction = true;
        _cardMenuItemSprite->runAction(Sequence::create(
            backSpriteHide,
            CallFunc::create(
            [this](){
                this->flipEffect();
                }
            ),
            frontSpriteShow,
            CallFunc::create(
                [this](){
                    _isRunAction = false;
                    if (_callBack)
                        this->_callBack(this);
                    }
                ),
            nullptr
            )
        );
    }
}
