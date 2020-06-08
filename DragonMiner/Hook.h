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
	static int originx, originy;		//图像中心点的坐标;
	static int topx, topy;	//图像中心最上方的坐标
	
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

	//无用但不可删
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }


private:
	int frame;		//控制帧数
	int status;		//初始状态为HOOK_KEEP即钩子在矿车附近左右旋转;HOOK_OUT时表示出钩,此时不旋转,会进行平移操作;HOOK_IN表示收钩
	int angle,vangle;	//钩子的角度与变化速率
	double vgo,vback;	//钩子出钩与收钩时的移动速度(出钩速度恒定,收钩速度受出钩速度与矿物重量影响)
	int orient;		//向右旋转为1,向左旋转为-1
	double offsetx, offsety;	//钩子左上角坐标相对于初始位置的偏移量,在出钩与收钩时用到.
};

