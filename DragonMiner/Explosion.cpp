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
	//����ͼ����Դ
	CBitmap bmp;
	bmp.LoadBitmap(IDB_EXPLOSION);
	mImages.Create(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, ILC_COLOR24 | ILC_MASK, 15, 0);
	// ���ӱ�ըͼ��CImageList��
	mImages.Add(&bmp, RGB(255, 0, 255));

}

void Explosion::Draw(CDC * pDC)
{
	mImages.Draw(pDC, frame++, mPointPos, ILD_TRANSPARENT); //���Ƶ�ǰ֡�ı�ըͼ��
	// frame��¼��ը��֡���������ʾ���15֡�������ը����
}
