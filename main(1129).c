#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "Map.h" //맵
#include "door.h"//각 방의 문 좌표
#include "item.h"//아이템 정보 

#define UP 119 //W
#define LEFT 97 //A
#define DOWN 115 //S
#define RIGHT 100 //D
#define SPACEBAR 32

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

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];		//인터페이스 중 게임 실행창에 해당되는 부분
int itemBoardInfo[IBOARD_HEIGHT + 1][IBOARD_WIDTH + 2];
int messageBoardInfo[MBOARD_HEIGHT + 1][MBOARD_WIDTH + 2];

COORD player = { 52, 15 }; //x좌표 짝수로 설정(홀수는 충돌체크 씹힘)
int currentRoomNumber = 7; //현재 플레이어가 있는 방 번호
int mapSize[10][2]; //각 방의 가로,세로 크기 저장

//미니게임 변수*********************************************************************
//가장 최근에 어디 방향 눌렀는지 저장
int left = 0;
int right = 0;
int up = 0;
int down = 0;

//4번방 변수*******************
int talk_to_superior = 0; //상사와 대화 했는지 확인(이전과 이후 대화창 출력 다르게)
int mini4_finish = 0; //4번방 게임 통과 여부 확인
int mini4_door = 0; //4번방 들어가면 문 잠그기
int mini4_come = 0; //4번방에 완전히 들어왔는지 확인
int mini4_print = 0; //들어갈 수 없다 출력했는지 확인
int mini4_disappear = 0; //문잠금 삭제했는지 확인
int LifeLimit = 2;
//*****************************

//5번방 변수*******************
int mini5_finish = 0; //5번방 게임 통과 여부 확인
int mini5_door = 0; //5번방 들어가면 문 잠그기
int mini5_come = 0; //5번방에 완전히 들어왔는지 확인
//*****************************
//**********************************************************************************


typedef struct Item_ListNode {
	int room;
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

void insertNode(int r, int key) {
	LN* newNode;
	LN* temp;
	newNode = (LN*)malloc(sizeof(LN));
	newNode->room = r;
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

LN* searchNode(int r, int item) {
	LN* temp;
	temp = L->head;
	while (temp != NULL) {
		if (temp->item == item && temp->room == r) {
			return temp;
		}
		else {
			temp = temp->link;
		}
	}
	return temp;
}

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

void ShowInputCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

int DetectCollision(int posX, int posY, char Map[][17][36]) {
	//충돌시 반환값: 0

	if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - 9) / 2] != 0) {
		return 0;
	}

	return 1;
}

void PrintList() {
	LN* p;
	COORD q;
	int i;
	p = L->head;
	SetCurrentCursorPos(86, 3);
	q = GetCurrentCursorPos();
	while (p != NULL) {
		SetCurrentCursorPos(q.X, q.Y);
		printf("%s", MovableItem_Name[p->room][p->item]);
		q.Y = q.Y + 1;
		p = p->link;
	}
}

void DeleteList() {
	int i, j;
	COORD tmp;

	for (i = IBOARD_ORIGIN_X + 2; i < IBOARD_ORIGIN_X + 2 + IBOARD_WIDTH * 2; i++) {
		for (j = IBOARD_ORIGIN_Y + 1; j < IBOARD_ORIGIN_Y + IBOARD_HEIGHT; j++) {
			SetCurrentCursorPos(i, j);
			printf(" ");
		}
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
	//SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
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
}

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
		if (Map[currentRoomNumber][player.Y - GBOARD_ORIGIN_Y - 2][(player.X - 2 - GBOARD_ORIGIN_X - 9) / 2] != 1) {
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
		if (Map[currentRoomNumber][player.Y - GBOARD_ORIGIN_Y - 2][(player.X+2 - GBOARD_ORIGIN_X - 9) / 2] != 1) {
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
		if (Map[currentRoomNumber][player.Y - 1 - GBOARD_ORIGIN_Y - 2][(player.X - GBOARD_ORIGIN_X - 9) / 2] != 1) {
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
		if (Map[currentRoomNumber][player.Y + 1 - GBOARD_ORIGIN_Y - 2][(player.X - GBOARD_ORIGIN_X - 9) / 2] != 1) {
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


//대화UI 출력함수 //대화 중에는 움직일 수 x //쓰고+지우기
void Conversation(char talk[][101])
{
	int i, j, key;
	int next_row = 1; //몇 줄 내려갈 지 결정
	int k = 0; //위치 설정할 때 j대신 사용(줄바꿈이 일어났을 때)
	int judge = 0;
	COORD next; //다음 줄로 넘어가는지 확인

	for (i = 0; i < 1; i++) //쓰고
	{
		k = 0;
		next_row = 1;
		for (j = 0; talk[i][j]; j++)
		{
			if (talk[i][j] == ' '){ //띄어쓰기만
				if (j == 0 || ((j != 0) && ((talk[i][j - 1] >= 'A'&&talk[i][j - 1] <= 'Z') || (talk[i][j - 1] >= 'a'&&talk[i][j - 1] <= 'z'))))
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k + 2, MBOARD_ORIGIN_Y + next_row);
				else SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k + 4, MBOARD_ORIGIN_Y + next_row);
			}
			else if (talk[i][j] == '.' || talk[i][j] == ',' || talk[i][j] == '"' || talk[i][j] == '!' || talk[i][j] == '?' || talk[i][j] == ':')
				SetCurrentCursorPos(MBOARD_ORIGIN_X + k + 3, MBOARD_ORIGIN_Y + next_row);
			else if ((talk[i][j] >= 'A'&&talk[i][j] <= 'Z') || (talk[i][j] >= 'a'&&talk[i][j] <= 'z')) {
				if (k == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
				else SetCurrentCursorPos(MBOARD_ORIGIN_X + 3 + k, MBOARD_ORIGIN_Y + next_row);
			}
			else if (talk[i][j] >= '0'&&talk[i][j] <= '9') //숫자
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
			else //한글
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
			next = GetCurrentCursorPos();
			if (next.X >= 80)
			{
				next_row++;
				k = -1;
			}
			printf("%c", talk[i][j]);
			k++;
			Sleep(50);
		}
		Sleep(500);

		//문장 지우기(스페이스바 누르기 전까지는 안움직이도록/누르면 다음 문장 출력)
		next_row = 1;
		k = 0;
		while (1)
		{
			key = _getch();
			if (key == SPACEBAR) {
				for (j = 0; talk[i][j]; j++) {
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("  ");
					k++;
				}
				Sleep(300);
				break;
			}
		}
	}
}

//******************************************************미니게임함수****************************************************************
//상사의 대사 출력
void superior()
{
	char superior1[3][51] = { "어서와요", "나는 상사니까", "너가 범인을 찾으렴!" };
	char superior2[1][51] = { "얼른얼른 움직여 뚠뚠따라라랏" };
	char talk[1][51] = { "범인은? (한글자로 입력하세요)" };
	char input[2] = ""; //플레이어가 답을 입력하면 여기로 받아들이기
	char stop[1][51] = { "이제 나가도 될 것 같다." }; //정답 맞췄으면
	int i, j, key;
	int next_row = 1; //몇 줄 내려갈 지 결정
	int k = 0; //위치 설정할 때 j대신 사용(줄바꿈이 일어났을 때)
	int judge = 0;
	COORD next; //다음 줄로 넘어가는지 확인

	if (talk_to_superior == 0){
		for (i = 0; i < 3; i++)
			Conversation(superior1[i]);
		talk_to_superior = 1;
		SetCurrentCursorPos(72, 4);
		printf("Life: ♥♥");
	}
	else if (talk_to_superior == 1) Conversation(superior2);
	else {
		if (!mini4_finish)
		{
			for (i = 0; i < 1; i++){ //talk 문장 출력
				k = 0;
				next_row = 1;
				for (j = 0; talk[i][j]; j++)
				{
					if (talk[i][j] == ' ') //띄어쓰기만
						SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k + 2, MBOARD_ORIGIN_Y + next_row);
					else if (talk[i][j] == '.' || talk[i][j] == ',' || talk[i][j] == '"' || talk[i][j] == '!' || talk[i][j] == '?') //한글을 제외한 특수문자들
						SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k + 1, MBOARD_ORIGIN_Y + next_row);
					else if ((talk[i][j] >= 'A'&&talk[i][j] <= 'Z') || (talk[i][j] >= 'a'&&talk[i][j] <= 'z'))
						SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					else //한글
						SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					next = GetCurrentCursorPos();
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("%c", talk[i][j]);
					k++;
					Sleep(50);
				} 
				Sleep(500);
			}
			ShowInputCursor();
			scanf("%s", input); //답 입력
			if (strcmp(input, "A") == 0) {
				mini4_finish = 1;
				RemoveCursor();
				next_row = 1;
				k = 0;
				for (j = 0; talk[i][j]; j++) {
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("  ");
					k++;
				}
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
				printf("정답!");
				Sleep(500);
				next_row = 1;
				k = 0;
				for (j = 0; j < 10; j++) {
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("  ");
					k++;
				}
				SetCurrentCursorPos(72, 4);
				printf("          ");
				mini4_finish = 1;
			}
			else {
				RemoveCursor();
				next_row = 1;
				k = 0;
				for (j = 0; talk[i][j]; j++) {
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("  ");
					k++;
				}
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
				printf("오답!");
				Sleep(500);
				SetCurrentCursorPos(72, 4);
				LifeLimit--;
				if (LifeLimit == 1) printf("Life: ♥   ");
				else printf("Life:     ");
				Sleep(300);
				next_row = 1;
				k = 0;
				for (j = 0; j < 10; j++) {
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2 + k, MBOARD_ORIGIN_Y + next_row);
					if (next.X >= 80) {
						next_row++;
						k = -1;
					}
					printf("  ");
					k++;
				}
				if (LifeLimit == 0) { //게임오버 논의하기!!
					system("cls");
					SetCurrentCursorPos(10, 10);
					printf("끝끝끝끝이제그만 잘했어요 유후유후~!~!\n");
					exit(0);
				}
			}
		}
		else Conversation(stop);
	}
}

//총이다 빵야빵야 대사 출력
void gun()
{
	char gun1[1][51] = { "누군가 사용한 흔적이 있는 총이다." }; //상사와 대화하기 전 총을 눌렀을 시(범죄도구가 무엇인지 파악x)
	char gun2[1][51] = { "이번 극장총기난사사건에 사용되었던 총이다." }; //상사와 대화한 후 총을 눌렀을 시(범죄도구 인식)
	if (talk_to_superior == 0) Conversation(gun1); //상사와 대화하기 전이면 gun1 대사 출력
	else Conversation(gun2); //상사와 대화한 후면 gun2 대사 출력
}

//거짓말 D, 범인 A
//용의자 A대사 출력
void suspectA()
{
	char A1[1][51] = { "사람이 있다." };
	char A2[1][51] = { "A: 거짓말을 하고 있는건 D 입니다!" };
	char stop[1][51] = { "이제 나가도 될 것 같다." };

	if (talk_to_superior == 0) Conversation(A1);
	else {
		if (mini4_finish) Conversation(stop);
		else {
			Conversation(A2);
			talk_to_superior = 2;
		}
	}
}

//용의자 B대사 출력
void suspectB()
{
	char B1[1][51] = { "사람이 있다." };
	char B2[1][51] = { "B: 범인은 A 아니면 D 둘 중에 하나에요." };
	char stop[1][51] = { "이제 나가도 될 것 같다." };

	if (talk_to_superior == 0) Conversation(B1);
	else {
		if (mini4_finish) Conversation(stop);
		else {
			Conversation(B2);
			talk_to_superior = 2;
		}
	}
}

//용의자 C대사 출력
void suspectC()
{
	char C1[1][51] = { "사람이 있다." };
	char C2[1][51] = { "C: A, E 중 1명이 범인일 겁니다. 확실해요." };
	char stop[1][51] = { "이제 나가도 될 것 같다." };

	if (talk_to_superior == 0) Conversation(C1);
	else {
		if (mini4_finish) Conversation(stop);
		else {
			Conversation(C2);
			talk_to_superior = 2;
		}
	}
}

//용의자 D대사 출력
void suspectD()
{
	char D1[1][51] = { "사람이 있다." };
	char D2[1][51] = { "D: B는 거짓말을 하고 있어요." };
	char stop[1][51] = { "이제 나가도 될 것 같다." };

	if (talk_to_superior == 0) Conversation(D1);
	else {
		if (mini4_finish) Conversation(stop);
		else {
			Conversation(D2);
			talk_to_superior = 2;
		}
	}
}

//용의자 E대사 출력
void suspectE()
{
	char E1[1][51] = { "사람이 있다." };
	char E2[1][51] = { "E: 저는 아무것도 모릅니다.전 범인이 아니에요!" };
	char stop[1][51] = { "이제 나가도 될 것 같다." };

	if (talk_to_superior == 0) Conversation(E1);
	else {
		if (mini4_finish) Conversation(stop);
		else {
			Conversation(E2);
			talk_to_superior = 2;
		}
	}
}

//문 안열림 출력
void mini_lock_door()
{
	//4번방 문 좌표: 48,17   50,17  - 가로
	//5번방 문 좌표: 70,10   70,11  - 세로
	char lock[1][51] = { "하까나이" };
	int key;
	int i;
	if (currentRoomNumber == 4) {
		if (((player.X == 48 && player.Y == 16) || (player.X == 50 && player.Y == 16)) && (up == 1)) {
			if (!mini4_finish) { //한번 들어오면 못나가게 하기
				if (!mini4_come) { //한번 잠갔으면 다음에는 출력 안할 수 있도록
					SetCurrentCursorPos(48, 17);
					for (i = 0; i < 4; i++) {
						printf("=");
						Sleep(500);
					}
					Map[4][13][18] = 1;
					Map[4][13][17] = 1;

					mini4_come = 1;
					SetCurrentCursorPos(0, 0);
				}
				//안열린다고 출력이 안됨. 추후 수정하기!!
				key = _getch();
				if (key == DOWN)
					Conversation(lock);
			}
		}
		if (((player.X == 48 && player.Y == 16) || (player.X == 50 && player.Y == 16)) && (down == 1)){
			if (!mini4_finish) {
				key = _getch();
				if (key == DOWN)
					Conversation(lock);
			}
		}
		if (mini4_finish) {
			if (!mini4_disappear) {
				SetCurrentCursorPos(48, 17);
				Sleep(200);
				for (i = 0; i < 4; i++) {
					printf(" ");
					Sleep(500);
				}
				Map[4][13][18] = 0;
				Map[4][13][17] = 0;
				mini4_disappear = 1;
			}
		}
	}
}

//타이머
void timer()
{
}

//5번방 미니게임 함수
void MiniGame5()
{
}
//*****************************************************************************************************************************

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
	item = item - 2;
	MovableItem[currentRoomNumber][item] = 1;
	insertNode(currentRoomNumber, item);
	DeleteList();
	PrintList();
}

void UseItem(int M_item, int Im_item) {
	LN* p;
	MovableItem[currentRoomNumber][M_item - 2] = 2;
	ImmovableItem[currentRoomNumber][Im_item - 2] = 1;
	p = searchNode(currentRoomNumber, M_item - 2);
	if (p == NULL) {
		printf("None");
	}
	else {
		DeleteNode(p);
	}
	DeleteList();
	PrintList();
}

void CollectCheck() {
	LN *p1 = NULL, *p2 = NULL, *p3 = NULL;
	if (currentRoomNumber == 1 && MovableItem[1][1] == 1 && MovableItem[1][2] == 1 && MovableItem[1][4] == 0) {
		p1 = searchNode(1, 1);
		if (p1 != NULL) {
			DeleteNode(p1);
		}
		p2 = searchNode(1, 2);
		if (p2 != NULL) {
			DeleteNode(p2);
		}
		MovableItem[currentRoomNumber][4] = 1;
		insertNode(currentRoomNumber, 4);
	}
	DeleteList();
	PrintList();
}

void Item_Dis() {
	COORD p;
	LN* lp = NULL;
	int i, j;
	p = GetCurrentCursorPos();
	if (currentRoomNumber == 0) {
		if (((p.X == 40 && p.Y == 8) || (p.X == 38 && p.Y == 9)) && MovableItem[currentRoomNumber][0] == 0) {
			SetCurrentCursorPos(36, 8);
			printf("  ");
			for (i = 0; i < 17; i++) {
				for (j = 0; j < 36; j++) {
					if (Map[currentRoomNumber][i][j] == 3) {
						Map[currentRoomNumber][i][j] = 0;
						DrawGameUI(Map);
					}
				}
			}
			insertNode(currentRoomNumber, 0);
			MovableItem[currentRoomNumber][0] = 1;

			PrintList();
			//대사출력
		}
		else if (((p.X == 52 && p.Y == 11) || (p.X == 50 && p.Y == 12) || (p.X == 54 && p.Y == 12) || (p.X == 52 && p.Y == 13)) && MovableItem[currentRoomNumber][0] == 1 && ImmovableItem[currentRoomNumber][0] == 0) {
			MovableItem[currentRoomNumber][0] = 2;
			ImmovableItem[currentRoomNumber][0] = 1;

			lp = searchNode(currentRoomNumber, 0);
			if (lp != NULL) {
				DeleteNode(lp);
				insertNode(currentRoomNumber, 1);
				MovableItem[currentRoomNumber][1] = 1;
			}
			DeleteList();
			PrintList();
			lp = NULL;
		}
	}
	else if (currentRoomNumber == 1) {
		//티켓조각
		if (((p.X == 52 && p.Y == 14) || (p.X == 50 && p.Y == 15) || (p.X == 54 && p.Y == 15) || (p.X == 52 && p.Y == 16)) && MovableItem[currentRoomNumber][0] == 0) {
			GetItem(2);
			SetCurrentCursorPos(50, 15);
			printf("  ");
		}
		// 연필
		else if (((p.X == 60 && p.Y == 7) || (p.X == 60 && p.Y == 9) || (p.X == 58 && p.Y == 8) || (p.X == 62 && p.Y == 8)) && MovableItem[currentRoomNumber][1] == 0) {
			GetItem(3);
			CollectCheck();
			SetCurrentCursorPos(58, 8);
			printf("  ");
		}
		// 메모장
		else if (((p.X == 66 && p.Y == 7) || (p.X == 64 && p.Y == 8) || (p.X == 68 && p.Y == 8) || (p.X == 66 && p.Y == 9)) && MovableItem[currentRoomNumber][2] == 0) {
			GetItem(4);
			CollectCheck();
			SetCurrentCursorPos(64, 8);
			printf("  ");
		}
		// 책상 서랍
		else if (((p.X == 40 && p.Y == 7) || (p.X == 38 && p.Y == 8) || (p.X == 42 && p.Y == 8) || (p.X == 40 && p.Y == 9)) && MovableItem[currentRoomNumber][3] == 0 && MovableItem[currentRoomNumber][4] == 1 && ImmovableItem[currentRoomNumber][1] == 0) {
			UseItem(6, 13);
			GetItem(5);
		}
	}
	else if (currentRoomNumber == 2) {
		printf("%d %d", p.X, p.Y);
	}

	else if	(currentRoomNumber == 4) {
		//printf("%d %d", p.X, p.Y);
		//X, Y 순으로
		//superior(상사): 62,12
		//gun(총): 62 14
		//A: 42,9	B: 46,9	  C: 50,9	 D: 54,9	E: 58,9
		if (left) {
			if ((p.X - 2 == 42) && (p.Y == 9)) suspectA();
			if ((p.X - 2 == 46) && (p.Y == 9)) suspectB();
			if ((p.X - 2 == 50) && (p.Y == 9)) suspectC();
			if ((p.X - 2 == 54) && (p.Y == 9)) suspectD();
			if ((p.X - 2 == 58) && (p.Y == 9)) suspectE();
			if ((p.X - 2 == 62) && (p.Y == 12)) superior();
			if ((p.X - 2 == 62) && (p.Y == 14)) gun();
		}
		if (right) {
			if ((p.X + 2 == 42) && (p.Y == 9)) suspectA();
			if ((p.X + 2 == 46) && (p.Y == 9)) suspectB();
			if ((p.X + 2 == 50) && (p.Y == 9)) suspectC();
			if ((p.X + 2 == 54) && (p.Y == 9)) suspectD();
			if ((p.X + 2 == 58) && (p.Y == 9)) suspectE();
			if ((p.X + 2 == 62) && (p.Y == 12)) superior();
			if ((p.X + 2 == 62) && (p.Y == 14)) gun();
		}
		if (up) {
			if ((p.X == 42) && (p.Y - 1 == 9)) suspectA();
			if ((p.X == 46) && (p.Y - 1 == 9)) suspectB();
			if ((p.X == 50) && (p.Y - 1 == 9)) suspectC();
			if ((p.X == 54) && (p.Y - 1 == 9)) suspectD();
			if ((p.X == 58) && (p.Y - 1 == 9)) suspectE();
			if ((p.X == 62) && (p.Y - 1 == 12)) superior();
			if ((p.X == 62) && (p.Y - 1 == 14)) gun();
		}
		if (down) {
			if ((p.X == 42) && (p.Y + 1 == 9)) suspectA();
			if ((p.X == 46) && (p.Y + 1 == 9)) suspectB();
			if ((p.X == 50) && (p.Y + 1 == 9)) suspectC();
			if ((p.X == 54) && (p.Y + 1 == 9)) suspectD();
			if ((p.X == 58) && (p.Y + 1 == 9)) suspectE();
			if ((p.X == 62) && (p.Y + 1 == 12)) superior();
			if ((p.X == 62) && (p.Y + 1 == 14)) gun();
		}
	}
	else if (currentRoomNumber == 5) {
		printf("%d %d", p.X, p.Y);
	}
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
			left = 1;
			right = 0;
			up = 0;
			down = 0;
			break;
		case RIGHT:
			ShiftRight();
			left = 0;
			right = 1;
			up = 0;
			down = 0;
			break;
		case UP:
			ShiftUp();
			left = 0;
			right = 0;
			up = 1;
			down = 0;
			break;
		case DOWN:
			ShiftDown();
			left = 0;
			right = 0;
			up = 0;
			down = 1;
			break;
			//New
		case SPACEBAR:
			Item_Dis();
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
	currentRoomNumber = 0; //현재 방 번호
	DrawGameUI(Map);
	SetCurrentCursorPos(player.X, player.Y);
	printf("▲");
	Interface();
	L = create_Linklist_h();

	while (1) {
		if (currentRoomNumber != 7) Check_exit_door();
		if (currentRoomNumber == 7 || currentRoomNumber == 10) Check_entrance_door();
		if (currentRoomNumber == 4) mini_lock_door();
		ProcessKeyInput();
	}
	return 0;
}