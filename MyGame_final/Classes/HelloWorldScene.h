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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "MyPokerCard.hpp"
#include "ToolBox/GGNumberLabel.h"
#include "ToolBox/GSTimeLabel.h"
#include "ToolBox/CodeUtility.h"

USING_NS_CC;

class HelloWorld : public Scene
{
public:
    ~HelloWorld();
    
    static Scene* createScene();
    virtual bool init();
    
    void createMyPokerCard();                                   //建立MyPokerCard物件並繪製出來
    void createTimerLabel();                                    //建立倒數計時器並繪製出來
    void menuCloseCallback(Ref* sender);                        //關閉程式callback function
    void detectIsPaired(MyPokerCard* card);                     //檢查_cardsFlipped中是否有配對成功的牌,若沒有翻回去
    void scoreChanged(EventCustom* event);                      //分數變動事件
    void resetScene();                                          //重設遊戲內容
    void startTimer();                                          //啟動倒數計時器
    void stopTimer();                                           //停止倒數計時器
    void showTimeIsUpMessage();                                 //顯示時間到的視窗
    void showCompleteMessage();                                 //顯示完成遊戲的視窗
    
    bool resetSceneTouchEvent(Touch* touch, Event* event);      //點擊重設按鈕事件
            
    CREATE_FUNC(HelloWorld);
    
private:
    Vector<MyPokerCard *> _cardsVector;             //存放MyPokerCard物件vector
    Vector<MyPokerCard *> _cardsFlippedVector;      //存放被翻開的MyPokerCard物件vector
    
    int _pairedAmount;                              //計算已完成配對的張數
    
    GGNumberLabel* _scoreLabel;                     //分數
    
    GSTimeLabel* _timeLabel;                        //倒數計時器
};

#endif // __HELLOWORLD_SCENE_H__
