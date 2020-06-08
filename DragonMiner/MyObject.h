#pragma once
#include "resource.h"

#define PI acos(-1.0)
class MyObject: public CObject
{
public:
	CPoint GetPos() { return mPointPos; }     //位置
	void SetIsCatch(int value) { isCatch = value; }
	int GetIsCatch() { return isCatch; }
	void SetX(int x) { mPointPos.x = x; }
	void SetY(int y) { mPointPos.y = y; }
	virtual CRect GetRect() = 0;                    //包围盒
	virtual void Draw(CDC* pDC) = 0;   //绘制函数
	virtual int GetWeight() = 0;   //获取矿物质量
	virtual int GetScore() = 0;   //获取矿物分数
	virtual CBitmap * GetMyBmp() = 0;   //获取矿物图像
	//旋转原始图像orgBmp,Angle度(正为顺时针旋转)得到目标图像dstBmp
	void BmpRotate(CBitmap * orgBmp, CBitmap * dstBmp, double Angle); 
	//绘制函数 将orgBmp绕旋转圆心(钩具中心)旋转angle度后,再平移(offsetx,offsety)之后所得到的图像.originx,y为将orgBmp旋转至钩具中心正下方时的图形中心点坐标
	void DrawRotateBmp(CDC * pDC, CBitmap *orgBmp, int angle, int originx, int originy, int offsetx, int offsety);
	MyObject();
	~MyObject();

protected:
	CPoint mPointPos;
	int isCatch;
	int centerx, centery;	//所有矿物和钩子的旋转圆心(钩具中心)
	CRect rotaryRect;	//储存图片旋转后的矩形信息.
};

