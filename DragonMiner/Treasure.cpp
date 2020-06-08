#include "stdafx.h"
#include "Treasure.h"

Treasure::Treasure(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
	mType = rand() % 3;	
}

treasure Treasure::mTreasure;

void Treasure::DeleteImage()
{
	mTreasure.bmp.DeleteObject();
}

void Treasure::LoadImage()
{
	BITMAP mineBMP;
	mTreasure.bmp.LoadBitmap(IDB_TREASURE);
	mTreasure.bmp.GetBitmap(&mineBMP);
	mTreasure.height = mineBMP.bmHeight;
	mTreasure.width = mineBMP.bmWidth;
	mTreasure.score = rand()%400+100;	//�������Ϊ100-500
	mTreasure.weight = rand()%680+20;	//�������Ϊ20-700(��ʯ~���������)
}

CRect Treasure::GetRect()
{
	if (rotaryRect.Width())	//����Ѿ���ת����,rotaryRect�Ŀ�Ͳ�Ϊ0
		return rotaryRect;	//������ת����ͼ�ξ�����Ϣ.
	else
		return CRect(mPointPos.x, mPointPos.y, mPointPos.x + mTreasure.width, mPointPos.y + mTreasure.height);
}

void Treasure::Draw(CDC * pDC)
{
	CDC memDC;

	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&mTreasure.bmp);

	pDC->TransparentBlt(mPointPos.x, mPointPos.y, mTreasure.width, mTreasure.height, &memDC, 0, 0, mTreasure.width, mTreasure.height, RGB(255, 255, 255));

	memDC.SelectObject(old);
}


Treasure::~Treasure()
{
}
