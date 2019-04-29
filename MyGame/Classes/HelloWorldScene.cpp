/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
    );
    
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    //產生重置按鈕
    auto resetButton = cocos2d::Label::createWithTTF("Reset", "fonts/arial.ttf", 12);
    resetButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - resetButton->getBoundingBox().size.width, Director::getInstance()->getVisibleSize().height - resetButton->getBoundingBox().size.height));
    this->addChild(resetButton);
    
    //設定重置按鈕Listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::resetSceneTouchEvent, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, resetButton);
    
    //建立PokerCard
    this->createPokerCard();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* sender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::createPokerCard()
{
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 產生亂數陣列
    std::vector<int> randomNumber;
    for(int i = 0; i < 20; i++)
        randomNumber.push_back(i % 10);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(randomNumber.begin(), randomNumber.end(), std::default_random_engine(seed));
    
    //PokerCard Default position
    Vec2 cardPosition = Vec2(50, visibleSize.height - 50);
    
    for (int i = 0; i < 20; i++)
    {
        auto card = PokerCard::create(randomNumber.at(i));
        if(i % 5 == 0 && i != 0)
        {
            cardPosition.y -= card->getContentSize().height * 1.5;
        }
        cardPosition.x = 50 + card->getContentSize().width * 2 * (i % 5);
        card->setPosition(cardPosition);
        card->setCallBack(CC_CALLBACK_0(HelloWorld::detectIsPaired, this, card));
        this->addChild(card);
        _cards.pushBack(card);
    }
}

void HelloWorld::detectIsPaired(PokerCard* card)
{
    //如果不是顯示背面且沒被配對，加到_cardsFilped中
    if(!(card->getIsBackSide()) && !(card->getIsPaired()))
    {
        _cardsFilped.pushBack(card);
    }
    //同一張又被點擊(顯示背面且沒被配對)，從_cardsFilped移除
    else if (card->getIsBackSide() && !(card->getIsPaired()))
    {
        _cardsFilped.eraseObject(card);
    }
    
    //如果_cardsFilped如果裡面有兩張代表要判斷是否有被配對
    if(2 == _cardsFilped.size())
    {
        if(_cardsFilped.at(0)->getCardNum() == _cardsFilped.at(1)->getCardNum()){
            _cardsFilped.at(0)->setIsPaired(true);
            _cardsFilped.at(1)->setIsPaired(true);
        }
        else
        {
            _cardsFilped.at(0)->runFlip();
            _cardsFilped.at(1)->runFlip();
        }
        _cardsFilped.clear();
    }
}

bool HelloWorld::resetSceneTouchEvent(cocos2d::Touch* touch, cocos2d::Event* event)
{
    cocos2d::Vec2 p = touch->getLocation();
    cocos2d::Rect rect = event->getCurrentTarget()->getBoundingBox();
    if(rect.containsPoint(p))
    {
        for(PokerCard* card:_cards){
            this->removeChild(card);
        }
        _cardsFilped.clear();
        _cards.clear();
        this->createPokerCard();
        return true;
    }
    return false;
}
