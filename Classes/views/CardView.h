/**
 * CardView.h
 * 卡牌视图类，负责卡牌UI展示
 */

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"

/**
 * 卡牌视图类，负责显示卡牌UI
 */
class CardView : public cocos2d::Node
{
public:
    /**
     * 创建卡牌视图
     * @param model 卡牌数据模型
     * @return 卡牌视图对象
     */
    static CardView* create(const CardModel* model);
    
    /**
     * 初始化卡牌视图
     * @param model 卡牌数据模型
     * @return 是否初始化成功
     */
    virtual bool init(const CardModel* model);
    
    /**
     * 获取卡牌ID
     * @return 卡牌ID
     */
    int getCardId() const;
    
    /**
     * 播放移动动画
     * @param targetPos 目标位置
     * @param duration 动画持续时间
     * @param callback 动画完成回调
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPos, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * 播放从上到下的移动动画（桌面到手牌区）
     * @param targetPos 目标位置
     * @param duration 动画持续时间
     * @param callback 动画完成回调
     */
    void playMoveDownAnimation(const cocos2d::Vec2& targetPos, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * 设置卡牌是否可点击
     * @param enabled 是否可点击
     */
    void setTouchEnabled(bool enabled);
    
    /**
     * 设置点击回调
     * @param callback 点击回调函数
     */
    void setOnClickCallback(const std::function<void(int)>& callback);
    
private:
    const CardModel* _model;               // 卡牌数据模型
    cocos2d::Sprite* _cardSprite;          // 卡牌精灵
    bool _touchEnabled;                    // 是否可点击
    std::function<void(int)> _onClickCallback;  // 点击回调函数
    
    /**
     * 触摸事件监听器
     */
    void setupTouchEvents();
};

#endif // __CARD_VIEW_H__ 