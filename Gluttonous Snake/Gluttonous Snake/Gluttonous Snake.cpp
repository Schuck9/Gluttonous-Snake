#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define ground_SIZE_WIDTH 60
#define ground_SIZE_LONG 25

typedef struct {
	int x;//������
	int y;//������
}location;//������������

int Snake_SIZE = 5;//�ߵĳ�ʼ����
int MEAT_x, MEAT_y;//ʳ�������
double speed = 0.1;//�ߵ��ٶ�
location *Snake_BODY = (location*)malloc(sizeof(location) * Snake_SIZE);//����
location *Snake_NEW;//���������������
location Snake_TAIL;//��β
char choice = 0;//��ʼ��switch��֧����
char ch = 'd';//�û��������
char Speed_choice = '0';//��ʼ���Ѷ�ѡ���֧����
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
	Snake_TAIL = Snake_BODY[0];//������β ���������
	score += 3;//��ʱ���������
	Snake_LIFE -= 0.4;//������ʱ���½�
	for (int i = 1; i < Snake_SIZE; i++)
	{
		Snake_BODY[i - 1] = Snake_BODY[i];//�����������
	}
	switch (ch)
	{
	case 'w':Snake_BODY[Snake_SIZE - 1].y--; break;//��
	case 's':Snake_BODY[Snake_SIZE - 1].y++; break;//��
	case 'd':Snake_BODY[Snake_SIZE - 1].x++; break;//��
	case 'a':Snake_BODY[Snake_SIZE - 1].x--; break;//��
	}//�ƶ���
	for (int i = 0; i < Snake_SIZE; i++)
	{
		gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
		printf("*");
	}//��ӡ����

	gotoxy(68, 7);
	printf("������%ld", score);
	gotoxy(68, 8);
	if (Snake_LIFE < 0)
	{
		printf("����ֵ��0");
	}
	else
	{
		printf("����ֵ��%.2f", Snake_LIFE);
	}
	gotoxy(68, 10);
	printf("W:�� S:�� A:�� D:��");
	gotoxy(68, 12);
	printf("���¿ո��������Ϸ");
}
void delay(double delay_time)//��ʱ����
{
	clock_t delay = delay_time*CLOCKS_PER_SEC;
	clock_t t = clock();
	while (clock() - t < delay);
}
void init()
{
	gotoxy(66, 1);

	printf("Made by Ī͢��");//ˮӡ
	Snake_SIZE = 5;//���¶����߳�
				   //speed = 0.1;//���¶����ٶ�
	srand((unsigned)time(NULL));//��ʱ������
	for (int i = 0; i < Snake_SIZE; i++)
	{
		Snake_BODY[i].x = 10;
		Snake_BODY[i].y = i + 10;
	}//����
	for (int i = 0; i < Snake_SIZE; i++)
	{
		gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
		printf("*");
	}//�����ʼ��
	Wall();//ǽ���ʼ��
	genarate_MEAT();//ʳ���ʼ��
}
void Snake_inflate()
{
	Snake_SIZE++;//����
	score += 20; //�Ե�ʳ�������20

	if (Snake_LIFE > 87)
	{
		Snake_LIFE = 100;
	}
	else Snake_LIFE += 12;

	if (Snake_SIZE % 5 == 0)
	{
		speed /= 1.2;//�ı��ٶ�
	}
	Snake_NEW = (location*)malloc(sizeof(location) * Snake_SIZE);//���¶�̬����������
	for (int i = 0; i < Snake_SIZE - 1; i++)
	{
		Snake_NEW[i + 1] = Snake_BODY[i];//��ԭ�ߵ��Ŵ����ʸ��Ƶ�����
	}
	Snake_NEW[0] = Snake_TAIL;//����µ���β
	location *p = Snake_BODY;//��ԭ���ڴ��ͷ�
	Snake_BODY = Snake_NEW;//���ߵ���
	free(p);//�ͷ�ԭ��
}//�߱䳤
void check()//��麯��
{
	if (Snake_BODY[Snake_SIZE - 1].x == MEAT_x && Snake_BODY[Snake_SIZE - 1].y == MEAT_y)//�Ե�ʳ��
	{
		Snake_inflate();//������
						//gotoxy(MEAT_x, MEAT_y);
						//printf(" ");
		genarate_MEAT();//���²���ʳ��
	}
	else
	{
		gotoxy(Snake_TAIL.x - 1, Snake_TAIL.y - 1);//�ƶ���굽��β��
		printf(" ");//�����β
	}//��ʳ
	for (int i = 0; i < Snake_SIZE - 1; i++)
	{
		if (Snake_BODY[Snake_SIZE - 1].x == Snake_BODY[i].x && Snake_BODY[Snake_SIZE - 1].y == Snake_BODY[i].y)//ҧ������
		{
			ch = 32; break;
		}
	}
	if (Snake_BODY[Snake_SIZE - 1].x == 2 || Snake_BODY[Snake_SIZE - 1].x == ground_SIZE_WIDTH + 1)//ײ����ǽ
	{
		ch = 32;
	}
	if (Snake_BODY[Snake_SIZE - 1].y == 1 || Snake_BODY[Snake_SIZE - 1].y == ground_SIZE_LONG + 1)//ײ����ǽ
	{
		ch = 32;
	}
	if (Snake_LIFE < 0)
	{
		ch = 32;
	}

}
void genarate_MEAT()//ʳ����������
{
	random();//�������ʳ������
	gotoxy(MEAT_x - 1, MEAT_y - 1);
	printf("*");//��ӡʳ��
}
void random()//���������
{
	MEAT_x = rand() % ground_SIZE_WIDTH - 1;//���ʳ�������
	MEAT_y = rand() % ground_SIZE_LONG - 1;//���ʳ��������

	if (MEAT_x < 3 || MEAT_y < 3)
	{
		random();
	}//���¶�λǽ�ϵ�ʳ��
	for (int i = 0; i < Snake_SIZE; i++)
	{
		if (Snake_BODY[i].x == MEAT_x && Snake_BODY[i].y == MEAT_y)
		{
			random();
			break;
		}
	}//���¶�λ�����������ϵ�ʳ��
}
void gotoxy(int x, int y)//���λ�ú���
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void hide_CURSOR()//���ع��
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = FALSE;//�궨��FALSE=0��겻�ɼ�
	cci.dwSize = sizeof(cci);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);
}
void spy(char *object)
{
	if (kbhit())
	{
		*object = getch();
	}//����Ƿ����
}
void gameover()
{
	//for (int i = 0; i < Snake_SIZE; i++)
	//{
	//	gotoxy(Snake_BODY[i].x - 1, Snake_BODY[i].y - 1);
	//	printf(" ");
	//}//�������
	//gotoxy(MEAT_x - 1, MEAT_y - 1);
	//printf(" ");//���ʳ��


	for (int i = 3; i > 0; i--)
	{
		gotoxy(68, 8);
		printf("����ֵ:0     ");
		gotoxy(24, 10);
		printf("GAME OVER!!");
		gotoxy(18, 12);
		printf("�����Ϸ����Ϊ��%d", score);
		gotoxy(13, 14);
		printf("%d second later back to the interface!", i);
		delay(1);
	}
	score = 0;//��������
	choice = 0;//�����ֹ����PLAY
	speed = 0.1;//�����ٶ�
	Snake_LIFE = 100;//��������ֵ
	system("cls");
}
void Interface()//���溯��
{
	system("color Bc");
	hide_CURSOR();
	gotoxy(66, 1);
	printf("Made by Ī͢��");
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
		init();//��ʼ��
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
	free(Snake_BODY);//�ͷ���ռ�õ��ڴ�
	gotoxy(62, 25);
	exit(0);
}
void Wall()
{
	for (int i = 0; i < ground_SIZE_WIDTH; )
	{
		gotoxy(i, 0);
		printf("��");
		i += 2;
	}//��ǽ��ʼ��(
	for (int i = 0; i < ground_SIZE_WIDTH + 1; )
	{
		gotoxy(i, ground_SIZE_LONG);
		printf("��");
		i += 2;
	}//��ǽ��ʼ��

	for (int i = 0; i < ground_SIZE_LONG; i++)
	{
		gotoxy(0, i);
		printf("��");

	}//��ǽ��ʼ��
	for (int i = 0; i < ground_SIZE_LONG; i++)
	{
		gotoxy(ground_SIZE_WIDTH, i);
		printf("��");
	}//��ǽ��ʼ��
}
void Level_ajustment()
{
	int flag = 1;
	system("cls");
	gotoxy(66, 1);
	printf("Made by Ī͢��");
	Wall();
	gotoxy(22, 7);
	printf("��Ϸ�Ѷȵ�������");
	gotoxy(10, 9);
	printf("(1)Easy (2)Middle (3)Hard (Q)����������");
	gotoxy(8, 11);
	printf("��ʾ:��ҿ������Ѷȵ������Ӧ������Ԥ����Ϸ�Ѷ�");
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
	Speed_choice = 0;//�����ѶȽ���ѡ������
	choice = 0;//����������ѡ������
}
void Demonstrate(double speed_player_set)
{
	char flag = 0;
	int i = 0;
	for (; i < 6; i++)
	{
		gotoxy(10 + i, 18);
		printf("*");
	}//��ʼ��չʾ��
	while (i != 40)
	{
		gotoxy(10 + i, 18);
		printf("*");
		gotoxy(3 + i, 18);//�ƶ���굽��β��
		printf(" ");//�����β
		i++;
		delay(speed_player_set);
	}//�ƶ�չʾ��
	for (; i>22; i--)
	{
		gotoxy(10 + i, 18);
		printf(" ");
	}//չʾ���� ���չʾ��
	gotoxy(11, 13);
	printf("ȷ���Ը��ٶ�Ϊ��Ϸ�Ѷȣ�(1)Yes (2)No");
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
		Speed_choice = 'q';//����������ѡ������
		for (int i = 3; i > 0; i--)
		{
			gotoxy(12, 13);
			printf("��Ϸ�Ѷ����������  %d��󷵻�������", i);
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
			printf("[%d]��������������ѡ����Ϸ�Ѷ�", i);
			delay(1);
		}
		for (int i = 0; i < 33; i++)
		{
			gotoxy(15 + i, 13);
			printf(" ");
		}
		gotoxy(18, 13);
		printf("������ѡ����Ϸ�Ѷ�");
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
	printf("Made by Ī͢��");
	Wall();
	gotoxy(23, 7);
	printf("��Ϸָ�Ͻ���");
	gotoxy(3, 9);
	printf("(W)�����ƶ��� (S)�����ƶ��� (A)�����ƶ��� (D)�����ƶ���");
	gotoxy(3, 11);
	printf("(SPACE)��;�˳���Ϸ");
	gotoxy(3, 13);
	printf("��������:(1)����ֵ�ľ�(2)ײǽ(3)��ҧ");
	gotoxy(20, 17);
	printf("�밴��'Q'������������");
	while (1)
	{
		spy(&choice);
		if (choice == 'q')
		{
			break;
		}
	}
	system("cls");
	//choice = 0;//����������ѡ������
}