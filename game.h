#define  _CRT_SECURE_NO_WARNINGS
#include<easyx.h>
#include<iostream>
#include<cstring>
#include<sstream>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<fstream>
#include<Windows.h>
#include <Mmsystem.h>
#include<vector>
#include<set>
using namespace std;
#pragma comment(lib, "winmm.lib")
#pragma once
#define xuanlan

// ----------数据设计-------------
//素材：地图素材，障碍物
IMAGE MAP; IMAGE Obstacle;
//定义窗口大小800x800
const int HEIGHT = 800;
const int WIDTH = 800;

//添加一个变量，地图
int map[80][80];
//游戏是否结束
bool isGameOver = false;
//是否是双人
bool istwoplayer = false;
// 用户结构体
struct UserData {
	char username[20];
	char password[20];
	int unlockedLevel = 1;//解锁关卡（默认为1）
};
UserData current_user;//当前 注册/登录 用户
UserData read_user;   //使用该结构体对象按 struct 为单位从文件中读取
//玩家排行榜数据
struct player_rank
{
	char name[20];
	int score;
	bool operator<(const player_rank& u)const
	{
		return this->score > u.score;
	}
};
//判断在哪里调用的函数，方便关闭和开启音乐
bool isFree = false;
bool isMenu = false;
bool isTwo = false;
bool isLevel = false;

#ifdef xuanlan
//迷宫 
int maze[5][16][16] = {
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	},
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1},
		{1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
		{1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
		{1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},
	{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	}
};
int beginX[6] = { 0, 3, 3, 3, 3, 3 }, beginY[6] = {0, 1, 1, 1, 1, 1};
int endX[6] = { 0, 14, 14, 14, 14, 14 }, endY[6] = {0, 14, 14, 14, 14, 14};
//关卡
int Level[10];//1:解锁；0:没解锁 
int unlock = 1;//解锁的数量
#endif

/*
	蛇：
	蛇身：
		我们给蛇设置一个vector数组，放置蛇的每一节的x和y坐标
	方向
	长度
*/
struct Snake {
	char dir;//蛇的方向
	int len;//蛇的长度
	int x[80];//蛇身的x
	int y[80];//蛇身的y
	//--------------扩展----------------//
	int speed = 200;//蛇的移动间隔
	int score;
	bool alive = true;
	bool ismove = false;
	//----------------------------------//
};
Snake snake; //蛇（自由模式 闯关模式）

//双人模式
Snake snake1;//P1
Snake snake2;//P2


int foodx[5];//果实的x坐标
int foody[5];//果实的y坐标
int buffx[3];
int buffy[3];//buff果实坐标
int debuffx[3];
int debuffy[3];//debuff果实坐标
//获得分数
int score;

//判断是否移动
bool isMove = false;

// Re0 自用工具函数 功能：居中字体显示
void centertext(int x, int y, int w, int h, int testsize, const char* text) {
	settextstyle(testsize, 0, "黑体");
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;
	outtextxy(tx, ty, text);
}
// ----------数据设计-------------


//----------service--------------
/*
	负责人：diup
	功能：初始化游戏数据
		  设置蛇的长度和位置（初始长度为3 中间的最右边）
		  设置蛇的初始方向(初始向左)
		  创建游戏窗口
		  生成第一个食物
		  分数初始化为零
		  分数框显示在分数居中向上
		  GameOver=false
	参数：void
	返回值：void
*/
void InitGame();

/*
	负责人：田文星
	功能：蛇按当前方向移动 isMove=true;
		  如果可以移动，更新蛇身坐标（蛇身每个坐标依次+1）
		 参数：void
		 返回值：void
*/
void MoveSnake();
void MoveSnake(Snake& snake);

void MoveTwoSnake(Snake& snake);

/*
	负责人: 哈吉米
	功能: 生成新食物
		在地图空地中随机生成食物位置
	参数: void
	返回值: void
*/
void generateFood(int i);

#ifdef xuanlan
/*
	负责人:
	功能: 生成新buff
		在地图空地中随机生成食物位置
	参数: void
	返回值: void
*/
void generateBuffFood(int i);

/*
	负责人:
	功能: 生成新debuff
		在地图空地中随机生成食物位置
	参数: void
	返回值: void
*/
void generateDebuffFood(int i);
#endif

/*
	负责人：月月鸟
	功能：根据当前移动方向延长蛇尾，增加蛇长
	参数：void
	返回值：void
*/
void ExtendTail();
void ExtendTail(Snake& snake);

/*
	难点2
	负责人：yuan
	功能：碰撞检测
		 检查蛇移动之后是否撞到自己或者墙
		 若有，修改GameOver=true，进入游戏结算
		 若无，继续游戏
	参数：void
	返回值：void
*/
void check(float& moveInterval);

void checkTwoSnake(Snake& snake, Snake& snake2);

/*
	负责人：哈吉米
	功能：暂停游戏
		停止蛇的移动 isMove=false;
	参数：void
	返回值：void
void StopMove();*/

/*
	负责人：xxx
	功能：继续游戏
		继续蛇的移动
	参数：void
	返回值：void

void Continue();*/

/*
	难点2
	负责人：绚烂
	功能：判断是否能改变方向
	需要判断，转换方向之前，蛇身的x，y坐标是否一致，避免反向操作
	参数值：bool char dir
	返回值：bool
*/
bool isChangeDirection(char dir);
bool isChangeDirectionAndGostraight(char dir);

/*
	负责人：绚烂
	功能：改变方向
	当接受到玩家的wasd或者上下左右时
	将蛇头的坐标标记(map[x][y] = 4)，每次蛇头或者蛇身移动到
	该标记坐标的时候修改方向，修改完成之后，将该坐标重置成0
	参数值：void char dir
	返回值: void
*/
void ChangeDirection(char dir);

/*
	负责人：田文星
	功能：用于结束当前还在进行的游戏
		  当结束游戏之后，进入游戏结算界面（调用gameoverView()）
	参数值：void
	返回值：void
*/
void EndGame();

/*
	负责人:diup
	功能：用于解析用户输入
		方向键 修改方向 调用changeDirection(char dir)
		空格键 暂停 调用Stop()
*/
void handleInput();
void handleInput(Snake& snake1, Snake& snake2);


/*
* 负责人：李瑞
* 功能：设计一个迷宫
*/
void designOneMaze();

/*
	负责人：绚烂
	功能：在自由模式移动蛇
		思路基本和MoveSnake()一样
		但要处理蛇头超出地图的问题
			例如: 蛇的方向是'W',而snake.y[0] = 0
				  那么就要设计为snake.y[0] = 79;
	参数：level -- 关卡
	返回值：void
*/
void FreeModuleMoveSnake();

/*
	负责人：diup
	功能：初始化自由模式 -- 基本模仿InitGame()，但不要画图
		初始化snake的起始位置和方向
		初始化地图 -- 将map初始化为0 -- 因为没有墙
		初始化分数 -- score = 0
		初始化果实 -- generateFood();
		isGameOver 设为 false;
*/
void initFreeGame();

//void initOneMaze();

bool isWall(char dir, int level);

void InittwoplayerGame(Snake& snake1, Snake& snake2);

void competieiveMouduleGame();

//更新排行榜
void updatarank();

//----------service--------------

//----------view-----------------
/*
	负责人: Re0
	功能: 展示选项, 玩家可以在这里选择开始游戏、进入设置或退出游戏
		while(true){
			1. 展示选项
			2. 用户输入
			3. 根据输入进行对应处理
				开始游戏: 调用游戏界面函数gameView();
				进入设置: 调用设置界面ShowSetUp()
				退出游戏: 调用exit(0);
		}
	参数: void
	返回值: void
*/
void menuView();

/*
	负责人: diup
	功能: 根据map数组 打印游戏地图
		  0显示为空格，1显示为墙，2显示为食物,3为蛇
		  根据小蛇现在的长度（调用ExtendTail()），使用easyx绘画更新
	参数: void
	返回值: void
*/
void gameView_ShowMap();

/*
	难点4
	负责人：yuan
	功能：gameView
		整合游戏画面
		调用InitGame()初始化
	while(true){
		展示地图 调用gameView_ShowMap()

		移动小蛇 调用MoveSnake()
		吃到了果实 调用ExtendTail()
		如果接收到了玩家的键盘输入 调用handleInput()

		判断游戏是否结束 调用Check()	或者是 EndeGame()
	}
	参数值 void
	返回值 void
*/
void gameView();

/*
	负责人：月月鸟
	功能：暂停
		当玩家按空格键时，进入暂停界面
		此时需要停止蛇的移动 isMove=false
		并且需要展示继续游戏和结束游戏两种选择（按钮）
		当玩家点击相应的按钮之后，需要触发对应的功能
		继续游戏就调用gameView()
		结束游戏就调用EndGame()
	参数：void
	返回值：void
*/
void Stop();

/*
	负责人：Re0
	功能：展示设置界面
		  提供查看排行榜、查看游戏团队、查看游戏说明、返回菜单
		  while(1){
			查看排行榜：调用SeeRank()
			查看游戏团队:调用teamView()
			查看游戏说明：调用howToPlayView()
			返回菜单：调用menuView()
		  }
*/
void ShowSetUp();

/*
	负责人: 田文星
	功能: 展示团队介绍界面
		打印团队成员信息，用户按任意键返回设置
	参数: void
	返回值: void
*/
void teamView();

/*
	负责人: 哈吉米
	功能: 展示玩法说明界面
		打印游戏规则和操作说明，用户按任意键返回设置
	参数: void
	返回值: void
*/
void howToPlayView();

/*
	负责人：diup
	功能：查看排行榜，玩家按任意键返回设置
	参数：void
	返回值：void
*/
void SeeRank();

/*
	负责人：Re0
	功能：展示游戏结算界面
		在界面中心显示GamOver
		在GameOver上方打印游戏的分数
		玩家点击任意键返回菜单（调用menuView()）
	参数：void
	返回值：void
*/
void GameOverView();
void TwoplayerGameOverView();

/*
* 负责人：绚烂
* 功能：关卡
*    设计一个迷宫 -- designOneMaze();
*    设计关卡界面,未解锁的关卡画一个锁
*	 点击解锁的关卡进入游戏界面
*    返回主菜单
*/
void levelChoiceView();

/*
	负责人：
	功能：关卡模式
		initMaze(int level);
		while(true) {
			·解析键盘输入
			·判断是否移动 -> 若是，则移动一格
			·绘制游戏画面
			·判断是否到达终点 -> 若是，进入levelWinView(), 且修改Level[++unlock]
		}
*/
void levelModuleGameView(int level);

/*
	负责人：田文星
	功能：自由模式
		initFreeGame();
		while(true) {
			基本和gameView()一样
			MoveSnake()改为FreeModuleMoveSnake();
		}
*/
void freeModuleGameView();

/*
	负责人：月月鸟
	功能：绘制游戏选择界面,可以选择“自由模式”“关卡模式”“竞技模式”
		  点击“自由模式”，进入freeModuleGame();
		  点击“关卡模式”，进入levelChoiceView();
		  点击“竞技模式”，进入competitiveModuleGame();
*/
void gameModuleChoiceView();

/*
* 负责人：绚烂
* 功能：
*	关卡胜利界面：显示关卡level胜利
*	点击返回关卡选择界面
*/
void levelWinView(int level);

/*
* 负责人：Re0
* 功能：登录
*	1.设计登录界面  登录框、登录按钮、新用户注册、退出游戏
*   2.登录：
*		   1>点击账号框、密码框
*          2>输入账号和密码
*          3>点击“确认登录”，将其 账号 密码 赋值给 current_user
*          4>从文件 user.dat 中逐个读取用户存储到 read_user中，并与 current_user 比对
*          5>匹配：成功：进入主菜单（跳出循环）        失败：显示错误置空 current_user 后进入下一个循环
*   3.新用户注册
*          1>点击“新用户注册”
*          2>进入注册界面 调用logon();
*   4.退出游戏
*          exit(0);
*
*  备注：成功登录后，当前用户信息应该被保存在 current_user 中
*/
void login();

/*
* 负责人：yuan
* 功能：...
*/
void logon();

/*
* 负责人：
* 功能：播放音乐
*/
void playBackgroundMusic(const char* music);

void competitiveModuleGame();

void gameView_Twoplayer_ShowMap();

void addLock(int flag, int x, int y);

void LevelStop();