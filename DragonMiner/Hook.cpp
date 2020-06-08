#include "stdafx.h"
#include "Hook.h"


Hook::Hook()
{
	mPointPos.x = 471;		//钩子左上角坐标
	mPointPos.y = 110;	
	frame = 0;
	status = HOOK_KEEP;	//初始状态绕着中心点旋转
	orient = 1;	//初始向右旋转
	angle = 0;	//初始角度为0
	vangle = 2;	//角度变化速率为1
	vgo = vback = 12;	//空钩时的出钩与收钩速度为7.
	offsetx = offsety = 0;	//未出钩时钩子坐标偏移量为0
}

//声明出钩与收钩两张初始图像
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
	originy = 121;		//初始位置图像中心点的坐标
	topy = 110;	//图像中心最上方
}

CRect Hook::GetRect()
{
	if (rotaryRect.Width())	//如果已经旋转过了,rotaryRect的宽就不为0
		return rotaryRect;	//返回旋转过的图形矩阵信息.
	else
		return CRect(mPointPos, CSize(HOOK_WIDTH, HOOK_HEIGHT));
}

/*
图形的旋转公式：
x1   =   (x - xcenter) *  cosθ - (y - ycenter)  * sinθ + xcenter;
y1   =   (x - xcenter) *  sinθ +  (y- ycenter)  * cosθ + ycenter;
x, y：为原先点的坐标位置。
x1, y1旋转后点的坐标位置。
Xcenter, ycenter：为所绕的中心点的坐标
初始状态旋转圆心为center(489,97)  钩子中心点origin(489,121)  钩子中心最上方点top(489,110)
*/
void Hook::Draw(CDC * pDC)
{
	CDC memDC;
	if (status == HOOK_KEEP) {		//不出钩且不收钩时,绕着中心点旋转
		if (angle == 80)	orient = -1;	//当顺时针旋转80度后变换旋转方向
		if (angle == -80)	orient = 1;	//当逆时针旋转80度后变换旋转方向
		angle += orient * vangle;
		DrawRotateBmp(pDC, &hookBmp[0], angle, originx,originy, 0, 0);	//将出钩图片绕旋转圆心(钩具中心)旋转angle度后,再平移(0,0)之后所得到的图像
	}
	else if (status == HOOK_OUT){		//出钩时绘制:钩子朝某一方向平移直至碰到矿物或者达到钩子长度时回收
		offsetx -= sin(angle*PI / 180)*vgo;
		offsety += cos(angle*PI / 180)*vgo;
		//(471,110)为初始情况左上角的坐标,对于超出边界的钩子,直接收回
		if (offsetx <= -471 || offsetx >= (WIN_WIDTH - 471) || offsety >= (WIN_HEIGHT-110)) {
			status = HOOK_IN;
		}
		else {	//若未超出边界则绘制钩子
			DrawRotateBmp(pDC, &hookBmp[0], angle, originx, originy, (int)offsetx, (int)offsety);
		}
	}
	else if (status == HOOK_IN) {			//收钩时绘制:钩子平移至起始点
		offsetx += sin(angle*PI / 180)*vback;
		offsety -= cos(angle*PI / 180)*vback;
		//如果已经很接近初始位置时,就当做已经到了初始位置,调整偏移量为0,设置钩子状态为HOOK_KEEP
		if ( (offsetx < 1.1 * vback && offsetx > -1.1 * vback) && (offsety < 1.1 * vback &&offsety > -1.1 * vback)) {
			offsetx = 0;
			offsety = 0;
			status = HOOK_KEEP;
		}
		else {
			DrawRotateBmp(pDC, &hookBmp[1], angle, originx, originy, (int)offsetx, (int)offsety);
		}
	}
	
	//画钩线,从钩具中心画至钩子顶部
	int newtopx, newtopy;	//获得钩子顶部绕钩具中心旋转后的坐标
	newtopx = (int)((topx - centerx) *  cos(angle / 180.0 * PI) - (topy - centery)  * sin(angle / 180.0 * PI) + centerx);
	newtopy = (int)((topx - centerx) *  sin(angle / 180.0 * PI) + (topy - centery)  * cos(angle / 180.0 * PI) + centery);
	CPoint start, end;
	start.x = (int)centerx, start.y = (int)centery;
	end.x = newtopx + (int)offsetx, end.y = newtopy + (int)offsety;
	pDC->MoveTo(start);
	//划线，终点
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