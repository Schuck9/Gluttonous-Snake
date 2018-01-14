#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define ground_SIZE_WIDTH 60
#define ground_SIZE_LONG 25

typedef struct {
	int x;//列坐标
	int y;//行坐标
}location;//蛇体所在坐标

int Snake_SIZE = 5;//蛇的初始长度
int MEAT_x, MEAT_y;//食物的坐标
double speed = 0.1;//蛇的速度
location *Snake_BODY = (location*)malloc(sizeof(location) * Snake_SIZE);//蛇身
location *Snake_NEW;//变大蛇身所需替身
location Snake_TAIL;//蛇尾
char choice = 0;//初始化switch分支条件
char ch = 'd';//用户输出操作
char Speed_choice = '0';//初始化难度选择分支条件
long int score = 0;
float Snake_LIFE = 100;

void move();
void delay(double delay_time);
void hide_CURSOR();
void init();
void Snake_inflate();
void genarate_MEAT();
void random();
void check();
void gotoxy(int x, int y);
void spy(char *object);
void gameover();
void Interface();
void PLAY();
void EXIT();
void Wall();
void Level_ajustment();
void Demonstrate(double Speed_level);
void process();
void Instructions();
int main()
{

	Interface();
	process();

}
void move()
{
	Snake_TAIL = Snake_BODY[0];//保存蛇尾 备后续清除
	score += 3;//随时间分数增长
	Snake_LIFE -= 0.4;//生命随时间下降
	for (int i = 1; i < Snake_SIZE; i++)
	{
		Snake_BODY[i - 1] = Snake_BODY[i];//蛇身跟踪蛇首
	}
	switch (ch)
	{
	case 'w':Snake_BODY[Snake_SIZE - 1].y--; break;//上
	case 's':Snake_BODY[Snake_SIZE - 1].y++; break;//下
	case 'd':Snake_BODY[Snake_SIZE - 1].x++; break;//右
	case 'a':Snake_BODY[Snake_SIZE - 1].x--; break;//左
	}//移动蛇
	for (int i = 0; i < Snake_SIZE; i++)
	{
		gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
		printf("*");
	}//打印蛇身

	gotoxy(68, 7);
	printf("分数：%ld", score);
	gotoxy(68, 8);
	if (Snake_LIFE < 0)
	{
		printf("生命值：0");
	}
	else
	{
		printf("生命值：%.2f", Snake_LIFE);
	}
	gotoxy(68, 10);
	printf("W:上 S:下 A:左 D:右");
	gotoxy(68, 12);
	printf("按下空格键结束游戏");
}
void delay(double delay_time)//延时函数
{
	clock_t delay = delay_time*CLOCKS_PER_SEC;
	clock_t t = clock();
	while (clock() - t < delay);
}
void init()
{
	gotoxy(66, 1);

	printf("Made by 莫廷钰");//水印
	Snake_SIZE = 5;//重新定义蛇长
				   //speed = 0.1;//重新定义速度
	srand((unsigned)time(NULL));//种时间种子
	for (int i = 0; i < Snake_SIZE; i++)
	{
		Snake_BODY[i].x = 10;
		Snake_BODY[i].y = i + 10;
	}//造蛇
	for (int i = 0; i < Snake_SIZE; i++)
	{
		gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
		printf("*");
	}//蛇身初始化
	Wall();//墙体初始化
	genarate_MEAT();//食物初始化
}
void Snake_inflate()
{
	Snake_SIZE++;//长大
	score += 20; //吃到食物分数加20

	if (Snake_LIFE > 87)
	{
		Snake_LIFE = 100;
	}
	else Snake_LIFE += 12;

	if (Snake_SIZE % 5 == 0)
	{
		speed /= 1.2;//改变速度
	}
	Snake_NEW = (location*)malloc(sizeof(location) * Snake_SIZE);//重新动态分配替身蛇
	for (int i = 0; i < Snake_SIZE - 1; i++)
	{
		Snake_NEW[i + 1] = Snake_BODY[i];//将原蛇的遗传物质复制到新蛇
	}
	Snake_NEW[0] = Snake_TAIL;//添加新的蛇尾
	location *p = Snake_BODY;//将原蛇内存释放
	Snake_BODY = Snake_NEW;//新蛇诞生
	free(p);//释放原蛇
}//蛇变长
void check()//检查函数
{
	if (Snake_BODY[Snake_SIZE - 1].x == MEAT_x && Snake_BODY[Snake_SIZE - 1].y == MEAT_y)//吃到食物
	{
		Snake_inflate();//蛇身长大
						//gotoxy(MEAT_x, MEAT_y);
						//printf(" ");
		genarate_MEAT();//重新产生食物
	}
	else
	{
		gotoxy(Snake_TAIL.x - 1, Snake_TAIL.y - 1);//移动光标到蛇尾处
		printf(" ");//清除蛇尾
	}//吞食
	for (int i = 0; i < Snake_SIZE - 1; i++)
	{
		if (Snake_BODY[Snake_SIZE - 1].x == Snake_BODY[i].x && Snake_BODY[Snake_SIZE - 1].y == Snake_BODY[i].y)//咬到自身
		{
			ch = 32; break;
		}
	}
	if (Snake_BODY[Snake_SIZE - 1].x == 2 || Snake_BODY[Snake_SIZE - 1].x == ground_SIZE_WIDTH + 1)//撞左右墙
	{
		ch = 32;
	}
	if (Snake_BODY[Snake_SIZE - 1].y == 1 || Snake_BODY[Snake_SIZE - 1].y == ground_SIZE_LONG + 1)//撞上下墙
	{
		ch = 32;
	}
	if (Snake_LIFE < 0)
	{
		ch = 32;
	}

}
void genarate_MEAT()//食物生产函数
{
	random();//随机产生食物坐标
	gotoxy(MEAT_x - 1, MEAT_y - 1);
	printf("*");//打印食物
}
void random()//随机数函数
{
	MEAT_x = rand() % ground_SIZE_WIDTH - 1;//随机食物横坐标
	MEAT_y = rand() % ground_SIZE_LONG - 1;//随机食物纵坐标

	if (MEAT_x < 3 || MEAT_y < 3)
	{
		random();
	}//重新定位墙上的食物
	for (int i = 0; i < Snake_SIZE; i++)
	{
		if (Snake_BODY[i].x == MEAT_x && Snake_BODY[i].y == MEAT_y)
		{
			random();
			break;
		}
	}//重新定位出现在蛇身上的食物
}
void gotoxy(int x, int y)//光标位置函数
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void hide_CURSOR()//隐藏光标
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = FALSE;//宏定义FALSE=0光标不可见
	cci.dwSize = sizeof(cci);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);
}
void spy(char *object)
{
	if (kbhit())
	{
		*object = getch();
	}//检查是否键入
}
void gameover()
{
	//for (int i = 0; i < Snake_SIZE; i++)
	//{
	//	gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
	//	printf(" ");
	//}//清除蛇身
	//gotoxy(MEAT_x - 1, MEAT_y - 1);
	//printf(" ");//清除食物


	for (int i = 3; i > 0; i--)
	{
		gotoxy(68, 8);
		printf("生命值:0     ");
		gotoxy(24, 10);
		printf("GAME OVER!!");
		gotoxy(18, 12);
		printf("玩家游戏分数为：%d", score);
		gotoxy(13, 14);
		printf("%d second later back to the interface!", i);
		delay(1);
	}
	score = 0;//分数清零
	choice = 0;//清零防止无限PLAY
	speed = 0.1;//重置速度
	Snake_LIFE = 100;//重置生命值
	system("cls");
}
void Interface()//界面函数
{
	system("color Bc");
	hide_CURSOR();
	gotoxy(66, 1);
	printf("Made by 莫廷钰");
	Wall();
	gotoxy(20, 8);
	printf("Gluttonous Snake");
	gotoxy(20, 10);
	printf("(1)PLAY");
	gotoxy(20, 12);
	printf("(2)Instructions");
	gotoxy(20, 14);
	printf("(3)Level ajustment");
	gotoxy(20, 16);
	printf("(4)EXIT");
	gotoxy(60, 25);
}
void PLAY()
{
	while (1)
	{
		system("cls");
		init();//初始化
		ch = 'd';
		while (ch != 32)
		{

			hide_CURSOR();
			move();
			check();
			delay(speed);
			spy(&ch);

		}
		gameover();
		break;
	}
}
void EXIT()
{
	free(Snake_BODY);//释放蛇占用的内存
	gotoxy(62, 25);
	exit(0);
}
void Wall()
{
	for (int i = 0; i < ground_SIZE_WIDTH; )
	{
		gotoxy(i, 0);
		printf("▓");
		i += 2;
	}//上墙初始化(
	for (int i = 0; i < ground_SIZE_WIDTH + 1; )
	{
		gotoxy(i, ground_SIZE_LONG);
		printf("▓");
		i += 2;
	}//下墙初始化

	for (int i = 0; i < ground_SIZE_LONG; i++)
	{
		gotoxy(0, i);
		printf("▓");

	}//左墙初始化
	for (int i = 0; i < ground_SIZE_LONG; i++)
	{
		gotoxy(ground_SIZE_WIDTH, i);
		printf("▓");
	}//右墙初始化
}
void Level_ajustment()
{
	int flag = 1;
	system("cls");
	gotoxy(66, 1);
	printf("Made by 莫廷钰");
	Wall();
	gotoxy(22, 7);
	printf("游戏难度调整界面");
	gotoxy(10, 9);
	printf("(1)Easy (2)Middle (3)Hard (Q)返回主界面");
	gotoxy(8, 11);
	printf("提示:玩家可摁下难度的序号相应数字来预览游戏难度");
	while (flag)
	{
		spy(&Speed_choice);
		switch (Speed_choice)
		{
		case '1':Demonstrate(0.2); break;
		case '2':Demonstrate(0.1); break;
		case '3':Demonstrate(0.05); break;
		case 'q':flag = 0; break;
		}

	}
	system("cls");
	Speed_choice = 0;//清零难度界面选择条件
	choice = 0;//清零主界面选择条件
}
void Demonstrate(double speed_player_set)
{
	char flag = 0;
	int i = 0;
	for (; i < 6; i++)
	{
		gotoxy(10 + i, 18);
		printf("*");
	}//初始化展示蛇
	while (i != 40)
	{
		gotoxy(10 + i, 18);
		printf("*");
		gotoxy(3 + i, 18);//移动光标到蛇尾处
		printf(" ");//清除蛇尾
		i++;
		delay(speed_player_set);
	}//移动展示蛇
	for (; i>22; i--)
	{
		gotoxy(10 + i, 18);
		printf(" ");
	}//展示结束 清除展示蛇
	gotoxy(11, 13);
	printf("确定以该速度为游戏难度：(1)Yes (2)No");
	while (1)
	{
		spy(&flag);
		if (flag == '1'||flag == '2')
		{
			break;
		}
	}
	for (int i = 0; i <37; i++)
	{
		gotoxy(11 + i, 13);
		printf(" ");
	}
	if (flag == '1')
	{
		speed = speed_player_set;
		Speed_choice = 'q';//清零主界面选择条件
		for (int i = 3; i > 0; i--)
		{
			gotoxy(12, 13);
			printf("游戏难度已设置完毕  %d秒后返回主界面", i);
			delay(1);
		}

		for (int i = 0; i < 44; i++)
		{
			gotoxy(10 + i, 13);
			printf(" ");
		}
	}
	else
	{
		Speed_choice = 0;
		for (int i = 3; i > 0; i--)
		{
			gotoxy(15, 13);
			printf("[%d]秒后请玩家请重新选择游戏难度", i);
			delay(1);
		}
		for (int i = 0; i < 33; i++)
		{
			gotoxy(15 + i, 13);
			printf(" ");
		}
		gotoxy(18, 13);
		printf("请重新选择游戏难度");
	}

}
void process()
{
	while (1)
	{

		spy(&choice);
		switch (choice)
		{
		case '1':PLAY(); Interface(); break;
		case '2':Instructions(); Interface(); break;
		case '3':Level_ajustment(); Interface(); break;
		case '4':EXIT();
		}
	}
}
void Instructions()
{
	system("cls");
	gotoxy(66, 1);
	printf("Made by 莫廷钰");
	Wall();
	gotoxy(23, 7);
	printf("游戏指南界面");
	gotoxy(3, 9);
	printf("(W)向上移动蛇 (S)向下移动蛇 (A)向左移动蛇 (D)向右移动蛇");
	gotoxy(3, 11);
	printf("(SPACE)中途退出游戏");
	gotoxy(3, 13);
	printf("死亡条件:(1)生命值耗尽(2)撞墙(3)自咬");
	gotoxy(20, 17);
	printf("请按下'Q'键返回主界面");
	while (1)
	{
		spy(&choice);
		if (choice == 'q')
		{
			break;
		}
	}
	system("cls");
	//choice = 0;//清零主界面选择条件
}