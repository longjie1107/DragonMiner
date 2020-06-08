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

	//���õ�����ɾ
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }

private:
	COLORREF mColor;
	static int mMyScore;
	static int mTotalScore;
	static int mGameLevel;
	bool ifDrawAddScore;	//�Ƿ���ƻ������ӵĻ���,�����������isDrawAddScore ��addScore����Ϊ��ʱ,�������ӷ����Ļ���
	int addScore;	//���ӵķ���
	int frame;	//�������ӷ��������֡��
};

