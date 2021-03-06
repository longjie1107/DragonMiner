
// DragonMinerView.cpp: CDragonMinerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DragonMiner.h"
#endif

#include "DragonMinerDoc.h"
#include "DragonMinerView.h"

#include "StaticMine.h"
#include "MyObject.h"
#include "PigMine.h"
#include "Treasure.h"
#include "Tnt.h"
#include "Dragon.h"
#include "Hook.h"
#include "Score.h"
#include "Explosion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//activityMode的取值,0为主界面,1为游戏界面,2为设置界面,3为说明界面
#define MAIN_ACTIVITY 0
#define GAME_ACTIVITY 1
#define SETTING_ACTIVITY 2
#define BRIEF_ACTIVITY 3
//七类Objects
#define DRAGON 0
#define HOOK 1
#define PIGS 2
#define TNT 3
#define TREASURE 4
#define STATIC_MINE 5
#define EXPLOSION 6
//时间条的信息
#define LEFT_OFFSET     600
#define TOP_OFFSET      5
#define BAR_LEN			400
#define TOTAL_TIME      60

// CDragonMinerView

IMPLEMENT_DYNCREATE(CDragonMinerView, CView)

BEGIN_MESSAGE_MAP(CDragonMinerView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CDragonMinerView 构造/析构

CDragonMinerView::CDragonMinerView()
{
	StaticMine::LoadImage();
	PigMine::LoadImage();
	Treasure::LoadImage();
	Tnt::LoadImage();
	Dragon::LoadImage();
	Hook::LoadImage();
	Explosion::LoadImage();
	// TODO: 在此处添加构造代码
}

CDragonMinerView::~CDragonMinerView()
{
	// free memory
	// delete all objects
	POSITION pos;	
	int i;
	for (i = 0; i < 7; i++)
	{
		pos = mObjects[i].GetHeadPosition();	//定位第一个Objects的索引
		while (pos != NULL)
		{
			delete mObjects[i].GetNext(pos);	//删除pos的元素并将pos指向下一个元素的位置
		}
		mObjects[i].RemoveAll();	//删除列表中所有的元素
	}
	// clear image lists
	StaticMine::DeleteImage();
	PigMine::DeleteImage();
	Treasure::DeleteImage();
	Tnt::DeleteImage();
	Dragon::DeleteImage();
	Hook::DeleteImage();
	Explosion::DeleteImage();
}

BOOL CDragonMinerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}


// CDragonMinerView 诊断

#ifdef _DEBUG
void CDragonMinerView::AssertValid() const
{
	CView::AssertValid();
}

void CDragonMinerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDragonMinerDoc* CDragonMinerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDragonMinerDoc)));
	return (CDragonMinerDoc*)m_pDocument;
}
#endif //_DEBUG


// CDragonMinerView 消息处理程序


int CDragonMinerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CClientDC dc(this);
	memDC.CreateCompatibleDC(&dc);
	SetTimer(100, 20, NULL); //UINT SetTimer( UINT nIDEvent, UINT nElapse, NULL)
	srand((unsigned)time(NULL)); // 设置随机数种子
	activityMode = MAIN_ACTIVITY;	//首先显示游戏主界面
	mouseX = mouseY = 0;	//鼠标x,y坐标
	isGamePause = false;
	isMusicOn = true;
	isSoundEffectsOn = true;
	isAutoModeOn = false;
	isLegendModeOn = false;
	ifReadSave = false;
	frame = 0;
	score = new Score(CPoint(15, 15), RGB(255, 0, 0));		//分数,龙宝每一关卡的位置固定,所以在这生成
	mObjects[DRAGON].AddTail(new Dragon(454, 49));	//龙宝

	mciSendString(_T("play res/mus/bgm.mp3 repeat"), NULL, 0, NULL);	//放背景音乐
	return 0;
}

void CDragonMinerView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(100);
	// TODO: 在此处添加消息处理程序代码
}

// CDragonMinerView 绘图

void CDragonMinerView::OnDraw(CDC* pDC)
{
	CDragonMinerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBitmap memBMP, *tmpBmp;
	CRect hookRect, explosionRect;
	static int ticks = 0;	//时间滴答(实际上是记录了帧数)
	CBrush brush;
	CRect bar;	//时间条
	CString msg;	//消息
	CFont font, *oldFont;

	switch (activityMode)
	{
	case MAIN_ACTIVITY:
		memBMP.LoadBitmapW(IDB_MAIN);
		memDC.SelectObject(&memBMP);
		break;
	case GAME_ACTIVITY:
		if (isGamePause)	return;
		//倒计时处理
		ticks++;
		if (ticks % 30 == 0)		//每30帧
		{
			mTimeLeft--;	//时间减1
			if (mTimeLeft <= 0)		//若时间结束
			{
				isGamePause = true;	//先暂停游戏程序
				int myScore, targetScore;
				myScore = Score::GetMyScore();
				targetScore = (int)(Score::GetTotalScore() * 2 / 3);	//目标分数为所有矿物分数的2/3
				if (myScore >= 21470000) {	//通关,int只有2147483647,再玩下去要溢出了
					if (AfxMessageBox(_T("恭喜通关~~~"), MB_OK, 0) == IDOK)		
					{
						activityMode = MAIN_ACTIVITY;	//返回主界面
						break;
					}
				}
				if (myScore >= targetScore) {	//达成目标
					if (AfxMessageBox(_T("是否进入下一关?"), MB_YESNO, 0) == IDYES)		//选是
					{
						isGamePause = false;	//继续游戏进程
						isGoNextLevel = true;	//可以前往下一关
					}
					else {	//选否,触发esc事件,会询问是否存档
						keybd_event(27, 0, 0, 0);		//按"esc"键
						keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
						break;
					}
				}
				else {	//未达成目标
					if (AfxMessageBox(_T("是否重新开始?"), MB_YESNO, 0) == IDYES)		//选是
					{
						isGamePause = false;	//继续游戏进程
						initGame();	//初始化游戏
					}
					else {	//选否,返回主界面
						activityMode = MAIN_ACTIVITY;
						break;
					}
				}//if
			}//if
		}//if

		memBMP.LoadBitmapW(IDB_BKGROUND);
		memDC.SelectObject(&memBMP);
		MyObject * myObject;
		if (isGoNextLevel) {
			initLevel();	//生成当前关卡的矿物
			isGoNextLevel = false;		//刚开始关卡当然不能去下一关关卡啦
		}
		Dragon * mDragon;
		Hook * mHook;
		mDragon = (Dragon*)mObjects[DRAGON].GetHead();
		mHook = (Hook*)mObjects[HOOK].GetHead();
		//龙宝绘制
		if (mHook->GetStatus() == HOOK_KEEP)	mDragon->SetIsCatch(0);		//若未出钩/收钩,通知龙宝绘制静止画面
		else mDragon->SetIsCatch(1);	//否则通知龙宝绘制动画
		mDragon->Draw(&memDC);
		//钩子绘制
		mHook->Draw(&memDC);
		
		POSITION pos1, pos2, pos3, pos4;
		
		for (int i = PIGS; i <= STATIC_MINE; i++) {	//绘制所有矿物
			for (pos1 = mObjects[i].GetHeadPosition(); (pos2 = pos1) != NULL; ) {	
				myObject = (MyObject*)mObjects[i].GetNext(pos1);	//获取pos1当前元素并将pos1指向下一个元素位置
				if (myObject->GetIsCatch() == 0) {	//如果矿物尚未被抓则绘制正常图像,否则绘制坐标随着钩子变化的图像
					myObject->Draw(&memDC);
					if (i == PIGS)
						((PigMine*)myObject)->Move();
				}
				else{		//如果矿物被抓了
					int originx, originy, angle;	//originx,y:矿物绕钩具旋转到其正下方的坐标 angle:矿物从origin开始绕钩具顺时针旋转的角度
					int offsetx, offsety;	//矿物相对于钩具中心的偏移量
					angle = mHook->GetAngle();
					originx = mHook->originx;
					originy = mHook->originy + dMineHookCenter;
					offsetx = (int)mHook->GetOffsetx();
					offsety = (int)mHook->GetOffsety();
					tmpBmp = myObject->GetMyBmp();
					myObject->DrawRotateBmp(&memDC, tmpBmp, angle, originx, originy, offsetx, offsety);
				}
			}
		}

		Explosion * mExplosion;
		for (pos1 = mObjects[EXPLOSION].GetHeadPosition(); (pos2 = pos1) != NULL;) {	//绘制爆炸效果
			mExplosion = (Explosion*)mObjects[EXPLOSION].GetNext(pos1);		//获取爆炸对象
			if (mExplosion->GetIsOver()) {		//爆炸效果已经结束
				mObjects[EXPLOSION].RemoveAt(pos2);	// 删除爆炸效果
				delete mExplosion;
			}
			else
				mExplosion->Draw(&memDC);	// 绘制爆炸效果
		}

		//  检测爆炸与矿物之间是否发生碰撞，
		//  如果爆炸附近有TNT，则引起二次爆炸
		for (pos1 = mObjects[EXPLOSION].GetHeadPosition(); (pos2 = pos1) != NULL;)	//遍历爆炸
		{
			mExplosion = (Explosion*)mObjects[EXPLOSION].GetNext(pos1);		//获取爆炸对象
			//遍历所有矿物
			for (int i = PIGS; i <= STATIC_MINE; i++) {
				for (pos3 = mObjects[i].GetHeadPosition(); (pos4 = pos3) != NULL;)
				{
					myObject = (MyObject*)mObjects[i].GetNext(pos3);	//获取矿物对象
					explosionRect = mExplosion->GetRect();		//爆炸包围盒
					//检测是否发生碰撞，myObject->GetRect() (矿物)和explosionRect（爆炸包围盒）是否相交
					if ((explosionRect.IntersectRect(myObject->GetRect(), explosionRect)))
					{
						if (i == TNT) {		//如果碰到TNT,继续生成爆炸
							int x, y;	//设置爆炸效果的位置
							x = (int)((myObject->GetPos().x) - (EXPLOSION_WIDTH - TNT_WIDTH) / 2);
							y = (int)((myObject->GetPos().y) - (EXPLOSION_HEIGHT - TNT_HEIGHT) / 2);
							PlaySound((LPCWSTR)IDR_EXPLODE, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
							mObjects[EXPLOSION].AddTail(new Explosion(x, y));
						}
						mObjects[i].RemoveAt(pos4);	// 删除该矿物
						delete myObject;
						break;
					}//if
				}//for
			}//for
		}//for

		// 检测钩子是否钩到矿物
		for (int i = PIGS; i <= STATIC_MINE; i++) {
			for (pos1 = mObjects[i].GetHeadPosition(); (pos2 = pos1) != NULL;) //遍历所有矿物
			{
				myObject = (MyObject*)mObjects[i].GetNext(pos1);  // save for deletion
				hookRect = mHook->GetRect();

				//一旦发生碰撞,钩子与矿物共同返回
				if ((hookRect.IntersectRect(myObject->GetRect(), hookRect)))
				{
					//如果碰到了TNT
					if (i == TNT) {
						int x, y;	//设置爆炸效果的位置
						x = (int)((myObject->GetPos().x) - (EXPLOSION_WIDTH - TNT_WIDTH) / 2);
						y = (int)((myObject->GetPos().y) - (EXPLOSION_HEIGHT - TNT_HEIGHT) / 2);
						PlaySound((LPCWSTR)IDR_EXPLODE, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
						mObjects[EXPLOSION].AddTail(new Explosion(x,y));
						// 删除该TNT
						mObjects[i].RemoveAt(pos2);
						delete myObject;
						mHook->SetStatus(HOOK_IN);//钩子回收
						break;
					}
					//矿物回收
					//以下语句只需要在第一次碰撞时调用一次
					if (useForFirstTime == 0) {
						int dStaticMineOffset = 0;	// 不同矿物的中心与钩子中心的相对偏移量不同
						if (i == TREASURE)	dStaticMineOffset = 16;
						if (i == STATIC_MINE) {
							switch (((StaticMine*)myObject)->GetType())
							{
							case DIAMOND:	dStaticMineOffset = 2; break;
							case LARGEGOLD:	dStaticMineOffset = 45; break;
							case MIDGOLD:	dStaticMineOffset = 30; break;
							case LITTLEGOLD:	dStaticMineOffset = 20; break;
							case BIGSTONE:	dStaticMineOffset = 18; break;
							case STONE:	dStaticMineOffset = 10; break;
							}
						}
						dMineHookCenter = (int)((myObject->GetRect().Height() + hookRect.Height())*0.5 - dStaticMineOffset);
						myObject->SetIsCatch(1);	//设置矿物的isCatch值为1表示矿物被抓住了,该参数决定绘制时矿物是否跟随钩子移动
						mHook->SetStatus(HOOK_IN);//钩子回收
						if (!isGetStrenthBuff)	//没有力量buff时
							mHook->SetVback(myObject->GetWeight());	//根据矿物重量设置回收速度
						useForFirstTime = 1;
						if(isSoundEffectsOn)
							PlaySound((LPCWSTR)IDR_CATCH, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
					}
					//当钩子到达原位时,矿物消失转化为分数
					if (mHook->GetStatus() == HOOK_KEEP) {
						if (i == TREASURE) {		//	抓到宝藏
							if (((Treasure*)myObject)->GetType() == STRENTH)	//获得大力水,得到力量buff加成
								isGetStrenthBuff = true;
							else if (((Treasure*)myObject)->GetType() == LUCK)	//获得幸运草,得到金币buff加成
								isGetMoneyBuff = true;
							if (((Treasure*)myObject)->GetType() != MONEY && isSoundEffectsOn)
								PlaySound((LPCWSTR)IDR_GETBUFF, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						}
						// 删除矿物
						mObjects[i].RemoveAt(pos2);
						mHook->SetVback(0);	//重置钩子速度
						int memScore;
						if (isGetMoneyBuff)	//有金币buff就获得双倍积分
							memScore = myObject->GetScore() * 2;
						else
							memScore = myObject->GetScore();
						Score::AddMyScore(memScore);	//增加分数
						score->SetIfDrawAddScore(1, memScore);	//通知Score绘制加分画面
						if (memScore != 0 && isSoundEffectsOn)
							PlaySound((LPCWSTR)IDR_GETMONEY, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						delete myObject;
						useForFirstTime = 0;
						break;
					}
					if(isSoundEffectsOn)
						PlaySound((LPCWSTR)IDR_PULLMINE, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
					break;
				}//if
			}//for
		}//for

		 //自动挖矿(催眠模式)
		if (isAutoModeOn) {
			//原理:检测 矿物中心与钩具(489,97)的角度 与 钩子与钩具的角度之差,当差值<=2°时,自动出钩  抓猪的话只能随缘了hh
			int centerx, centery, angle1, angle2;	//center:矿物中心点 angle1: 钩子与钩具的角度 angle2:矿物与钩具的角度
			angle1 = mHook->GetAngle();
			//遍历所有矿物
			for (int i = PIGS; i <= STATIC_MINE; i++) {
				for (pos1 = mObjects[i].GetHeadPosition(); (pos2 = pos1) != NULL;)
				{
					myObject = (MyObject*)mObjects[i].GetNext(pos1);	//获取矿物对象
					centerx = myObject->GetRect().left + (int)myObject->GetRect().Width() / 2;
					centery = myObject->GetRect().top + (int)myObject->GetRect().Height() / 2;
					double mcos = (centery - 97) / sqrt((centerx - 489)*(centerx - 489) + (centery - 97)*(centery - 97));
					angle2 = (int)(acos(mcos) / PI * 180);
					angle2 = (centerx - 489) < 0 ? angle2 : -angle2;
					if (abs(angle2 - angle1) <= 2 && myObject->GetIsCatch() == 0) {	//钩具,钩子,矿物在一条直线上,而且该矿物不在钩子上
						if (mHook->GetStatus() == HOOK_KEEP) {
							mHook->SetStatus(HOOK_OUT);
							if (isSoundEffectsOn)
								PlaySound((LPCWSTR)IDR_HOOKOUT, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						}
						i = STATIC_MINE + 1;
						break;
					}
				}//for
			}//for
		}
		//大威天龙!
		if (isLegendModeOn && useSkill) {	//生成全屏炸弹清屏,并且获取10000分
			int addScore = 10000;
			Score::AddMyScore(addScore);	//增加分数
			score->SetIfDrawAddScore(1, addScore);	//通知Score绘制加分画面
			for(int x = 0;x<1024;x+=256)	//全屏炸弹
				for(int y=128;y<768;y+=160)
					mObjects[EXPLOSION].AddTail(new Explosion(x, y));
			useSkill = 0;
			frame = 50;
			PlaySound((LPCWSTR)IDR_SKILLSOUND, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}

		memDC.SetTextAlign(TA_CENTER);
		if (frame) {		//绘制"大威天龙!"大字
			//设置字体
			font.CreateFont(192,                                    //   字体的高度   
				0,                                          //   字体的宽度  
				0,                                          //  nEscapement 
				0,                                          //  nOrientation   
				FW_NORMAL,                                  //   nWeight   
				FALSE,                                      //   bItalic   
				FALSE,                                      //   bUnderline   
				0,                                                   //   cStrikeOut   
				ANSI_CHARSET,                             //   nCharSet   
				OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
				CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
				DEFAULT_QUALITY,                       //   nQuality   
				DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
				_T("微软雅黑"));

			oldFont = memDC.SelectObject(&font);
			msg.Format(_T("大威天龙!"));
			memDC.TextOut(512, 334, msg);
			memDC.SelectObject(oldFont);//选择回老字体
			font.DeleteObject();//删除新字体
			frame--;
		}

		score->Draw(&memDC);

		//绘制时间条
		brush.CreateSolidBrush(RGB(255, 0, 0));
		bar.top = TOP_OFFSET ;
		bar.left = LEFT_OFFSET;
		bar.right = (int)(LEFT_OFFSET + BAR_LEN * mTimeLeft* 1.0 / TOTAL_TIME);
		bar.bottom = bar.top + 20;
		memDC.FillRect(bar, &brush);

		//设置字体
		font.CreateFont(24,                                    //   字体的高度   
			0,                                          //   字体的宽度  
			0,                                          //  nEscapement 
			0,                                          //  nOrientation   
			FW_NORMAL,                                  //   nWeight   
			FALSE,                                      //   bItalic   
			FALSE,                                      //   bUnderline   
			0,                                                   //   cStrikeOut   
			ANSI_CHARSET,                             //   nCharSet   
			OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
			CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
			DEFAULT_QUALITY,                       //   nQuality   
			DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
			_T("微软雅黑"));

		oldFont = memDC.SelectObject(&font);
		msg.Format(_T("剩余时间: %d"), mTimeLeft);
		memDC.TextOut((int)(bar.left + bar.right) / 2, bar.bottom + 4, msg);
		memDC.SetTextAlign(TA_LEFT);
		if (isGetStrenthBuff) {	//绘制buff信息
			msg.Format(_T("+大力BUFF"));
			memDC.TextOut(310, 65, msg);
		}
		if (isGetMoneyBuff) {
			msg.Format(_T("+双倍金币BUFF"));
			memDC.TextOut(310, 85, msg);
		}
		if (isAutoModeOn) {
			msg.Format(_T("自动模式已开启"));
			memDC.TextOut(445, 20, msg);
		}
		memDC.SelectObject(oldFont);//选择回老字体
		font.DeleteObject();//删除新字体

		break;
	case SETTING_ACTIVITY:
		memBMP.LoadBitmapW(IDB_SETTING);
		memDC.SelectObject(&memBMP);
		//根据开关是否"on"决定是否画上"√"
		if (isSoundEffectsOn)
			DrawYes(&memDC, 347, 288);
		if (isMusicOn)
			DrawYes(&memDC, 347, 393);
		if (isAutoModeOn)
			DrawYes(&memDC, 347, 498);
		if (isLegendModeOn)
			DrawYes(&memDC, 347, 606);
		break;
	case BRIEF_ACTIVITY:
		memBMP.LoadBitmapW(IDB_BRIEF);
		memDC.SelectObject(&memBMP);
		break;
	default:
		break;
	}

	////调试用,单击后显示鼠标坐标
	//CString str;
	//str.Format(_T("[%d,%d]   "), mouseX, mouseY);
	//memDC.SetTextColor(RGB(255, 0, 0));
	//memDC.SetTextAlign(TA_CENTER);
	//memDC.TextOut(500, 50, str);

	pDC->BitBlt(0, 0, WIN_WIDTH, WIN_HEIGHT, &memDC, 0, 0, SRCCOPY);
}

void CDragonMinerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnTimer(nIDEvent);
	//客户区重绘(FALSE不会清空原屏幕,TRUE会清空再重绘)
	Invalidate(FALSE);
}


// 在指定位置画√
void CDragonMinerView::DrawYes(CDC * pDC, int startX, int startY)
{
	CBitmap mBMP;
	CDC mDC;
	BITMAP bmpinfo;
	int bmWidth, bmHeight;
	mBMP.LoadBitmapW(IDB_YES);
	mDC.CreateCompatibleDC(pDC);
	CBitmap* old = mDC.SelectObject(&mBMP);
	mBMP.GetBitmap(&bmpinfo);
	bmWidth = bmpinfo.bmWidth;
	bmHeight = bmpinfo.bmHeight;
	pDC->TransparentBlt(startX, startY, bmWidth, bmHeight, &mDC, 0, 0,bmWidth,bmHeight, RGB(255,255,255));
	mDC.SelectObject(old);
	old->DeleteObject();
}

void CDragonMinerView::initGame() {		//从主界面进入游戏界面时的初始化
	//countStaticMine = countPigMine = countTreasure = countTnt = 0;
	useForFirstTime = 0;
	useSkill = 0;
	isGameOver = false;
	isGamePause = false;
	isGoNextLevel = true;	//可以前往第一关
	
	//若不读档则并重置分数与关卡等级
	if (!ifReadSave) {
		Score::SetMyScore(0);
		Score::SetTotalScore(0);
		Score::SetGameLevel(0);
	}
	//清空上一次游戏的钩子与所有矿物与爆炸效果
	//清空mObjects,销毁其中的对象释放内存
	POSITION pos;	
	int i;
	for (i = HOOK; i<=EXPLOSION; i++)
	{
		pos = mObjects[i].GetHeadPosition();		//定位i矿物的第一个元素索引
		while (pos != NULL)
		{
			delete mObjects[i].GetNext(pos);	//删除pos的元素并将pos指向下一个元素的位置
		}
		mObjects[i].RemoveAll();	//删除列表中所有的元素
	}
}

void CDragonMinerView::initLevel() {		//每个关卡的初始化,当进入下一关/第一关时调用
	mTimeLeft = TOTAL_TIME;		//重置剩余时间
	isGetStrenthBuff = false;	//大力水buff清空
	isGetMoneyBuff = false;		//金币buff清空
	useForFirstTime = 0;
	Score::SetGameLevel(Score::GetGameLevel() + 1);		//更新关卡等级;
	//清空上一关未被抓完的矿物与可能不在原处的钩子与爆炸效果
	//清空mObjects,销毁其中的对象释放内存
	POSITION pos;	
	int i;
	for (i = HOOK; i<=EXPLOSION; i++)
	{
		pos = mObjects[i].GetHeadPosition();
		while (pos != NULL)
		{
			delete mObjects[i].GetNext(pos);	//删除pos的元素并将pos指向下一个元素的位置
		}
		mObjects[i].RemoveAll();	//删除列表中所有的元素
	}

	//生成新关卡的矿物与钩子,不能重叠,计算新关卡的目标分数
	int count,sum,x,y;
	MyObject *myObj;
	CRect rect;
	sum = 0;
	mObjects[HOOK].AddTail(new Hook());	//钩子
	// 猪/钻石猪,0-2只
	count = rand() % 3;
	sum += count;
	while (count--) {
		x = rand() % 551;	// 550 + PigWidth + 400(小猪向右移动最远距离) = 1024
		y = rand() % 594 + 128;	//593 + PigHeight +128 = 768    128为矿洞的topY
		myObj = new PigMine(x, y);
		rect = myObj->GetRect();
		rect.right += 400;	//猪会往右走400像素
		if (isIntersectRect(rect)) {	//如果和其他矿物重叠
			delete myObj;	//删除这只猪
			count++;	//重新生成
		}
		else {
			mObjects[PIGS].AddTail(myObj);
			if(!ifReadSave)	//读档时第一次初始化关卡不增加目标分数时
				Score::AddTotalScore(myObj->GetScore());
		}
	}
	// TNT 0-2个
	count = rand() % 3;
	sum += count;
	while (count--) {
		x = rand() % 960;
		y = rand() % 576 + 128;
		myObj = new Tnt(x, y);
		rect = myObj->GetRect();
		if (isIntersectRect(rect)) {	//如果和其他矿物重叠
			delete myObj;	//删除
			count++;	//重新生成
		}
		else {
			mObjects[TNT].AddTail(myObj);
			if (!ifReadSave)	//读档时第一次初始化关卡不增加目标分数时
				Score::AddTotalScore(myObj->GetScore());
		}
	}
	// treasure 0-2个
	count = rand() % 3;
	sum += count;
	while (count--) {
		x = rand() % 928;
		y = rand() % 544 + 128;
		myObj = new Treasure(x, y);
		rect = myObj->GetRect();
		if (isIntersectRect(rect)) {	//如果和其他矿物重叠
			delete myObj;	//删除
			count++;	//重新生成
		}
		else {
			mObjects[TREASURE].AddTail(myObj);
			if (!ifReadSave)	//读档时第一次初始化关卡不增加目标分数时
				Score::AddTotalScore(myObj->GetScore());
		}
	}
	// 静态矿物的数量取 random(5,6,7,8) 与 (10-sum)的最大值,保证关卡至少有10个矿物.
	count = rand() % 4 + 5;
	count = count > (10 - sum) ? count : (10 - sum);
	int createTime = 0;
	while (count--) {
		x = rand() % 833;
		y = rand() % 512 + 128;
		myObj = new StaticMine(x, y);
		rect = myObj->GetRect();
		if (isIntersectRect(rect)) {	//如果和其他矿物重叠
			delete myObj;	//删除
			count++;	//重新生成
			createTime++;
			if (createTime == 20) {		//尝试生成20次还不行说明可能无法生成不相交的矿物了,所以直接退出生成
				break;
			}
		}
		else {
			mObjects[STATIC_MINE].AddTail(myObj);
			if (!ifReadSave)	//读档时第一次初始化关卡不增加目标分数时
				Score::AddTotalScore(myObj->GetScore());
		}
	}
	ifReadSave = false;
}

bool CDragonMinerView::isIntersectRect(CRect rect) {		//判断新生成的矿物是否与原有的其他矿物重叠
	MyObject *tmpObj;
	CRect explosionRect,junkRect;
	POSITION pos;	
	for (int i = PIGS; i <= STATIC_MINE; i++) {
		pos = mObjects[i].GetHeadPosition();//定位i矿物的第一个的索引
		while (pos != NULL)
		{
			tmpObj = (MyObject*)mObjects[i].GetNext(pos);	//获取pos处的元素并将pos指向下一个元素的索引
			explosionRect = tmpObj->GetRect();
			if (i == PIGS) explosionRect.right += 400;	//猪会往右走400像素
			if (junkRect.IntersectRect(explosionRect, rect))
				return true;	//重叠返回true
		}
	}
	return false;	//未重叠返回false
}

void CDragonMinerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	mouseX = point.x;
	mouseY = point.y;
	CFile file;
	switch (activityMode)
	{
	case MAIN_ACTIVITY:
		//判断在主界面时鼠标是否点击到了按钮.第一个按钮"开始游戏"左上顶点为(358,259),右下顶点为(680,381)
		if (mouseX >= 358 && mouseX <= 680 && mouseY >= 259 && mouseY <= 381) {
			if (AfxMessageBox(_T("是否读档?"), MB_YESNO, 0) == IDYES)		//选是,写入当前关卡与当前分数
			{
				if (!file.Open(_T("save.txt"), CFile::modeRead, NULL)) {	//若打不开存档,重开游戏
					ifReadSave = false;
					activityMode = GAME_ACTIVITY;	//变更活动模式为GAME_ACTIVITY
					initGame();		//初始化游戏
					break;
				}
				file.SeekToBegin();
				int Rev;
				file.Read(&Rev, sizeof(int));
				score->SetGameLevel(Rev-1);		//在initLevel时会+1,所以此处-1
				file.Read(&Rev, sizeof(int));
				score->SetMyScore(Rev);
				file.Read(&Rev, sizeof(int));
				score->SetTotalScore(Rev);
				ifReadSave = true;
			}else
				ifReadSave = false;
			activityMode = GAME_ACTIVITY;	//变更活动模式为GAME_ACTIVITY
			initGame();		//初始化游戏
		}
		else if (mouseX >= 358 && mouseX <= 680 && mouseY >= 427 && mouseY <= 551) {
			activityMode = SETTING_ACTIVITY;	//变更活动模式为SETTING_ACTIVITY
		}
		else if (mouseX >= 358 && mouseX <= 680 && mouseY >= 592 && mouseY <= 717) {
			activityMode = BRIEF_ACTIVITY;	//变更活动模式为BRIEF_ACTIVITY
		}
		break;
	case SETTING_ACTIVITY:
		if (mouseX >= 341 && mouseX <= 590 && mouseY >= 276 && mouseY <= 347) {
			isSoundEffectsOn = !isSoundEffectsOn;	//音效开关变更
		}
		else if (mouseX >= 341 && mouseX <= 590 && mouseY >= 381 && mouseY <= 452) {
			isMusicOn = !isMusicOn;	//音乐开关变更
			if (isMusicOn) {
				mciSendString(_T("play res/mus/bgm.mp3 repeat"), NULL, 0, NULL);
			}
			else {
				mciSendString(_T("close res/mus/bgm.mp3"), NULL, 0, NULL);
			}
		}
		else if (mouseX >= 341 && mouseX <= 712 && mouseY >= 486 && mouseY <= 556) {
			isAutoModeOn = !isAutoModeOn;
		}
		else if (mouseX >= 341 && mouseX <= 728 && mouseY >= 594 && mouseY <= 665) {
			isLegendModeOn = !isLegendModeOn;
		}
		break;
	default:
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CDragonMinerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MyObject * myObject;
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_ESCAPE) {
		//在游戏界面按"esc"键会先询问是否存档
		if (activityMode == GAME_ACTIVITY) {
			if (AfxMessageBox(_T("是否存档?"), MB_YESNO, 0) == IDYES)		//选是,写入当前关卡与当前分数
			{
				CFile file;
				file.Open(_T("save.txt"), CFile::modeCreate | CFile::modeWrite, NULL);
				int value;
				value = Score::GetGameLevel();
				file.Write(&value, sizeof(int));
				value = Score::GetMyScore();
				file.Write(&value, sizeof(int));
				value = Score::GetTotalScore();
				file.Write(&value, sizeof(int));
				file.Close();
			}
		}
		activityMode = MAIN_ACTIVITY;	//esc返回主界面
	}
		

	if (activityMode == GAME_ACTIVITY) {//主游戏界面中对按键的反映
		switch (nChar)
		{
		case 'p':
		case 'P':
			isGamePause = !isGamePause;
			break;
		case 'n':
		case 'N':
			if (Score::GetMyScore() >= int(Score::GetTotalScore() *2 / 3)) {	//如果符合过关条件就去下一关
				isGoNextLevel = 1;
			}
			else {
				isGamePause = true;
				if (AfxMessageBox(_T("分数不够还想蒙混过关?"), MB_OK, 0) == IDOK)
					isGamePause = false;
			}
			
			break;
		case 'a':
		case 'A':
			isAutoModeOn = !isAutoModeOn;
			break;
		case 'r':
		case 'R':
			useSkill = 1;
			break;
		case VK_UP:
			myObject = (MyObject*)mObjects[HOOK].GetHead();
			//收钩(只能在出钩时收钩,否则钩子会往天上飞hhh)
			if (((Hook*)myObject)->GetStatus() == HOOK_OUT) {
				((Hook*)myObject)->SetStatus(HOOK_IN);
			}
			break;
		case VK_DOWN:
			myObject = (MyObject*)mObjects[HOOK].GetHead();
			//只能在等待时出钩
			if (((Hook*)myObject)->GetStatus() == HOOK_KEEP) {
				((Hook*)myObject)->SetStatus(HOOK_OUT);
				if(isSoundEffectsOn)
					PlaySound((LPCWSTR)IDR_HOOKOUT, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			break;
		}
	}
}