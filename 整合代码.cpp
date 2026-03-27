#include "game.h"

#define xuanlan

//----------view-----------------
void InitGame()
{
	snake.dir = 'D';
	snake.len = 6;
	snake.x[0] = 40; snake.y[0] = 40; //  头部居中
	snake.x[1] = 39; snake.y[1] = 40;  // 身体1在左
	snake.x[2] = 38; snake.y[2] = 40;  // 身体2在左
	snake.x[3] = 37; snake.y[3] = 40;  // 身体3在左
	snake.x[4] = 36; snake.y[4] = 40;  // 身体4在左
	snake.x[5] = 35; snake.y[5] = 40;  // 身体5在左
	
	// 初始化地图边界
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			map[i][j] = (i == 0 || i == 79 || j == 0 || j == 79) ? 1 : 0;
		}
	}
	setbkcolor(WHITE);
	cleardevice();
	score = 0;//初始化分数
	fillrectangle(20, 20, 80, 80);
	settextcolor(RED);
	settextstyle(30, 0, "微软雅黑");
	char str1[50] = "";
	sprintf_s(str1, "Score:%d", score);
	outtextxy(20, 20, str1);
	isGameOver = false;
	//创建果实
	for (int i = 0; i < 5; i++) generateFood(i);
	for (int i = 0; i < 3; i++) generateBuffFood(i);
	for (int i = 0; i < 3; i++) generateDebuffFood(i);
}

//更新排行榜
void updatarank()
{
	string filmname = "player_rank.dat";
	player_rank newplayer,readplayer;// 新成员 和 读取成员
	multiset<player_rank>m;//set容器排序
	strcpy(newplayer.name, current_user.username);
	newplayer.score = score;
	ifstream fin(filmname, ios::in | ios::binary);
	if (!fin) {//不存在则创建
		ofstream fout(filmname, ios::out | ios::binary);
		fout.write((const char*)&newplayer, sizeof(player_rank));
		fout.close();
		return;
	}
	while (fin.read((char*)&readplayer, sizeof(player_rank))) {
		m.insert(readplayer);
	}
	fin.close();
	m.insert(newplayer);
	while (m.size() > 10) {
		m.erase(prev(m.end()));// 大于 10 则删去末尾元素
	}
	ofstream fout(filmname, ios::out | ios::binary);
	for (auto& it : m) {
		fout.write((const char*)&it, sizeof(player_rank));
	}
	fout.close();
	m.clear();
	return;
}

void MoveSnake() {
	isMove = true;
	// 保存蛇头位置用于碰撞检测
	int snakeX = snake.x[0];
	int snakeY = snake.y[0];
	// 更新蛇身体各部分的位置
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	// 根据当前方向移动蛇头
	if (snake.dir == 'W') {      // 上
		snake.y[0]--;
	}
	else if (snake.dir == 'S') { // 下
		snake.y[0]++;
	}
	else if (snake.dir == 'A') { // 左
		snake.x[0]--;
	}
	else if (snake.dir == 'D') { // 右
		snake.x[0]++;
	}
}

void MoveSnake(Snake& snake) {
	snake.ismove = true;
	// 保存蛇头位置用于碰撞检测
	int snakeX = snake.x[0];
	int snakeY = snake.y[0];
	// 更新蛇身体各部分的位置
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	// 根据当前方向移动蛇头
	if (snake.dir == 'W') {      // 上
		snake.y[0]--;
	}
	else if (snake.dir == 'S') { // 下
		snake.y[0]++;
	}
	else if (snake.dir == 'A') { // 左
		snake.x[0]--;
	}
	else if (snake.dir == 'D') { // 右
		snake.x[0]++;
	}
}

void generateFood(int i) {
	//指定系统时间为随机种子，效果是每次运行生成的随机数都不一样。
	//调用生成随机数函数需要引入头文件time.h和stdlib.h
	srand(time(0));
		while (1) {
			//用i，j记录随机生成的数
			foodx[i] = rand() % 80;
			foody[i] = rand() % 80;
			if (map[foody[i]][foodx[i]] == 0) {
				map[foody[i]][foodx[i]] = 2;
				break;
			}
		}
}

#ifdef xuanlan
void generateBuffFood(int i) {
	srand(time(0));
		while (1) {
			//用i，j记录随机生成的数
			buffx[i] = rand() % 80;
			buffy[i] = rand() % 80;
			if (map[buffy[i]][buffx[i]] == 0) {
				map[buffy[i]][buffx[i]] = 4;
				break;
			}
		}
}

void generateDebuffFood(int i) {
	srand(time(0));
		while (1) {
			//用i，j记录随机生成的数
			debuffx[i] = rand() % 80;
			debuffy[i] = rand() % 80;
			if (map[debuffy[i]][debuffx[i]] == 0) {
				map[debuffy[i]][debuffx[i]] = 5;
				break;
			}
		}
}
#endif
void CloseMusic() {
	if (isMenu==true) { mciSendString("stop ./素材/music/贪吃蛇的欢乐时光.mp3", 0, NULL, NULL); isMenu = false; }
	if (isFree==true) { mciSendString("stop ./素材/music/自由模式.mp3", 0, NULL, NULL); isFree = false; }
	if (isLevel==true) { mciSendString("stop ./素材/music/关卡模式.mp3", 0, NULL, NULL); isLevel = false; }
	if (isTwo==true) { mciSendString("stop ./素材/music/双人模式.mp3", 0, NULL, NULL); isTwo = false; }
}

void ExtendTail()
{
	int len = snake.len;
	int _x = snake.x[len - 1], _y = snake.y[len - 1];
	MoveSnake();
	snake.x[len] = _x;
	snake.y[len] = _y;
	snake.len++;
}

void ExtendTail(Snake& snake)
{
	int len = snake.len;
	int _x = snake.x[len - 1], _y = snake.y[len - 1];
	MoveSnake(snake);
	snake.x[len] = _x;
	snake.y[len] = _y;
	snake.len++;
}

void check(float& moveInterval) {
	// 检测食物：蛇头坐标与食物坐标重合
	for (int i = 0; i < 5; i++) {
		if (snake.x[0] == foodx[i] && snake.y[0] == foody[i]) {
			map[foody[i]][foodx[i]] = 0;
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			moveInterval = 100;
			ExtendTail();
			generateFood(i);
			score++;
			break;
		}
	}

	// 检测buff：蛇头坐标与食物坐标重合
	for (int i = 0; i < 3; i++) {
		if (snake.x[0] == buffx[i] && snake.y[0] == buffy[i]) {
			map[buffy[i]][buffx[i]] = 0;
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			moveInterval = 50;
			ExtendTail();
			generateBuffFood(i);
			score++;
			break;
		}
	}

	// 检测debuff：蛇头坐标与食物坐标重合
	for (int i = 0; i < 3; i++) {
		if (snake.x[0] == debuffx[i] && snake.y[0] == debuffy[i]) {
			map[debuffy[i]][debuffx[i]] = 0;
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			moveInterval = 200;
			ExtendTail();
			generateDebuffFood(i);
			score++;
			break;
		}
	}

	// 检测墙壁碰撞
	for (int i = 0; i < snake.len; i++) {
		// 检查是否超出地图边界
		if (snake.x[i] < 0 || snake.x[i] >= 80 ||
			snake.y[i] < 0 || snake.y[i] >= 80) {
			isGameOver = true;
			return;
		}

		// 检查是否碰到地图中的墙壁
		if (map[snake.x[i]][snake.y[i]] == 1) {
			isGameOver = true;
			return;
		}
	}

	// 检测自身碰撞：蛇头与蛇身其他部分重合
	for (int i = 1; i < snake.len; i++) {
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
			isGameOver = true;
			return;
		}
	}
}

bool isChangeDirection(char dir) {
	if (snake.dir == 'W' || snake.dir == 'S' || snake.dir == 'w' || snake.dir == 's') {
		return (dir == 'A' || dir == 'D' || dir == 'a' || dir == 'd');
	}
	if (snake.dir == 'A' || snake.dir == 'D' || dir == 'a' || dir == 'd') {
		return (dir == 'W' || dir == 'S' || dir == 'w' || dir == 's');
	}
	return 0;
}

bool isChangeDirectionAndGostraight(char dir) {
	if (snake.dir == 'W' || snake.dir == 'w') {
		return (dir == 'A' || dir == 'D' || dir == 'a' || dir == 'd' || dir == 'W' || dir == 'w');
	}
	if (snake.dir == 'A' || snake.dir == 'a') {
		return (dir == 'A' || dir == 'a' || dir == 'W' || dir == 'w' || dir == 'S' || dir == 's');
	}
	if (snake.dir == 'S' || snake.dir == 's') {
		return (dir == 'A' || dir == 'D' || dir == 'a' || dir == 'd' || dir == 'S' || dir == 's');
	}
	if (snake.dir == 'D' || snake.dir == 'd') {
		return (dir == 'W' || dir == 'w' || dir == 'S' || dir == 's' || dir == 'D' || dir == 'd');
	}
	return 1;
}

void ChangeDirection(char dir) {
	snake.dir = dir;
	return;
}

void EndGame() {
	// 标记游戏结束状态
	isGameOver = true;
	// 停止蛇的移动
	isMove = false;
	// 调用游戏结算界面
	if (istwoplayer) {
		snake1.alive = snake2.alive = false;
	}
	else
		return;
}

void handleInput()
{
	char dir;
	ExMessage msg;
	while (peekmessage(&msg, EX_KEY))
	{
		switch (msg.vkcode)
		{
		case 'W':
		case VK_UP:
			dir = 'W';
			if (isChangeDirection(dir)) { ChangeDirection(dir); }
			break;
		case 'S':
		case VK_DOWN:
			dir = 'S';
			if (isChangeDirection(dir)) { ChangeDirection(dir); }
			break;
		case 'A':
		case VK_LEFT:
			dir = 'A';
			if (isChangeDirection(dir)) { ChangeDirection(dir); }
			break;
		case 'D':
		case VK_RIGHT:
			dir = 'D';
			if (isChangeDirection(dir)) { ChangeDirection(dir); }
			break;
		case VK_SPACE:
			Stop();
			break;
		}
	}
}

void handleInput(Snake& snake1, Snake& snake2)
{
	// 玩家1：WASD 控制
	if (GetAsyncKeyState('W') & 0x8000) {
		if (snake1.dir != 'S') snake1.dir = 'W';
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		if (snake1.dir != 'W') snake1.dir = 'S';
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		if (snake1.dir != 'D') snake1.dir = 'A';
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		if (snake1.dir != 'A') snake1.dir = 'D';
	}

	// 玩家2：方向键控制
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (snake2.dir != 'S') snake2.dir = 'W';
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (snake2.dir != 'W') snake2.dir = 'S';
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		if (snake2.dir != 'D') snake2.dir = 'A';
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		if (snake2.dir != 'A') snake2.dir = 'D';
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		Stop();
	}
}

void menuView() {
	
	IMAGE img;
	loadimage(&img, "./素材/image/title.png");
	ExMessage msg;
	CloseMusic();
	isMenu = true;
	mciSendString("play ./素材/music/贪吃蛇的欢乐时光.mp3 repeat", 0, NULL, NULL);
	while (true) {
		BeginBatchDraw();
		setbkmode(TRANSPARENT);
		setbkcolor(RGB(234, 234, 234));
		setfillcolor(RGB(255, 253, 180));
		settextcolor(RGB(127, 212, 130));
		cleardevice();
		putimage(35, -1, &img);
		fillroundrect(250, 370, 250 + 300, 370 + 100, 50, 50);
		centertext(250, 370, 300, 100, 50, "开始游戏");

		fillroundrect(250, 500, 250 + 300, 500 + 100, 50, 50);
		centertext(250, 500, 300, 100, 50, "设置");

		fillroundrect(250, 630, 250 + 300, 630 + 100, 50, 50);
		centertext(250, 630, 300, 100, 50, "退出游戏");
		EndBatchDraw();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (msg.x >= 250 && msg.x <= 250 + 300) {
					if (msg.y >= 370 && msg.y <= 370 + 100) {
						gameModuleChoiceView();
					}
					if (msg.y >= 500 && msg.y <= 500 + 100) {
						ShowSetUp();
					}
					if (msg.y >= 630 && msg.y <= 630 + 100) {
						exit(0);
					}
				}
			}
		}
		Sleep(10);
	}
}

void gameView_ShowMap()
{
		setbkcolor(WHITE);
		cleardevice();

		// 加载资源
		IMAGE img_space, img_wall, img_food, img_buff, img_debuff;
		loadimage(&img_space, "./素材/image/地图.jpg", 10, 10);
		loadimage(&img_wall, "./素材/image/墙.jpg", 10, 10);
		loadimage(&img_food, "./素材/image/普通果实.png", 10, 10);
		loadimage(&img_buff, "./素材/image/特殊果实.png", 10, 10);
		loadimage(&img_debuff, "./素材/image/debuff果实.png", 10, 10);
		//map[foody][foodx] = 2;
		// 绘制地图
		for (int row = 0; row < 80; row++) {
			for (int col = 0; col < 80; col++) {
				int screenX = col * 10;
				int screenY = row * 10;

				
				// 根据地图元素绘制
				switch (map[row][col]) {
				case 1: // 墙
					putimage(screenX, screenY, &img_wall);
					break;
				case 2: // 食物
					putimage(screenX, screenY, &img_food);
					break;
				case 4:
					putimage(screenX, screenY, &img_buff);
					break;
				case 5:
					putimage(screenX, screenY, &img_debuff);
					break;
				default: // 空地和其他情况
					putimage(screenX, screenY, &img_space);
					break;
				}
			}
		}
		//绘制蛇头
		setfillcolor(BLUE);
		setlinecolor(BLACK);
		fillroundrect(snake.x[0]*10 + 1, snake.y[0]*10 + 1,
			snake.x[0]*10 + 9, snake.y[0]*10 + 9,
			3, 3);
		// 绘制蛇身
		for (int i = 1; i < snake.len; i++) {
			int screenX = snake.x[i] * 10;
			int screenY = snake.y[i] * 10;

			setfillcolor(RED);
			setlinecolor(BLACK);
			fillroundrect(screenX + 1, screenY + 1,
				screenX + 9, screenY + 9,
				3, 3);
		}

		// 实时更新分数显示
		settextcolor(BLACK);
		settextstyle(24, 0, "Consolas");
		char scoreText[32];
		sprintf(scoreText, "SCORE: %d", score);
		outtextxy(20, 20, scoreText);
}

void Stop()
{
	cleardevice();

	ExMessage stop_msg = { 0 }; //定义消息结构体变量
	setfillcolor(RGB(230, 231, 232));
	fillrectangle(200, 100, 600, 250);
	settextstyle(100, 0, ("微软雅黑"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(250, 125, "继续游戏");
	fillrectangle(200, 550, 600, 700);
	outtextxy(250, 575, "结束游戏");

	while (true)
	{
		peekmessage(&stop_msg, EX_MOUSE); //用来获取继续游戏和结束游戏消息
		if (stop_msg.message == WM_LBUTTONDOWN) //按下鼠标左键
		{
			//按继续游戏
			if (stop_msg.x >= 200 && stop_msg.x <= 600 && stop_msg.y >= 100 && stop_msg.y <= 250)
			{
				break;
			}
			//按结束游戏
			else if (stop_msg.x >= 200 && stop_msg.x <= 600 && stop_msg.y >= 550 && stop_msg.y <= 700)
			{
				EndGame();
				break;
			}
		}
		Sleep(10);
	}
}

void gameView() {
	InitGame();
	DWORD lastUpdateTime = GetTickCount(); // 逻辑更新计时
	DWORD lastFrameTime = GetTickCount();  // 帧渲染计时
	float moveInterval = 50;        // 蛇移动间隔200ms
	const float frameInterval = 1000 / 60.0;  // 60帧/秒

	while (!isGameOver) {
		DWORD currentTime = GetTickCount();

		// 处理输入
		handleInput();

		// 独立逻辑更新：固定时间间隔移动
		if (currentTime - lastUpdateTime >= moveInterval) {

			//-------------测试------------------
			MoveSnake();
			//FreeModuleMoveSnake();
			check(moveInterval);
			//-----------------------------------
			//gameModuleChoiceView();

			lastUpdateTime = currentTime;
		}

		// 独立渲染更新：控制最大帧率
		if (currentTime - lastFrameTime >= frameInterval) {
			BeginBatchDraw();
			gameView_ShowMap();
			EndBatchDraw();
			lastFrameTime = currentTime;
		}

		Sleep(1); // 避免CPU空转
		//mciSendString("close C:\\Users\\张鹏\\Documents\\朝花夕拾\\第一次项目-贪吃蛇\\初步核心代码1.2\\整合代码\\snakegame\\素材\\music\\吃食物.mp3", NULL, 0, NULL);
	}
	GameOverView();
}

void ShowSetUp() {
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234)); // 背景色与菜单一致
	setfillcolor(RGB(255, 253, 180)); // 按钮填充色
	settextcolor(RGB(127, 212, 130)); // 文字颜色

	// 绘制功能按钮（纵向均匀分布）
	const int btnWidth = 300;
	const int btnHeight = 100;
	const int startY = 200;
	const int spacing = 130;

	// 交互处理
	ExMessage msg;
	while (true) {
		BeginBatchDraw();
		cleardevice();
		centertext(0, 50, 800, 100, 60, "游戏设置");
		// 排行榜按钮
		fillroundrect(250, startY, 250 + btnWidth, startY + btnHeight, 50, 50);
		centertext(250, startY, btnWidth, btnHeight, 50, "排行榜");

		// 游戏团队按钮
		fillroundrect(250, startY + spacing, 250 + btnWidth, startY + spacing + btnHeight, 50, 50);
		centertext(250, startY + spacing, btnWidth, btnHeight, 50, "游戏团队");

		// 游戏说明按钮
		fillroundrect(250, startY + 2 * spacing, 250 + btnWidth, startY + 2 * spacing + btnHeight, 50, 50);
		centertext(250, startY + 2 * spacing, btnWidth, btnHeight, 50, "游戏说明");

		// 返回菜单按钮
		fillroundrect(250, startY + 3 * spacing, 250 + btnWidth, startY + 3 * spacing + btnHeight, 50, 50);
		centertext(250, startY + 3 * spacing, btnWidth, btnHeight, 50, "返回菜单");
		EndBatchDraw();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				// 检测按钮点击区域
				const int xMin = 250;
				const int xMax = 250 + btnWidth;

				// 排行榜点击 (Y范围: 200-300)
				if (msg.x >= xMin && msg.x <= xMax && msg.y >= startY && msg.y <= startY + btnHeight) {
					SeeRank();
				}
				// 游戏团队点击 (Y范围: 330-430)
				else if (msg.x >= xMin && msg.x <= xMax && msg.y >= startY + spacing && msg.y <= startY + spacing + btnHeight) {
					teamView();
				}
				// 游戏说明点击 (Y范围: 460-560)
				else if (msg.x >= xMin && msg.x <= xMax && msg.y >= startY + 2 * spacing && msg.y <= startY + 2 * spacing + btnHeight) {
					howToPlayView();
				}
				// 返回菜单点击 (Y范围: 590-690)
				else if (msg.x >= xMin && msg.x <= xMax && msg.y >= startY + 3 * spacing && msg.y <= startY + 3 * spacing + btnHeight) {
					break;
				}
			}
		}
		Sleep(10);
	}
}

void teamView() {
	// 清屏并设置背景
	setbkcolor(WHITE);
	cleardevice();
	settextcolor(BLACK);

	// 标题文字（黑体，24号）
	settextstyle(24, 0, "黑体");
	outtextxy((WIDTH - 240) / 2, 100, "团队成员介绍");

	// 成员列表文字（宋体，18号）
	settextstyle(18, 0, "宋体");
	outtextxy(100, 150, "负责人：diup       功能：初始化游戏、地图绘制、菜单逻辑");
	outtextxy(100, 180, "负责人：田文星     功能：蛇移动逻辑、游戏结束处理");
	outtextxy(100, 210, "负责人：哈吉米     功能：食物生成、暂停系统、玩法说明");
	outtextxy(100, 240, "负责人：绚烂       功能：方向校验与转向逻辑");
	outtextxy(100, 270, "负责人：月月鸟     功能：蛇身延长、暂停界面交互");
	outtextxy(100, 300, "负责人：Re0        功能：菜单界面、设置界面、结算界面");
	outtextxy(100, 330, "负责人：yuan       功能：游戏主循环整合");

	// 绘制返回按钮（圆角矩形，带阴影效果）
	const int btnX = 300, btnY = 500;
	const int btnWidth = 200, btnHeight = 50;
	setfillcolor(RGB(255, 253, 180)); // 按钮主色（与菜单按钮一致）
	fillroundrect(btnX, btnY, btnX + btnWidth, btnY + btnHeight, 10, 10); // 主按钮
	setfillcolor(RGB(255, 253, 180)); // 内阴影色（浅灰色）
	fillroundrect(btnX + 2, btnY + 2, btnX + btnWidth - 2, btnY + btnHeight - 2, 8, 8); // 内阴影

	// 计算文字居中坐标
	settextstyle(24, 0, "微软雅黑"); // 设置字体大小和样式
	int textWidth = textwidth("返回");   // 获取文字宽度
	int textHeight = textheight("返回"); // 获取文字高度
	int x = btnX + (btnWidth - textWidth) / 2; // 水平居中
	int y = btnY + (btnHeight - textHeight) / 2; // 垂直居中
	settextcolor(RGB(127, 212, 130));
	outtextxy(x, y, "返回"); // 绘制居中文字

	// 鼠标事件处理循环
	ExMessage msg;
	while (true) {
		if (peekmessage(&msg, EX_MOUSE)) { // 非阻塞式获取鼠标消息
			if (msg.message == WM_LBUTTONDOWN) { // 左键点击事件
				// 检测点击位置是否在按钮范围内
				if (msg.x >= btnX && msg.x <= btnX + btnWidth &&
					msg.y >= btnY && msg.y <= btnY + btnHeight) {
					return; // 点击按钮后返回设置界面
				}
			}
		}
		Sleep(20); // 降低CPU占用
	}
}

void howToPlayView() {
	cleardevice();
	settextstyle(24, 0, "微软雅黑");
	settextcolor(BROWN);

	outtextxy(10, 40, "操作说明:");
	outtextxy(10, 80, "使用‘w’‘a’‘s’‘d’或者‘↑’‘↓’‘←’‘→’来操控蛇");
	outtextxy(10, 120, "游戏规则:");
	outtextxy(10, 160, "撞墙或撞到自身则游戏失败，吃到食物会使蛇的长度增加，按空格可停止游戏");

	// 绘制返回提示（下方小字体）
	centertext(0, 600, 800, 50, 30, "按任意键返回主菜单");
	ExMessage msg;
	bool exitFlag = false;

	while (!exitFlag) {
		if (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
			switch (msg.message) {
			case WM_KEYDOWN:    // 任意键盘按键
			case WM_LBUTTONDOWN:// 鼠标左键点击
				cleardevice();
				ShowSetUp();     // 返回菜单
				exitFlag = true;
				break;
			}
		}
		Sleep(10); // 降低CPU占用
	}
};

void SeeRank() {
	// 检查文件是否存在
	if (!std::ifstream("player_rank.dat")) {
		cleardevice();
		centertext(100, 350, 600, 100, 24, "排行榜数据文件不存在");
		Sleep(2000);
		return;
	}

	// 清空历史事件
	flushmessage();

	// 读取数据
	std::vector<player_rank> rankings;
	std::ifstream fin("player_rank.dat", std::ios::binary);
	if (fin) {
		player_rank temp;
		while (fin.read(reinterpret_cast<char*>(&temp), sizeof(player_rank))) {
			rankings.push_back(temp);
			if (rankings.size() >= 10) break;
		}
		fin.close();
	}

	// 绘制界面
	setbkcolor(RGB(245, 245, 245));
	cleardevice();
	BeginBatchDraw();

	// 标题
	setfillcolor(RGB(180, 210, 225));
	solidroundrect(100, 60, 700, 160, 20, 20);
	centertext(100, 60, 600, 100, 48, "游 戏 排 行 榜");

	// 条目
	const int ITEM_HEIGHT = 60, START_Y = 200;
	for (size_t i = 0; i < rankings.size(); ++i) {
		setfillcolor((i % 2) ? RGB(255, 255, 255) : RGB(230, 240, 245));
		solidroundrect(100, START_Y + i * ITEM_HEIGHT, 700, START_Y + (i + 1) * ITEM_HEIGHT, 15, 15);

		// 排名
		char rank[4];
		sprintf(rank, "%02d", i + 1);
		centertext(120, START_Y + i * ITEM_HEIGHT, 80, ITEM_HEIGHT, 32, rank);

		// 名称（截断防溢出）
		char name[21];
		strncpy(name, rankings[i].name, 19);
		name[19] = '\0';
		centertext(220, START_Y + i * ITEM_HEIGHT, 280, ITEM_HEIGHT, 32, name);

		// 分数
		char score[16];
		sprintf(score, "%d 分", rankings[i].score);
		centertext(520, START_Y + i * ITEM_HEIGHT, 160, ITEM_HEIGHT, 32, score);
	}

	// 返回按钮
	const int BTN_X = 300, BTN_Y = 700, BTN_W = 200, BTN_H = 50;
	setfillcolor(RGB(255, 253, 180));
	fillroundrect(BTN_X, BTN_Y, BTN_X + BTN_W, BTN_Y + BTN_H, 10, 10);
	centertext(BTN_X, BTN_Y, BTN_W, BTN_H, 24, "返 回");

	EndBatchDraw();

	// 事件检测
	ExMessage msg;
	bool exitFlag = false;
	while (!exitFlag) {
		if (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (msg.x >= BTN_X && msg.x <= BTN_X + BTN_W &&
					msg.y >= BTN_Y && msg.y <= BTN_Y + BTN_H) {
					cleardevice();
					ShowSetUp();
					exitFlag = true;
				}
			}
			else if (msg.message == WM_KEYDOWN) {
				cleardevice();
				ShowSetUp();
				exitFlag = true;
			}
		}
		Sleep(10);
	}
}

void GameOverView() {
	CloseMusic();
	// 初始化图形环境
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234));
	cleardevice();
	settextcolor(RGB(127, 212, 130)); // 保持菜单配色
	//mciSendString("play./素材/music/贪吃蛇败北之音 (online-audio-converter.com).mp3", 0, NULL, NULL);
	mciSendString("play ./素材/music/GameOver.mp3", 0, NULL, NULL);
	// 使用sstream转换分数
	stringstream ss;
	ss << "score：" << score;
	string s = ss.str();
	const char* cstr = s.c_str();

	// 绘制分数（上方大字体）
	centertext(0, 200, 800, 100, 80, cstr);

	// 绘制GAME OVER（中心超大字体）
	centertext(0, 350, 800, 150, 120, "GAME OVER");

	// 绘制返回提示（下方小字体）
	centertext(0, 600, 800, 50, 30, "按任意键返回主菜单");
	//清空消息队列
	flushmessage();
	ExMessage msg;
	while (true) {
		if (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
			if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN) {
				menuView();
				break;
			}
		}
		Sleep(15);
	}
}
void TwoplayerGameOverView() {
	CloseMusic();
	// 初始化图形环境
	mciSendString("play ./素材/music/GameOver.mp3", 0, NULL, NULL);
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234));
	cleardevice();
	settextcolor(RGB(127, 212, 130));

	// 生成独立的分数字符串
	stringstream ss1, ss2;
	ss1 << "P1 score:" << snake1.score;  // 玩家1分数
	ss2 << "P2 score:" << snake2.score;  // 玩家2分数

	// 转换为C字符串
	string str1 = ss1.str();
	string str2 = ss2.str();
	const char* p1score = str1.c_str();
	const char* p2score = str2.c_str();

	// 绘制分数（左右分列）
	centertext(50, 200, 300, 100, 40, p1score);  // 左半屏
	centertext(450, 200, 300, 100, 40, p2score);  // 右半屏

	// 绘制胜利信息
	const char* resultText = nullptr;
	if (snake1.score > snake2.score) {
		resultText = "玩家1胜利";
	}
	else if (snake1.score < snake2.score) {
		resultText = "玩家2胜利";
	}
	else {
		resultText = "平   局";
	}
	centertext(0, 350, 800, 150, 120, resultText);

	// 绘制操作提示
	settextcolor(LIGHTGRAY);
	centertext(0, 600, 800, 50, 20, "按任意键或点击鼠标返回主菜单");
	//清空消息队列
	flushmessage();
	// 输入检测循环
	ExMessage msg;
	while (true) {
		if (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
			if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN) {
				menuView();
				break;
			}
		}
		Sleep(15);
	}
}

#ifdef xuanlan
//关卡选择界面
void levelChoiceView() {
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234)); // 背景色与菜单一致
	setfillcolor(RGB(255, 253, 180)); // 按钮填充色
	settextcolor(RGB(127, 212, 130)); // 文字颜色

	// 绘制功能按钮（纵向均匀分布）
	const int btnWidth = 50;
	const int btnHeight = 50;
	const int startY = 200;
	const int spacing = 200;

	// 交互处理
	ExMessage msg;
	while (true) {
		BeginBatchDraw();
		cleardevice();
		centertext(0, 50, 800, 100, 60, "关卡选择");
		// 关卡1按钮
		fillroundrect(200, startY, 200 + btnWidth, startY + btnHeight, 50, 50);
		centertext(200, startY, btnWidth, btnHeight, 10, "关卡1");
		Level[1] = 1;
		addLock(Level[1], 220-5, 220+10);
		//关卡2按钮
		fillroundrect(300, startY + 100, 300 + btnWidth, startY + 100 + btnHeight, 50, 50);
		centertext(300, startY + 100, btnWidth, btnHeight, 10, "关卡2");
		addLock(Level[2], 320-5, 320+10);
		//关卡3按钮
		fillroundrect(400, startY, 400 + btnWidth, startY + btnHeight, 50, 50);
		centertext(400, startY, btnWidth, btnHeight, 10, "关卡3");
		addLock(Level[3], 420-5, 220+10);
		//关卡4按钮
		fillroundrect(400, startY + 200, 400 + btnWidth, startY + 200 + btnHeight, 50, 50);
		centertext(400, startY + 200, btnWidth, btnHeight, 10, "关卡4");
		addLock(Level[4], 420-5, 420+10);
		//关卡5按钮
		fillroundrect(300, startY + 300, 300 + btnWidth, startY + 300 + btnHeight, 50, 50);
		centertext(300, startY + 300, btnWidth, btnHeight, 10, "关卡5");
		addLock(Level[5], 320-5, 520+10);
		// 返回菜单按钮
		fillroundrect(200, 600, 200 + 400, 600 + 100, 50, 50);
		centertext(200, 600, 400, 100, 50, "返回游戏选择");

		EndBatchDraw();

		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				// 检测按钮点击区域
				const int xMin = 200;
				const int xMax = 200 + btnWidth;
				//关卡1点击
				if (msg.x >= 200 && msg.x <= xMax && msg.y >= startY && msg.y <= startY + btnHeight) {
					Level[1] = 1;
					if (Level[1]) {
						printf("进入关卡1\n");
						levelModuleGameView(1);
					}
				}
				//关卡2点击
				else if (msg.x >= xMin + 100 && msg.x <= xMax + 100 && msg.y >= startY + 100 && msg.y <= startY + 100 + btnHeight) {
					if (Level[2]) {
						printf("进入关卡2\n");
						levelModuleGameView(2);
					}
					else {
						printf("抱歉，关卡未解锁!\n");
					}
				}
				//关卡3点击
				else if (msg.x >= xMin + 200 && msg.x <= xMax + 200 && msg.y >= startY && msg.y <= startY + btnHeight) {
					if (Level[3]) {
						printf("进入关卡3\n");
						levelModuleGameView(3);
					}
					else {
						printf("抱歉，关卡未解锁!\n");
					}
				}
				//关卡4点击
				else if (msg.x >= xMin + 200 && msg.x <= xMax + 200 && msg.y >= startY + 200 && msg.y <= startY + 200 + btnHeight) {
					if (Level[4]) {
						printf("进入关卡4\n");
						levelModuleGameView(4);
					}
					else {
						printf("抱歉，关卡未解锁!\n");
					}
				}
				//关卡5点击
				else if (msg.x >= xMin + 100 && msg.x <= xMax + 100 && msg.y >= startY + 300 && msg.y <= startY + 300 + btnHeight) {
					if (Level[5]) {
						printf("进入关卡5\n");
						levelModuleGameView(5);
					}
					else {
						printf("抱歉，关卡未解锁!\n");
					}
				}
				//返回游戏选择界面
				else if (msg.x >= 200 && msg.x <= 200 + 400 && msg.y >= 600 && msg.y <= 600 + 100) {
					printf("进入游戏选择界面\n");
					gameModuleChoiceView();
				}
			}
		}
		Sleep(10);
	}
}
#endif

void designOneMaze() {}

void levelModuleGameView(int level) {
	// 初始化蛇的位置和方向
	CloseMusic();
	isLevel = true;
	mciSendString("play ./素材/music/关卡模式.mp3 repeat", 0, NULL, NULL);
	snake.dir = 'D';
	snake.len = 3;
	snake.x[0] = beginX[level]; snake.y[0] = beginY[level];   // 头部居中
	snake.x[1] = beginX[level] - 1; snake.y[1] = beginY[level];  // 身体1在左
	snake.x[2] = beginX[level] - 2; snake.y[2] = beginY[level];  // 身体2在左

	// 加载图像资源
	IMAGE img_space, img_wall, img_end;
	loadimage(&img_space, "./素材/image/地图.jpg", 50, 50);
	loadimage(&img_wall, "./素材/image/地图背景.jpg", 50, 50);
	loadimage(&img_end, "./素材/image/终点.png", 50, 50);

	ExMessage msg;
	BeginBatchDraw();  // 开始批量绘制
	// 帧率控制变量
	clock_t lastTime = clock();
	const int FPS = 24;  // 控制游戏帧率为24帧/秒

	while (snake.x[0] != endX[level] || snake.y[0] != endY[level]) {
		// 帧率控制
		clock_t currentTime = clock();
		if (currentTime - lastTime < 1000 / FPS) {
			continue;  // 跳过本次循环，控制帧率
		}
		lastTime = currentTime;

		cleardevice();
		setbkcolor(WHITE);

		// 绘制地图
		for (int row = 0; row < 16; row++) {
			for (int col = 0; col < 16; col++) {
				int screenX = col * 10 * 5;
				int screenY = row * 10 * 5;

				// 根据地图元素绘制
				switch (maze[level - 1][row][col]) {
				case 1: // 墙
					putimage(screenX, screenY, &img_wall);
					break;
				case 2: // 墙
					putimage(screenX, screenY, &img_end);
					break;
				default: // 空地和其他情况
					putimage(screenX, screenY, &img_space);
					break;
				}
			}
		}

		// 绘制蛇头
		setfillcolor(BLUE);
		setlinecolor(BLACK);
		fillroundrect(snake.x[0] * 50 + 5, snake.y[0] * 50 + 5,
			snake.x[0] * 50 + 45, snake.y[0] * 50 + 45,
			3 * 5, 3 * 5);

		// 绘制蛇身
		for (int i = 1; i < snake.len; i++) {
			int screenX = snake.x[i] * 50;
			int screenY = snake.y[i] * 50;

			setfillcolor(RED);
			setlinecolor(BLACK);
			fillroundrect(screenX + 5, screenY + 5,
				screenX + 9 * 5, screenY + 9 * 5,
				3 * 5, 3 * 5);
		}

		FlushBatchDraw();  // 刷新批量绘制缓冲区

		char dir;
		while (peekmessage(&msg, EX_KEY))
		{
			switch (msg.vkcode)
			{
			case 'W':
			case VK_UP:
				dir = 'W';
				if (!isWall(dir, level)) { ChangeDirection(dir);  MoveSnake(); }
				break;
			case 'S':
			case VK_DOWN:
				dir = 'S';
				if (!isWall(dir, level)) { ChangeDirection(dir);  MoveSnake(); }
				break;
			case 'A':
			case VK_LEFT:
				dir = 'A';
				if (!isWall(dir, level)) { ChangeDirection(dir);  MoveSnake(); }
				break;
			case 'D':
			case VK_RIGHT:
				dir = 'D';
				if (!isWall(dir, level)) { ChangeDirection(dir);  MoveSnake(); }
				break;
			case VK_SPACE:
				LevelStop();
				break;
			}
		}

		if (snake.x[0] == endX[level] && snake.y[0] == endY[level]) {
			if (unlock <= 4) {
				Level[++unlock] = 1;
			}
			//GameOverView();
			levelWinView(level);
		}
		EndBatchDraw();
	}
}

void FreeModuleMoveSnake() {
	isMove = true;
	// 保存蛇头位置用于碰撞检测
	int snakeX = snake.x[0];
	int snakeY = snake.y[0];
	// 更新蛇身体各部分的位置
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	// 根据当前方向移动蛇头
	if (snake.dir == 'W') {      // 上
		snake.y[0]--;
		if (snake.y[0] < 0) snake.y[0] = 79;
	}
	else if (snake.dir == 'S') { // 下
		snake.y[0]++;
		if (snake.y[0] == 80) snake.y[0] = 0;
	}
	else if (snake.dir == 'A') { // 左
		snake.x[0]--;
		if (snake.x[0] < 0) snake.x[0] = 79;
	}
	else if (snake.dir == 'D') { // 右
		snake.x[0]++;
		if (snake.x[0] == 80) snake.x[0] = 0;
	}
}

void freeModuleGameView() {
	initFreeGame();
	//停止背景音乐,从menu调用
	CloseMusic();
	isFree = true;
	mciSendString("play ./素材/music/自由模式.mp3 repeat", 0, NULL, NULL);
	DWORD lastUpdateTime = GetTickCount(); // 逻辑更新计时
	DWORD lastFrameTime = GetTickCount();  // 帧渲染计时
	float moveInterval = 100;        // 蛇移动间隔200ms
	const float frameInterval = 1000 / 60.0;  // 60帧/秒

	while (!isGameOver) {
		DWORD currentTime = GetTickCount();

		// 处理输入
		handleInput();

		// 独立逻辑更新：固定时间间隔移动
		if (currentTime - lastUpdateTime >= moveInterval) {
			FreeModuleMoveSnake();
			check(moveInterval);
			lastUpdateTime = currentTime;
		}

		// 独立渲染更新：控制最大帧率
		if (currentTime - lastFrameTime >= frameInterval) {
			BeginBatchDraw();
			gameView_ShowMap();
			EndBatchDraw();
			lastFrameTime = currentTime;
		}

		Sleep(1); // 避免CPU空转
	}
	updatarank();
	GameOverView();
}

void initFreeGame() { 
	snake.dir = 'D';
	snake.len = 6;
	snake.x[0] = 40; snake.y[0] = 40; //  头部居中
	snake.x[1] = 39; snake.y[1] = 40;  // 身体1在左
	snake.x[2] = 38; snake.y[2] = 40;  // 身体2在左
	snake.x[3] = 37; snake.y[3] = 40;  // 身体3在左
	snake.x[4] = 36; snake.y[4] = 40;  // 身体4在左
	snake.x[5] = 35; snake.y[5] = 40;  // 身体5在左

	// 初始化地图边界
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			map[i][j] = 0;
		}
	}
	setbkcolor(WHITE);
	cleardevice();
	score = 0;//初始化分数
	fillrectangle(20, 20, 80, 80);
	settextcolor(RED);
	settextstyle(30, 0, "微软雅黑");
	char str1[50] = "";
	sprintf_s(str1, "Score:%d", score);
	outtextxy(20, 20, str1);
	isGameOver = false;
	//创建果实
	for (int i = 0; i < 5; i++) generateFood(i);
	for (int i = 0; i < 3; i++) generateBuffFood(i);
	for (int i = 0; i < 3; i++) generateDebuffFood(i);
	return; 
}

void gameModuleChoiceView()
{
	cleardevice();

	ExMessage msg = { 0 };
	setfillcolor(RGB(255, 253, 180));
	settextstyle(100, 0, ("微软雅黑"));
	setbkmode(TRANSPARENT);
	settextcolor(RGB(127, 212, 130));
	outtextxy(180, 30, "游戏模式选择");
	settextstyle(70, 0, ("微软雅黑"));
	fillrectangle(230, 280, 570, 370);
	outtextxy(290, 290, "自由模式");
	fillrectangle(230, 430, 570, 520);
	outtextxy(290, 440, "关卡模式");
	fillrectangle(230, 580, 570, 670);
	outtextxy(290, 590, "竞技模式");
	centertext(0, 700, 800, 50, 30, "按任意键返回主菜单");

	while (true)
	{
		peekmessage(&msg, EX_MOUSE | EX_KEY); //用来获取鼠标消息
		if (msg.message == WM_LBUTTONDOWN) //按下鼠标左键
		{
			//按自由模式
			if (msg.x >= 230 && msg.x <= 570 && msg.y >= 280 && msg.y <= 370)
			{
				freeModuleGameView();
				break;
			}
			//按关卡模式
			else if (msg.x >= 230 && msg.x <= 570 && msg.y >= 430 && msg.y <= 520)
			{
				levelChoiceView();
				break;
			}
			//按竞技模式
			else if (msg.x >= 230 && msg.x <= 570 && msg.y >= 580 && msg.y <= 670)
			{
				istwoplayer = true;
				competitiveModuleGame();
				istwoplayer = false;
				break;
			}

			else {
				menuView();
				break;
			}
		}
		else if (msg.message == WM_KEYDOWN) {
			menuView();
			break;
		}
	}
}

void logon() {
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234));
	setfillcolor(RGB(255, 253, 180));
	settextcolor(RGB(127, 212, 130));
	ExMessage msg;
	
	while (true) {
		//设计注册界面
		while (strlen(current_user.username) == 0) {
			InputBox(current_user.username, 20, "输入账户", "注册", NULL, 0, 0, true);
		}
		while (strlen(current_user.password) == 0) {
			InputBox(current_user.password, 20, "输入密码", "注册", NULL, 0, 0, true);
		}

		//如果不存在文件，需要先创建user.dat
		ofstream fout("user.dat", ios::app);
		if (!fout.is_open()) {
			MessageBox(GetHWnd(), "创建失败", "错误", MB_OK);
			exit(EXIT_FAILURE);
		}
		fout.close();
		//检查是否重复
		bool isRepeat = false;	
		ifstream fin("user.dat", ios::binary);
		if (!fin.is_open()) {
			cout << "打开文件失败！" << endl;
			exit(EXIT_FAILURE);
		}
		else {
			while (fin.read((char*)&read_user, sizeof(UserData))) {
				if (strcmp(read_user.username, current_user.username) == 0) {
					isRepeat = true;
					break;
				}
			}
			fin.close();
		}
		
		//如果不重复，将文件写入user.dat，并将current_user置空
		if (!isRepeat) {
			ofstream fout("user.dat", ios::binary | ios::app);
			if (!fout.is_open()) {
				MessageBox(GetHWnd(), "无法写入用户数据！", "错误", MB_OK);
				exit(EXIT_FAILURE);
			}
			fout.write((const char*)&current_user, sizeof(UserData));  //写进read_user
			strncpy(current_user.username, "",20);
			strncpy(current_user.password, "",20);
			MessageBox(GetHWnd(), "注册成功！", "注册", MB_OK);
			fout.close();
			break;
		}
		//如果重复了，需要重新注册
		else {
			MessageBox(GetHWnd(), "账号重复！", "注册", MB_OK);
			strncpy(current_user.username, "", 20);
			strncpy(current_user.password, "", 20);
			continue;
		}
	}

}

void login() {
	IMAGE img;
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(234, 234, 234));
	setfillcolor(RGB(255, 253, 180));
	settextcolor(RGB(127, 212, 130));
	loadimage(&img, "./素材/image/title.png");
	ExMessage msg;
	cleardevice();

	while (true) {
		BeginBatchDraw();
		cleardevice();
		putimage(35, -1, &img);

		// 绘制界面元素
		fillroundrect(250, 370, 550, 470, 50, 50);
		centertext(250, 370, 300, 100, 50, "登录");

		fillroundrect(250, 500, 550, 600, 50, 50);
		centertext(250, 500, 300, 100, 50, "新用户注册");

		fillroundrect(250, 630, 550, 730, 50, 50);
		centertext(250, 630, 300, 100, 50, "退出游戏");
		EndBatchDraw();

		if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= 250 && msg.x <= 550) {
				// 登录按钮
				if (msg.y >= 370 && msg.y <= 470) {
					bool cancel = false;
					bool inputCanceled = false;
					inputCanceled = inputCanceled || !InputBox(current_user.username, 20, "请输入用户名:","登录", NULL,0, 0, false);
					inputCanceled = inputCanceled || !InputBox(current_user.password, 20, "请输入密码:","登录", NULL,0, 0, true);

					if (!cancel) {
						if (strlen(current_user.username) == 0 || strlen(current_user.password) == 0) {
							MessageBox(GetHWnd(), "输入不能为空", "错误", MB_OK);
							continue;
						}

						// C++ 文件流操作
						std::ifstream ifs("user.dat", std::ios::binary);
						bool found = false;
						bool isPassword = true;
						if (ifs) {
							UserData temp;
							while (ifs.read(reinterpret_cast<char*>(&temp), sizeof(UserData))) {
								if (strcmp(temp.username, current_user.username) == 0 &&
									strcmp(temp.password, current_user.password) != 0) {
									MessageBox(GetHWnd(), "密码输入错误！", "登录", MB_OK);
									isPassword = false;
									break;
								}
								if (strcmp(temp.username, current_user.username) == 0 &&
									strcmp(temp.password, current_user.password) == 0) {
									current_user.unlockedLevel = temp.unlockedLevel;
									found = true;
									break;
								}
							}
							ifs.close();
						}
						if (found)
						{
							MessageBox(GetHWnd(), "登录成功！", "登录", MB_OK); return; // 登录成功
						}
						if(isPassword) MessageBox(GetHWnd(), "无该用户！", "登录", MB_OK);
						memset(&current_user, 0, sizeof(UserData));
					}
				}
				// 注册按钮
				else if (msg.y >= 500 && msg.y <= 600) {
					logon();
				}
				// 退出按钮
				else if (msg.y >= 630 && msg.y <= 730) {
					exit(0);
				}
			}
		}
		Sleep(10);
	}
}


//play
void playBackgroundMusic(const char* music) {
	//static int i = 0;
	char cmd[150] = { 0 };
	sprintf(cmd, "open %s alias bgm", music);
	MCIERROR ret = mciSendString(cmd, NULL, 0, NULL);
	if (ret != 0) {
		char err[100] = { 0 };
		mciGetErrorString(ret, err, sizeof(err));
		printf("[open] : %s", err);
	}
	//sprintf(cmd, "play bgm %s");
	mciSendString("play bgm", NULL, 0, NULL);
	mciSendString("close bgm", NULL, 0, NULL);
	
	//i++;
}

bool isWall(char dir, int level) {
	//int _y = snake.y[0];
	if (dir == 'W' || dir == 'w') {
		return (maze[level - 1][snake.y[0] - 1][snake.x[0]] == 1);
	}
	if (dir == 'S' || dir == 's') {
		return (maze[level - 1][snake.y[0] + 1][snake.x[0]] == 1);
	}
	if (dir == 'A' || dir == 'a') {
		return (maze[level - 1][snake.y[0]][snake.x[0] - 1] == 1);
	}
	if (dir == 'D' || dir == 'd') {
		return (maze[level - 1][snake.y[0]][snake.x[0] + 1] == 1);
	}
	return false;
}

void competitiveModuleGame() {
	CloseMusic();
	isTwo = true;
	mciSendString("play ./素材/music/双人模式.mp3 repeat", 0, NULL, NULL);
	InittwoplayerGame(snake1, snake2);
	DWORD lastUpdateTime1 = GetTickCount();// 逻辑更新计时
	DWORD lastUpdateTime2 = GetTickCount();// 逻辑更新计时
	DWORD lastFrameTime = GetTickCount();  // 帧渲染计时
	const float frameInterval = 1000 / 60.0;// 60帧/秒

	while (snake1.alive || snake2.alive) {
		float moveInterval1 = snake1.speed;    // 蛇1
		float moveInterval2 = snake2.speed;    // 蛇2 
		DWORD currentTime = GetTickCount();
		// 处理输入
		handleInput(snake1, snake2);
		// 独立逻辑更新：固定时间间隔移动
		if (currentTime - lastUpdateTime1 >= moveInterval1 && snake1.alive) {
			MoveTwoSnake(snake1);
			checkTwoSnake(snake1, snake2);
			lastUpdateTime1 = currentTime;
		}
		if (currentTime - lastUpdateTime2 >= moveInterval2 && snake2.alive) {
			MoveTwoSnake(snake2);
			checkTwoSnake(snake2, snake1);
			lastUpdateTime2 = currentTime;
		}
		// 独立渲染更新：控制最大帧率
		if (currentTime - lastFrameTime >= frameInterval) {
			BeginBatchDraw();
			gameView_Twoplayer_ShowMap();
			EndBatchDraw();
			lastFrameTime = currentTime;
		}
		Sleep(1);
	}
	TwoplayerGameOverView();
}

void InittwoplayerGame(Snake& snake1, Snake& snake2)
{
	snake1.dir = 'D';
	snake1.len = 6;
	snake1.x[0] = 40; snake1.y[0] = 30;  //  头部居中
	snake1.x[1] = 39; snake1.y[1] = 30;  // 身体1在左
	snake1.x[2] = 38; snake1.y[2] = 30;  // 身体2在左
	snake1.x[3] = 37; snake1.y[3] = 30;  // 身体3在左
	snake1.x[4] = 36; snake1.y[4] = 30;  // 身体4在左
	snake1.x[5] = 35; snake1.y[5] = 30;  // 身体5在左
	snake1.alive = true;
	snake1.speed = 200;
	snake2.dir = 'A';
	snake2.len = 6;
	snake2.x[0] = 40; snake2.y[0] = 50;  //  头部居中
	snake2.x[1] = 41; snake2.y[1] = 50;  // 身体1在左
	snake2.x[2] = 42; snake2.y[2] = 50;  // 身体2在左
	snake2.x[3] = 43; snake2.y[3] = 50;  // 身体3在左
	snake2.x[4] = 44; snake2.y[4] = 50;  // 身体4在左
	snake2.x[5] = 45; snake2.y[5] = 50;  // 身体5在左
	snake2.alive = true;
	snake2.speed = 200;
	// 初始化地图边界
	for (int i = 0; i < 80; i++) {
		for (int j = 0; j < 80; j++) {
			map[i][j] = (i == 0 || i == 79 || j == 0 || j == 79) ? 1 : 0;
		}
	}
	setbkcolor(WHITE);
	cleardevice();
	snake1.score = 0;//初始化分数
	snake2.score = 0;
	/*fillrectangle(20, 20, 80, 80);
	fillrectangle(100, 20, 160, 80);
	settextcolor(RED);
	settextstyle(30, 0, "微软雅黑");
	char str1[50] = "";
	sprintf_s(str1, "P1:Score:%d", score);
	char str2[50] = "";
	sprintf_s(str2, "P2:Score:%d", score);
	outtextxy(20, 20, str1);
	outtextxy(100, 20, str2);*/
	//创建果实
	for (int i = 0; i < 5; i++) generateFood(i);
	for (int i = 0; i < 3; i++) generateBuffFood(i);
	for (int i = 0; i < 3; i++) generateDebuffFood(i);
}

void MoveTwoSnake(Snake& snake) {
	snake.ismove = true;
	// 保存蛇头位置用于碰撞检测
	int snakeX = snake.x[0];
	int snakeY = snake.y[0];
	// 更新蛇身体各部分的位置
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	// 根据当前方向移动蛇头
	if (snake.dir == 'W') {      // 上
		snake.y[0]--;
	}
	else if (snake.dir == 'S') { // 下
		snake.y[0]++;
	}
	else if (snake.dir == 'A') { // 左
		snake.x[0]--;
	}
	else if (snake.dir == 'D') { // 右
		snake.x[0]++;
	}
}

void checkTwoSnake(Snake& snake, Snake& snake2) {
	// 检测食物：蛇头坐标与食物坐标重合
	for (int i = 0; i < 5; i++) {
		if (snake.x[0] == foodx[i] && snake.y[0] == foody[i]) {
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			map[foody[i]][foodx[i]] = 0;
			snake.speed = 200;
			ExtendTail(snake);
			generateFood(i);
			snake.score++;
			break;
		}
	}

	// 检测buff：蛇头坐标与食物坐标重合
	for (int i = 0; i < 3; i++) {
		if (snake.x[0] == buffx[i] && snake.y[0] == buffy[i]) {
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			map[buffy[i]][buffx[i]] = 0;
			ExtendTail(snake);
			generateBuffFood(i);
			snake.score += 10;
			break;
		}
	}

	// 检测debuff：蛇头坐标与食物坐标重合
	for (int i = 0; i < 3; i++) {
		if (snake.x[0] == debuffx[i] && snake.y[0] == debuffy[i]) {
			mciSendString("play ./素材/music/吃食物.mp3", 0, NULL, NULL);
			map[debuffy[i]][debuffx[i]] = 0;
			snake.speed = 50;
			generateDebuffFood(i);
			break;
		}
	}

	// 检测墙壁碰撞
	for (int i = 0; i < snake.len; i++) {
		// 检查是否超出地图边界
		if (snake.x[i] < 0 || snake.x[i] >= 80 ||
			snake.y[i] < 0 || snake.y[i] >= 80) {
			mciSendString("play ./素材/music/撞墙.mp3", 0, NULL, NULL);
			snake.alive = false;
			return;
		}

		// 检查是否碰到地图中的墙壁
		if (map[snake.x[i]][snake.y[i]] == 1) {
			mciSendString("play ./素材/music/撞墙.mp3", 0, NULL, NULL);
			snake.alive = false;
			return;
		}
	}

	// 检测自身碰撞 和 与第二条蛇碰撞
	for (int i = 1; i < snake.len; i++) {
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
			snake.alive = false;
			return;
		}
	}
	for (int i = 1; i < snake2.len; i++) {
		if (snake.x[0] == snake2.x[i] && snake.y[0] == snake2.y[i]) {
			snake.alive = false;
			return;
		}
	}
}

void gameView_Twoplayer_ShowMap()
{
	setbkcolor(WHITE);
	cleardevice();
	// 加载资源
	IMAGE img_space, img_wall, img_food, img_buff, img_debuff;
	loadimage(&img_space, "./素材/image/地图.jpg", 10, 10);
	loadimage(&img_wall, "./素材/image/墙.jpg", 10, 10);
	loadimage(&img_food, "./素材/image/普通果实.png", 10, 10);
	loadimage(&img_buff, "./素材/image/特殊果实.png", 10, 10);
	loadimage(&img_debuff, "./素材/image/debuff果实.png", 10, 10);
	//map[foody][foodx] = 2;
	// 绘制地图
	for (int row = 0; row < 80; row++) {
		for (int col = 0; col < 80; col++) {
			int screenX = col * 10;
			int screenY = row * 10;


			// 根据地图元素绘制
			switch (map[row][col]) {
			case 1: // 墙
				putimage(screenX, screenY, &img_wall);
				break;
			case 2: // 食物
				putimage(screenX, screenY, &img_food);
				break;
			case 4:
				putimage(screenX, screenY, &img_buff);
				break;
			case 5:
				putimage(screenX, screenY, &img_debuff);
				break;
			default: // 空地和其他情况
				putimage(screenX, screenY, &img_space);
				break;
			}
		}
	}
	if (snake1.alive) {
		//绘制蛇头
		setfillcolor(BLUE);
		setlinecolor(BLACK);
		fillroundrect(snake1.x[0] * 10 + 1, snake1.y[0] * 10 + 1,
			snake1.x[0] * 10 + 9, snake1.y[0] * 10 + 9,
			3, 3);
		//绘制蛇身
		for (int i = 1; i < snake1.len; i++) {
			int screenX = snake1.x[i] * 10;
			int screenY = snake1.y[i] * 10;

			setfillcolor(RED);
			setlinecolor(BLACK);
			fillroundrect(screenX + 1, screenY + 1,
				screenX + 9, screenY + 9,
				3, 3);
		}
	}
	if (snake2.alive) {
		setfillcolor(RED);
		fillroundrect(snake2.x[0] * 10 + 1, snake2.y[0] * 10 + 1,
			snake2.x[0] * 10 + 9, snake2.y[0] * 10 + 9,
			3, 3);
		for (int i = 1; i < snake2.len; i++) {
			int screenX = snake2.x[i] * 10;
			int screenY = snake2.y[i] * 10;

			setfillcolor(BLUE);
			fillroundrect(screenX + 1, screenY + 1,
				screenX + 9, screenY + 9,
				3, 3);
		}
	}
	// 实时更新分数显示
	settextcolor(BLACK);
	settextstyle(24, 0, "Consolas");
	char scoreText1[32];
	sprintf(scoreText1, "P1:SCORE: %d", snake1.score);
	outtextxy(20, 20, scoreText1);
	char scoreText2[32];
	sprintf(scoreText2, "P2:SCORE: %d", snake2.score);
	outtextxy(160, 20, scoreText2);
}

void levelWinView(int level) {
	ExMessage msg;
	while (true) {
		BeginBatchDraw();
		setbkmode(TRANSPARENT);
		setbkcolor(RGB(234, 234, 234));
		setfillcolor(RGB(255, 253, 180));
		settextcolor(RGB(127, 212, 130));
		cleardevice();
		solidroundrect(150, 170, 150 + 500, 170 + 200, 50, 50);
		centertext(150, 170, 500, 200, 50, "闯关成功！");

		if (level < 4) {
			fillroundrect(250, 500, 250 + 300, 500 + 100, 50, 50);
			centertext(250, 500, 300, 100, 50, "进入下一关");
		}
		else {
			fillroundrect(250, 500, 250 + 300, 500 + 100, 50, 50);
			centertext(250, 500, 300, 100, 35, "没有下一关了哦！");
		}

		fillroundrect(250, 630, 250 + 300, 630 + 100, 50, 50);
		centertext(250, 630, 300, 100, 50, "返回");
		EndBatchDraw();
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (msg.x >= 250 && msg.x <= 250 + 300) {
					if (msg.y >= 500 && msg.y <= 500 + 100) {
						if (level < 4) {
							levelModuleGameView(level + 1);
						}
					}
					if (msg.y >= 630 && msg.y <= 630 + 100) {
						levelChoiceView();
					}
				}
			}
		}
		Sleep(10);
	}
}

void addLock(int flag, int x, int y) {
	IMAGE img_lock, img_unlock;
	loadimage(&img_lock, "./素材/image/锁.jpg", 30, 30);
	loadimage(&img_unlock, "./素材/image/解锁.jpg", 30, 30);
	switch (flag) {
	case 0:
		putimage(x, y, &img_lock);
		break;
	case 1:
		putimage(x, y, &img_unlock);
		break;
	}
}

void LevelStop() {
	cleardevice();

	ExMessage stop_msg = { 0 }; //定义消息结构体变量
	setfillcolor(RGB(230, 231, 232));
	fillrectangle(200, 100, 600, 250);
	settextstyle(100, 0, ("微软雅黑"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(250, 125, "继续游戏");
	fillrectangle(200, 550, 600, 700);
	outtextxy(250, 575, "结束游戏");

	while (true)
	{
		peekmessage(&stop_msg, EX_MOUSE); //用来获取继续游戏和结束游戏消息
		if (stop_msg.message == WM_LBUTTONDOWN) //按下鼠标左键
		{
			//按继续游戏
			if (stop_msg.x >= 200 && stop_msg.x <= 600 && stop_msg.y >= 100 && stop_msg.y <= 250)
			{
				break;
			}
			//按结束游戏
			else if (stop_msg.x >= 200 && stop_msg.x <= 600 && stop_msg.y >= 550 && stop_msg.y <= 700)
			{
				levelChoiceView();
				break;
			}
		}
		Sleep(10);
	}
}

int main() {
	initgraph(HEIGHT, WIDTH);
	login();
	menuView();
	return 0;
}