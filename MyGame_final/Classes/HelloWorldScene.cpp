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

HelloWorld::~HelloWorld()
{
    GSCustomEventManager::getInstance()->removeAllCustomEventByTarget(this);
}

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
    if (!Scene::init())
    {
        return false;
    }

    _pairedAmount = 0;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "res/CloseNormal.png",
        "res/CloseSelected.png",
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
    auto resetButton = Label::createWithTTF("Reset", "fonts/arial.ttf", 12);
    resetButton->setPosition(Vec2(origin.x + visibleSize.width - resetButton->getContentSize().width - closeItem->getContentSize().width, origin.y + closeItem->getContentSize().height/2));
    this->addChild(resetButton);
    
    //設定重置按鈕Listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::resetSceneTouchEvent, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, resetButton);
    
    //設定分數文字
    _scoreLabel = GGNumberLabel::create("0", CodeUtility::getDefaultFontname(true), 20);
    _scoreLabel->setColor(Color3B::YELLOW);
    _scoreLabel->setUpdateTime(0.5f);
    _scoreLabel->setMaxLineWidth(30.0f);
    _scoreLabel->setPosition(Vec2(origin.x + visibleSize.width - 30, origin.y + visibleSize.height/2));
    this->addChild(_scoreLabel);
    
    GSCustomEventManager::getInstance()->addCustomEventListener("NotifyScoreChanged", CC_CALLBACK_1(HelloWorld::scoreChanged, this), this);
    
    this->createTimerLabel();
    
    //建立MyPokerCard
    this->createMyPokerCard();

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

void HelloWorld::createTimerLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _timeLabel = GSTimeLabel::create();
    _timeLabel->setTextColor(Color4B::WHITE);
    _timeLabel->setEndTime(300);
    _timeLabel->setString("00:05:00");
    _timeLabel->setPosition(Vec2(origin.x + visibleSize.width - _timeLabel->getContentSize().width, visibleSize.height - _timeLabel->getContentSize().height));
    _timeLabel->setCallback([this](){
        this->showTimeIsUpMessage();
        this->resetScene();
    });
    this->addChild(_timeLabel);
}

void HelloWorld::createMyPokerCard()
{
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 產生亂數陣列
    std::vector<int> randomNumber;
    for(int i = 0; i < 20; i++)
        randomNumber.push_back(i % 10);
    unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(randomNumber.begin(), randomNumber.end(), std::default_random_engine(seed));
    
    //MyPokerCard Default position
    Vec2 cardPosition = Vec2(150, visibleSize.height - 50);
    
    for (int i = 0; i < 20; i++)
    {
        auto card = MyPokerCard::create(randomNumber.at(i));
        if(i % 5 == 0 && i != 0)
        {
            cardPosition.y -= card->getContentSize().height * 1.2;
        }
        cardPosition.x = 150 + card->getContentSize().width * 1.5 *(i % 5);
        card->setPosition(cardPosition);
        card->setCallBack(CC_CALLBACK_0(HelloWorld::detectIsPaired, this, card));
        this->addChild(card);
        _cardsVector.pushBack(card);
    }
}

void HelloWorld::detectIsPaired(MyPokerCard* card)
{
    this->startTimer();

    //如果不是顯示背面且沒被配對，加到_cardsFlipped中
    if(!(card->getIsBackSide()) && !(card->getIsPaired()))
    {
        _cardsFlippedVector.pushBack(card);
    }
    //同一張又被點擊(顯示背面且沒被配對)，從_cardsFlipped移除
    else if (card->getIsBackSide() && !(card->getIsPaired()))
    {
        _cardsFlippedVector.eraseObject(card);
    }
    
    //如果_cardsFlipped如果裡面有兩張代表要判斷是否有被配對
    if(2 == _cardsFlippedVector.size())
    {
        cocos2d::Value value;
        if(_cardsFlippedVector.at(0)->getCardNum() == _cardsFlippedVector.at(1)->getCardNum()){
            _cardsFlippedVector.at(0)->setIsPaired(true);
            _cardsFlippedVector.at(1)->setIsPaired(true);
            _pairedAmount +=2;
            value = Value(+50);
        }
        else
        {
            _cardsFlippedVector.at(0)->runFlip();
            _cardsFlippedVector.at(1)->runFlip();
            value = Value(-10);
        }
        _cardsFlippedVector.clear();
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("NotifyScoreChanged", &value);
        if(_pairedAmount == _cardsVector.size())
        {
            this->stopTimer();
            this->showCompleteMessage();
        }
    }
}

void HelloWorld::scoreChanged(EventCustom* event)
{
    if (event->getUserData())
    {
        cocos2d::Value *eventValue = (cocos2d::Value*)event->getUserData();
        long long int value = _scoreLabel->getTotalNumber() + eventValue->asLongLongInt();
        
        if(0 < value)
        {
            _scoreLabel->setToNumber(value);
        }
        else
        {
            _scoreLabel->setToNumber(0);
        }
    }
}

void HelloWorld::resetScene()
{
    for(MyPokerCard* card:_cardsVector){
        this->removeChild(card);
    }
    this->removeChild(_timeLabel);
    
    _cardsFlippedVector.clear();
    _cardsVector.clear();
    _scoreLabel->setToNumber(0);
    _pairedAmount = 0;
    
    this->createTimerLabel();
    this->createMyPokerCard();
}

void HelloWorld::startTimer()
{
    _timeLabel->runTimer();
}

void HelloWorld::stopTimer()
{
    _timeLabel->stopTimer();
}

void HelloWorld::showTimeIsUpMessage()
{
    MessageBox("Game Over", "Time is up");
}

void HelloWorld::showCompleteMessage()
{
    MessageBox("Good Job", "Game Completed");
}

bool HelloWorld::resetSceneTouchEvent(Touch* touch, Event* event)
{
    Vec2 p = touch->getLocation();
    Rect rect = event->getCurrentTarget()->getBoundingBox();
    if(rect.containsPoint(p))
    {
        this->resetScene();
        return true;
    }
    return false;
}
