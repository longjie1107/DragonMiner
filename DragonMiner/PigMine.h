#pragma once
#include "MyObject.h"

//对应mType
#define PIG 0
#define DIAMONDPIG 1

typedef struct pigMine {
	CBitmap bmp[2];	//储存猪向左走和向右走的图片
	int score;		//矿物分数
	int weight;		//矿物重量(影响速度)
	int width;		//矿物宽
	int height;		//矿物长
}pigMine;

class PigMine :
	public MyObject
{
public:
	static void DeleteImage();
	static void LoadImage();

	static pigMine mPigMine[2];	//共两种猪,普通猪和钻石猪.
	static CBitmap mPigBmp[2];	//储存猪的图片,仅用于GetMyBmp()函数

	CRect GetRect();
	void Draw(CDC* pDC);
	void Move();
	int GetScore() { return mPigMine[mType].score; }
	int GetWeight() { return mPigMine[mType].weight; }
	CBitmap * GetMyBmp() {  return &mPigBmp[mType]; }
	PigMine(int startX, int startY);
	~PigMine();

private:
	int mType;	//猪类型
	int frame;	//不同帧的动画
	int wait;	//每一帧延缓wait帧播放
	int vx;		//水平移动速度
	int orient;	//0表示向左,1表示向右.初始为向右.移动一定距离后变换.
	int startx, endx;	//x坐标到这两个位置时变换方位
};

