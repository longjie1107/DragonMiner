#pragma once
#include "MyObject.h"


#define TNT_WIDTH	64
#define TNT_HEIGHT	64

class Tnt :
	public MyObject
{
public:
	static void DeleteImage();
	static void LoadImage();
	static CBitmap tntBmp;

	CRect GetRect();
	void Draw(CDC* pDC);
	Tnt(int startX, int startY);
	~Tnt();
	
	//无用但不可删
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }
};

