#include "stdafx.h"
#include "Dragon.h"


Dragon::Dragon(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
	frame = 0;
	wait = 4;	//每4帧动一次
}

CBitmap Dragon::dragonBmp;

Dragon::~Dragon()
{
}

void Dragon::DeleteImage()
{
	dragonBmp.DeleteObject();
}

void Dragon::LoadImage()
{
	dragonBmp.LoadBitmap(IDB_DRAGON);
}

void Dragon::Draw(CDC * pDC)
{
	CDC memDC;
	if (isCatch) {
		if (wait-- == 0) {
			frame = (frame + 1) % 4;	//做了四帧动画
			wait = 4;
		}
		memDC.CreateCompatibleDC(pDC);
		CBitmap* old = memDC.SelectObject(&dragonBmp);
		pDC->TransparentBlt(mPointPos.x, mPointPos.y, DRAGON_WIDTH, DRAGON_HEIGHT, &memDC, frame * DRAGON_WIDTH, 0, DRAGON_WIDTH, DRAGON_HEIGHT, RGB(255, 255, 255));
		memDC.SelectObject(old);
	}
	else {
		memDC.CreateCompatibleDC(pDC);
		CBitmap* old = memDC.SelectObject(&dragonBmp);
		pDC->TransparentBlt(mPointPos.x, mPointPos.y, DRAGON_WIDTH, DRAGON_HEIGHT, &memDC, 0, 0, DRAGON_WIDTH, DRAGON_HEIGHT, RGB(255, 255, 255));
		memDC.SelectObject(old);
	}
	
}
