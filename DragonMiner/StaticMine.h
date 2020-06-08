#pragma once
#include "MyObject.h"

//��ӦmType
#define DIAMOND 0
#define LARGEGOLD 1
#define MIDGOLD 2
#define LITTLEGOLD 3
#define BIGSTONE 4
#define STONE 5

typedef struct staticMine {
	CBitmap bmp;	//�������ͼƬ
	int score;		//�������
	int weight;		//��������(Ӱ���ٶ�)
	int width;		//�����
	int height;		//���ﳤ
}staticMine;

class StaticMine :
	public MyObject
{
	
public:
	static void DeleteImage();
	static void LoadImage();

	static staticMine mStaticMine[6];	//�����ྲֹ����,������ʯ,����С���,��Сʯͷ.�ڳ�ʼ��ʱ��Ҫ����λͼ����������ֵ.
	static int score[6];	//�ֱ������ʯ,����С���,��Сʯͷ�ķ���������
	static int weight[6];

	StaticMine(int startX, int startY);
	~StaticMine();
	CRect GetRect();
	void Draw(CDC* pDC);
	int GetType() { return mType; }
	int GetScore() { return score[mType]; }
	int GetWeight() { return weight[mType]; }
	CBitmap * GetMyBmp() { return &mStaticMine[mType].bmp; }

private:
	int mType;
};

