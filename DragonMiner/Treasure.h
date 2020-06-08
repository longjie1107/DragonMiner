#pragma once
#include "MyObject.h"

#define MONEY 0
#define STRENTH 1
#define LUCK 2

typedef struct treasure {
	CBitmap bmp;	//�������ͼƬ
	int score;		//�������
	int weight;		//��������(Ӱ���ٶ�)
	int width;		//�����
	int height;		//���ﳤ
}treasure;

class Treasure :
	public MyObject
{
public:
	static void DeleteImage();
	static void LoadImage();

	static treasure mTreasure;	

	CRect GetRect();
	void Draw(CDC* pDC);
	int GetWeight() { return mTreasure.weight; }
	int GetType() { return mType; }
	int GetScore() { return mType == 0 ? mTreasure.score : 0; }		//���������Ǯ���л���,����û����
	CBitmap * GetMyBmp() { return &mTreasure.bmp; }
	Treasure(int startX, int startY);
	~Treasure();

private:
	int mType;		//0Ϊ��ͨ����,1Ϊ����ˮ--ץ�κζ���������׾�,2Ϊ���˲�--���в���ļ�ֵ����
};

