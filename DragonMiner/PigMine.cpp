#include "stdafx.h"
#include "PigMine.h"

PigMine::PigMine(int startX, int startY)
{
	mPointPos.x = startX;
	mPointPos.y = startY;
	mType = rand() % 2;		//0普通猪,1钻石猪
	frame = 0;	//刚开始第0帧,然后0,1,0,1.....
	wait = 2;	//每帧延缓2帧播放
	vx = 5;	//小猪水平移动速度
	startx = startX;
	endx = startX + 400;	//向右移动400像素后转向
	orient = 1;		//初始向右
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
	//分别代表不同猪的分数与价值
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
	if (rotaryRect.Width())	//如果已经旋转过了,rotaryRect的宽就不为0
		return rotaryRect;	//返回旋转过的图形矩阵信息.
	else
	return CRect(mPointPos.x, mPointPos.y, mPointPos.x + mPigMine[mType].width, mPointPos.y + mPigMine[mType].height);
}

void PigMine::Draw(CDC * pDC)
{
	CDC memDC;
	if (wait-- == 0) {
		frame = (frame + 1) % 2;	//做了两帧动画
		wait = 2;
	}
	
	memDC.CreateCompatibleDC(pDC);
	CBitmap* old = memDC.SelectObject(&mPigMine[mType].bmp[orient]);
	pDC->TransparentBlt(mPointPos.x, mPointPos.y, mPigMine[mType].width, mPigMine[mType].height, &memDC, frame * mPigMine[mType].width, 0, mPigMine[mType].width, mPigMine[mType].height, RGB(255, 255, 255));
	memDC.SelectObject(old);
}

void PigMine::Move()
{
	if (mPointPos.x <= startx)	//当到达最左侧时,变换运动方位为向右
		orient = 1;		
	if (mPointPos.x >= endx)	//当到达最右侧时,变换运动方位为向左
		orient = 0;		
	if (orient == 1) 	//小猪向右运动
		mPointPos.x += vx;
	else if(orient == 0)
		mPointPos.x -= vx;
}



PigMine::~PigMine()
{
}
