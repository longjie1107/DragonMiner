#pragma once
#include "MyObject.h"

#define HOOK_WIDTH	36
#define HOOK_HEIGHT	22
#define HOOK_KEEP 0
#define HOOK_OUT 1
#define HOOK_IN 2


class Hook :
	public MyObject
{
public:
	Hook();
	~Hook();
	static void DeleteImage();
	static void LoadImage();
	static CBitmap hookBmp[2];
	static int originx, originy;		//ͼ�����ĵ������;
	static int topx, topy;	//ͼ���������Ϸ�������
	
	CRect GetRect();
	void Draw(CDC* pDC);
	void SetStatus(int value);
	int GetStatus() { return status; }
	double GetOffsetx() { return offsetx; }
	double GetOffsety() { return offsety; }
	int GetAngle() { return angle; }
	void SetVback(int weight);
	/*void BmpRotate(CBitmap * orgBmp, CBitmap * dstBmp, double Angle);
	void DrawRotateBmp(CDC * pDC, CBitmap *orgBmp, int angle, int offsetx, int offsety);*/

	//���õ�����ɾ
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }


private:
	int frame;		//����֡��
	int status;		//��ʼ״̬ΪHOOK_KEEP�������ڿ󳵸���������ת;HOOK_OUTʱ��ʾ����,��ʱ����ת,�����ƽ�Ʋ���;HOOK_IN��ʾ�չ�
	int angle,vangle;	//���ӵĽǶ���仯����
	double vgo,vback;	//���ӳ������չ�ʱ���ƶ��ٶ�(�����ٶȺ㶨,�չ��ٶ��ܳ����ٶ����������Ӱ��)
	int orient;		//������תΪ1,������תΪ-1
	double offsetx, offsety;	//�������Ͻ���������ڳ�ʼλ�õ�ƫ����,�ڳ������չ�ʱ�õ�.
};

