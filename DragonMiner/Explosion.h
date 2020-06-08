#pragma once
#include "MyObject.h"

#define EXPLOSION_WIDTH		248
#define EXPLOSION_HEIGHT	252

class Explosion :
	public MyObject
{
public:
	Explosion(int x,int y);
	~Explosion();
	static void DeleteImage();
	static void LoadImage();

	CRect GetRect() {
		return CRect(mPointPos,
			CSize(EXPLOSION_WIDTH, EXPLOSION_HEIGHT));
	}
	void Draw(CDC* pDC);
	bool GetIsOver(){ return frame>15?true:false; }	//如果显示已经到达所有帧，那么返回True，通知主函数，删除爆炸对象。

													//无用但不可删
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }

private:
	int frame;		//绘制至第15帧时销毁
	static CImageList mImages;
};

