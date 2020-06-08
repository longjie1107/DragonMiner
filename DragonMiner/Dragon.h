#pragma once
#include "MyObject.h"

#define DRAGON_WIDTH	125
#define DRAGON_HEIGHT	65

class Dragon :
	public MyObject
{
public:
	Dragon(int startX, int startY);
	~Dragon();
	static void DeleteImage();
	static void LoadImage();
	static CBitmap dragonBmp;

	CRect GetRect() {
		return CRect(mPointPos ,
			CSize(DRAGON_WIDTH, DRAGON_HEIGHT));
	}
	void Draw(CDC* pDC);
	
	//无用但不可删
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }

private:
	int frame;		//控制帧数
	int wait;		//每一帧动画延缓wait帧播放
};

