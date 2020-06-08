#include "stdafx.h"
#include "Explosion.h"


CImageList Explosion::mImages;

Explosion::Explosion(int x, int y)
{
	mPointPos.x = x;
	mPointPos.y = y;
	frame = 0;
}


Explosion::~Explosion()
{
}

void Explosion::DeleteImage()
{
	mImages.DeleteImageList();
}

void Explosion::LoadImage()
{
	//读入图像资源
	CBitmap bmp;
	bmp.LoadBitmap(IDB_EXPLOSION);
	mImages.Create(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ILC_COLOR24 | ILC_MASK, 15, 0);
	// 增加爆炸图像到CImageList中
	mImages.Add(&bmp, RGB(255, 0, 255));

}

void Explosion::Draw(CDC * pDC)
{
	mImages.Draw(pDC, frame++, mPointPos, ILD_TRANSPARENT); //绘制当前帧的爆炸图像
	// frame记录爆炸的帧数，如果显示完毕15帧则结束爆炸过程
}
