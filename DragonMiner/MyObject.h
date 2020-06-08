#pragma once
#include "resource.h"

#define PI acos(-1.0)
class MyObject: public CObject
{
public:
	CPoint GetPos() { return mPointPos; }     //λ��
	void SetIsCatch(int value) { isCatch = value; }
	int GetIsCatch() { return isCatch; }
	void SetX(int x) { mPointPos.x = x; }
	void SetY(int y) { mPointPos.y = y; }
	virtual CRect GetRect() = 0;                    //��Χ��
	virtual void Draw(CDC* pDC) = 0;   //���ƺ���
	virtual int GetWeight() = 0;   //��ȡ��������
	virtual int GetScore() = 0;   //��ȡ�������
	virtual CBitmap * GetMyBmp() = 0;   //��ȡ����ͼ��
	//��תԭʼͼ��orgBmp,Angle��(��Ϊ˳ʱ����ת)�õ�Ŀ��ͼ��dstBmp
	void BmpRotate(CBitmap * orgBmp, CBitmap * dstBmp, double Angle); 
	//���ƺ��� ��orgBmp����תԲ��(��������)��תangle�Ⱥ�,��ƽ��(offsetx,offsety)֮�����õ���ͼ��.originx,yΪ��orgBmp��ת�������������·�ʱ��ͼ�����ĵ�����
	void DrawRotateBmp(CDC * pDC, CBitmap *orgBmp, int angle, int originx, int originy, int offsetx, int offsety);
	MyObject();
	~MyObject();

protected:
	CPoint mPointPos;
	int isCatch;
	int centerx, centery;	//���п���͹��ӵ���תԲ��(��������)
	CRect rotaryRect;	//����ͼƬ��ת��ľ�����Ϣ.
};

