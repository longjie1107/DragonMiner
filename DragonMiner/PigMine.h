#pragma once
#include "MyObject.h"

//��ӦmType
#define PIG 0
#define DIAMONDPIG 1

typedef struct pigMine {
	CBitmap bmp[2];	//�����������ߺ������ߵ�ͼƬ
	int score;		//�������
	int weight;		//��������(Ӱ���ٶ�)
	int width;		//�����
	int height;		//���ﳤ
}pigMine;

class PigMine :
	public MyObject
{
public:
	static void DeleteImage();
	static void LoadImage();

	static pigMine mPigMine[2];	//��������,��ͨ�����ʯ��.
	static CBitmap mPigBmp[2];	//�������ͼƬ,������GetMyBmp()����

	CRect GetRect();
	void Draw(CDC* pDC);
	void Move();
	int GetScore() { return mPigMine[mType].score; }
	int GetWeight() { return mPigMine[mType].weight; }
	CBitmap * GetMyBmp() {  return &mPigBmp[mType]; }
	PigMine(int startX, int startY);
	~PigMine();

private:
	int mType;	//������
	int frame;	//��ͬ֡�Ķ���
	int wait;	//ÿһ֡�ӻ�wait֡����
	int vx;		//ˮƽ�ƶ��ٶ�
	int orient;	//0��ʾ����,1��ʾ����.��ʼΪ����.�ƶ�һ�������任.
	int startx, endx;	//x���굽������λ��ʱ�任��λ
};

