//
//  MyPokerCard.hpp
//
//  Created by ivan.wang on 2019/4/19.
//

#ifndef MyPokerCard_hpp
#define MyPokerCard_hpp

#include "cocos2d.h"

USING_NS_CC;

class MyPokerCard : public Node
{
private:
    int _cardNum;                                       //卡片數字
    bool _isBackSide;                                   //卡片是否在背面
    bool _isPaired;                                     //卡片是否已配對
    bool _isRunAction;                                  //卡片是否正在執行動畫
    MenuItemSprite* _cardMenuItemSprite;                //卡片的MenuItemSprite
    std::function<void(MyPokerCard *card)> _callBack;   //卡片的callback funcation
    
public:
    MyPokerCard(const int cardNum);
    ~MyPokerCard();
    static MyPokerCard* create(const int cardNum);
    
    const int getCardNum();                                                 //回傳卡片數值
    
    const bool getIsBackSide();                                             //回傳卡片是否在背面
    const bool getIsPaired();                                               //回傳卡片是否被配對
    
    void setIsBackSide(const bool isBackSide);                              //設定卡片是否在背面
    void setIsPaired(const bool isPaired);                                  //設定卡片是否被配對
    void setCallBack(std::function<void(MyPokerCard *card)> callBack);      //設定偵測是否配對callback function
    void flipEffect();                                                      //卡片顯示畫面設定
    void runFlip();                                                         //卡片翻轉動作
};

#endif /* MyPokerCard_hpp */
