#pragma once
#include "MyObject.h"
class Score :
	public MyObject
{
public:
	
	Score(CPoint ptPos,  DWORD nColor);
	virtual ~Score();
	void Draw(CDC* pDC);
	CRect GetRect() { return NULL; }
	static int GetTotalScore() { return mTotalScore; }
	static int GetMyScore() { return mMyScore; }
	static int GetGameLevel() { return mGameLevel; }
	static void AddMyScore(int score) { mMyScore += score; }
	static void AddTotalScore(int score) { mTotalScore += score; }
	static void SetMyScore(int score) { mMyScore = score; }
	static void SetTotalScore(int score) { mTotalScore = score; }
	static void SetGameLevel(int level) { mGameLevel = level; }
	void SetIfDrawAddScore(int judge, int value) { ifDrawAddScore = judge; addScore = value; }

	//无用但不可删
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }

private:
	COLORREF mColor;
	static int mMyScore;
	static int mTotalScore;
	static int mGameLevel;
	bool ifDrawAddScore;	//是否绘制积分增加的画面,当主程序给的isDrawAddScore 和addScore均不为空时,绘制增加分数的画面
	int addScore;	//增加的分数
	int frame;	//绘制增加分数画面的帧数
};

