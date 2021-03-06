
// DragonMinerView.h: CDragonMinerView 类的接口
//

#pragma once
#include "Score.h"


class CDragonMinerView : public CView
{
private:
	//	0: 龙宝
	//	1: 钩子
	//	2: 特殊矿物:猪与钻石猪,可左右移动.
	//	3: 特殊矿物:TNT,抓到即爆炸,周围一定范围内的矿物消失
	//	4: 特殊矿物:宝藏,随机获得以下奖励之一:1.大力水 抓矿物速率增快 2.大金矿以上钻石以下的奖金 3.点石成金,当前关卡石头价格变为原来的5倍.
	//	5: 静止的矿物:有钻石,大小石头,大中小金矿六种.
	//  6. 爆炸:显示爆炸效果
	CObList mObjects[7];  //CObList不是Vector，因为管理CObject派生的对象
	// mObjects[0]: ->Dragon
	// mObjects[1]: ->Hook
	// mObjects[2]: ->Pig1->DiamondPig1->Pig2...
	CDC memDC;		//双缓冲用DC
	int activityMode;	//活动模式(主界面,游戏界面,设置界面与简介界面)
	int mouseX, mouseY;	//鼠标点击坐标X与Y
	bool isGamePause,isGameOver, isMusicOn, isSoundEffectsOn,isAutoModeOn,isLegendModeOn;	//判断游戏是否停止,结束,是否开启音乐,音效,自动挖矿模式,一键挖矿模式.
	bool isGoNextLevel;	//判断是否可以前往下一关
	bool isGetStrenthBuff;	//是否在当前关卡获得了大力Buff
	bool isGetMoneyBuff;	//是否在当前关卡获得了金钱Buff
	bool ifReadSave;		//是否读档

	//int countStaticMine,countPigMine,countTreasure,countTnt;	//记数各种矿物数目
	int dMineHookCenter;	//矿物中心坐标与钩子中心坐标的距离
	int useForFirstTime;	//为了当矿物与钩子碰撞时,只调用一次计算偏移量的代码
	int mTimeLeft;		//剩余时间
	int useSkill,frame;		//使用大威天龙!   frame:绘制"大威天龙"的帧数
	Score *score;	//分数

protected: // 仅从序列化创建
	CDragonMinerView();
	DECLARE_DYNCREATE(CDragonMinerView)

// 特性
public:
	CDragonMinerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CDragonMinerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void DrawYes(CDC * pDC, int startX, int startY);	// 在指定位置画√
	void initGame();	//从主界面进入游戏界面时的初始化
	void initLevel();	//每个关卡的初始化
	bool isIntersectRect(CRect rect);
};

#ifndef _DEBUG  // DragonMinerView.cpp 中的调试版本
inline CDragonMinerDoc* CDragonMinerView::GetDocument() const
   { return reinterpret_cast<CDragonMinerDoc*>(m_pDocument); }
#endif

