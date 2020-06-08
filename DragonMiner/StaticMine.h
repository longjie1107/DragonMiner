#pragma once
#include "MyObject.h"

//对应mType
#define DIAMOND 0
#define LARGEGOLD 1
#define MIDGOLD 2
#define LITTLEGOLD 3
#define BIGSTONE 4
#define STONE 5

typedef struct staticMine {
	CBitmap bmp;	//储存矿物图片
	int score;		//矿物分数
	int weight;		//矿物重量(影响速度)
	int width;		//矿物宽
	int height;		//矿物长
}staticMine;

class StaticMine :
	public MyObject
{
	
public:
	static void DeleteImage();
	static void LoadImage();

	static staticMine mStaticMine[6];	//共六类静止矿物,包括钻石,大中小金矿,大小石头.在初始化时需要加载位图并赋予属性值.
	static int score[6];	//分别代表钻石,大中小金矿,大小石头的分数与重量
	static int weight[6];

	StaticMine(int startX, int startY);
	~StaticMine();
	CRect GetRect();
	void Draw(CDC* pDC);
	int GetType() { return mType; }
	int GetScore() { return score[mType]; }
	int GetWeight() { return weight[mType]; }
	CBitmap * GetMyBmp() { return &mStaticMine[mType].bmp; }

private:
	int mType;
};

