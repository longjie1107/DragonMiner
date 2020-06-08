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
	font.CreateFont(24,                                    //   ����ĸ߶�   
		0,                                          //   ����Ŀ��  
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
		_T("΢���ź�"));
	CFont * oldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(mColor);
	pDC->SetTextAlign(TA_LEFT);	 //�������
	pDC->SetBkMode(TRANSPARENT); //����͸��
	CString strScore;
	
	
	if (ifDrawAddScore == 1) {	//ץ���˶���
		if (frame == 0) {		//������Ƽӷֻ������õ�֡���Ѿ��ù�
			frame = 30;	//���û���ʱ��
			ifDrawAddScore = 0;	
			addScore = 0;
		}
		else {		//����֡����Ϊ0,��ʾ���Ի��Ƽӷֻ���
			if (addScore) {	//ץ�������м�ֵ�Ķ���
				strScore.Format(_T("��ǰ�ؿ�: %d"), mGameLevel);
				pDC->TextOut(mPointPos.x, mPointPos.y, strScore);
				strScore.Format(_T("��ǰ����: %d + %d"), mMyScore - addScore, addScore);
				pDC->TextOut(mPointPos.x, mPointPos.y + 24, strScore);
				strScore.Format(_T("Ŀ�����: %d"), (int)(mTotalScore * 2 / 3));
				pDC->TextOut(mPointPos.x, mPointPos.y + 48, strScore);
				frame--;
				pDC->SelectObject(oldFont);//ѡ���������
			}
			else {	//ץ���Ķ���û�м�ֵ,��ץ����tnt,����ˮ֮��Ķ���
				ifDrawAddScore = 0;
			}
			font.DeleteObject();//ɾ��������
		}
	}
	if (ifDrawAddScore == 0) {	//ƽ�����,ûץ������ʱ,ֱ�ӻ��Ƶ�ǰ������Ŀ�����
		strScore.Format(_T("��ǰ�ؿ�: %d"), mGameLevel);
		pDC->TextOut(mPointPos.x, mPointPos.y, strScore);
		strScore.Format(_T("��ǰ����: %d"), mMyScore);
		pDC->TextOut(mPointPos.x, mPointPos.y + 24, strScore);
		strScore.Format(_T("Ŀ�����: %d"), (int)mTotalScore * 2/ 3);
		pDC->TextOut(mPointPos.x, mPointPos.y + 48, strScore);
		pDC->SelectObject(oldFont);//ѡ���������
		font.DeleteObject();//ɾ��������
	}
}
