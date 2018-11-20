
#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"Map.h" //��
//#include"MiniGame.h" //�̴ϰ��� �Լ�

#define UP 119 //W
#define LEFT 97 //A
#define DOWN 115 //S
#define RIGHT 100 //D

//Size of gameboard,itemBoard,messageBoard
#define GBOARD_WIDTH 38
#define GBOARD_HEIGHT 19
#define IBOARD_WIDTH 10
#define IBOARD_HEIGHT 26
#define MBOARD_WIDTH 38
#define MBOARD_HEIGHT 6
#define ROOM2_WIDTH 30
#define ROOM2_HEIGHT 17

//Starting point of gameboard, itemBoard, messageBoard
#define GBOARD_ORIGIN_X 5
#define GBOARD_ORIGIN_Y 2
#define IBOARD_ORIGIN_X 84
#define IBOARD_ORIGIN_Y 2
#define MBOARD_ORIGIN_X 5
#define MBOARD_ORIGIN_Y 22

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];		//�������̽� �� ���� ����â�� �ش�Ǵ� �κ�
int itemBoardInfo[IBOARD_HEIGHT + 1][IBOARD_WIDTH + 2];
int messageBoardInfo[MBOARD_HEIGHT + 1][MBOARD_WIDTH + 2];

COORD player = { 50, 15 }; //x��ǥ ¦���� ����(Ȧ���� �浹üũ ����)
int currentRoomNumber = 5; //���� �÷��̾ �ִ� �� ��ȣ
int mapSize[10][2]; //�� ���� ����,���� ũ�� ����
int LifeLimit = 2;

void SetCurrentCursorPos(int x, int y)
{
	COORD player = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), player);
}

COORD GetCurrentCursorPos()
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;
	return curPoint;
}

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int DetectCollision(int posX, int posY, char Map[][17][36]) {
	//�浹�� ��ȯ��: 0

	if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - 9) / 2] != 0) {
		return 0;
	}

	return 1;
}

void Interface()				// ���� ��ü �������̽��� �׸��� �Լ�
{
	int x, y;
	//���� ���� �������̽�
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == GBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == GBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (x = 1; x < (GBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		//�� �̷��� �����ϴ� ���ϱ�?: �ϴ� ������ ���� ���� �Է����� x=1���� �����ϰ� �ȴ�.
		//���� Ŀ���� ���´� ORIGIN_X�� ���� ������������, 
		printf("��");
	}
	for (x = 1; x < (GBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y);
		printf("��");
	}
	//������â �������̽�
	for (y = 0; y <= IBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X, IBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == IBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (y = 0; y <= IBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + (IBOARD_WIDTH + 1) * 2, IBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == IBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (x = 1; x < (IBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + x, IBOARD_ORIGIN_Y + IBOARD_HEIGHT);
		//�� �̷��� �����ϴ� ���ϱ�?: �ϴ� ������ ���� ���� �Է����� x=1���� �����ϰ� �ȴ�.
		//���� Ŀ���� ���´� ORIGIN_X�� ���� ������������, 
		printf("��");
	}
	for (x = 1; x < (IBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + x, IBOARD_ORIGIN_Y);
		printf("��");
	}
	//�޼���â �������̽� 
	
	for (y = 0; y <= MBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X, MBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == MBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (y = 0; y <= MBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + (MBOARD_WIDTH + 1) * 2, MBOARD_ORIGIN_Y + y);
		if (y == 0) printf("��");
		else if (y == MBOARD_HEIGHT) printf("��");
		else printf("��");
	}
	for (x = 1; x < (MBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + x, MBOARD_ORIGIN_Y + MBOARD_HEIGHT);
		//�� �̷��� �����ϴ� ���ϱ�?: �ϴ� ������ ���� ���� �Է����� x=1���� �����ϰ� �ȴ�.
		//���� Ŀ���� ���´� ORIGIN_X�� ���� ������������, 
		printf("��");
	}
	for (x = 1; x < (MBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + x, MBOARD_ORIGIN_Y);
		printf("��");
	}

	//�޼��� â �׽�Ʈ
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
	printf("Hello");
	//printf("%d", Map[currentRoomNumber][player.X+1][player.Y+5]);
	//printf("Hello");
	/*SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2);
	printf("Hello");
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 3);
	printf("Hello");
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 4);
	printf("Hello");
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 5);
	printf("Hello");*/


	//Collision Check
	//GameBoard
	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (x = 0; x < GBOARD_WIDTH + 2; x++)
	{
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
	}

	//MessageBoard
	for (y = 0; y < MBOARD_HEIGHT; y++)
	{
		messageBoardInfo[y][0] = 1;
		messageBoardInfo[y][MBOARD_WIDTH + 1] = 1;
	}

	for (x = 0; x < MBOARD_WIDTH + 2; x++)
	{
		messageBoardInfo[MBOARD_HEIGHT][x] = 1;
	}

	//SetCurrentCursorPos(0, 0);
}

void DrawGameUI(char Map[][17][36])
{
	int i, j;
	for (i = 0; i < 36; i++) {
		for (j = 0; j < 17; j++) {
			if (currentRoomNumber == 0)//Ʃ�丮��
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					//��� ���� ������ �����Ϸ��� ���� 9 ��� �ذ��ؾ��� �� ����-����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					//�ƿ� ���� ���߾ӿ� ���Բ� ����� �ߴµ� ������ ������ ���� �� �� ����.-����
					printf("��");
				}
			}

			if (currentRoomNumber == 4) //4����
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
			}

			if (currentRoomNumber == 5) //5����
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
			}

			if (currentRoomNumber == 7) //����
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					printf("��");
				}
			}

			if (currentRoomNumber == 10) //������
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//�ʿ�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//�� �渶�� ũ�Ⱑ �ٸ��� �߾ӿ� ��ġ�ϵ��� ������ ���� �����Դϴ� -����
					printf("��");
				}

			}
		}
	}
}

void ShiftLeft()
{
	if (!DetectCollision(player.X - 2, player.Y, Map)) {
		if (Map[currentRoomNumber][player.Y-4][(player.X-10-6)/2] != 1) {
			//���� �ƴ϶� �����ۿ� �ε��� ���, �þ��� ���� ��ȯ�� �����ϵ��� ��.
		//	printf("2 %d %d %d", Map[currentRoomNumber][player.Y - 4][(player.X - 10 - 4) / 2], player.Y-4, (player.X - 10 - 4) / 2);
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("��");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X -= 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("��");
}

void ShiftRight()
{
	if (!DetectCollision(player.X + 2, player.Y, Map)) {
		//printf("1");
		if (Map[currentRoomNumber][player.Y - 4][(player.X - 10 - 2)/2] != 1) {
			//���� �ƴ϶� �����ۿ� �ε��� ���, �þ��� ���� ��ȯ�� �����ϵ��� ��.
		//	printf(" 2");
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("��");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X += 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("��");
}

void ShiftUp()
{
	if (!DetectCollision(player.X, player.Y - 1, Map)) {
		if (Map[currentRoomNumber][player.Y - 5][(player.X-10-4)/2] != 1) {
			//���� �ƴ϶� �����ۿ� �ε��� ���, �þ��� ���� ��ȯ�� �����ϵ��� ��.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("��");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y -= 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("��");
}

void ShiftDown()
{
	if (!DetectCollision(player.X, player.Y + 1, Map)) {
		if (Map[currentRoomNumber][player.Y - 3][(player.X - 10 - 4) / 2] != 1) {
			//���� �ƴ϶� �����ۿ� �ε��� ���, �þ��� ���� ��ȯ�� �����ϵ��� ��.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("��");
		}
		return 0;
	}
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y += 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("��");
}

void ProcessKeyInput()
{
	int key, i;
	for (i = 0; i < 1; i++)
	{
		key = _getch();
		switch (key) {
		case LEFT:
			ShiftLeft();
			break;
		case RIGHT:
			ShiftRight();
			break;
		case UP:
			ShiftUp();
			break;
		case DOWN:
			ShiftDown();
			break;
		}
		Sleep(10);
	}
}

int main()
{
	system("title soRRow"); //�ܼ�â ����
	system("mode con cols=112 lines=33"); //�ܼ�â ũ�� ����: cols=ĭ/��(����)   lines=��/��(����)
	RemoveCursor();
	GetCurrentCursorPos(player);
	currentRoomNumber = 4; //���� �� ��ȣ
	DrawGameUI(Map);
	SetCurrentCursorPos(player.X, player.Y);
	printf("��");
	Interface();
	
	while (1) {
		ProcessKeyInput();
		if ((player.X == 48 && player.Y == 17) || (player.X == 50 && player.Y == 17)) {}

	}
	return 0;
}