/**
 * GameView.h
 * 游戏视图类，负责整个游戏UI的展示
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CardView.h"
#include "../models/GameModel.h"

// 前置声明，避免循环引用
class GameController;
class UndoManager;

/**
 * 游戏视图类，管理游戏UI展示
 */
class GameView : public cocos2d::Node
{
public:
    // 将GameController和UndoManager设为友元类，使其能够访问私有成员
    friend class GameController;
    friend class UndoManager;
    
    /**
     * 创建游戏视图
     * @param model 游戏数据模型
     * @return 游戏视图对象
     */
    static GameView* create(const GameModel* model);
    
    /**
     * 初始化游戏视图
     * @param model 游戏数据模型
     * @return 是否初始化成功
     */
    virtual bool init(const GameModel* model);
    
    /**
     * 设置游戏控制器引用
     * @param controller 游戏控制器指针
     */
    void setGameController(GameController* controller) { _gameController = controller; }
    
    /**
     * 设置主牌区卡牌点击回调
     * @param callback 点击回调函数
     */
    void setOnPlayfieldCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * 获取主牌区卡牌点击回调
     * @return 点击回调函数
     */
    std::function<void(int)> getCardClickCallback() const;
    
    /**
     * 设置备用牌堆点击回调
     * @param callback 点击回调函数
     */
    void setOnStackClickCallback(const std::function<void()>& callback);
    
    /**
     * 设置回退按钮点击回调
     * @param callback 点击回调函数
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);
    
    /**
     * 更新手牌区顶部卡牌
     * @param card 卡牌模型
     */
    void updateTrayTopCard(const CardModel* card);
    
    /**
     * 播放卡牌从主牌区移动到手牌区的动画
     * @param cardId 要移动的卡牌ID
     */
    void playCardMoveToTrayAnimation(int cardId);
    
    /**
     * 播放卡牌直接覆盖手牌的动画
     * @param cardId 要移动的卡牌ID
     */
    void playDirectCoverAnimation(int cardId);
    
    /**
     * 播放从备用牌堆到手牌区的动画
     */
    void playStackToTrayAnimation();
    
    /**
     * 播放卡牌从手牌区移动到目标位置的动画
     * @param targetPos 目标位置
     * @param callback 动画完成回调
     */
    void playTrayToPositionAnimation(const cocos2d::Vec2& targetPos, const std::function<void()>& callback = nullptr);
    
    /**
     * 移除主牌区卡牌
     * @param cardId 卡牌ID
     */
    void removePlayfieldCard(int cardId);
    
    /**
     * 使用新的卡牌视图更新手牌区
     * @param cardView 新的卡牌视图
     */
    void setTrayTopCardView(CardView* cardView);
    
    /**
     * 启用/禁用主牌区卡牌交互
     * @param enabled 是否启用交互
     */
    void setPlayfieldCardsInteractive(bool enabled);
    
    /**
     * 启用/禁用备用牌堆交互
     * @param enabled 是否启用交互
     */
    void setStackInteractive(bool enabled);
    
    /**
     * 启用/禁用回退按钮
     * @param enabled 是否启用
     */
    void setUndoButtonEnabled(bool enabled);
    
private:
    const GameModel* _model;                     // 游戏数据模型
    GameController* _gameController;             // 游戏控制器
    std::map<int, CardView*> _playfieldCardViews; // 主牌区卡牌视图
    CardView* _trayTopCardView;                  // 手牌区顶部卡牌视图
    cocos2d::Node* _stackNode;                   // 备用牌堆节点
    cocos2d::ui::Button* _undoButton;            // 回退按钮
    
    cocos2d::Node* _playfieldLayer;              // 主牌区层
    cocos2d::Node* _trayLayer;                   // 手牌区层
    cocos2d::Node* _stackLayer;                  // 备用牌堆层
    
    std::function<void(int)> _cardClickCallback;  // 卡牌点击回调函数
    
    /**
     * 初始化游戏区域
     */
    void initGameAreas();
    
    /**
     * 初始化主牌区卡牌
     */
    void initPlayfieldCards();
    
    /**
     * 初始化手牌区
     */
    void initTray();
    
    /**
     * 初始化备用牌堆
     */
    void initStack();
    
    /**
     * 初始化UI控件
     */
    void initUI();
};

#endif // __GAME_VIEW_H__ 