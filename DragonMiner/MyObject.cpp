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

/*图形的旋转公式：
x1 = (x - xcenter) *  cosθ - (y - ycenter)  * sinθ + xcenter;
y1 = (x - xcenter) *  sinθ + (y - ycenter)  * cosθ + ycenter;
x, y：为原先点的坐标位置。
x1, y1旋转后点的坐标位置。
Xcenter, ycenter：为所绕的中心点的坐标*/
//旋转原始图像orgBmp,Angle度(正为顺时针旋转)得到目标图像dstBmp
void MyObject::BmpRotate(CBitmap* orgBmp, CBitmap* dstBmp, double Angle)
{
	BITMAP bmp;
	orgBmp->GetBitmap(&bmp);	//获取位图信息
	BYTE *pBits = new BYTE[bmp.bmWidthBytes*bmp.bmHeight];
	orgBmp->GetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, pBits);	//原始信息存储至pBits中
	Angle = Angle * PI / 180;	//角度转换为弧度制
	int interval = bmp.bmWidthBytes / bmp.bmWidth;	//每像素所需字节数

	int newWidth, newHeight, newbmWidthBytes;	//新图的高宽与每行字节数
												//得到cos和sin的绝对值以计算高宽.
	double abscos, abssin;
	abscos = cos(Angle) > 0 ? cos(Angle) : -cos(Angle);
	abssin = sin(Angle) > 0 ? sin(Angle) : -sin(Angle);
	newWidth = (int)(bmp.bmWidth * abscos + bmp.bmHeight * abssin);
	newHeight = (int)(bmp.bmWidth * abssin + bmp.bmHeight * abscos);
	newbmWidthBytes = newWidth * interval;
	BYTE *TempBits = new BYTE[newWidth * newHeight * interval];	//新图的信息存储至TempBits中
																//初始化新图信息,全部涂为白色.
	for (int j = 0; j < newHeight; j++) {
		for (int i = 0; i < newWidth; i++) {
			for (int k = 0; k < interval; k++) {
				TempBits[i*interval + j * newbmWidthBytes + k] = 0xff;
			}
		}
	}
	double newrx0 = newWidth * 0.5, rx0 = bmp.bmWidth * 0.5;	//变换后的中心点
	double newry0 = newHeight * 0.5, ry0 = bmp.bmHeight * 0.5;	//变换前的中心点
																//遍历新图的每一个像素点
	for (int j = 0; j < newHeight; j++) {
		for (int i = 0; i< newWidth; i++) {
			int tempI, tempJ;	//原图对应点
								//首先要明确:新图和原图的左上方坐标都为(0,0).在此情况下,下式可以这样理解:
								//对于新图的每一个点,让其跟随新图中心点平移至中心点为(0,0),然后旋转-Angle度,
								//再让该点跟随中心点平移,当中心点平移至原图的中心点.该点就回到了旋转前的位置.
			tempI = (int)((i - newrx0)*cos(Angle) + (j - newry0)*sin(Angle) + rx0);
			tempJ = (int)(-(i - newrx0)*sin(Angle) + (j - newry0)*cos(Angle) + ry0);
			//如果该点在原图中找到了对应点
			if (tempI >= 0 && tempI<bmp.bmWidth)
				if (tempJ >= 0 && tempJ < bmp.bmHeight)
				{
					//将原图的对应点信息赋给该点
					for (int m = 0; m < interval; m++)
						TempBits[i*interval + j * newbmWidthBytes + m] = pBits[tempI*interval + bmp.bmWidthBytes * tempJ + m];
				}
		}
	}
	//更新位图信息
	bmp.bmWidth = newWidth;
	bmp.bmHeight = newHeight;
	bmp.bmWidthBytes = newbmWidthBytes;
	//创建位图
	dstBmp->CreateBitmapIndirect(&bmp);
	//将位图信息传入位图
	dstBmp->SetBitmapBits(bmp.bmWidthBytes*bmp.bmHeight, TempBits);
	delete pBits;
	delete TempBits;	//释放内存
}

//绘制函数 将orgBmp(中心点在旋转圆心下方)绕旋转圆心(钩具中心)旋转angle度后,再平移(offsetx,offsety)之后所得到的图像
void MyObject::DrawRotateBmp(CDC * pDC, CBitmap* orgBmp, int angle, int originx, int originy, int offsetx, int offsety) {
	double newx, newy;
	CDC memDC;
	//计算图像中心点经角度变换后的坐标
	newx = (originx - centerx) *  cos(angle / 180.0 * PI) - (originy - centery)  * sin(angle / 180.0 * PI) + centerx;
	newy = (originx - centerx) *  sin(angle / 180.0 * PI) + (originy - centery)  * cos(angle / 180.0 * PI) + centery;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *tmpBmp = new CBitmap;
	BITMAP tmpBmpInfo;
	BmpRotate(orgBmp, tmpBmp, angle);
	tmpBmp->GetBitmap(&tmpBmpInfo);
	//由中心点坐标与新图形的宽高与平移的量得出左上角坐标
	mPointPos.x = (int)(newx - 0.5*tmpBmpInfo.bmWidth + offsetx);
	mPointPos.y = (int)(newy - 0.5*tmpBmpInfo.bmHeight + offsety);
	//更新旋转矩形信息
	rotaryRect.left = mPointPos.x;
	rotaryRect.right = mPointPos.x + tmpBmpInfo.bmWidth;
	rotaryRect.top = mPointPos.y;
	rotaryRect.bottom = mPointPos.y + tmpBmpInfo.bmHeight;
	CBitmap* old = memDC.SelectObject(tmpBmp);
	pDC->TransparentBlt(mPointPos.x, mPointPos.y, tmpBmpInfo.bmWidth, tmpBmpInfo.bmHeight, &memDC, 0, 0, tmpBmpInfo.bmWidth, tmpBmpInfo.bmHeight, RGB(255, 255, 255));
	memDC.SelectObject(old);
	tmpBmp->DeleteObject();	//释放内存
}