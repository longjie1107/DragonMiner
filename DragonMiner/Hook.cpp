#include "stdafx.h"
#include "Hook.h"


Hook::Hook()
{
	mPointPos.x = 471;		//�������Ͻ�����
	mPointPos.y = 110;	
	frame = 0;
	status = HOOK_KEEP;	//��ʼ״̬�������ĵ���ת
	orient = 1;	//��ʼ������ת
	angle = 0;	//��ʼ�Ƕ�Ϊ0
	vangle = 2;	//�Ƕȱ仯����Ϊ1
	vgo = vback = 12;	//�չ�ʱ�ĳ������չ��ٶ�Ϊ7.
	offsetx = offsety = 0;	//δ����ʱ��������ƫ����Ϊ0
}

//�����������չ����ų�ʼͼ��
CBitmap Hook::hookBmp[2];
int Hook::originx, Hook::originy, Hook::topx, Hook::topy;

Hook::~Hook()
{
}

void Hook::DeleteImage()
{
	hookBmp[0].DeleteObject();
	hookBmp[1].DeleteObject();
}

void Hook::LoadImage()
{
	hookBmp[0].LoadBitmap(IDB_HOOKOUT);
	hookBmp[1].LoadBitmap(IDB_HOOKIN);

	originx  = topx = 489;
	originy = 121;		//��ʼλ��ͼ�����ĵ������
	topy = 110;	//ͼ���������Ϸ�
}

CRect Hook::GetRect()
{
	if (rotaryRect.Width())	//����Ѿ���ת����,rotaryRect�Ŀ�Ͳ�Ϊ0
		return rotaryRect;	//������ת����ͼ�ξ�����Ϣ.
	else
		return CRect(mPointPos, CSize(HOOK_WIDTH, HOOK_HEIGHT));
}

/*
ͼ�ε���ת��ʽ��
x1   =   (x - xcenter) *  cos�� - (y - ycenter)  * sin�� + xcenter;
y1   =   (x - xcenter) *  sin�� +  (y- ycenter)  * cos�� + ycenter;
x, y��Ϊԭ�ȵ������λ�á�
x1, y1��ת��������λ�á�
Xcenter, ycenter��Ϊ���Ƶ����ĵ������
��ʼ״̬��תԲ��Ϊcenter(489,97)  �������ĵ�origin(489,121)  �����������Ϸ���top(489,110)
*/
void Hook::Draw(CDC * pDC)
{
	CDC memDC;
	if (status == HOOK_KEEP) {		//�������Ҳ��չ�ʱ,�������ĵ���ת
		if (angle == 80)	orient = -1;	//��˳ʱ����ת80�Ⱥ�任��ת����
		if (angle == -80)	orient = 1;	//����ʱ����ת80�Ⱥ�任��ת����
		angle += orient * vangle;
		DrawRotateBmp(pDC, &hookBmp[0], angle, originx,originy, 0, 0);	//������ͼƬ����תԲ��(��������)��תangle�Ⱥ�,��ƽ��(0,0)֮�����õ���ͼ��
	}
	else if (status == HOOK_OUT){		//����ʱ����:���ӳ�ĳһ����ƽ��ֱ������������ߴﵽ���ӳ���ʱ����
		offsetx -= sin(angle*PI / 180)*vgo;
		offsety += cos(angle*PI / 180)*vgo;
		//(471,110)Ϊ��ʼ������Ͻǵ�����,���ڳ����߽�Ĺ���,ֱ���ջ�
		if (offsetx <= -471 || offsetx >= (WIN_WIDTH - 471) || offsety >= (WIN_HEIGHT-110)) {
			status = HOOK_IN;
		}
		else {	//��δ�����߽�����ƹ���
			DrawRotateBmp(pDC, &hookBmp[0], angle, originx, originy, (int)offsetx, (int)offsety);
		}
	}
	else if (status == HOOK_IN) {			//�չ�ʱ����:����ƽ������ʼ��
		offsetx += sin(angle*PI / 180)*vback;
		offsety -= cos(angle*PI / 180)*vback;
		//����Ѿ��ܽӽ���ʼλ��ʱ,�͵����Ѿ����˳�ʼλ��,����ƫ����Ϊ0,���ù���״̬ΪHOOK_KEEP
		if ( (offsetx < 1.1 * vback && offsetx > -1.1 * vback) && (offsety < 1.1 * vback &&offsety > -1.1 * vback)) {
			offsetx = 0;
			offsety = 0;
			status = HOOK_KEEP;
		}
		else {
			DrawRotateBmp(pDC, &hookBmp[1], angle, originx, originy, (int)offsetx, (int)offsety);
		}
	}
	
	//������,�ӹ������Ļ������Ӷ���
	int newtopx, newtopy;	//��ù��Ӷ����ƹ���������ת�������
	newtopx = (int)((topx - centerx) *  cos(angle / 180.0 * PI) - (topy - centery)  * sin(angle / 180.0 * PI) + centerx);
	newtopy = (int)((topx - centerx) *  sin(angle / 180.0 * PI) + (topy - centery)  * cos(angle / 180.0 * PI) + centery);
	CPoint start, end;
	start.x = (int)centerx, start.y = (int)centery;
	end.x = newtopx + (int)offsetx, end.y = newtopy + (int)offsety;
	pDC->MoveTo(start);
	//���ߣ��յ�
	pDC->LineTo(end);
}

void Hook::SetStatus(int value)
{
	status = value;
}


void Hook::SetVback(int weight)
{
	if (weight == 0) vback = vgo;
	vback = vgo * 100 / (100+weight);
}