//
//  PokerCard.cpp
//  MyGame
//
//  Created by ivan.wang on 2019/4/10.
//

#include "PokerCard.hpp"
#include "HelloWorldScene.h"

PokerCard::PokerCard(const int cardNum)
{
    Node::init();
    _cardNum = cardNum;
    _isBackSide = true;
    _isPaired = false;
    _isRunAction = false;
    
    auto cardBackSprite = cocos2d::Sprite::create("res/PokerCard/card_back.png");
    _cardMenuItemSprite = cocos2d::MenuItemSprite::create(cardBackSprite, cardBackSprite);
    _cardMenuItemSprite->setScale(0.4f);
    
    _cardMenuItemSprite->setCallback(CC_CALLBACK_0(PokerCard::runFlip, this));
    _cardMenu = (cocos2d::Menu::create(_cardMenuItemSprite, nullptr));
    _cardMenu->setPosition(cocos2d::Vec2::ZERO);
    
    this->setContentSize(_cardMenuItemSprite->getBoundingBox().size);
    this->addChild(_cardMenu);
}

PokerCard* PokerCard::create(const int cardNum)
{
    PokerCard *pokerCard = new (std::nothrow) PokerCard(cardNum);
    
    if (pokerCard)
    {
        pokerCard->autorelease();
        return pokerCard;
    }
    
    CC_SAFE_DELETE(pokerCard);
    return nullptr;
}

PokerCard::~PokerCard()
{
}

const int PokerCard::getCardNum()
{
    return _cardNum;
}

const bool PokerCard::getIsBackSide()
{
    return _isBackSide;
}

void PokerCard::setIsBackSide(const bool isBackSide)
{
    _isBackSide = isBackSide;
}

const bool PokerCard::getIsPaired()
{
    return _isPaired;
}

void PokerCard::setIsPaired(const bool isPaired)
{
    _isPaired = isPaired;
}

void PokerCard::setCallBack(std::function<void (PokerCard *card)> callBack){
    _callBack = callBack;
}

void PokerCard::flipEffect()
{
    if(this->getIsBackSide())
    {
        auto cardFrontSprite = cocos2d::Sprite::create("res/PokerCard/c_b_1" + std::to_string(this->getCardNum()) + ".png");
        _cardMenuItemSprite->setNormalImage(cardFrontSprite);
        this->setIsBackSide(!this->getIsBackSide());
    }
    else
    {
        auto cardFrontSprite = cocos2d::Sprite::create("res/PokerCard/card_back.png");
        _cardMenuItemSprite->setNormalImage(cardFrontSprite);
        this->setIsBackSide(!this->getIsBackSide());
    }
}

void PokerCard::runFlip()
{
    if(false == _isPaired && false == _isRunAction)
    {
        auto backSpriteHide = cocos2d::ScaleTo::create(0.2f, 0.0f, 0.4f);
        auto frontSpriteShow = cocos2d::ScaleTo::create(0.2f, 0.4f, 0.4f);
        
        _isRunAction = true;
        _cardMenuItemSprite->runAction(cocos2d::Sequence::create(
            backSpriteHide,
            cocos2d::CallFunc::create(
                [this](){
                    this->flipEffect();
                }
            ),
            frontSpriteShow,
            cocos2d::CallFunc::create(
                [this](){
                    _isRunAction = false;
                    this->_callBack(this);
                }
            ),
            nullptr
            )
        );
    }
}
