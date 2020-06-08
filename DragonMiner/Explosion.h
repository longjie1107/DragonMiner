#pragma once
#include "MyObject.h"

#define EXPLOSION_WIDTH		248
#define EXPLOSION_HEIGHT	252

class Explosion :
	public MyObject
{
public:
	Explosion(int x,int y);
	~Explosion();
	static void DeleteImage();
	static void LoadImage();

	CRect GetRect() {
		return CRect(mPointPos,
			CSize(EXPLOSION_WIDTH, EXPLOSION_HEIGHT));
	}
	void Draw(CDC* pDC);
	bool GetIsOver(){ return frame>15?true:false; }	//�����ʾ�Ѿ���������֡����ô����True��֪ͨ��������ɾ����ը����

													//���õ�����ɾ
	int GetWeight() { return 0; }
	int GetScore() { return 0; }
	CBitmap * GetMyBmp() { return new CBitmap(); }

private:
	int frame;		//��������15֡ʱ����
	static CImageList mImages;
};

