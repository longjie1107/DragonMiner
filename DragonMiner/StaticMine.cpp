#include "stdafx.h"
#include "StaticMine.h"


StaticMine::StaticMine(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
	mType = rand() % 6;	//���ֿ����е����һ��
	isCatch = 0;
}

staticMine StaticMine::mStaticMine[6];
int StaticMine::score[6] = { 500,500,300,100,100,30 };
int StaticMine::weight[6] = { 20,700,300,100,700,100 };

void StaticMine::DeleteImage()
{
	for(int i=0;i<6;i++)
		mStaticMine[i].bmp.DeleteObject();
}

//����ÿ�־�̬�����ͼƬ����������
void StaticMine::LoadImage()
{
	BITMAP mineBMP;
	mStaticMine[0].bmp.LoadBitmap(IDB_DIAMOND);
	mStaticMine[1].bmp.LoadBitmap(IDB_LARGEGOLD);
	mStaticMine[2].bmp.LoadBitmap(IDB_MIDGOLD);
	mStaticMine[3].bmp.LoadBitmap(IDB_LITTLEGOLD);
	mStaticMine[4].bmp.LoadBitmap(IDB_BIGSTONE);
	mStaticMine[5].bmp.LoadBitmap(IDB_STONE);

	for (int i = 0; i < 6; i++) {
		mStaticMine[i].bmp.GetBitmap(&mineBMP);
		mStaticMine[i].height = mineBMP.bmHeight;
		mStaticMine[i].width = mineBMP.bmWidth;
		mStaticMine[i].score = score[i];
		mStaticMine[i].weight = weight[i];
	}
}

CRect StaticMine::GetRect()
{
	if (rotaryRect.Width())	//����Ѿ���ת����,rotaryRect�Ŀ�Ͳ�Ϊ0
		return rotaryRect;	//������ת����ͼ�ξ�����Ϣ.
	else
		return CRect(mPointPos.x, mPointPos.y, mPointPos.x + mStaticMine[mType].width, mPointPos.y + mStaticMine[mType].height);
}

void StaticMine::Draw(CDC * pDC)
{
	CDC memDC;

	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&mStaticMine[mType].bmp);

	pDC->TransparentBlt(mPointPos.x, mPointPos.y, mStaticMine[mType].width, mStaticMine[mType].height, &memDC, 0, 0, mStaticMine[mType].width, mStaticMine[mType].height, RGB(255, 255, 255));

	memDC.SelectObject(old);

}

StaticMine::~StaticMine()
{
}
