#include "stdafx.h"
#include "Tnt.h"


Tnt::Tnt(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
}

CBitmap Tnt::tntBmp;

void Tnt::DeleteImage()
{
	tntBmp.DeleteObject();
}

void Tnt::LoadImage()
{
	tntBmp.LoadBitmap(IDB_TNT);
}

CRect Tnt::GetRect()
{
	return CRect(mPointPos.x, mPointPos.y, mPointPos.x + TNT_WIDTH, mPointPos.y + TNT_HEIGHT);
}

void Tnt::Draw(CDC * pDC)
{
	CDC memDC;

	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&tntBmp);

	pDC->TransparentBlt(mPointPos.x, mPointPos.y, TNT_WIDTH, TNT_HEIGHT, &memDC, 0, 0, TNT_WIDTH, TNT_HEIGHT, RGB(255, 255, 255));

	memDC.SelectObject(old);
}


Tnt::~Tnt()
{
}
