#pragma once
#include "MyObject.h"

#define MONEY 0
#define STRENTH 1
#define LUCK 2

typedef struct treasure {
	CBitmap bmp;	//储存矿物图片
	int score;		//矿物分数
	int weight;		//矿物重量(影响速度)
	int width;		//矿物宽
	int height;		//矿物长
}treasure;

class Treasure :
	public MyObject
{
public:
	static void DeleteImage();
	static void LoadImage();

	static treasure mTreasure;	

	CRect GetRect();
	void Draw(CDC* pDC);
	int GetWeight() { return mTreasure.weight; }
	int GetType() { return mType; }
	int GetScore() { return mType == 0 ? mTreasure.score : 0; }		//如果里面是钱就有积分,否则没积分
	CBitmap * GetMyBmp() { return &mTreasure.bmp; }
	Treasure(int startX, int startY);
	~Treasure();

private:
	int mType;		//0为普通财物,1为大力水--抓任何东西都轻而易举,2为幸运草--所有财物的价值翻倍
};

