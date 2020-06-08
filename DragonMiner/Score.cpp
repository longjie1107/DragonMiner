#include "stdafx.h"
#include "Score.h"


int Score::mTotalScore = 0;
int Score::mMyScore = 0;
int Score::mGameLevel = 0;
Score::Score(CPoint ptPos, DWORD nColor)
{
	mPointPos = ptPos;
	mColor = nColor;
	ifDrawAddScore = 0;
	addScore = 0;
	frame = 30;
}


Score::~Score()
{
}

void Score::Draw(CDC * pDC)
{
	CFont font;
	font.CreateFont(24,                                    //   字体的高度   
		0,                                          //   字体的宽度  
		0,                                          //  nEscapement 
		0,                                          //  nOrientation   
		FW_NORMAL,                                  //   nWeight   
		FALSE,                                      //   bItalic   
		FALSE,                                      //   bUnderline   
		0,                                                   //   cStrikeOut   
		ANSI_CHARSET,                             //   nCharSet   
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
		DEFAULT_QUALITY,                       //   nQuality   
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		_T("微软雅黑"));
	CFont * oldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(mColor);
	pDC->SetTextAlign(TA_LEFT);	 //向左对齐
	pDC->SetBkMode(TRANSPARENT); //背景透明
	CString strScore;
	
	
	if (ifDrawAddScore == 1) {	//抓到了东西
		if (frame == 0) {		//如果绘制加分画面能用的帧数已经用光
			frame = 30;	//重置绘制时间
			ifDrawAddScore = 0;	
			addScore = 0;
		}
		else {		//可用帧数不为0,表示可以绘制加分画面
			if (addScore) {	//抓到的是有价值的东西
				strScore.Format(_T("当前关卡: %d"), mGameLevel);
				pDC->TextOut(mPointPos.x, mPointPos.y, strScore);
				strScore.Format(_T("当前积分: %d + %d"), mMyScore - addScore, addScore);
				pDC->TextOut(mPointPos.x, mPointPos.y + 24, strScore);
				strScore.Format(_T("目标积分: %d"), (int)(mTotalScore * 2 / 3));
				pDC->TextOut(mPointPos.x, mPointPos.y + 48, strScore);
				frame--;
				pDC->SelectObject(oldFont);//选择回老字体
			}
			else {	//抓到的东西没有价值,即抓到了tnt,大力水之类的东西
				ifDrawAddScore = 0;
			}
			font.DeleteObject();//删除新字体
		}
	}
	if (ifDrawAddScore == 0) {	//平常情况,没抓到矿物时,直接绘制当前分数与目标分数
		strScore.Format(_T("当前关卡: %d"), mGameLevel);
		pDC->TextOut(mPointPos.x, mPointPos.y, strScore);
		strScore.Format(_T("当前积分: %d"), mMyScore);
		pDC->TextOut(mPointPos.x, mPointPos.y + 24, strScore);
		strScore.Format(_T("目标积分: %d"), (int)mTotalScore * 2/ 3);
		pDC->TextOut(mPointPos.x, mPointPos.y + 48, strScore);
		pDC->SelectObject(oldFont);//选择回老字体
		font.DeleteObject();//删除新字体
	}
}
