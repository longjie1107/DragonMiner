#include "stdafx.h"
#include "PigMine.h"

PigMine::PigMine(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
	mType = rand() % 2;		//0��ͨ��,1��ʯ��
	frame = 0;	//�տ�ʼ��0֡,Ȼ��0,1,0,1.....
	wait = 2;	//ÿ֡�ӻ�2֡����
	vx = 5;	//С��ˮƽ�ƶ��ٶ�
	startx = startX;
	endx = startX + 400;	//�����ƶ�400���غ�ת��
	orient = 1;		//��ʼ����
}

pigMine PigMine::mPigMine[2];
CBitmap PigMine::mPigBmp[2];

void PigMine::DeleteImage()
{
	for (int i = 0; i<2; i++)
		for(int j=0;j<2;j++)
		mPigMine[i].bmp[j].DeleteObject();
	mPigBmp[PIG].DeleteObject();
	mPigBmp[DIAMONDPIG].DeleteObject();
}

void PigMine::LoadImage()
{
	BITMAP mineBMP;
	mPigMine[PIG].bmp[0].LoadBitmap(IDB_PIG);
	mPigMine[PIG].bmp[1].LoadBitmap(IDB_RPIG);
	mPigMine[DIAMONDPIG].bmp[0].LoadBitmap(IDB_DIAMONDPIG);
	mPigMine[DIAMONDPIG].bmp[1].LoadBitmap(IDB_RDIAMONDPIG);
	mPigBmp[PIG].LoadBitmap(IDB_SINGLEPIG);
	mPigBmp[DIAMONDPIG].LoadBitmap(IDB_SINGLEDPIG);
	//�ֱ����ͬ��ķ������ֵ
	int score[2] = { 100, 600 };
	int weight[2] = { 20, 20 };
	for (int i = 0; i < 2; i++) {
		mPigMine[i].bmp[0].GetBitmap(&mineBMP);
		mPigMine[i].height = mineBMP.bmHeight;
		mPigMine[i].width = mineBMP.bmWidth / 2;
		mPigMine[i].score = score[i];
		mPigMine[i].weight = weight[i];
	}
}

CRect PigMine::GetRect()
{
	if (rotaryRect.Width())	//����Ѿ���ת����,rotaryRect�Ŀ�Ͳ�Ϊ0
		return rotaryRect;	//������ת����ͼ�ξ�����Ϣ.
	else
	return CRect(mPointPos.x, mPointPos.y, mPointPos.x + mPigMine[mType].width, mPointPos.y + mPigMine[mType].height);
}

void PigMine::Draw(CDC * pDC)
{
	CDC memDC;
	if (wait-- == 0) {
		frame = (frame + 1) % 2;	//������֡����
		wait = 2;
	}
	
	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&mPigMine[mType].bmp[orient]);
	pDC->TransparentBlt(mPointPos.x, mPointPos.y, mPigMine[mType].width, mPigMine[mType].height, &memDC, frame * mPigMine[mType].width, 0, mPigMine[mType].width, mPigMine[mType].height, RGB(255, 255, 255));
	memDC.SelectObject(old);
}

void PigMine::Move()
{
	if (mPointPos.x <= startx)	//�����������ʱ,�任�˶���λΪ����
		orient = 1;		
	if (mPointPos.x >= endx)	//���������Ҳ�ʱ,�任�˶���λΪ����
		orient = 0;		
	if (orient == 1) 	//С�������˶�
		mPointPos.x += vx;
	else if(orient == 0)
		mPointPos.x -= vx;
}



PigMine::~PigMine()
{
}
