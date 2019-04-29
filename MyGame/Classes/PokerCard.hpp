//
//  PokerCard.hpp
//  MyGame
//
//  Created by ivan.wang on 2019/4/10.
//

#ifndef PokerCard_hpp
#define PokerCard_hpp

#include <stdio.h>

class PokerCard : public cocos2d::Node
{
private:
    int _cardNum;
    bool _isBackSide;
    bool _isPaired;
    bool _isRunAction;
    cocos2d::MenuItemSprite* _cardMenuItemSprite;
    cocos2d::Menu* _cardMenu;
    std::function<void(PokerCard *card)> _callBack;
    
public:
    PokerCard(const int cardNum);
    ~PokerCard();
    static PokerCard* create(const int cardNum);
    
    //回傳卡片數值
    const int getCardNum();
    
    //回傳卡片是否在背面
    const bool getIsBackSide();
    //回傳卡片是否被配對
    const bool getIsPaired();
    
    //設定卡片是否在背面
    void setIsBackSide(const bool isBackSide);
    //設定卡片是否被配對
    void setIsPaired(const bool isPaired);
    //設定偵測是否配對callback function
    void setCallBack(std::function<void(PokerCard *card)> callBack);
    //卡片顯示畫面設定
    void flipEffect();
    //卡片翻轉動作
    void runFlip();
};
#endif /* PokerCard_hpp */
