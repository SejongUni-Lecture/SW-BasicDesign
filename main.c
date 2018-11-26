#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"Map.h" //맵
#include "door.h"//각 방의 문 좌표
#include "item.h"//아이템 정보 

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

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];      //인터페이스 중 게임 실행창에 해당되는 부분
int itemBoardInfo[IBOARD_HEIGHT + 1][IBOARD_WIDTH + 2];
int messageBoardInfo[MBOARD_HEIGHT + 1][MBOARD_WIDTH + 2];

COORD player = { 52, 17 }; //x좌표 짝수로 설정(홀수는 충돌체크 씹힘)
int currentRoomNumber = 7; //현재 플레이어가 있는 방 번호
int mapSize[10][2]; //각 방의 가로,세로 크기 저장
int LifeLimit = 2;

//New
typedef struct Item_ListNode {
	int item;
	struct Item_ListNode* link;
}LN;

typedef struct HEAD {
	LN* head;
}Head;

Head* L;

Head* create_Linklist_h() {
	Head* L;
	L = (Head*)malloc(sizeof(Head));
	L->head = NULL;
}

void insertNode(int key) {
	LN* newNode;
	LN* temp;
	newNode = (LN*)malloc(sizeof(LN));
	newNode->item = key;
	newNode->link = NULL;
	if (L->head == NULL) {
		L->head = newNode;
		return;
	}
	temp = L->head;
	while (temp->link != NULL) {
		temp = temp->link;
	}
	temp->link = newNode;
}

void DeleteNode(LN* p) {
	LN* pre;
	if (L->head == NULL) { return; }
	if (L->head->link == NULL) {
		free(L->head);
		L->head = NULL;
		return;
	}
	else if (p == NULL) { return; }
	else {
		pre = L->head;
		while (pre->link != p) {
			pre = pre->link;
		}
		pre->link = p->link;
		free(p);
	}
}

LN* searchNode(int item) {
	LN* temp;
	temp = L->head;
	while (temp != NULL) {
		if (temp->item == item) {
			return temp;
		}
		else {
			temp = temp->link;
		}
	}
	return temp;
}
//New

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
	//충돌시 반환값: 0

	if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - 9) / 2] != 0) {
		return 0;
	}

	return 1;
}

//New
void printList() {
	LN* p;
	COORD q;
	int i;
	p = L->head;
	SetCurrentCursorPos(86, 3);
	q = GetCurrentCursorPos();
	while (p != NULL) {
		printf("%s", MovableItem[currentRoomNumber][p->item]);
		SetCurrentCursorPos(q.X, q.Y);
		q.Y++;
		p = p->link;
	}
}

void Interface()            // 게임 전체 인터페이스를 그리는 함수
{
	int x, y;
	//게임 실행 인터페이스
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┌");
		else if (y == GBOARD_HEIGHT) printf("└");
		else printf("│");
	}
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┐");
		else if (y == GBOARD_HEIGHT) printf("┘");
		else printf("│");
	}
	for (x = 1; x < (GBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		//왜 이렇게 설정하는 것일까?: 일단 위에서 ㄴ자 모형 입력으로 x=1부터 시작하게 된다.
		//현재 커서의 상태는 ORIGIN_X로 부터 떨어져있으며, 
		printf("─");
	}
	for (x = 1; x < (GBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y);
		printf("─");
	}
	//아이템창 인터페이스
	for (y = 0; y <= IBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X, IBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┌");
		else if (y == IBOARD_HEIGHT) printf("└");
		else printf("│");
	}
	for (y = 0; y <= IBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + (IBOARD_WIDTH + 1) * 2, IBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┐");
		else if (y == IBOARD_HEIGHT) printf("┘");
		else printf("│");
	}
	for (x = 1; x < (IBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + x, IBOARD_ORIGIN_Y + IBOARD_HEIGHT);
		//왜 이렇게 설정하는 것일까?: 일단 위에서 ㄴ자 모형 입력으로 x=1부터 시작하게 된다.
		//현재 커서의 상태는 ORIGIN_X로 부터 떨어져있으며, 
		printf("─");
	}
	for (x = 1; x < (IBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(IBOARD_ORIGIN_X + x, IBOARD_ORIGIN_Y);
		printf("─");
	}
	//메세지창 인터페이스 
	for (y = 0; y <= MBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X, MBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┌");
		else if (y == MBOARD_HEIGHT) printf("└");
		else printf("│");
	}
	for (y = 0; y <= MBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + (MBOARD_WIDTH + 1) * 2, MBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┐");
		else if (y == MBOARD_HEIGHT) printf("┘");
		else printf("│");
	}
	for (x = 1; x < (MBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + x, MBOARD_ORIGIN_Y + MBOARD_HEIGHT);
		//왜 이렇게 설정하는 것일까?: 일단 위에서 ㄴ자 모형 입력으로 x=1부터 시작하게 된다.
		//현재 커서의 상태는 ORIGIN_X로 부터 떨어져있으며, 
		printf("─");
	}
	for (x = 1; x < (MBOARD_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(MBOARD_ORIGIN_X + x, MBOARD_ORIGIN_Y);
		printf("─");
	}

	//메세지 창 테스트
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
	printf("Hello");
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
}

//방 벽 그리는 함수
void DrawRoomFrame(char Map[][17][36], int x, int y)
{   //x=i, y=j

	if (Map[currentRoomNumber][y][x + 1] == 1)
	{
		if (Map[currentRoomNumber][y][x - 1] == 1)
		{
			printf("─");
		}
		else if (Map[currentRoomNumber][y + 1][x] == 1)
		{
			printf("┌");
		}
		else if (Map[currentRoomNumber][y - 1][x] == 1)
		{
			printf("└");
		}
		else if (Map[currentRoomNumber][y][x - 1] == 0)
		{
			printf("─");
		}
	}

	else if (Map[currentRoomNumber][y - 1][x] == 1 && Map[currentRoomNumber][y + 1][x] == 1)
	{
		printf("│");
	}


	else if (Map[currentRoomNumber][y][x - 1] == 1)
	{
		if (Map[currentRoomNumber][y + 1][x] == 1) {
			printf("┐");
		}
		else if (Map[currentRoomNumber][y - 1][x] == 1)
		{
			printf("┘");
		}
		else if (Map[currentRoomNumber][y][x + 1] == 0)
		{
			printf("─");
		}
	}

	else if (Map[currentRoomNumber][y - 1][x] == 0 && Map[currentRoomNumber][y + 1][x] == 1)
	{
		printf("│");
	}
	else if (Map[currentRoomNumber][y - 1][x] == 1 && Map[currentRoomNumber][y + 1][x] == 0)
	{
		printf("│");
	}

	else printf("■");
}

void DrawGameUI(char Map[][17][36])
{
	int i, j;
	for (i = 0; i < 36; i++) {
		for (j = 0; j < 17; j++) {
			if (currentRoomNumber == 0)//나눈 이유: 그냥 한눈에 아이템뭐 있는지 보려고
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//수정!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("▣");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					//아예 맵을 정중앙에 오게끔 만들긴 했는데 조금의 수정만 들어가면 될 것 같음.-서영
					printf("♀");
				}
			}

			if (currentRoomNumber == 1)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//여기!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111 
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//수정!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("⊙");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("★");
				}


				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┘");
				}
			}

			if (currentRoomNumber == 2)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 24) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Ω");
				}
				if (Map[currentRoomNumber][j][i] == 22) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("∮");
				}
				if (Map[currentRoomNumber][j][i] == 23) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("￥");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("★");
				}


				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┘");
				}
			}

			if (currentRoomNumber == 3)//나눈 이유: 그냥 한눈에 아이템뭐 있는지 보려고
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//수정!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┼");
				}
				if (Map[currentRoomNumber][j][i] == 76) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}
				if (Map[currentRoomNumber][j][i] == 7 || Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("○");
				}
				if (Map[currentRoomNumber][j][i] == 3 || Map[currentRoomNumber][j][i] == 4 || Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("ㆀ");
				}
			}

			if (currentRoomNumber == 4) //갈래방
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 42) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Γ");
				}
				if (Map[currentRoomNumber][j][i] == 43) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("●");
				}
				if (Map[currentRoomNumber][j][i] == 44) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 45) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 46) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 47) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 48) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
			}

			if (currentRoomNumber == 5)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 54) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 55) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 52) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("ℓ");
				}
				if (Map[currentRoomNumber][j][i] == 53) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Ф");
				}
				if (Map[currentRoomNumber][j][i] == 56) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 57) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}

			}

			if (currentRoomNumber == 6)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//수정!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 63) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("↖");
				}
				if (Map[currentRoomNumber][j][i] == 62) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("☎");
				}
			}

			if (currentRoomNumber == 7)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("├");
				}
				if (Map[currentRoomNumber][j][i] == 9) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┤");
				}
				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}

			}

			if (currentRoomNumber == 9)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 92) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 93) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 94) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 95) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("★");
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}

			if (currentRoomNumber == 10) //갈래방
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}

			}
		}
	}
}

void DeleteMap(int nextRoom) { //GBOARD에 출력된 내용을 전부 지운다.
	int i, j;
	COORD tmp;
	char* playerICON = "▼";

	for (i = GBOARD_ORIGIN_X + 2; i < GBOARD_ORIGIN_X + 2 + GBOARD_WIDTH * 2; i++) {
		for (j = GBOARD_ORIGIN_Y + 1; j < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; j++) {
			SetCurrentCursorPos(i, j);
			printf(" ");
		}
	}

	//Sleep(10);   
	if (nextRoom == 7) {
		//각 방에서 복도로 이동
		tmp.X = entrance_door[currentRoomNumber][0];
		tmp.Y = entrance_door[currentRoomNumber][1];

		if (currentRoomNumber == 1 || currentRoomNumber == 2 || currentRoomNumber == 3) {
			tmp.X += 2;
			playerICON = "▶";
		}
		if (currentRoomNumber == 10 || currentRoomNumber == 6) {
			tmp.X -= 2;
			playerICON = "◀";
		}
		if (currentRoomNumber == 0) {
			tmp.Y += 1;
			playerICON = "▼";
		}
	}

	else if (nextRoom == 10) {
		if (currentRoomNumber == 7) {
			//7번방에서 갈래길(10번방)로 이동
			tmp.X = exit_door[nextRoom][0];
			tmp.Y = exit_door[nextRoom][1];
			tmp.Y -= 1;
			playerICON = "▲";
		}
		else if (currentRoomNumber == 4 || currentRoomNumber == 5) {
			//4, 5번방에서 갈래길(10번방)로 이동
			tmp.X = entrance_door[currentRoomNumber][0];
			tmp.Y = entrance_door[currentRoomNumber][1];
			if (currentRoomNumber == 4) {
				tmp.X += 2;
				playerICON = "▶";
			}
			if (currentRoomNumber == 5) {
				tmp.X -= 2;
				playerICON = "◀";
			}
		}
	}
	else {
		//복도에서 각 방으로 이동
		tmp.X = exit_door[nextRoom][0];
		tmp.Y = exit_door[nextRoom][1];

		if (nextRoom == 0 || nextRoom == 2 || nextRoom == 3) {
			tmp.Y += 1;
			playerICON = "▼";
		}
		if (nextRoom == 4 || nextRoom == 6 || nextRoom == 10) {
			tmp.Y -= 1;
			playerICON = "▲";
		}
		if (nextRoom == 1 || nextRoom == 5) {
			tmp.X -= 2;
			playerICON = "◀";
		}
	}

	currentRoomNumber = nextRoom;
	DrawGameUI(Map);
	player.X = tmp.X;
	player.Y = tmp.Y;
	SetCurrentCursorPos(player.X, player.Y);

	printf("%s", playerICON);
}

void Check_exit_door() { //현재 플레이어의 위치가 방에서 나가는 좌표인지 검사

	if ((player.X == exit_door[currentRoomNumber][0] && player.Y == exit_door[currentRoomNumber][1]) ||
		(player.X == exit_door[currentRoomNumber][2] && player.Y == exit_door[currentRoomNumber][3]))
	{
		if (currentRoomNumber == 4 || currentRoomNumber == 5) DeleteMap(10);
		else DeleteMap(7); //복도에서의 좌표를 설정해 줌
	}

}

void Check_entrance_door() { //현재 플레이어의 위치가 방으로 들어가는 좌표인지 검사
	int room_num[5] = { 1, 2, 3, 10, 6 }; //복도에서 들어갈 수 있는 방 번호
	int i;

	if (currentRoomNumber == 10) {
		if ((player.X == entrance_door[4][0] && player.Y == entrance_door[4][1]) ||
			(player.X == entrance_door[4][2] && player.Y == entrance_door[4][3])) {
			DeleteMap(4); //들어갈 방의 좌표를 설정해줌
		}
		if ((player.X == entrance_door[5][0] && player.Y == entrance_door[5][1]) ||
			(player.X == entrance_door[5][2] && player.Y == entrance_door[5][3])) {
			DeleteMap(5); //들어갈 방의 좌표를 설정해줌
		}
	}

	else {
		for (i = 0; i < 5; i++) {
			if ((player.X == entrance_door[room_num[i]][0] && player.Y == entrance_door[room_num[i]][1]) ||
				(player.X == entrance_door[room_num[i]][2] && player.Y == entrance_door[room_num[i]][3])) {
				DeleteMap(room_num[i]); //들어갈 방의 좌표를 설정해줌
				break;
			}
		}
	}
}


void ShiftLeft()
{
	if (!DetectCollision(player.X - 2, player.Y, Map)) {
		if (Map[currentRoomNumber][player.X - 2][player.Y] != 1) {
			//벽이 아니라 아이템에 부딪힐 경우, 시야의 방향 전환이 가능하도록 함.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("◀");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X -= 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("◀");
}

void ShiftRight()
{
	if (!DetectCollision(player.X + 2, player.Y, Map)) {
		if (Map[currentRoomNumber][player.X + 2][player.Y] != 1) {
			//벽이 아니라 아이템에 부딪힐 경우, 시야의 방향 전환이 가능하도록 함.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("▶");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X += 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("▶");
}

void ShiftUp()
{
	if (!DetectCollision(player.X, player.Y - 1, Map)) {
		if (Map[currentRoomNumber][player.X][player.Y - 1] != 1) {
			//벽이 아니라 아이템에 부딪힐 경우, 시야의 방향 전환이 가능하도록 함.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("▲");
		}
		return 0;
	}

	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y -= 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("▲");
}

void ShiftDown()
{
	if (!DetectCollision(player.X, player.Y + 1, Map)) {
		if (Map[currentRoomNumber][player.X][player.Y + 1] != 1) {
			//벽이 아니라 아이템에 부딪힐 경우, 시야의 방향 전환이 가능하도록 함.
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			SetCurrentCursorPos(player.X, player.Y);
			printf("▼");
		}
		return 0;
	}
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y += 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("▼");
}

//New
int GetItem(int item) {
	int i, j;
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 36; j++) {
			if (Map[currentRoomNumber][i][j] == item) {
				Map[currentRoomNumber][i][j] = 0;
				DrawGameUI(Map);
			}
		}
	}
	insertNode(item);
	//printList();
}

void UseItem(int M_item, int Im_item) {
	LN* p;
	MovableItem[currentRoomNumber][M_item - 3] = 2;
	ImmovableItem[currentRoomNumber][Im_item - 2] = 1;
	p = searchNode(M_item - 3);
	if (p == NULL) {
		printf("None");
	}
	else {
		DeleteNode(p);
	}
	//printList();
	printf("ok");
}

void Item_Dis() {
	COORD p;
	LN* lp;
	int i, j, item;
	p = GetCurrentCursorPos();
	if (currentRoomNumber == 0) {
		if ((p.X == 40 && p.Y == 8) || (p.X == 38 && p.Y == 9)) {
			SetCurrentCursorPos(36, 8);
			printf(" ");
			item = 3;
			for (i = 0; i < 17; i++) {
				for (j = 0; j < 36; j++) {
					if (Map[currentRoomNumber][i][j] == item) {
						Map[currentRoomNumber][i][j] = 0;
						DrawGameUI(Map);
					}
				}
			}
			insertNode(item - 3);
			MovableItem[currentRoomNumber][0] = 1;

			lp = L->head;
			SetCurrentCursorPos(86, 3);
			p = GetCurrentCursorPos();
			while (lp != NULL) {
				printf("%s", MovableItem_Name[currentRoomNumber][lp->item]);
				SetCurrentCursorPos(p.X, p.Y);
				p.Y++;
				lp = lp->link;
			}
			//대사출력
		}
		else if ((p.X == 52 && p.Y == 11) || (p.X == 50 && p.Y == 12) || (p.X == 54 && p.Y == 12) || (p.X == 52 && p.Y == 13)) {
			UseItem(3, 2);
			GetItem(4); //1번방 열쇠
			MovableItem[currentRoomNumber][1] = 1;
		}
	}
	else if (currentRoomNumber == 1) {
		printf("%d %d", p.X, p.Y);
	}
}

void ProcessKeyInput()
{
	//int key = _getch();
	int key;
	int i;
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
	system("title soRRow"); //콘솔창 제목
	system("mode con cols=112 lines=33"); //콘솔창 크기 조절: cols=칸/행(가로)   lines=줄/열(세로)
	RemoveCursor();
	GetCurrentCursorPos(player);
	//currentRoomNumber = 0; //현재 방 번호
	DrawGameUI(Map);
	SetCurrentCursorPos(player.X, player.Y);
	printf("▲");
	Interface();

	while (1) {
		if (currentRoomNumber != 7) Check_exit_door();
		if (currentRoomNumber == 7 || currentRoomNumber == 10) Check_entrance_door();
		ProcessKeyInput();
	}
	return 0;
}