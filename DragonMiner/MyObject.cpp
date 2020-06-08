#include "stdafx.h"
#include "MyObject.h"


MyObject::MyObject()
{
	isCatch = 0;
	centerx = 489;
	centery = 97;
	rotaryRect = CRect(0, 0, 0, 0);
}


MyObject::~MyObject()
{
}

/*ͼ�ε���ת��ʽ��
x1 = (x - xcenter) *  cos�� - (y - ycenter)  * sin�� + xcenter;
y1 = (x - xcenter) *  sin�� + (y - ycenter)  * cos�� + ycenter;
x, y��Ϊԭ�ȵ������λ�á�
x1, y1��ת��������λ�á�
Xcenter, ycenter��Ϊ���Ƶ����ĵ������*/
//��תԭʼͼ��orgBmp,Angle��(��Ϊ˳ʱ����ת)�õ�Ŀ��ͼ��dstBmp
void MyObject::BmpRotate(CBitmap* orgBmp, CBitmap* dstBmp, double Angle)
{
	BITMAP bmp;
	orgBmp->GetBitmap(&bmp);	//��ȡλͼ��Ϣ
	BYTE *pBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight];
	orgBmp->GetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, pBits);	//ԭʼ��Ϣ�洢��pBits��
	Angle = Angle * PI / 180;	//�Ƕ�ת��Ϊ������
	int interval = bmp.bmWidthBytes / bmp.bmWidth;	//ÿ���������ֽ���

	int newWidth, newHeight, newbmWidthBytes;	//��ͼ�ĸ߿���ÿ���ֽ���
												//�õ�cos��sin�ľ���ֵ�Լ���߿�.
	double abscos, abssin;
	abscos = cos(Angle) > 0 ? cos(Angle) : -cos(Angle);
	abssin = sin(Angle) > 0 ? sin(Angle) : -sin(Angle);
	newWidth = (int)(bmp.bmWidth * abscos + bmp.bmHeight * abssin);
	newHeight = (int)(bmp.bmWidth * abssin + bmp.bmHeight * abscos);
	newbmWidthBytes = newWidth * interval;
	BYTE *TempBits = new BYTE[newWidth * newHeight * interval];	//��ͼ����Ϣ�洢��TempBits��
																//��ʼ����ͼ��Ϣ,ȫ��ͿΪ��ɫ.
	for (int j = 0; j < newHeight; j++) {
		for (int i = 0; i < newWidth; i++) {
			for (int k = 0; k < interval; k++) {
				TempBits[i*interval + j * newbmWidthBytes + k] = 0xff;
			}
		}
	}
	double newrx0 = newWidth * 0.5, rx0 = bmp.bmWidth * 0.5;	//�任������ĵ�
	double newry0 = newHeight * 0.5, ry0 = bmp.bmHeight * 0.5;	//�任ǰ�����ĵ�
																//������ͼ��ÿһ�����ص�
	for (int j = 0; j < newHeight; j++) {
		for (int i = 0; i< newWidth; i++) {
			int tempI, tempJ;	//ԭͼ��Ӧ��
								//����Ҫ��ȷ:��ͼ��ԭͼ�����Ϸ����궼Ϊ(0,0).�ڴ������,��ʽ�����������:
								//������ͼ��ÿһ����,���������ͼ���ĵ�ƽ�������ĵ�Ϊ(0,0),Ȼ����ת-Angle��,
								//���øõ�������ĵ�ƽ��,�����ĵ�ƽ����ԭͼ�����ĵ�.�õ�ͻص�����תǰ��λ��.
			tempI = (int)((i - newrx0)*cos(Angle) + (j - newry0)*sin(Angle) + rx0);
			tempJ = (int)(-(i - newrx0)*sin(Angle) + (j - newry0)*cos(Angle) + ry0);
			//����õ���ԭͼ���ҵ��˶�Ӧ��
			if (tempI >= 0 && tempI<bmp.bmWidth)
				if (tempJ >= 0 && tempJ < bmp.bmHeight)
				{
					//��ԭͼ�Ķ�Ӧ����Ϣ�����õ�
					for (int m = 0; m < interval; m++)
						TempBits[i*interval + j * newbmWidthBytes + m] = pBits[tempI*interval + bmp.bmWidthBytes * tempJ + m];
				}
		}
	}
	//����λͼ��Ϣ
	bmp.bmWidth = newWidth;
	bmp.bmHeight = newHeight;
	bmp.bmWidthBytes = newbmWidthBytes;
	//����λͼ
	dstBmp->CreateBitmapIndirect(&bmp);
	//��λͼ��Ϣ����λͼ
	dstBmp->SetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, TempBits);
	delete pBits;
	delete TempBits;	//�ͷ��ڴ�
}

//���ƺ��� ��orgBmp(���ĵ�����תԲ���·�)����תԲ��(��������)��תangle�Ⱥ�,��ƽ��(offsetx,offsety)֮�����õ���ͼ��
void MyObject::DrawRotateBmp(CDC * pDC, CBitmap* orgBmp, int angle, int originx, int originy, int offsetx, int offsety) {
	double newx, newy;
	CDC memDC;
	//����ͼ�����ĵ㾭�Ƕȱ任�������
	newx = (originx - centerx) *  cos(angle / 180.0 * PI) - (originy - centery)  * sin(angle / 180.0 * PI) + centerx;
	newy = (originx - centerx) *  sin(angle / 180.0 * PI) + (originy - centery)  * cos(angle / 180.0 * PI) + centery;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *tmpBmp = new CBitmap;
	BITMAP tmpBmpInfo;
	BmpRotate(orgBmp, tmpBmp, angle);
	tmpBmp->GetBitmap(&tmpBmpInfo);
	//�����ĵ���������ͼ�εĿ����ƽ�Ƶ����ó����Ͻ�����
	mPointPos.x = (int)(newx - 0.5*tmpBmpInfo.bmWidth + offsetx);
	mPointPos.y = (int)(newy - 0.5*tmpBmpInfo.bmHeight + offsety);
	//������ת������Ϣ
	rotaryRect.left = mPointPos.x;
	rotaryRect.right = mPointPos.x + tmpBmpInfo.bmWidth;
	rotaryRect.top = mPointPos.y;
	rotaryRect.bottom = mPointPos.y + tmpBmpInfo.bmHeight;
	CBitmap* old = memDC.SelectObject(tmpBmp);
	pDC->TransparentBlt(mPointPos.x, mPointPos.y, tmpBmpInfo.bmWidth, tmpBmpInfo.bmHeight, &memDC, 0, 0, tmpBmpInfo.bmWidth, tmpBmpInfo.bmHeight, RGB(255, 255, 255));
	memDC.SelectObject(old);
	tmpBmp->DeleteObject();	//�ͷ��ڴ�
}