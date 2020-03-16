#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <mmsystem.h>
#include <time.h>
#include <process.h>
#include "Map.h" //맵
#include "door.h"//각 방의 문 좌표
#include "item.h"//아이템 정보 
#include "startgame.h"//game start 화면
#include "sound.h"//game 내 sound 파일
#include "colorPalette.h" //색 삽입
#include "Message.h" //대사

#pragma once
#define UP 119 //W
#define LEFT 97 //A
#define DOWN 115 //S
#define RIGHT 100 //D
#define SPACEBAR 32
#define bp 80 //대문자 P
#define sp 112 //소문자 p
#define by 89 //대문자 Y
#define sy 121 //소문자 y
#define bn 78 //대문자 N
#define sn 110 //소문자 n

//Size of gameboard,itemBoard,messageBoard
#define GBOARD_WIDTH 38
#define GBOARD_HEIGHT 19
#define IBOARD_WIDTH 10
#define IBOARD_HEIGHT 26
#define MBOARD_WIDTH 38
#define MBOARD_HEIGHT 6
#define ROOM2_WIDTH 30
#define ROOM2_HEIGHT 17
#define START_WIDTH 48
#define START_HEIGHT 25

//Starting point of gameboard, itemBoard, messageBoard
#define GBOARD_ORIGIN_X 5
#define GBOARD_ORIGIN_Y 2
#define IBOARD_ORIGIN_X 84
#define IBOARD_ORIGIN_Y 2
#define MBOARD_ORIGIN_X 5
#define MBOARD_ORIGIN_Y 22
#define START_ORIGIN_X 21
#define START_ORIGIN_Y 9

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];		//인터페이스 중 게임 실행창에 해당되는 부분
int itemBoardInfo[IBOARD_HEIGHT + 1][IBOARD_WIDTH + 2];
int messageBoardInfo[MBOARD_HEIGHT + 1][MBOARD_WIDTH + 2];

COORD player = { 46, 13 }; //x좌표 짝수로 설정(홀수는 충돌체크 씹힘)
int currentRoomNumber = 7; //현재 플레이어가 있는 방 번호
int mapSize[10][2]; //각 방의 가로,세로 크기 저장
int allign = 5;//맵 정렬 변수
int diff = 4;//9-5
//기존 작업에서 일하려면 allign=9, diff=0으로 수정해서 작업할 것
//allign 무조건 홀수여야 함 이유는 모르겠음
int can_Entrance_Check[11] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; //각 방으로 들어가기 위한 조건을 만족했는지 검사
int can_Entrance[11] = { 1, 1, 3, 1, 0, 2, 1, 0, 0, 2, 3 }; //각 방으로 들어갈 수 있는 조건!
int center[16] = { 2, 2, 4, 4, 2, 4, 10, 8, 8, 6, 4, 10, 6, 4, 8, 13 };

//가장 최근에 어디 방향 눌렀는지 저장
int left = 0;
int right = 0;
int up = 0;
int down = 0;

//0번방에서 플레이어의 움직임을 확인
int zerocnt = 0, CollectCntTicket;

//1번방 총성 후 복도로 나왔을 때 대사출력
int Shot_Check = 0;

//1번방 숫자퀴즈 변수**********
int random1; //랜덤 숫자 생성
int sequence[4] = { 0 }; //순서 저장
int quest1[4] = { 0 }; //문제 저장
char answer1[4] = ""; //답 저장
char input1[4] = ""; //플레이어 답 입력
int onecnt = 0; //숫자 4개 들어가는지 확인
int judge = 0; //숫자중복 확인용
int q1_random = 0; //퀴즈 랜덤숫자 한번만 돌아가게 설정
int CollectCnt1 = 0; //연필과 메모장을 획득했는지 확인하는 CollectCnt1
int CollectCnt3 = 0;
int trigger = 0;

//2번방 암호학퀴즈 변수********
int random2; //랜덤 숫자 생성
char example2[] = "iloveu"; //예시문장
char example2_ceaser[10] = ""; //예시문장 암호로 변경
char answer2[] = "police"; //2번방 정답
char answer2_ceaser[10] = ""; //정답 암호로 변경
char input2[10] = ""; //플레이어 답 입력
int q2_random = 0; //퀴즈 랜덤 시저번호 한번만 돌아가도록 설정
int SecondQuizSolve = 0; //2번방퀴즈를 풀었는지 확인

//4번방 미니게임(범인찾기) 변수**********
int talk_to_superior = 0; //상사와 대화 했는지 확인(이전과 이후 대화창 출력 다르게)
int mini4_finish = 0; //4번방 게임 통과 여부 확인
int mini4_door = 0; //4번방 들어가면 문 잠그기
int mini4_come = 0; //4번방에 완전히 들어왔는지 확인
int mini4_print = 0; //들어갈 수 없다 출력했는지 확인
int mini4_disappear = 0; //문잠금 삭제했는지 확인
int mini4_die = 0; //4번방에서 죽었는지 확인
int LifeLimit = 2; //목숨
int mini4_load = 0; //4번방 앞으로 로드하기

//5번방+8번방 미니게임(타임어택) 변수******
int mini5_finish = 0; //5번방 게임 통과 여부 확인
int mini5_door = 0; //5번방 들어가면 문 잠그기
int mini5_come = 0; //5번방에 완전히 들어왔는지 확인
int mini5_print = 0; //들어갈 수 없다 출력했는지 확인
int mini5_disappear = 0; //문잠금 삭제했는지 확인
int mini5_start = 0; //미니게임 시작 전 열쇠 주우면 1로 변경
int mini5_end = 0; //미니게임 중 열쇠 획득하면 1로 변경
int mini5_die = 0; //8번방에서 죽었는지 확인
int times = 20; //타임어택 시간(15초)
int time_start = 0; //타임어택 시간초 시작
int mini5_key = 0; //랜덤 키 생성
int mini5_notcome = 0; //게임클리어 이후 다시 8번방 진입하지 못하도록
int mini5_load = 0; //5번방 앞으로 로드하기
int clock_check = 0; //시간 확인
int clock_reset = 0; //시간 초기화
int mini5_check = 0; //잠근거 다시 확인
COORD save; //커서위치 일시저장(시간 내려가는거 때문에)
clock_t start; //시간 언제부터 셀지

//6번방 변수******************************
int lever = 1; //컴퓨터 코드 꽂으면 1로 변경

//9번방 최종문제 변수**********************
char answer9[] = "worros"; //최종답(논의하기)
char input9[10] = ""; //플레이어 답 입력
int finish = 0; //정답 맞추면 1, 아니면 0
int check_lever = 0; //레버 내렸는가?

//순환되어버리는 함수는 여기로 선언********
void DrawGameOverDisplay();
//*****************************************

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
		if (pre == p) {
			pre = p->link;
			free(p);
			L->head = pre;
		}
		else {
			while (pre->link != p) {
				pre = pre->link;
			}
			pre->link = p->link;
			free(p);
		}
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

	if (currentRoomNumber == 11)
	{
		if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - allign) / 2] == 11) {
			return 1;
		}
		else if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - allign) / 2] != 0) {
			return 0;
		}

		return 1;
	}
	else {
		if (Map[currentRoomNumber][posY - GBOARD_ORIGIN_Y - 2][(posX - GBOARD_ORIGIN_X - allign) / 2] != 0) {
			return 0;
		}

		return 1;
	}
}

void PrintList() {
	LN* p;
	COORD q;
	int i;
	p = L->head;
	SetCurrentCursorPos(86, 4);
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
		for (j = IBOARD_ORIGIN_Y + 2; j < IBOARD_ORIGIN_Y + IBOARD_HEIGHT; j++) {
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
	SetCurrentCursorPos(88, 3);
	printf("《 Item  List 》");
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
	/*SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
	//printf("Hello");
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2);
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

void SoundTrack(int roomNumber)
{
	if (roomNumber == 1)
	{
		if (trigger == 1) {
			shotBGM();
			trigger = 0;
		}
		else stageBGM();
	}
	else if (roomNumber == 2 || roomNumber == 3 || roomNumber == 5 || roomNumber == 7 || roomNumber == 10 || roomNumber == 11)
	{
		stageBGM();
	}
	else if (roomNumber == 6)
	{
		illusionBGM();
	}
	else if (roomNumber == 4 || roomNumber == 1)
	{
		stopBGM();
	}
	else if (roomNumber == 8)
	{
		timeBGM();
	}
}

void DrawRoomFrame(char Map[][17][36], int x, int y)
{
	//x=i, y=j
	if (Map[currentRoomNumber][y][x + 1] == 1) {
		if (Map[currentRoomNumber][y][x - 1] == 1)
			printf("─");
		else if (Map[currentRoomNumber][y + 1][x] == 1)
			printf("┌");
		else if (Map[currentRoomNumber][y - 1][x] == 1)
			printf("└");
		else if (Map[currentRoomNumber][y][x - 1] == 0 || Map[currentRoomNumber][y][x - 1] == 9)
			printf("─");
	}
	else if (Map[currentRoomNumber][y - 1][x] == 1 && Map[currentRoomNumber][y + 1][x] == 1)
		printf("│");
	else if (Map[currentRoomNumber][y][x - 1] == 1) {
		if (Map[currentRoomNumber][y + 1][x] == 1)
			printf("┐");
		else if (Map[currentRoomNumber][y - 1][x] == 1)
			printf("┘");
		else if (Map[currentRoomNumber][y][x + 1] == 0 || Map[currentRoomNumber][y][x + 1] == 9)
			printf("─");
	}
	else if (Map[currentRoomNumber][y - 1][x] == 0 && Map[currentRoomNumber][y + 1][x] == 1)
		printf("│");
	else if (Map[currentRoomNumber][y - 1][x] == 1 && Map[currentRoomNumber][y + 1][x] == 0)
		printf("│");
	else printf("■");
}

void Conversation(char talk[][300], int row);

void DrawRoomNumber()
{
	if (currentRoomNumber == 7 || currentRoomNumber == 11) {
		SetCurrentCursorPos(28, 7);
		printf("Room 1");
		SetCurrentCursorPos(28, 11);
		printf("Room 2");
		SetCurrentCursorPos(28, 16);
		printf("Room 3");
		SetCurrentCursorPos(58, 9);
		printf("Room ?");
		SetCurrentCursorPos(58, 16);
		printf("Room 6");
	}
	if (currentRoomNumber == 10) {
		SetCurrentCursorPos(30, 11);
		printf("Room 4");
		SetCurrentCursorPos(55, 11);
		printf("Room 5");
	}
}

void DrawGameUI(char Map[][17][36])
{
	SoundTrack(currentRoomNumber);
	int i, j;
	for (i = 0; i < 36; i++) {
		for (j = 0; j < 17; j++) {
			if (currentRoomNumber == 0)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▣");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("♀");
				}
			}
			if (currentRoomNumber == 1)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("⊙");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}
				if (CollectCnt1 == 1 && Map[currentRoomNumber][j][i] == 16) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▒");
				}

				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					Yellow();
					printf("★");
					Gray();
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}
			if (currentRoomNumber == 2)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 24) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Ω");
				}
				if (Map[currentRoomNumber][j][i] == 22) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("∮");
				}
				if (Map[currentRoomNumber][j][i] == 23) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("￥");
				}
				if (Map[currentRoomNumber][j][i] == 4 && SecondQuizSolve == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					Yellow();
					printf("★");
					Gray();
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}
			if (currentRoomNumber == 3)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┼");
				}
				if (Map[currentRoomNumber][j][i] == 76) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}
				if (Map[currentRoomNumber][j][i] == 7 || Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("○");
				}
				if (Map[currentRoomNumber][j][i] == 3 || Map[currentRoomNumber][j][i] == 4 || Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("ㆀ");
				}
			}
			if (currentRoomNumber == 4) //갈래방
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 42) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Γ");
				}
				if (Map[currentRoomNumber][j][i] == 43) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("●");
				}
				if (Map[currentRoomNumber][j][i] == 44) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 45) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 46) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 47) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 48) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
			}
			if (currentRoomNumber == 5)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 54) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 55) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 52) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					Red();
					printf("ℓ");
					Gray();
				}
				if (Map[currentRoomNumber][j][i] == 53) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DarkRed();
					printf("Ф");
					Gray();
				}
				if (Map[currentRoomNumber][j][i] == 56) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 57) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}
			if (currentRoomNumber == 6)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 63) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("↖");
				}
				if (Map[currentRoomNumber][j][i] == 62) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					Red();
					printf("☎");
					Gray();
				}
			}
			if (currentRoomNumber == 7)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("├");
				}
				if (Map[currentRoomNumber][j][i] == 9) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┤");
				}
				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}

			}
			if (currentRoomNumber == 8)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 80 || Map[currentRoomNumber][j][i] == 81 || Map[currentRoomNumber][j][i] == 82 || Map[currentRoomNumber][j][i] == 83 || Map[currentRoomNumber][j][i] == 84 || Map[currentRoomNumber][j][i] == 85 || Map[currentRoomNumber][j][i] == 86 || Map[currentRoomNumber][j][i] == 87 || Map[currentRoomNumber][j][i] == 88 || Map[currentRoomNumber][j][i] == 89) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◎");
				}
			}
			if (currentRoomNumber == 9)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 92) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 93) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 94) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 95) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					Yellow();
					printf("★");
					Gray();
				}

				if (Map[currentRoomNumber][j][i] == 70) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 71) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 72) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 73) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 74) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 75) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}
			if (currentRoomNumber == 10) //갈래방
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 9) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}

			}
			if (currentRoomNumber == 11) {
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("├");
				}
				if (Map[currentRoomNumber][j][i] == 9) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┤");
				}
				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + allign + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DarkRed();
					printf("♣");
					Gray();
				}
				DrawRoomNumber();
			}
		}
	}
}

int cannot_Move(int roomNum, int which) {
	if (which == 0) {
		//밖에서 안으로 들어가려 할 때
		if (roomNum == 1 || roomNum == 2 || roomNum == 3) {
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			player.X += 2;
			SetCurrentCursorPos(player.X, player.Y);
			printf("◀");
		}
		else if (roomNum == 10 || roomNum == 6) {
			SetCurrentCursorPos(player.X, player.Y);
			printf("  ");
			player.X -= 2;
			SetCurrentCursorPos(player.X, player.Y);
			printf("▶");
		}
	}

	else if (which == 1) {
		//방에서 나가려는 시도를 제한하고 싶을 때
		if (roomNum == 0) {
			if (can_Entrance_Check[1] == can_Entrance[1]) { return 1; }
			else { return -1; }
		}
	}
}

void timer()
{
	SetCurrentCursorPos(10, 10);
	printf("      ");
	SetCurrentCursorPos(10, 10);
	if (times - (clock() - start) / CLOCKS_PER_SEC < 0) {
		mini5_die = 1;
		DrawGameOverDisplay();
	}
	else printf("00:%02d", times - (clock() - start) / CLOCKS_PER_SEC);
}

//1초마다 키보드 입력되는지 확인
int sleep_input(int n)
{
	int second = n * 1000;
	while (kbhit() == 0) {
		if (second <= 0) return -1;
		Sleep(10);
		second -= 100;
	}
	return getch();
}

//대화UI 출력함수 //대화 중에는 움직일 수 x //쓰고+지우기 //인자: 대화출력문장, 몇번째 줄에 쓸건지(한문장만 있으면 1 입력, 한번에 여러줄에 출력 시 총 몇 줄인지 입력)
void Conversation(char talk[][300], int row)
{
	int i = 0, next_row = 1 + row - 1;
	float cnt = 0;
	int key;
	for (i = 0; talk[0][i]; i++) {
		if (cnt == 0) {
			if (talk[0][i] == ' ') continue;
			else SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
		}
		if (talk[0][i] == '^') {
			next_row++;
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			cnt = 0;
		}
		else {
			printf("%c", talk[0][i]);
			//Sleep(40);
		}
		if (talk[0][i] == ' ') cnt += 0.5;
		else cnt++;
		if (cnt >= 70) {
			next_row++;
			cnt = 0;
		}
	}
	while (1) {
		next_row = 1, cnt = 0;
		while (1) {
			key = sleep_input(1);
			if (currentRoomNumber == 8) timer();
			if (key != -1) break;
		}
		if (key == SPACEBAR) {
			for (i = 0; talk[0][i]; i++) {
				if (cnt == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				if (talk[0][i] == '^') {
					next_row++;
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
					cnt = 0;
				}
				else {
					printf(" ", talk[0][i]);
				}
				if (talk[0][i] == ' ') cnt += 0.5;
				else cnt++;
				if (cnt >= 70) {
					next_row++;
					cnt = 0;
				}
			}
			break;
		}
	}
}

void DeleteMap(int nextRoom) { //GBOARD에 출력된 내용을 전부 지운다.
	int i, j;
	COORD tmp;
	char* playerICON = "▼";

	if (nextRoom == 7) {
		//각 방에서 복도로 이동
		tmp.X = entrance_door[currentRoomNumber][0] - diff;
		tmp.Y = entrance_door[currentRoomNumber][1];

		if (currentRoomNumber == 1 || currentRoomNumber == 2 || currentRoomNumber == 3) {
			if (currentRoomNumber == 1 && can_Entrance_Check[2] == 2) { nextRoom = 11; can_Entrance_Check[2] += 1; }
			tmp.X += 2;
			playerICON = "▶";
		}
		if (currentRoomNumber == 10 || currentRoomNumber == 6) {
			tmp.X -= 2;
			playerICON = "◀";
		}

		if (currentRoomNumber == 0) {
			if (cannot_Move(currentRoomNumber, 1) != 1) {
				SetCurrentCursorPos(player.X, player.Y);
				printf("  ");
				player.Y += 1;
				SetCurrentCursorPos(player.X, player.Y);
				printf("▲");
				return;
			}

			else {
				tmp.Y += 1;
				playerICON = "▼";
			}
		}
	}
	else if (nextRoom == 10) {
		if (currentRoomNumber == 7) {
			//7번방에서 갈래길(10번방)로 이동
			tmp.X = exit_door[nextRoom][0] - diff;
			tmp.Y = exit_door[nextRoom][1];
			tmp.Y -= 1;
			playerICON = "▲";
		}
		else if (currentRoomNumber == 4 || currentRoomNumber == 5 || currentRoomNumber == 8) {
			//4, 5번방에서 갈래길(10번방)로 이동
			tmp.X = entrance_door[currentRoomNumber][0] - diff;
			tmp.Y = entrance_door[currentRoomNumber][1];
			if (currentRoomNumber == 4) {
				tmp.X += 2;
				playerICON = "▶";
			}
			if (currentRoomNumber == 5 || currentRoomNumber == 8) {
				if (!mini5_end && mini5_start) nextRoom = 8;
				tmp.X -= 2;
				playerICON = "◀";
			}
		}
	}

	else if (currentRoomNumber == 5 && nextRoom == 8) {
		//5번방에서 미니게임 실행
		tmp.X = player.X;
		tmp.Y = player.Y;
	}

	else {
		//복도에서 각 방으로 이동
		tmp.X = exit_door[nextRoom][0] - diff;
		tmp.Y = exit_door[nextRoom][1];

		if (nextRoom == 0 || nextRoom == 2 || nextRoom == 3) {
			tmp.Y += 1;
			playerICON = "▼";
			if (nextRoom == 2) {
				if (searchNode(1, 3) != NULL) DeleteNode(searchNode(1, 3));
				DeleteList();
				PrintList();
			}
			if (nextRoom == 3) {
				if (searchNode(2, 2) != NULL) DeleteNode(searchNode(2, 2));
				DeleteList();
				PrintList();
			}
		}
		if (nextRoom == 4 || nextRoom == 6 || nextRoom == 10) {
			tmp.Y -= 1;
			playerICON = "▲";
			if (nextRoom == 4) {
				if (searchNode(3, 5) != NULL) DeleteNode(searchNode(3, 5));
				DeleteList();
				PrintList();
			}
			if (nextRoom == 6) {
				if (searchNode(5, 0) != NULL) DeleteNode(searchNode(5, 0));
				DeleteList();
				PrintList();
			}
		}
		if (nextRoom == 1 || nextRoom == 5) {
			if (nextRoom == 1) {
				if (searchNode(0, 1) != NULL) DeleteNode(searchNode(0, 1));
				DeleteList();
				PrintList();
			}
			tmp.X -= 2;
			playerICON = "◀";
		}
		if (nextRoom == 9) {
			tmp.X += 2;
			playerICON = "▶";
		}
	}

	for (i = GBOARD_ORIGIN_X + 2; i < GBOARD_ORIGIN_X + 2 + GBOARD_WIDTH * 2; i++) {
		for (j = GBOARD_ORIGIN_Y + 1; j < GBOARD_ORIGIN_Y + GBOARD_HEIGHT; j++) {
			SetCurrentCursorPos(i, j);
			printf(" ");
		}
	}

	currentRoomNumber = nextRoom;
	DrawGameUI(Map);
	player.X = tmp.X;
	player.Y = tmp.Y;
	SetCurrentCursorPos(player.X, player.Y);
	if (currentRoomNumber == 8) { //타임어택 미니게임 진입 시 이전에 방향 보던 곳으로 출력
		if (left) printf("◀");
		if (right) printf("▶");
		if (up) printf("▲");
		if (down) printf("▼");
	}
	else printf("%s", playerICON);
	if (currentRoomNumber == 11 && Shot_Check == 1){
		Conversation(Hallway[7], 1);
		Shot_Check = 2;
	}
	DeleteList();
	PrintList();
}

void Check_exit_door() { //현재 플레이어의 위치가 방에서 나가는 좌표인지 검사

	if ((player.X == exit_door[currentRoomNumber][0] - diff && player.Y == exit_door[currentRoomNumber][1]) ||
		(player.X == exit_door[currentRoomNumber][2] - diff && player.Y == exit_door[currentRoomNumber][3]))
	{
		if (currentRoomNumber == 4 || currentRoomNumber == 5 || currentRoomNumber == 8) DeleteMap(10);
		else DeleteMap(7); //복도에서의 좌표를 설정해 줌
	}

}

void Check_entrance_door() { //현재 플레이어의 위치가 방으로 들어가는 좌표인지 검사
	int room_num[5] = { 1, 2, 3, 10, 6 }; //복도에서 들어갈 수 있는 방 번호
	int i;

	if (currentRoomNumber == 10) {
		if ((player.X == entrance_door[4][0] - diff && player.Y == entrance_door[4][1]) ||
			(player.X == entrance_door[4][2] - diff && player.Y == entrance_door[4][3])) {
			if (MovableItem[3][5] == 1) { //10번방 들어갈 때
				Conversation(Hallway[4], 1); //문 열고 들어갈 때 대사 출력
				MovableItem[3][5] = 2; //극장티켓 사용 상태로 변경
			}
			DeleteMap(4); //들어갈 방의 좌표를 설정해줌
		}
		if ((player.X == entrance_door[5][0] - diff && player.Y == entrance_door[5][1]) ||
			(player.X == entrance_door[5][2] - diff && player.Y == entrance_door[5][3])) {
			DeleteMap(5); //들어갈 방의 좌표를 설정해줌
		}
	}
	else {
		for (i = 0; i < 5; i++) {
			if ((player.X == entrance_door[room_num[i]][0] - diff && player.Y == entrance_door[room_num[i]][1]) ||
				(player.X == entrance_door[room_num[i]][2] - diff && player.Y == entrance_door[room_num[i]][3])) {
				if (can_Entrance_Check[room_num[i]] == can_Entrance[room_num[i]]) {
					//나갔다가 다시 들어올 때 대사 출력 안 되게 하려고 나눔
					if ((room_num[i] == 1) && (MovableItem[0][1] == 1)) { //1번방 들어갈 때
						Conversation(Hallway[room_num[i]], 1); //문 열고 들어갈 때 대사 출력
						MovableItem[0][1] = 2; //1번방 열쇠 사용 상태로 변경
					}
					else if ((room_num[i] == 1) && (can_Entrance[9] == can_Entrance_Check[9])) { //다 깨고 9번방 들어갈 때         
						DeleteMap(9);
						break;
					}
					else if ((room_num[i] == 2) && (MovableItem[1][3] == 1)) { //2번방 들어갈 때
						Conversation(Hallway[room_num[i]], 1); //문 열고 들어갈 때 대사 출력
						MovableItem[1][3] = 2; //2번방 열쇠 사용 상태로 변경
					}
					else if ((room_num[i] == 3) && (MovableItem[2][2] == 1)) { //3번방 들어갈 때
						Conversation(Hallway[room_num[i]], 1); //문 열고 들어갈 때 대사 출력
						MovableItem[2][2] = 2; //2번방 열쇠 사용 상태로 변경
					}
					else if ((room_num[i] == 6) && (MovableItem[5][0] == 1)) { //6번방 들어갈 때
						Conversation(Hallway[room_num[i]], 1); //문 열고 들어갈 때 대사 출력
						MovableItem[5][0] = 2; //6번방열쇠 사용 상태로 변경
					}
					DeleteMap(room_num[i]); //들어갈 방의 좌표를 설정해줌
					break;
				}
				else {
					//못들어가게 막아야 함. 들어갈 수 없다는 메시지도 출력!
					Conversation(Hallway[0], 1);
					cannot_Move(room_num[i], 0);
					break;
				}
			}
		}
	}
}

void ShiftLeft()
{
	if (!DetectCollision(player.X - 2, player.Y, Map)) {
		if (Map[currentRoomNumber][player.Y - GBOARD_ORIGIN_Y - 2][(player.X - 2 - GBOARD_ORIGIN_X - allign) / 2] != 1) {
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
	save = GetCurrentCursorPos();
}

void ShiftRight()
{
	if (!DetectCollision(player.X + 2, player.Y, Map)) {
		if (Map[currentRoomNumber][player.Y - GBOARD_ORIGIN_Y - 2][(player.X + 2 - GBOARD_ORIGIN_X - allign) / 2] != 1) {
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
	save = GetCurrentCursorPos();
}

void ShiftUp()
{
	if (!DetectCollision(player.X, player.Y - 1, Map)) {
		if (Map[currentRoomNumber][player.Y - 1 - GBOARD_ORIGIN_Y - 2][(player.X - GBOARD_ORIGIN_X - allign) / 2] != 1) {
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
	save = GetCurrentCursorPos();
}

void ShiftDown()
{
	if (!DetectCollision(player.X, player.Y + 1, Map)) {
		if (Map[currentRoomNumber][player.Y + 1 - GBOARD_ORIGIN_Y - 2][(player.X - GBOARD_ORIGIN_X - allign) / 2] != 1) {
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
	save = GetCurrentCursorPos();
}

//프롤로그 대사 출력 함수
void Prologue() {
	int i;
	Interface();
	for (i = 0; i < 27; i++) {
		Conversation(prolog[i], 1);
		Interface();
	}
}

//에필로그 부분
void Epilogue_interface()
{
	int x, y;
	for (y = 0; y <= START_HEIGHT; y++){
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┌");
		else if (y == START_HEIGHT) printf("└");
		else printf("│");
	}
	for (y = 0; y <= START_HEIGHT; y++){
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (START_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┐");
		else if (y == START_HEIGHT) printf("┘");
		else printf("│");
	}
	for (x = 1; x < (START_WIDTH + 1) * 2; x++){
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + START_HEIGHT);
		printf("─");
	}
	for (x = 1; x < (START_WIDTH + 1) * 2; x++){
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y);
		printf("─");
	}
}

void Epilogue_delete(int ind, int x, int y)
{
	int i;
	for (i = 0; i < sizeof(epilog[ind]); i++){
		SetCurrentCursorPos(x + i, y);
		printf(" ");
	}
}

void Epilogue()
{
	int x, y;
	int width = 48;
	int centerX;
	int centerY = 13;
	int i = 0;
	int cnt = 0;
	//게임 전체 창

	centerX = width + 3;
	SetCurrentCursorPos(centerX, centerY);
	printf(epilog[0]);
	Epilogue_interface();
	endBGM();
	Sleep(5000); //원래값 5000

	Epilogue_delete(0, centerX, centerY);
	Sleep(200); //원래값 200

	Epilogue_interface();
	centerX = width - 3;
	SetCurrentCursorPos(centerX, centerY);
	printf(epilog[1]);
	Epilogue_interface();
	Sleep(5000); //원래값 5000

	Epilogue_delete(1, centerX, centerY);
	Sleep(200); //원래값 2000


	for (i = 2; i < 16; i++){
		centerX = width - center[i - 2];
		SetCurrentCursorPos(centerX, centerY);
		printf(epilog[i]);
		Epilogue_interface();
		Sleep(5000); //원래값 5000
		Epilogue_delete(i, centerX, centerY);
		Sleep(200); //원래값 2000
	}

	centerX = width - center[15];
	SetCurrentCursorPos(centerX, centerY);
	printf(epilog[16]);
	Epilogue_interface();
	Sleep(2000); //원래값 2000;
	//stopBGM();
	SetCurrentCursorPos(0, 0);
	exit(0);
}

//0번방 대사 출력 함수
void Zero(int a, int b) {
	int i;
	for (i = a; i <= b; i++)
		Conversation(ZeroRoomMessage[i], 1);
}

//0번방의 예외사항 처리 함수
void ZeroRoomKey()
{
	int i, j;
	SetCurrentCursorPos(32, 8);
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
}

void ZeroRoomSafe()
{
	LN* lp = NULL;
	Zero(6, 6);
	MovableItem[currentRoomNumber][0] = 2;
	ImmovableItem[currentRoomNumber][0] = 1;

	lp = searchNode(currentRoomNumber, 0);
	if (lp != NULL) {
		DeleteNode(lp);
		insertNode(currentRoomNumber, 1);
		MovableItem[currentRoomNumber][1] = 1;
		can_Entrance_Check[1] += 1; //혜원 추가
	}
	DeleteList();
	Zero(8, 8);
	PrintList();
	Zero(9, 9);
	lp = NULL;
	Zero(10, 13);
}

//1번방 대사 출력 함수
void First(int a, int b) {
	int i;
	for (i = a; i <= b; i++)
		Conversation(FirstRoomMessage[i], 1);
}

//1번방 퀴즈: 숫자, 순서 랜덤생성
void Quiz1_Random() {
	int i;
	if (!q1_random) {
		srand(time(NULL));
		//랜덤문제 생성
		while (1) {
			random1 = rand() % 10; //0~9까지 랜덤숫자 생성
			for (i = 0; i < onecnt; i++) {
				if (random1 == quest1[i]) { //번호중복 제외
					judge = 1;
				}
			}
			if (judge == 1) judge = 0;
			else {
				quest1[onecnt] = random1;
				onecnt++;
				judge = 0;
			}
			if (onecnt == 4) break;
		}

		//랜덤순서 생성
		onecnt = 0;
		while (1) {
			random1 = rand() % 4; //0~3까지 랜덤숫자 생성
			for (i = 0; i < onecnt; i++) {
				if (random1 == sequence[i]) { //번호중복 제외
					judge = 1;
				}
			}
			if (judge == 1) judge = 0;
			else {
				sequence[onecnt] = random1;
				onecnt++;
				judge = 0;
			}
			if (onecnt == 4) break;
		}

		//정답 설정
		SetCurrentCursorPos(0, 0);
		for (i = 0; i < 4; i++)
			answer1[sequence[i]] = quest1[i] + '0';
		SetCurrentCursorPos(0, 1);
		//printf("%s", answer1);
		q1_random = 1;
	}
}

void Room1_Quiz_Desk() {
	int i, result;
	char Quiz1_Message[2][100] = { "깔끔히 정리된 책상 밑 서랍에 비밀번호를 입력할 수 있는 번호키가 달려 있다.", "정답(네 자리 숫자 입력): " };

	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1); for (i = 0; Quiz1_Message[0][i]; i++) { printf("%c", Quiz1_Message[0][i]); /*Sleep(50);*/ }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2); for (i = 0; Quiz1_Message[1][i]; i++) { printf("%c", Quiz1_Message[1][i]); /*Sleep(50);*/ }

	ShowInputCursor(); //답 입력을 위해 커서 보이도록 설정
	scanf("%s", input1); //답 입력
	RemoveCursor(); //답을 입력했으니 커서를 지우고
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1); for (i = 0; Quiz1_Message[0][i]; i++) { printf(" "); /*Sleep(50);*/ }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2); for (i = 0; Quiz1_Message[1][i]; i++) { printf(" "); /*Sleep(50);*/ }
	printf("      ");

	for (i = 0; i < 4; i++) {
		if (answer1[i] == input1[i]) result = 1;
		else {
			result = 0;
			break;
		}
	}
	if (result) { //정답
		First(14, 14); GetItem(5); First(15, 15); can_Entrance_Check[2] += 1; shotBGM(); First(17, 17); can_Entrance_Check[2] += 1;
		trigger = 1;
		SoundTrack(1);
	}
	else { //오답
		First(16, 16);
	}
	stopBGM();
}

void Room1_Quiz_Number() {
	int next_row = 1, i, key;
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
	for (i = 0; i < 4; i++) { printf("%d", quest1[i]); }
	next_row++;
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
	for (i = 0; i < 4; i++) { printf("%d", sequence[i] + 1); }
	while (1) {
		next_row = 1;
		key = _getch();
		if (key == SPACEBAR) {
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			printf("       ");
			next_row++;
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			printf("       ");
			break;
		}
	}
}

//2번방 대사 출력 함수
void Second(int a, int b)
{
	int i;
	for (i = a; i <= b; i++)
		Conversation(SecondRoomMessage[i], 1);
}

//2번방 퀴즈: 시저암호 랜덤생성
void Quiz2_Random()
{
	int i;
	int len1 = strlen(example2); //예시 문장 길이
	int len2 = strlen(answer2); //답(police) 문장 길이
	if (!q2_random) {
		srand(time(NULL));
		random2 = rand() % 15 + 5; //5 ~ 15 랜덤숫자 생성
		for (i = 0; i < len1; i++) {
			//예시문장 암호화
			example2_ceaser[i] = ((example2[i] - 'a') + random2) % 26;
			example2_ceaser[i] = example2_ceaser[i] + 'a';
		}
		for (i = 0; i < len2; i++) {
			//문제 암호화
			answer2_ceaser[i] = ((answer2[i] - 'a') + random2) % 26;
			answer2_ceaser[i] = answer2_ceaser[i] + 'a';
		}
		//printf("암호(예시):%s\n", example2_ceaser);
		//printf("원래(예시):%s\n",example2);
		//printf("문제:%s\n",answer2_ceaser);
		//printf("답:"%s\n",answer2); //police
		q2_random = 1;
	}
}

void Room2_Quiz_Chart() {
	int i;
	char Quiz2_Message[2][100] = { "다음 알파벳은 특정한 규칙을 가지고 있다.", "위 예시의 규칙에 따라 다음 영단어를 변형하시오." };
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1); for (i = 0; Quiz2_Message[0][i]; i++) { printf("%c", Quiz2_Message[0][i]); /*Sleep(50);*/ }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2); for (i = 0; example2_ceaser[i]; i++) { printf("%c", example2_ceaser[i]); /*Sleep(50);*/ }
	printf(" -"); /*Sleep(50);*/ printf("> "); /*Sleep(50);*/
	for (i = 0; example2[i]; i++) { printf("%c", example2[i]); /*Sleep(50);*/ }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 3); for (i = 0; Quiz2_Message[1][i]; i++) { printf("%c", Quiz2_Message[1][i]); /*Sleep(50);*/ }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 4); for (i = 0; answer2_ceaser[i]; i++) { printf("%c", answer2_ceaser[i]); /*Sleep(50);*/ }
	printf(" -"); /*Sleep(50);*/ printf("> "); /*Sleep(50);*/

	ShowInputCursor();
	scanf("%s", input2);
	RemoveCursor();

	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1); for (i = 0; Quiz2_Message[0][i]; i++) { printf(" "); }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 2); for (i = 0; example2_ceaser[i]; i++) { printf(" "); }
	printf("    ");
	for (i = 0; example2[i]; i++) { printf(" "); }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 3); for (i = 0; Quiz2_Message[1][i]; i++) { printf(" "); }
	SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 4); for (i = 0; answer2_ceaser[i]; i++) { printf(" "); }
	printf("              ");

	if (strcmp(input2, answer2) == 0) { //정답
		Map[2][4][22] = 4;  SecondQuizSolve = 1; DrawGameUI(Map); Second(10, 10); can_Entrance_Check[3] += 1;
	}
	else { //오답
		Second(11, 11);
	}
}

void Third(int a, int b)
{
	int i;
	for (i = a; i <= b; i++)
		Conversation(ThirdRoomMessage[i], 1);
}

//4번방 상사의 대사 출력
void superior()
{
	char superior1[5][301] = { "후배:“아, 선배. 오셨어요?”ː", "“수사 상황은?”", "후배:“지금 용의자들을 추려서 범인을 알아내려고 하는데 만만치가 않아요. 범인이 사람들 사이에 껴서 거짓말을 하고 있는 것 같은데…….”ː", "“내가 찾아보지. 잠깐 쉬고 있어.”ː", "거짓말을 하고 있는 범인을 찾은 뒤, 후배에게 다시 말을 걸어보자. ː" };
	char superior2[1][101] = { "후배:“용의자들에게 말을 걸어 직접 말을 들어보시는 편이 좋을 것 같아요.”ː" };
	char talk[1][105] = { "후배:“범인은 알아내셨어요?”(한 글자로 입력. 대소문자 구분 있음.) : " };
	char input[10] = ""; //플레이어가 답을 입력하면 여기로 받아들이기
	char stop[1][51] = { "후배:“수고하셨습니다!”ː" }; //정답 맞췄으면
	char chk[1][51] = { "수갑을 이용하여 범인을 체포하자 문이 열렸다. ː" };
	char ok[1][30] = { "알맞은 답을 입력했다. ː" };
	char no[1][80] = { "틀린 답을 입력한 것 같다. 다시 한 번 생각해보자. ː" };
	int i = 0, next_row = 1, cnt = 0;

	if (talk_to_superior == 0) {
		for (i = 0; i < 5; i++)
			Conversation(superior1[i], 1);
		talk_to_superior = 1;
		SetCurrentCursorPos(72, 4);
		White();
		printf("Life: ");
		Red();
		printf("♥♥");
		Gray();
	}
	else if (talk_to_superior == 1) Conversation(superior2, 1);
	else {
		if (!mini4_finish){
			for (i = 0; talk[0][i]; i++) { //문장 입력
				if (cnt == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				printf("%c", talk[0][i]);
				//Sleep(50); 느려서 빠르게 나오도록 잠깐 설정
				cnt++;
				if (cnt >= 80) {
					next_row++;
					cnt = 0;
				}
			}
			ShowInputCursor(); //답 입력을 위해 커서 보이도록 설정
			scanf("%s", input); //답 입력
			RemoveCursor(); //답을 입력했으니 커서를 지우고
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
			next_row = 1, cnt = 0;
			for (i = 0; talk[0][i]; i++) { //일단 문장 지우고
				if (cnt == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				printf(" ", talk[0][i]);
				if (talk[0][i] == ' ') cnt += 0.5;
				else cnt++;
				if (cnt >= 73) {
					next_row++;
					cnt = 0;
				}
			}
			printf("    ");
			if (strcmp(input, "A") == 0) { //답이 맞았으면
				mini4_finish = 1;
				Conversation(ok, 1);
				SetCurrentCursorPos(72, 4); //라이프는 필요 없으므로 공백출력
				printf("          ");
				Conversation(chk, 1);
				DeleteNode(3, 4);
				MovableItem[3][4] = 2;
				DeleteList();
				PrintList();
				can_Entrance_Check[5] += 1;
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);

			}
			else { //답이 틀렸다면
				Conversation(no, 1);
				SetCurrentCursorPos(72, 4); //라이프 관리
				LifeLimit--;
				if (LifeLimit == 1) {
					White();
					printf("Life: ");
					Red();
					printf("♥   ");
					Gray();
				}
				else {
					White();
					printf("Life:     ");
					mini4_die = 1; //어디서 죽었는지 확인
					Gray();
				}
			}
		}
		else Conversation(stop, 1);
	}
}

//4번방 총 대사 출력
void gun()
{
	char gun1[1][51] = { "누군가 사용한 흔적이 있는 총이다 ː." }; //상사와 대화하기 전 총을 눌렀을 시(범죄도구가 무엇인지 파악x)
	char gun2[1][51] = { "이번 극장 총기난사사건에 사용되었던 총이다. ː" }; //상사와 대화한 후 총을 눌렀을 시(범죄도구 인식)
	if (talk_to_superior == 0) Conversation(gun1, 1); //상사와 대화하기 전이면 gun1 대사 출력
	else Conversation(gun2, 1); //상사와 대화한 후면 gun2 대사 출력
}

//거짓말 D, 범인 A
//4번방 용의자 A대사 출력
void suspectA()
{
	char A1[1][51] = { "사람이 있다. ː" };
	char A2[1][51] = { "A: 거짓말을 하고 있는건 D 입니다! ː" };
	char stop[1][51] = { "이제 나가도 될 것 같다. ː" };

	if (talk_to_superior == 0) Conversation(A1, 1);
	else {
		if (mini4_finish) Conversation(stop, 1);
		else {
			Conversation(A2, 1);
			talk_to_superior = 2;
		}
	}
}

//4번방 용의자 B대사 출력
void suspectB()
{
	char B1[1][51] = { "사람이 있다. ː" };
	char B2[1][51] = { "B: 범인은 A 아니면 D 둘 중에 하나에요. ː" };
	char stop[1][51] = { "이제 나가도 될 것 같다. ː" };

	if (talk_to_superior == 0) Conversation(B1, 1);
	else {
		if (mini4_finish) Conversation(stop, 1);
		else {
			Conversation(B2, 1);
			talk_to_superior = 2;
		}
	}
}

//4번방 용의자 C대사 출력
void suspectC()
{
	char C1[1][51] = { "사람이 있다. ː" };
	char C2[1][51] = { "C: A,E 중 1명이 범인일 겁니다. 확실해요. ː" };
	char stop[1][51] = { "이제 나가도 될 것 같다. ː" };

	if (talk_to_superior == 0) Conversation(C1, 1);
	else {
		if (mini4_finish) Conversation(stop, 1);
		else {
			Conversation(C2, 1);
			talk_to_superior = 2;
		}
	}
}

//4번방 용의자 D대사 출력
void suspectD()
{
	char D1[1][51] = { "사람이 있다. ː" };
	char D2[1][51] = { "D: B는 거짓말을 하고 있어요. ː" };
	char stop[1][51] = { "이제 나가도 될 것 같다. ː" };

	if (talk_to_superior == 0) Conversation(D1, 1);
	else {
		if (mini4_finish) Conversation(stop, 1);
		else {
			Conversation(D2, 1);
			talk_to_superior = 2;
		}
	}
}

//4번방 용의자 E대사 출력
void suspectE()
{
	char E1[1][51] = { "사람이 있다. ː" };
	char E2[1][51] = { "E: 저는 아무것도 모릅니다. 전 범인이 아니에요! ː" };
	char stop[1][51] = { "이제 나가도 될 것 같다. ː" };

	if (talk_to_superior == 0) Conversation(E1, 1);
	else {
		if (mini4_finish) Conversation(stop, 1);
		else {
			Conversation(E2, 1);
			talk_to_superior = 2;
		}
	}
}

//4, 8번방 미니게임함수
//문이 안열린다 출력
void not_open_door()
{
	char lock[1][51] = { "문이 열리지 않는다. ː" };
	Conversation(lock, 1);
}

//5번방 물건 대사창 출력
void Room5_message(int n)
{
	//시체:Ф 46,11 - 대화창: 1
	//흉기:ℓ 62,8 - 대화창: 2
	//(먼지가 쌓인)의자:▩ 62,15 - 대화창: 3
	//컴퓨터: @ 30,8 - 대화창: 4
	//책상서랍: □ 34,8 - 대화창: 5
	//신문: ▥ 34,15 - 대화창: 6
	char ar[9][70] = {
		"한 여성의 시체가 있다. ː",
		"피가 묻어있는 칼이 있다. ː",
		"피가 묻은 소파가 있다. ː",
		"전원이 꺼져 있는 컴퓨터가 있다. ː",
		"어질러진 책상이 있다. ː",
		"구겨진 신문이 있다. ː",
		"……… ː",
		"6번방 열쇠를 획득했다. ː",
		"칼로 찢어진 자국이 남은 베개가 있다. ː"
	};
	Conversation(ar[n - 1], 1);
	if (n == 1) {
		if (!mini5_notcome) { //미니게임 들어올수 있다면
			if (!mini5_start) { //열쇠를 획득한 적이 없다면
				Conversation(ar[6], 1);
				Conversation(ar[7], 1);
				GetItem(2);
				mini5_start = 1;
			}
		}
	}
	//추후 이벤트 연결이 된다면 수정하겠음!(경남)
}

void Sixth(int a, int b)
{
	int i;
	for (i = a; i <= b; i++) {
		Conversation(SixthRoomMessage[i], 1);
		Interface();
	}
}

//8번방 물건 대사창 출력
void Room8_message(int n)
{
	char ar[4][40] = {
		"아무것도 들어있지 않다. ː",
		"5번방 탈출 열쇠를 발견했다. ː",
		"더 이상 뒤지지 않아도 될 것 같다. ː",
		"열쇠를 사용하여 문을 열였다. ː"
	};
	Conversation(ar[n - 1], 1);
}

//8번방 랜덤 부여 함수(80~89 랜덤숫자 생성)
int random_keynumber()
{
	srand(time(NULL));
	int random = rand() % 10; //0~9까지의 숫자 생성
	return random + 80;
}

//8번방 타임어택 게임 설명
void mini5_rule()
{
	char talk[1][200] = { "갑자기 문이 잠겼다. 제한시간 내에 열쇠를 찾아 탈출해야 한다. ː" };
	int row = 1;
	int i = 0, next_row = 1 + row - 1;
	float cnt = 0;
	int key;
	for (i = 0; talk[0][i]; i++) {
		if (cnt == 0) {
			if (talk[0][i] == ' ') continue;
			else SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
		}
		if (talk[0][i] == '^') {
			next_row++;
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			cnt = 0;
		}
		else {
			printf("%c", talk[0][i]);
			//Sleep(40);
		}
		if (talk[0][i] == ' ') cnt += 0.5;
		else cnt++;
		if (cnt >= 70) {
			next_row++;
			cnt = 0;
		}
	}
	next_row = 1, cnt = 0;
	while (1) {
		key = _getch();
		if (key == SPACEBAR) {
			for (i = 0; talk[0][i]; i++) {
				if (cnt == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				if (talk[0][i] == '^') {
					next_row++;
					SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
					cnt = 0;
				}
				else {
					printf(" ", talk[0][i]);
				}
				if (talk[0][i] == ' ') cnt += 0.5;
				else cnt++;
				if (cnt >= 70) {
					next_row++;
					cnt = 0;
				}
			}
			break;
		}
	}
}

//8번방 랜덤숫자가 80일 때
void Room8_Check_80()
{
	COORD mid;
	//80번: 32,9
	if (left) {
		if (save.X - 2 == 32 && save.Y == 9) { //80번에 있을때
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 34;
			save.Y = 9;
		}
		else if ((save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11) || (save.X - 2 == 38 && save.Y == 15)
			|| (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 32 && save.Y == 9) { //80번에 있을때
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 30;
			save.Y = 9;
		}
		else if ((save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11) || (save.X + 2 == 38 && save.Y == 15)
			|| (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 32 && save.Y - 1 == 9) { //80번에 있을때
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 32;
			save.Y = 10;
		}
		else if ((save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11) || (save.X == 38 && save.Y - 1 == 15)
			|| (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 32 && save.Y + 1 == 9) { //80번에 있을때
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 32;
			save.Y = 8;
		}
		else if ((save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11) || (save.X == 38 && save.Y + 1 == 15)
			|| (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}

	//64 10
}

//8번방 랜덤숫자가 81일 때
void Room8_Check_81()
{
	COORD mid;
	//81번: 32,14
	if (left) {
		if (save.X - 2 == 32 && save.Y == 14) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 34;
			save.Y = 14;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 38 && save.Y == 11) || (save.X - 2 == 38 && save.Y == 15)
			|| (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 32 && save.Y == 14) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 30;
			save.Y = 14;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 38 && save.Y == 11) || (save.X + 2 == 38 && save.Y == 15)
			|| (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 32 && save.Y - 1 == 14) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 32;
			save.Y = 15;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 38 && save.Y - 1 == 11) || (save.X == 38 && save.Y - 1 == 15)
			|| (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 32 && save.Y + 1 == 14) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 32;
			save.Y = 13;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 38 && save.Y + 1 == 11) || (save.X == 38 && save.Y + 1 == 15)
			|| (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 82일 때
void Room8_Check_82()
{
	COORD mid;
	//82번: 38,11
	if (left) {
		if (save.X - 2 == 38 && save.Y == 11) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 40;
			save.Y = 11;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 15)
			|| (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 38 && save.Y == 11) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 36;
			save.Y = 11;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 15)
			|| (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 38 && save.Y - 1 == 11) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 38;
			save.Y = 12;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 15)
			|| (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 38 && save.Y + 1 == 11) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 38;
			save.Y = 10;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 15)
			|| (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 83일 때
void Room8_Check_83()
{
	COORD mid;
	//83번: 38,15
	if (left) {
		if (save.X - 2 == 38 && save.Y == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 40;
			save.Y = 15;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 38 && save.Y == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 36;
			save.Y = 15;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 38 && save.Y - 1 == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 38;
			save.Y = 16;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 38 && save.Y + 1 == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 38;
			save.Y = 14;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 84일 때
void Room8_Check_84()
{
	COORD mid;
	//84번: 44,8
	if (left) {
		if (save.X - 2 == 44 && save.Y == 8) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 46;
			save.Y = 8;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 13) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 44 && save.Y == 8) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 42;
			save.Y = 8;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 13) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 44 && save.Y - 1 == 8) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 44;
			save.Y = 9;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 13) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 44 && save.Y + 1 == 8) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 44;
			save.Y = 7;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 13) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 85일 때
void Room8_Check_85()
{
	COORD mid;
	//85번: 44,13
	if (left) {
		if (save.X - 2 == 44 && save.Y == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 46;
			save.Y = 13;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 50 && save.Y == 10)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 44 && save.Y == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 42;
			save.Y = 13;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 50 && save.Y == 10)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 44 && save.Y - 1 == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 44;
			save.Y = 14;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 8) || (save.X == 50 && save.Y - 1 == 10)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 44 && save.Y + 1 == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 44;
			save.Y = 12;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 8) || (save.X == 50 && save.Y + 1 == 10)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 86일 때
void Room8_Check_86()
{
	COORD mid;
	//86번: 50,10
	if (left) {
		if (save.X - 2 == 50 && save.Y == 10) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 52;
			save.Y = 10;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13)
			|| (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 50 && save.Y == 10) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 48;
			save.Y = 10;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13)
			|| (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 50 && save.Y - 1 == 10) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 50;
			save.Y = 11;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13)
			|| (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 50 && save.Y + 1 == 10) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 50;
			save.Y = 9;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13)
			|| (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 87일 때
void Room8_Check_87()
{
	COORD mid;
	//87번: 50,15
	if (left) {
		if (save.X - 2 == 50 && save.Y == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 52;
			save.Y = 15;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13)
			|| (save.X - 2 == 50 && save.Y == 10) || (save.X - 2 == 56 && save.Y == 13) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 50 && save.Y == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 48;
			save.Y = 15;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13)
			|| (save.X + 2 == 50 && save.Y == 10) || (save.X + 2 == 56 && save.Y == 13) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 50 && save.Y - 1 == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 50;
			save.Y = 16;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13)
			|| (save.X == 50 && save.Y - 1 == 10) || (save.X == 56 && save.Y - 1 == 13) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 50 && save.Y + 1 == 15) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 50;
			save.Y = 14;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13)
			|| (save.X == 50 && save.Y + 1 == 10) || (save.X == 56 && save.Y + 1 == 13) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 88일 때
void Room8_Check_88()
{
	COORD mid;
	//88번: 56,13
	if (left) {
		if (save.X - 2 == 56 && save.Y == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 58;
			save.Y = 13;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13)
			|| (save.X - 2 == 50 && save.Y == 10) || (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 56 && save.Y == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 54;
			save.Y = 13;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13)
			|| (save.X + 2 == 50 && save.Y == 10) || (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 60 && save.Y == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 56 && save.Y - 1 == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 56;
			save.Y = 14;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13)
			|| (save.X == 50 && save.Y - 1 == 10) || (save.X == 50 && save.Y - 1 == 15) || (save.X == 60 && save.Y - 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 56 && save.Y + 1 == 13) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 56;
			save.Y = 12;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13)
			|| (save.X == 50 && save.Y + 1 == 10) || (save.X == 50 && save.Y + 1 == 15) || (save.X == 60 && save.Y + 1 == 9)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//8번방 랜덤숫자가 89일 때
void Room8_Check_89()
{
	COORD mid;
	//89번: 60,9
	if (left) {
		if (save.X - 2 == 60 && save.Y == 9) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 62;
			save.Y = 9;
		}
		else if ((save.X - 2 == 32 && save.Y == 9) || (save.X - 2 == 32 && save.Y == 14) || (save.X - 2 == 38 && save.Y == 11)
			|| (save.X - 2 == 38 && save.Y == 15) || (save.X - 2 == 44 && save.Y == 8) || (save.X - 2 == 44 && save.Y == 13)
			|| (save.X - 2 == 50 && save.Y == 10) || (save.X - 2 == 50 && save.Y == 15) || (save.X - 2 == 56 && save.Y == 13)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (right) {
		if (save.X + 2 == 60 && save.Y == 9) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 58;
			save.Y = 9;
		}
		else if ((save.X + 2 == 32 && save.Y == 9) || (save.X + 2 == 32 && save.Y == 14) || (save.X + 2 == 38 && save.Y == 11)
			|| (save.X + 2 == 38 && save.Y == 15) || (save.X + 2 == 44 && save.Y == 8) || (save.X + 2 == 44 && save.Y == 13)
			|| (save.X + 2 == 50 && save.Y == 10) || (save.X + 2 == 50 && save.Y == 15) || (save.X + 2 == 56 && save.Y == 13)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (up) {
		if (save.X == 60 && save.Y - 1 == 9) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 60;
			save.Y = 10;
		}
		else if ((save.X == 32 && save.Y - 1 == 9) || (save.X == 32 && save.Y - 1 == 14) || (save.X == 38 && save.Y - 1 == 11)
			|| (save.X == 38 && save.Y - 1 == 15) || (save.X == 44 && save.Y - 1 == 8) || (save.X == 44 && save.Y - 1 == 13)
			|| (save.X == 50 && save.Y - 1 == 10) || (save.X == 50 && save.Y - 1 == 15) || (save.X == 56 && save.Y - 1 == 13)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
	if (down) {
		if (save.X == 60 && save.Y + 1 == 9) {
			if (mini5_end) Room8_message(3);
			else {
				Room8_message(2);
				GetItem(2);
				mini5_end = 1;
			}
			save.X = 60;
			save.Y = 8;
		}
		else if ((save.X == 32 && save.Y + 1 == 9) || (save.X == 32 && save.Y + 1 == 14) || (save.X == 38 && save.Y + 1 == 11)
			|| (save.X == 38 && save.Y + 1 == 15) || (save.X == 44 && save.Y + 1 == 8) || (save.X == 44 && save.Y + 1 == 13)
			|| (save.X == 50 && save.Y + 1 == 10) || (save.X == 50 && save.Y + 1 == 15) || (save.X == 56 && save.Y + 1 == 13)) {
			mid = save;
			Room8_message(1);
			save = mid;
		}
	}
}

//4, 5, 8번방 문 안열림 설정
void mini_lock_door()
{
	//4번방 문 좌표: 48,17   50,17  - 가로
	//5번방 문 좌표: 70,10   70,11  - 세로
	int key, i;
	LN* lp = NULL;

	if (currentRoomNumber == 4) {
		if (((player.X == 44 && player.Y == 16) || (player.X == 46 && player.Y == 16)) && (up == 1)) {
			if (!mini4_finish) { //한번 들어오면 못나가게 하기
				if (!mini4_come) { //한번 잠갔으면 다음에는 출력 안할 수 있도록
					SetCurrentCursorPos(44, 17);
					for (i = 0; i < 4; i++) {
						printf("=");
						if (i != 3) Sleep(100);
					}
					Map[4][13][17] = 2;
					Map[4][13][18] = 2;
					mini4_come = 1;
				}
			}
		}
		if (mini4_finish) {
			if (!mini4_disappear) {
				SetCurrentCursorPos(44, 17);
				Sleep(200);
				for (i = 0; i < 4; i++) {
					printf(" ");
					if (i != 3) Sleep(100);
				}
				mini4_disappear = 1;
				Map[4][13][18] = 0;
				Map[4][13][17] = 0;
				SetCurrentCursorPos(save.X, save.Y);
			}
		}
	}
	if (currentRoomNumber == 5) {
		if (player.X >= 62 && player.X <= 63) {
			if (player.Y >= 10 && player.Y <= 11) { //해당위치에 있을 때
				if (mini5_start) { //열쇠를 획득했다면 문 잠그기
					if (!mini5_come) { //한번만 잠그도록
						Sleep(200);
						DeleteMap(8); //현재 방을 지우고 8번방, 미니게임방 출력
						SetCurrentCursorPos(64, 10);
						printf("||");
						Sleep(100);
						SetCurrentCursorPos(64, 11);
						printf("||");
						Sleep(100);
						Map[8][6][27] = 2;
						Map[8][7][27] = 2;
						mini5_come = 1; //게임 클리어 이후 다시 출력되지 않도록
						Sleep(300);
						mini5_rule(); //룰설명
					}
				}
			}
		}
	}
	if (currentRoomNumber == 8) {
		if (mini5_end) {
			if (!mini5_check) { //문잠긴거 한번만 다시 확인
				Map[8][7][27] = 2;
				Map[8][6][27] = 2;
				mini5_check = 1;
			}
			if (!mini5_disappear) {
				mini5_notcome = 1; //다음번에 5번방에 들어올 시 8번방 소환 안되도록
				mini5_disappear = 1;
			}
		}
	}
}

//9번방 대사 출력 함수
void Room9_message(int n)
{
	//쇼파: ▩ 62,15 - 대화창: 1
	//컴퓨터: @ 30,8 - 대화창: 2(꺼짐)
	//책상서랍: □ 34,8 - 대화창: 3
	//신문: ▥ 34,15 - 대화창: 4
	//베개: ■ - 대화창: 5
	//원서책: ▦ - 대화창: 6

	char ar[9][70] = {
		".다있여놓 가파소 한락안 ː",
		"컴퓨터 전원이 켜져 있다. 화면을 살펴보니 영단어가 적혀 있다. ː",
		".다있 이상책 된리정 히끔깔 ː",
		".다있여놓 이문신 에위 대침 진러질어 ː",
		".다진껴느 가기온 한스따 니보대 을손 .다있여놓 가개베 는이보 해신푹 ː",
	};
	Conversation(ar[n - 1], 1);
}

//9번방 최종문제 함수
void final()
{
	char talk[1][100] = { "'sorrow' 이 방의 규칙에 따라 단어를 변형하시오. ː" };
	int next_row = 1, cnt = 0, i = 0;
	if (lever) {
		SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
		for (i = 0; talk[0][i]; i++) {
			if (cnt == 0) {
				if (talk[0][i] == ' ') continue;
				else SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			}
			if (talk[0][i] == '^') {
				next_row++;
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				cnt = 0;
			}
			else {
				printf("%c", talk[0][i]);
				//Sleep(40);
			}
			if (talk[0][i] == ' ') cnt += 0.5;
			else cnt++;
			if (cnt >= 70) {
				next_row++;
				cnt = 0;
			}
		}
		next_row++;
		SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row); //다음줄에서 입력하도록
		ShowInputCursor(); //답 입력을 위해 커서 보이도록 설정
		scanf("%s", input9);
		RemoveCursor(); //답을 입력했으니 커서를 지우고
		SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
		next_row = 1, cnt = 0;
		for (i = 0; talk[0][i]; i++) { //일단 문장 지우고
			if (cnt == 0) SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
			printf(" ", talk[0][i]);
			if (talk[0][i] == ' ') cnt += 0.5;
			if (talk[0][i] == '^') {
				printf(" ");
				next_row++;
				SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
				cnt = 0;
			}
			else cnt++;
			if (cnt >= 75) {
				next_row++;
				cnt = 0;
			}
		}
		next_row++;
		SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + next_row);
		printf("                 "); //답입력한거 지우기
		if (strcmp(input9, answer9) == 0) { //답이 맞았으면
			finish = 1; //답 맞추면 1로 변경
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
			printf("알맞은 정답을 입력했다. ː");
			Sleep(500);
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
			for (i = 0; i < 30; i++)
				printf("  "); //대사창 지우기
		}
		else { //답이 틀렸다면
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
			printf("아무일도 일어나지 않는다. 답이 틀린 것 같다. ː");
			Sleep(500);
			SetCurrentCursorPos(MBOARD_ORIGIN_X + 2, MBOARD_ORIGIN_Y + 1);
			for (i = 0; i < 50; i++)
				printf("  "); //대사창 지우기
		}
	}
}

int GetItem(int item) {
	int i, j;
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 36; j++) {
			if (Map[currentRoomNumber][i][j] == item) {
				Map[currentRoomNumber][i][j] = 0;
				//DrawGameUI(Map);
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
	int tmpRoomNum;
	if (currentRoomNumber == 1 && MovableItem[1][1] == 1 && MovableItem[1][2] == 1) {
		First(2, 2);
		p1 = searchNode(1, 1);
		DeleteNode(p1);
		MovableItem[1][1] = 2;
		p2 = searchNode(1, 2);
		DeleteNode(p2);
		MovableItem[1][2] = 2;

		CollectCnt1 = 1;
		Map[1][4][21] = 16;
		DrawGameUI(Map);
		First(3, 3);
	}

	if (currentRoomNumber == 3 && MovableItem[3][1] == 1 && MovableItem[3][2] == 1 && MovableItem[3][3] == 1) {
		p1 = searchNode(3, 1);
		DeleteNode(p1);
		p2 = searchNode(3, 2);
		DeleteNode(p2);
		p3 = searchNode(3, 3);
		DeleteNode(p3);

		CollectCnt3 = 1;

		MovableItem[3][1] = 2;
		MovableItem[3][2] = 2;
		MovableItem[3][3] = 2;
	}

	if (MovableItem[3][0] == 1 && MovableItem[2][0] == 1 && MovableItem[1][0] == 1) {
		p1 = searchNode(1, 0);
		DeleteNode(p1);
		p2 = searchNode(2, 0);
		DeleteNode(p2);
		p3 = searchNode(3, 0);
		DeleteNode(p3);

		CollectCntTicket = 1;

		MovableItem[1][0] == 2;
		MovableItem[2][0] == 2;
		MovableItem[3][0] == 2;

		if (currentRoomNumber != 3 && CollectCntTicket == 1) {
			tmpRoomNum = currentRoomNumber;
			currentRoomNumber = 3;
			GetItem(7);
			can_Entrance_Check[10] += 1;
			currentRoomNumber = tmpRoomNum;
		}
		else {
			GetItem(7);
			Third(6, 6);
			can_Entrance_Check[10] += 1;
		}
	}
	DeleteList();
	PrintList();
}

void Item_Dis() {
	COORD p;
	LN* lp = NULL;
	int i, j, key;
	p = GetCurrentCursorPos();

	if (currentRoomNumber == 0) {
		if (left) { //마지막으로 누른 방향키
			if ((p.X - 2 == 34) && (p.Y == 8) && MovableItem[currentRoomNumber][0] == 0) { ZeroRoomKey(); Zero(4, 5); }
			else if ((p.X - 2 == 48) && (p.Y == 12)) {
				if (MovableItem[currentRoomNumber][0] == 1 && ImmovableItem[currentRoomNumber][0] == 0 && MovableItem[currentRoomNumber][1] == 0) { ZeroRoomSafe(); SetCurrentCursorPos(50, 12); }
				else if (MovableItem[currentRoomNumber][0] == 2) { Zero(7, 7); SetCurrentCursorPos(50, 12); }
				else if (MovableItem[currentRoomNumber][0] == 0) { Zero(6, 6); SetCurrentCursorPos(50, 12); }
			}
		}
		if (right) {
			if ((p.X + 2 == 48) && (p.Y == 12)) {
				if (MovableItem[currentRoomNumber][0] == 1 && ImmovableItem[currentRoomNumber][0] == 0 && MovableItem[currentRoomNumber][1] == 0) { ZeroRoomSafe(); SetCurrentCursorPos(46, 12); }
				else if (MovableItem[currentRoomNumber][0] == 2) { Zero(7, 7); SetCurrentCursorPos(46, 12); }
				else if (MovableItem[currentRoomNumber][0] == 0) { Zero(6, 6); SetCurrentCursorPos(46, 12); }
			}
		}
		if (up) {
			if ((p.X == 34) && (p.Y - 1 == 8) && MovableItem[currentRoomNumber][0] == 0) { ZeroRoomKey(); Zero(4, 5); }
			else if ((p.X == 48) && (p.Y - 1 == 12)) {
				if (MovableItem[currentRoomNumber][0] == 1 && ImmovableItem[currentRoomNumber][0] == 0 && MovableItem[currentRoomNumber][1] == 0) { ZeroRoomSafe(); SetCurrentCursorPos(48, 13); }
				else if (MovableItem[currentRoomNumber][0] == 2) { Zero(7, 7); SetCurrentCursorPos(48, 13); }
				else if (MovableItem[currentRoomNumber][0] == 0) { Zero(6, 6); SetCurrentCursorPos(48, 13); }
			}
		}
		if (down) {
			if ((p.X == 48) && (p.Y + 1 == 12)) {
				if (MovableItem[currentRoomNumber][0] == 1 && ImmovableItem[currentRoomNumber][0] == 0 && MovableItem[currentRoomNumber][1] == 0) { ZeroRoomSafe(); SetCurrentCursorPos(48, 11); }
				else if (MovableItem[currentRoomNumber][0] == 2) { Zero(7, 7); SetCurrentCursorPos(48, 11); }
				else if (MovableItem[currentRoomNumber][0] == 0) { Zero(6, 6); SetCurrentCursorPos(48, 11); }
			}
		}
	}
	else if (currentRoomNumber == 1) {
		//티켓조각:48,15  연필:58,8  메모장:62,8 <완료>
		//책상서랍:34,8 <완료>
		//컴퓨터:30,8  의자:62,15  숫자가적힌쪽지:54,8 (대사만 출력)
		//신문:34,15  베개:30,15 (대사만 출력, 두칸씩 떨어진 위치에서 읽어내야 함) <완료>
		//아이콘 지워줄 때 X좌표는 한칸 앞(-2)으로
		if (left) { //마지막으로 누른 방향키
			if ((p.X - 2 == 48) && (p.Y == 15) && (MovableItem[currentRoomNumber][0] == 0)) { GetItem(2); SetCurrentCursorPos(46, 15); printf("  "); First(4, 4); CollectCheck(); }
			else if ((p.X - 2 == 58) && (p.Y == 8) && MovableItem[currentRoomNumber][1] == 0) { GetItem(3); SetCurrentCursorPos(56, 8); printf("  "); First(1, 1); CollectCheck(); }
			else if ((p.X - 2 == 62) && (p.Y == 8) && MovableItem[currentRoomNumber][2] == 0) { GetItem(4); SetCurrentCursorPos(60, 8); printf("  "); First(0, 0); CollectCheck(); }
			else if ((p.X - 2 == 34) && (p.Y == 8)) {
				if (CollectCnt1 == 1 && MovableItem[currentRoomNumber][3] == 0) { Room1_Quiz_Desk(); }
				else if (MovableItem[currentRoomNumber][3] == 1 || MovableItem[currentRoomNumber][3] == 2) { First(6, 6); }
				else { First(5, 5); }
				SetCurrentCursorPos(36, 8);
			}
			else if ((p.X - 2 == 30) && (p.Y == 8)) { First(7, 8); SetCurrentCursorPos(32, 8); }
			else if ((p.X - 2 == 62) && (p.Y == 15)) { First(9, 9); SetCurrentCursorPos(64, 15); }
			else if ((p.X - 2 == 54) && (p.Y == 8) && (CollectCnt1 == 1)) { Room1_Quiz_Number(); SetCurrentCursorPos(56, 8); } ///////수정
			else if ((p.X - 4 == 34) && (p.Y == 15)) { First(10, 12); SetCurrentCursorPos(36, 15); }
		}
		if (right) {
			if ((p.X + 2 == 48) && (p.Y == 15) && (MovableItem[currentRoomNumber][0] == 0)) { GetItem(2); SetCurrentCursorPos(46, 15); printf("  "); First(4, 4); CollectCheck(); }
			else if ((p.X + 2 == 58) && (p.Y == 8) && MovableItem[currentRoomNumber][1] == 0) { GetItem(3); SetCurrentCursorPos(56, 8); printf("  "); First(1, 1); CollectCheck(); }
			else if ((p.X + 2 == 62) && (p.Y == 8) && MovableItem[currentRoomNumber][2] == 0) { GetItem(4); SetCurrentCursorPos(60, 8); printf("  "); First(0, 0); CollectCheck(); }
			else if ((p.X + 2 == 34) && (p.Y == 8)) {
				if (CollectCnt1 == 1 && MovableItem[currentRoomNumber][3] == 0) { Room1_Quiz_Desk(); }
				else if (MovableItem[currentRoomNumber][3] == 1 || MovableItem[currentRoomNumber][3] == 2) { First(6, 6); }
				else { First(5, 5); }
				SetCurrentCursorPos(32, 8);
			}
			else if ((p.X + 2 == 30) && (p.Y == 8)) { First(7, 8); SetCurrentCursorPos(28, 8); }
			else if ((p.X + 2 == 62) && (p.Y == 15)) { First(9, 9); SetCurrentCursorPos(60, 15); }
			else if ((p.X + 2 == 54) && (p.Y == 8) && (CollectCnt1 == 1)) { Room1_Quiz_Number(); SetCurrentCursorPos(52, 8); }
		}
		if (up) {
			if ((p.X == 48) && (p.Y - 1 == 15) && (MovableItem[currentRoomNumber][0] == 0)) { GetItem(2); SetCurrentCursorPos(46, 15); printf("  "); First(4, 4); CollectCheck(); }
			else if ((p.X == 58) && (p.Y - 1 == 8) && MovableItem[currentRoomNumber][1] == 0) { GetItem(3); SetCurrentCursorPos(56, 8); printf("  "); First(1, 1); CollectCheck(); }
			else if ((p.X == 62) && (p.Y - 1 == 8) && MovableItem[currentRoomNumber][2] == 0) { GetItem(4); SetCurrentCursorPos(60, 8); printf("  "); First(0, 0); CollectCheck(); }
			else if ((p.X == 34) && (p.Y - 1 == 8)) {
				if (CollectCnt1 == 1 && MovableItem[currentRoomNumber][3] == 0) { Room1_Quiz_Desk(); }
				else if (MovableItem[currentRoomNumber][3] == 1 || MovableItem[currentRoomNumber][3] == 2) { First(6, 6); }
				else { First(5, 5); }
				SetCurrentCursorPos(34, 9);
			}
			else if ((p.X == 30) && (p.Y - 1 == 8)) { First(7, 8); SetCurrentCursorPos(30, 9); }
			else if ((p.X == 62) && (p.Y - 1 == 15)) { First(9, 9); SetCurrentCursorPos(62, 16); }
			else if ((p.X == 54) && (p.Y - 1 == 8) && (CollectCnt1 == 1)) { Room1_Quiz_Number(); SetCurrentCursorPos(54, 9); }
		}
		if (down) {
			if ((p.X == 48) && (p.Y + 1 == 15) && (MovableItem[currentRoomNumber][0] == 0)) { GetItem(2); SetCurrentCursorPos(46, 15); printf("  "); First(4, 4); CollectCheck(); }
			else if ((p.X == 58) && (p.Y + 1 == 8) && MovableItem[currentRoomNumber][1] == 0) { GetItem(3); SetCurrentCursorPos(56, 8); printf("  "); First(1, 1); CollectCheck(); }
			else if ((p.X == 62) && (p.Y + 1 == 8) && MovableItem[currentRoomNumber][2] == 0) { GetItem(4); SetCurrentCursorPos(60, 8); printf("  "); First(0, 0); CollectCheck(); }
			else if ((p.X == 34) && (p.Y + 1 == 8)) {
				if (CollectCnt1 == 1 && MovableItem[currentRoomNumber][3] == 0) { Room1_Quiz_Desk(); }
				else if (MovableItem[currentRoomNumber][3] == 1 || MovableItem[currentRoomNumber][3] == 2) { First(6, 6); }
				else { First(5, 5); }
				SetCurrentCursorPos(34, 7);
			}
			else if ((p.X == 30) && (p.Y + 1 == 8)) { First(7, 8); SetCurrentCursorPos(30, 7); }
			else if ((p.X == 62) && (p.Y + 1 == 15)) { First(9, 9); SetCurrentCursorPos(62, 14); }
			else if ((p.X == 54) && (p.Y + 1 == 8) && (CollectCnt1 == 1)) { Room1_Quiz_Number(); SetCurrentCursorPos(54, 7); }
			else if ((p.X == 34) && (p.Y + 2 == 15)) { First(10, 12); SetCurrentCursorPos(34, 13); }
			else if ((p.X == 30) && (p.Y + 2 == 15)) { First(13, 13); SetCurrentCursorPos(30, 13); }
		}
	}
	else if (currentRoomNumber == 2) {
		//청진기:38,8  링거:38,12  주사기:38,16 (대사만 출력, 두칸씩 떨어진 위치에서 읽어내야 함) <완료>
		//티켓조각:56,16 <완료>
		//진료차트:56,12
		//경찰ID카드:56,8
		if (left) { //마지막으로 누른 방향키
			if ((p.X - 4 == 38) && (p.Y == 8)) { Second(0, 0); SetCurrentCursorPos(42, 8); }
			else if ((p.X - 4 == 38) && (p.Y == 12)) { Second(1, 1); SetCurrentCursorPos(42, 12); }
			else if ((p.X - 4 == 38) && (p.Y == 16)) { Second(2, 2); SetCurrentCursorPos(42, 16); }
			else if ((p.X - 2 == 56) && (p.Y == 16)) { GetItem(2); SetCurrentCursorPos(54, 16); printf("  "); Second(3, 3); CollectCheck(); }
		}
		if (right) {
			if ((p.X + 2 == 56) && (p.Y == 16)) { GetItem(2); SetCurrentCursorPos(54, 16); printf("  "); Second(3, 3); CollectCheck(); }
			else if ((p.X + 4 == 56) && (p.Y == 12)) {
				if (SecondQuizSolve == 0) { Second(6, 8); Room2_Quiz_Chart(); }
				else { Second(6, 9); }
				SetCurrentCursorPos(52, 12);
			}
			else if ((p.X + 4 == 56) && (p.Y == 8)) {
				if (SecondQuizSolve == 1) { SetCurrentCursorPos(54, 8); printf("  "); Second(4, 5); SetCurrentCursorPos(52, 8); GetItem(4); }
			}
		}
		if (up) {
			if ((p.X == 38) && (p.Y - 2 == 8)) { Second(0, 0); SetCurrentCursorPos(38, 10); }
			else if ((p.X == 38) && (p.Y - 2 == 12)) { Second(1, 1); SetCurrentCursorPos(38, 14); }
			else if ((p.X == 56) && (p.Y - 1 == 16)) { GetItem(2); SetCurrentCursorPos(54, 16); printf("  "); Second(3, 3); CollectCheck(); }
			else if ((p.X == 56) && (p.Y - 2 == 12)) {
				if (SecondQuizSolve == 0) { Second(6, 8); Room2_Quiz_Chart(); }
				else { Second(6, 9); }
				SetCurrentCursorPos(56, 14);
			}
			else if ((p.X == 56) && (p.Y - 2 == 8)) {
				if (SecondQuizSolve == 1) { SetCurrentCursorPos(54, 8); printf("  "); Second(4, 5); SetCurrentCursorPos(56, 10); GetItem(4); }
			}
		}
		if (down) {
			if ((p.X == 38) && (p.Y + 2 == 12)) { Second(1, 1); SetCurrentCursorPos(38, 10); }
			else if ((p.X == 38) && (p.Y + 2 == 16)) { Second(2, 2); SetCurrentCursorPos(38, 14); }
			else if ((p.X == 56) && (p.Y + 1 == 16)) { GetItem(2); SetCurrentCursorPos(54, 16); printf("  "); Second(3, 3); CollectCheck(); }
			else if ((p.X == 56) && (p.Y + 2 == 12)) {
				if (SecondQuizSolve == 0) { Second(6, 8); Room2_Quiz_Chart(); }
				else { Second(6, 9); }
				SetCurrentCursorPos(56, 10);
			}
		}
	}
	else if (currentRoomNumber == 3) {
		//사건파일1:36,11   사건파일2:36,13      사건파일3:54,11
		//수갑:54,13
		if (left) { //마지막으로 누른 방향키
			if ((p.X - 4 == 38) && (p.Y == 11) && MovableItem[currentRoomNumber][1] == 0) {
				GetItem(3); SetCurrentCursorPos(36, 11); printf("  "); Third(1, 1);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
			else if ((p.X - 4 == 38) && (p.Y == 13) && MovableItem[currentRoomNumber][2] == 0) {
				GetItem(4); SetCurrentCursorPos(36, 13); printf("  "); Third(2, 2);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
		}
		if (right) {
			if ((p.X + 4 == 56) && (p.Y == 11) && MovableItem[currentRoomNumber][3] == 0) {
				GetItem(5); SetCurrentCursorPos(54, 11); printf("  "); Third(3, 3);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
			else if ((p.X + 4 == 56) && (p.Y == 13) && MovableItem[currentRoomNumber][4] == 0) {
				GetItem(6); SetCurrentCursorPos(54, 13); printf("  "); can_Entrance_Check[10] += 1; Third(4, 4);
			}
		}
		if (up) {
			if ((p.X == 38) && (p.Y - 2 == 13) && MovableItem[currentRoomNumber][2] == 0) {
				GetItem(4); SetCurrentCursorPos(36, 13); printf("  ");  Third(2, 2);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
			else if ((p.X == 56) && (p.Y - 2 == 13) && MovableItem[currentRoomNumber][4] == 0) {
				GetItem(6); SetCurrentCursorPos(54, 13); printf("  "); can_Entrance_Check[10] += 1; Third(4, 4);
			}
		}
		if (down) {
			if ((p.X == 38) && (p.Y + 2 == 11) && MovableItem[currentRoomNumber][1] == 0) {
				GetItem(3); SetCurrentCursorPos(36, 11); printf("  "); Third(1, 1);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
			else if ((p.X == 56) && (p.Y + 2 == 11) && MovableItem[currentRoomNumber][3] == 0) {
				GetItem(5); SetCurrentCursorPos(54, 11); printf("  "); Third(3, 3);
				CollectCheck();
				if (CollectCnt3 == 1 && MovableItem[currentRoomNumber][0] == 0) {
					GetItem(2); Third(5, 5); CollectCheck();
				}
			}
		}
	}
	else if (currentRoomNumber == 4) {
		//printf("%d %d", p.X, p.Y);
		//X, Y 순으로
		//superior(상사): 58,12       gun(총): 58 14
		//A: 38,9	B: 42,9	  C: 46,9	 D: 50,9	E: 54,9
		if (left) { //마지막으로 누른 방향키 ()
			if ((p.X - 2 == 38) && (p.Y == 9)) {
				suspectA();
				SetCurrentCursorPos(40, 9);
			}
			if ((p.X - 2 == 42) && (p.Y == 9)) {
				suspectB();
				SetCurrentCursorPos(44, 9);
			}
			if ((p.X - 2 == 46) && (p.Y == 9)) {
				suspectC();
				SetCurrentCursorPos(48, 9);
			}
			if ((p.X - 2 == 50) && (p.Y == 9)) {
				suspectD();
				SetCurrentCursorPos(52, 9);
			}
			if ((p.X - 2 == 54) && (p.Y == 9)) {
				suspectE();
				SetCurrentCursorPos(56, 9);
			}
			if ((p.X - 2 == 58) && (p.Y == 12)) {
				superior();
				SetCurrentCursorPos(60, 12);
			}
			if ((p.X - 2 == 58) && (p.Y == 14)) {
				gun();
				SetCurrentCursorPos(60, 14);
			}
		}
		if (right) {
			if ((p.X + 2 == 38) && (p.Y == 9)) {
				suspectA();
				SetCurrentCursorPos(36, 9);
			}
			if ((p.X + 2 == 42) && (p.Y == 9)) {
				suspectB();
				SetCurrentCursorPos(40, 9);
			}
			if ((p.X + 2 == 46) && (p.Y == 9)) {
				suspectC();
				SetCurrentCursorPos(44, 9);
			}
			if ((p.X + 2 == 50) && (p.Y == 9)) {
				suspectD();
				SetCurrentCursorPos(48, 9);
			}
			if ((p.X + 2 == 54) && (p.Y == 9)) {
				suspectE();
				SetCurrentCursorPos(52, 9);
			}
			if ((p.X + 2 == 58) && (p.Y == 12)) {
				superior();
				SetCurrentCursorPos(56, 12);
			}
			if ((p.X + 2 == 58) && (p.Y == 14)) {
				gun();
				SetCurrentCursorPos(56, 14);
			}
		}
		if (up) {
			if ((p.X == 38) && (p.Y - 1 == 9)) {
				suspectA();
				SetCurrentCursorPos(38, 10);
			}
			if ((p.X == 42) && (p.Y - 1 == 9)) {
				suspectB();
				SetCurrentCursorPos(42, 10);
			}
			if ((p.X == 46) && (p.Y - 1 == 9)) {
				suspectC();
				SetCurrentCursorPos(46, 10);
			}
			if ((p.X == 50) && (p.Y - 1 == 9)) {
				suspectD();
				SetCurrentCursorPos(50, 10);
			}
			if ((p.X == 54) && (p.Y - 1 == 9)) {
				suspectE();
				SetCurrentCursorPos(54, 10);
			}
			if ((p.X == 58) && (p.Y - 1 == 12)) {
				superior();
				SetCurrentCursorPos(58, 13);
			}
			if ((p.X == 58) && (p.Y - 1 == 14)) {
				gun();
				SetCurrentCursorPos(58, 15);
			}
		}
		if (down) {
			if ((p.X == 38) && (p.Y + 1 == 9)) {
				suspectA();
				SetCurrentCursorPos(38, 8);
			}
			if ((p.X == 42) && (p.Y + 1 == 9)) {
				suspectB();
				SetCurrentCursorPos(42, 8);
			}
			if ((p.X == 46) && (p.Y + 1 == 9)) {
				suspectC();
				SetCurrentCursorPos(46, 8);
			}
			if ((p.X == 50) && (p.Y + 1 == 9)) {
				suspectD();
				SetCurrentCursorPos(50, 8);
			}
			if ((p.X == 54) && (p.Y + 1 == 9)) {
				suspectE();
				SetCurrentCursorPos(54, 8);
			}
			if ((p.X == 58) && (p.Y + 1 == 12)) {
				superior();
				SetCurrentCursorPos(58, 11);
			}
			if ((p.X == 58) && (p.Y + 1 == 14)) {
				gun();
				SetCurrentCursorPos(58, 13);
			}
			if ((p.X == 46 && p.Y == 16) || (p.X == 48 && p.Y == 16)) {
				if (!mini4_finish) not_open_door();
				if (p.X == 46) SetCurrentCursorPos(46, 16);
				if (p.X == 48) SetCurrentCursorPos(48, 16);
			}
		}
	}
	else if (currentRoomNumber == 5) {
		//시체:Ф 46,11 - 대화창: 1
		//흉기:ℓ 62,8 - 대화창: 2
		//(먼지가 쌓인)의자:▩ 62,15 - 대화창: 3
		//컴퓨터: @ 30,8 - 대화창: 4
		//책상서랍: □ 34,8 - 대화창: 5
		//신문: ▥ 34,15 - 대화창: 6
		if (left) {
			if ((p.X - 2 == 46) && (p.Y == 11)) {
				Room5_message(1);
				SetCurrentCursorPos(48, 11);
			}
			if ((p.X - 2 == 62) && (p.Y == 8)) {
				Room5_message(2);
				SetCurrentCursorPos(64, 8);
			}
			if ((p.X - 2 == 62) && (p.Y == 15)) {
				Room5_message(3);
				SetCurrentCursorPos(64, 15);
			}
			if ((p.X - 2 == 30) && (p.Y == 8)) {
				Room5_message(4);
				SetCurrentCursorPos(32, 8);
			}
			if ((p.X - 2 == 34) && (p.Y == 8)) {
				Room5_message(5);
				SetCurrentCursorPos(36, 8);
			}
			if ((p.X - 4 == 34) && (p.Y == 15)) {
				Room5_message(6);
				SetCurrentCursorPos(38, 15);
			}
		}
		if (right) {
			if ((p.X + 2 == 46) && (p.Y == 11)) {
				Room5_message(1);
				SetCurrentCursorPos(44, 11);
			}
			if ((p.X + 2 == 62) && (p.Y == 8)) {
				Room5_message(2);
				SetCurrentCursorPos(60, 8);
			}
			if ((p.X + 2 == 62) && (p.Y == 15)) {
				Room5_message(3);
				SetCurrentCursorPos(60, 15);
			}
			if ((p.X + 2 == 30) && (p.Y == 8)) {
				Room5_message(4);
				SetCurrentCursorPos(28, 8);
			}
			if ((p.X + 2 == 34) && (p.Y == 8)) {
				Room5_message(5);
				SetCurrentCursorPos(32, 8);
			}
		}
		if (up) {
			if ((p.X == 46) && (p.Y - 1 == 11)) {
				Room5_message(1);
				SetCurrentCursorPos(46, 12);
			}
			if ((p.X == 62) && (p.Y - 1 == 8)) {
				Room5_message(2);
				SetCurrentCursorPos(62, 9);
			}
			if ((p.X == 62) && (p.Y - 1 == 15)) {
				Room5_message(3);
				SetCurrentCursorPos(62, 16);
			}
			if ((p.X == 30) && (p.Y - 1 == 8)) {
				Room5_message(4);
				SetCurrentCursorPos(30, 9);
			}
			if ((p.X == 34) && (p.Y - 1 == 8)) {
				Room5_message(5);
				SetCurrentCursorPos(34, 9);
			}
			if ((p.X == 34) && (p.Y - 2 == 15)) {
				Room5_message(6);
				SetCurrentCursorPos(34, 16);
			}
		}
		if (down) {
			if ((p.X == 46) && (p.Y + 1 == 11)) {
				Room5_message(1);
				SetCurrentCursorPos(46, 10);
			}
			if ((p.X == 62) && (p.Y + 1 == 8)) {
				Room5_message(2);
				SetCurrentCursorPos(62, 7);
			}
			if ((p.X == 62) && (p.Y + 1 == 15)) {
				Room5_message(3);
				SetCurrentCursorPos(62, 14);
			}
			if ((p.X == 30) && (p.Y + 1 == 8)) {
				Room5_message(4);
				SetCurrentCursorPos(30, 7);
			}
			if ((p.X == 34) && (p.Y + 1 == 8)) {
				Room5_message(5);
				SetCurrentCursorPos(34, 7);
			}
			if ((p.X == 34) && (p.Y + 2 == 15)) {
				Room5_message(6);
				SetCurrentCursorPos(34, 13);
			}
			if ((p.X == 30) && (p.Y + 2 == 15)) {
				Room5_message(9);
				SetCurrentCursorPos(30, 13);
			}
		}
	}
	else if (currentRoomNumber == 6) {
		//숫자가 적힌 쪽지 34,8 //제거!
		//전화기 : 46,11
		//전기 레버 60,8
		if (left) { //마지막으로 누른 방향키
			if ((p.X - 2 == 46) && (p.Y == 11)) { Sixth(3, 9); can_Entrance_Check[9] += 1; SetCurrentCursorPos(48, 11); }
			else if ((p.X - 2 == 60) && (p.Y == 8)) {
				Sixth(0, 0);
				if (ImmovableItem[currentRoomNumber][1] == 0) { Sixth(1, 1); ImmovableItem[currentRoomNumber][1] = 1; can_Entrance_Check[9] += 1; }
				else if (ImmovableItem[currentRoomNumber][1] == 1) { Sixth(2, 2); }
				SetCurrentCursorPos(62, 8);
			}
		}

		if (right) {
			if ((p.X + 2 == 46) && (p.Y == 11)) { Sixth(3, 9); can_Entrance_Check[9] += 1; SetCurrentCursorPos(44, 11); }
			else if ((p.X + 2 == 60) && (p.Y == 8)) {
				Sixth(0, 0);
				if (ImmovableItem[currentRoomNumber][1] == 0) { Sixth(1, 1); ImmovableItem[currentRoomNumber][1] = 1; can_Entrance_Check[9] += 1; }
				else if (ImmovableItem[currentRoomNumber][1] == 1) { Sixth(2, 2); }
				SetCurrentCursorPos(58, 8);
			}
		}
		if (up) {
			if ((p.X == 46) && (p.Y - 1 == 11)) { Sixth(3, 9); can_Entrance_Check[9] += 1; SetCurrentCursorPos(46, 12); }
			else if ((p.X == 60) && (p.Y - 1 == 8)) {
				Sixth(0, 0);
				if (ImmovableItem[currentRoomNumber][1] == 0) { Sixth(1, 1); ImmovableItem[currentRoomNumber][1] = 1; can_Entrance_Check[9] += 1; }
				else if (ImmovableItem[currentRoomNumber][1] == 1) { Sixth(2, 2); }
				SetCurrentCursorPos(60, 9);
			}
		}
		if (down) {
			if ((p.X == 46) && (p.Y + 1 == 11)) { Sixth(3, 9); can_Entrance_Check[9] += 1; SetCurrentCursorPos(46, 10); }
			else if ((p.X == 60) && (p.Y + 1 == 8)) {
				Sixth(0, 0);
				if (ImmovableItem[currentRoomNumber][1] == 0) { Sixth(1, 1); ImmovableItem[currentRoomNumber][1] = 1; can_Entrance_Check[9] += 1; }
				else if (ImmovableItem[currentRoomNumber][1] == 1) { Sixth(2, 2); }
				SetCurrentCursorPos(60, 7);
			}
		}

		//7번방: 복도(x)
		else if (currentRoomNumber == 8) {

			mini5_key = 80; //확인용
			Map[8][6][27] = 2;
			Map[8][7][27] = 2;
			if (save.X == 64 && (save.Y == 10 || save.Y == 11)) {
				if (!mini5_end) not_open_door(); //문이 열리지 않는다 출력
				else {
					if (right) {
						Room8_message(4); //문이 열렸다 //아이템 리스트 재생성
						lp = searchNode(8, 0);
						DeleteNode(lp);
						DeleteList();
						PrintList();
						//문없애기
						SetCurrentCursorPos(64, 10);
						printf("  ");
						SetCurrentCursorPos(64, 11);
						printf("  ");
						//기타설정
						mini5_disappear = 1;
						Map[8][6][27] = 0;
						Map[8][7][27] = 0;
						can_Entrance_Check[6] = 1;
					}
					else {
						Map[8][6][27] = 2;
						Map[8][7][27] = 2;
					}
				}
			}
			if (mini5_key == 80) Room8_Check_80();
			if (mini5_key == 81) Room8_Check_81();
			if (mini5_key == 82) Room8_Check_82();
			if (mini5_key == 83) Room8_Check_83();
			if (mini5_key == 84) Room8_Check_84();
			if (mini5_key == 85) Room8_Check_85();
			if (mini5_key == 86) Room8_Check_86();
			if (mini5_key == 87) Room8_Check_87();
			if (mini5_key == 88) Room8_Check_88();
			if (mini5_key == 89) Room8_Check_89();
		}
		else if (currentRoomNumber == 9) {
			//printf("%d %d", p.X, p.Y);
			//▩: 30,15 의자 1
			//▤: 58,8 쪽지 2 ????????????????
			//@: 62,8 컴퓨터 꺼짐:3  전원켜짐:6
			//■: 62,15(장애물 있음) 베개 4
			//▥: 58,15(장애물 있음) 신문 5
			if (left) {
				if ((p.X - 2 == 30) && (p.Y == 15)) {
					Room9_message(1);
					SetCurrentCursorPos(32, 15);
				}
				if ((p.X - 2 == 58) && (p.Y == 8)) {
					//대사 출력
					SetCurrentCursorPos(60, 8);
				}
				if ((p.X - 2 == 62) && (p.Y == 8)) {
					if (!lever) Room9_message(3);
					else {
						if (!check_lever) {//켜져있다고 한번도 출력을 안했으면
							check_lever = 1;
							Room9_message(6);
						}
						final();
						SetCurrentCursorPos(64, 8);
					}
				}
			}
			if (right) {
				if ((p.X + 2 == 30) && (p.Y == 15)) {
					Room9_message(1);
					SetCurrentCursorPos(28, 15);
				}
				if ((p.X + 2 == 58) && (p.Y == 8)) {
					//대사 출력
					SetCurrentCursorPos(56, 8);
				}
				if ((p.X + 2 == 62) && (p.Y == 8)) {
					if (!lever) Room9_message(3);
					else {
						if (!check_lever) {//켜져있다고 한번도 출력을 안했으면
							check_lever = 1;
							Room9_message(6);
						}
						final();
						SetCurrentCursorPos(60, 8);
					}
				}
				if ((p.X + 4 == 58) && (p.Y == 15)) {
					Room9_message(5);
					SetCurrentCursorPos(54, 15);
				}
			}
			if (up) {
				if ((p.X == 30) && (p.Y - 1 == 15)) {
					Room9_message(1);
					SetCurrentCursorPos(30, 16);
				}
				if ((p.X == 58) && (p.Y - 1 == 8)) {
					//대사 출력
					SetCurrentCursorPos(58, 9);
				}
				if ((p.X == 62) && (p.Y - 1 == 8)) {
					if (!lever) Room9_message(3);
					else {
						if (!check_lever) {//켜져있다고 한번도 출력을 안했으면
							check_lever = 1;
							Room9_message(6);
						}
						final();
						SetCurrentCursorPos(62, 9);
					}
				}
			}
			if (down) {
				if ((p.X == 30) && (p.Y + 1 == 15)) {
					Room9_message(1);
					SetCurrentCursorPos(30, 14);
				}
				if ((p.X == 58) && (p.Y + 1 == 8)) {
					//대사 출력
					SetCurrentCursorPos(58, 7);
				}
				if ((p.X == 62) && (p.Y + 1 == 8)) {
					if (!lever) Room9_message(3);
					else {
						if (!check_lever) {//켜져있다고 한번도 출력을 안했으면
							check_lever = 1;
							Room9_message(6);
						}
						final();
						SetCurrentCursorPos(62, 7);
					}
				}
				if ((p.X == 62) && (p.Y + 2 == 15)) {
					Room9_message(1);
					SetCurrentCursorPos(62, 13);
				}
				if ((p.X == 58) && (p.Y + 2 == 15)) {
					Room9_message(5);
					SetCurrentCursorPos(58, 13);
				}
			}
		}
	}
	else if (currentRoomNumber == 8) {
		Map[8][6][27] = 2;
		Map[8][7][27] = 2;
		if (right&& save.X == 64 && (save.Y == 10 || save.Y == 11)) {
			if (!mini5_end) not_open_door(); //문이 열리지 않는다 출력
			else {
				Map[8][6][27] = 2;
				Map[8][7][27] = 2;
				Room8_message(4); //문이 열렸다 //아이템 리스트 재생성
				lp = searchNode(8, 0);
				DeleteNode(lp);
				DeleteList();
				PrintList();
				//문없애기
				SetCurrentCursorPos(64, 10);
				printf("  ");
				SetCurrentCursorPos(64, 11);
				printf("  ");
				//기타설정
				mini5_disappear = 1;
				Map[8][6][27] = 0;
				Map[8][7][27] = 0;
				can_Entrance_Check[6] = 1;
			}
		}
		else {
			Map[8][6][27] = 2;
			Map[8][7][27] = 2;
		}
		if (mini5_key == 80) Room8_Check_80();
		if (mini5_key == 81) Room8_Check_81();
		if (mini5_key == 82) Room8_Check_82();
		if (mini5_key == 83) Room8_Check_83();
		if (mini5_key == 84) Room8_Check_84();
		if (mini5_key == 85) Room8_Check_85();
		if (mini5_key == 86) Room8_Check_86();
		if (mini5_key == 87) Room8_Check_87();
		if (mini5_key == 88) Room8_Check_88();
		if (mini5_key == 89) Room8_Check_89();
	}
	else if (currentRoomNumber == 9) {
		//printf("%d %d", p.X, p.Y);
		//쇼파: ▩ 30,15 - 대화창: 1
		//컴퓨터: @ 62,8 - 대화창: 2꺼짐
		//책상서랍: □ 58,8 - 대화창: 3
		//신문: ▥ 58,15 - 대화창: 4
		//베개: ■ 62,15 - 대화창: 5
		//원서책: ▦ 50,8 - 대화창: 6

		if (left) {
			if ((p.X - 2 == 30) && (p.Y == 15)) {
				Room9_message(1);
				SetCurrentCursorPos(32, 15);
			}
			if ((p.X - 2 == 58) && (p.Y == 8)) {
				Room9_message(3);
				SetCurrentCursorPos(60, 8);
			}
			if ((p.X - 2 == 62) && (p.Y == 8)) {
				if (lever) final();//문제내기
				else Room9_message(2); //컴퓨터 꺼저있다
				SetCurrentCursorPos(64, 8);
			}
		}
		if (right) {
			if ((p.X + 2 == 30) && (p.Y == 15)) {
				Room9_message(1);
				SetCurrentCursorPos(28, 15);
			}
			if ((p.X + 2 == 58) && (p.Y == 8)) {
				Room9_message(3);
				SetCurrentCursorPos(56, 8);
			}
			if ((p.X + 2 == 62) && (p.Y == 8)) {
				if (lever) final();//문제내기
				else Room9_message(2); //컴퓨터 꺼저있다
				SetCurrentCursorPos(60, 8);
			}
			if ((p.X + 4 == 58) && (p.Y == 15)) {
				Room9_message(4);
				SetCurrentCursorPos(54, 15);
			}
		}
		if (up) {
			if ((p.X == 30) && (p.Y - 1 == 15)) {
				Room9_message(1);
				SetCurrentCursorPos(30, 16);
			}
			if ((p.X == 58) && (p.Y - 1 == 8)) {
				Room9_message(3);
				SetCurrentCursorPos(58, 9);
			}
			if ((p.X == 62) && (p.Y - 1 == 8)) {
				if (lever) final();//문제내기
				else Room9_message(2); //컴퓨터 꺼저있다
				SetCurrentCursorPos(62, 9);
			}
		}
		if (down) {
			if ((p.X == 30) && (p.Y + 1 == 15)) {
				Room9_message(1);
				SetCurrentCursorPos(30, 14);
			}
			if ((p.X == 58) && (p.Y + 1 == 8)) {
				Room9_message(3);
				SetCurrentCursorPos(58, 7);
			}
			if ((p.X == 62) && (p.Y + 1 == 8)) {
				if (lever) final();//문제내기
				else Room9_message(2); //컴퓨터 꺼저있다
				SetCurrentCursorPos(62, 7);
			}
			if ((p.X == 62) && (p.Y + 2 == 15)) {
				Room9_message(5);
				SetCurrentCursorPos(62, 13);
			}
			if ((p.X == 58) && (p.Y + 2 == 15)) {
				Room9_message(4);
				SetCurrentCursorPos(58, 13);
			}
		}
	}
	//10번방: 갈래길(x)
}

void DrawStartDisplay()//시작 메뉴 그리는 함수!
{
	int i, j;
	int x, y;
	int key;
	system("title soRRow"); //콘솔창 제목
	system("mode con cols=112 lines=33"); //콘솔창 크기 조절: cols=칸/행(가로)   lines=줄/열(세로)
	startBGM();
	//게임 실행 인터페이스
	for (y = 0; y <= START_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┌");
		else if (y == START_HEIGHT) printf("└");
		else printf("│");
	}
	for (y = 0; y <= START_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (START_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == 0) printf("┐");
		else if (y == START_HEIGHT) printf("┘");
		else printf("│");
	}
	for (x = 1; x < (START_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y + START_HEIGHT);
		printf("─");
	}
	for (x = 1; x < (START_WIDTH + 1) * 2; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x, GBOARD_ORIGIN_Y);
		printf("─");
	}

	Gray();
	for (j = 0; j < 8; j++) {
		SetCurrentCursorPos(START_WIDTH / 2, START_HEIGHT / 2 + j - allign);
		printf(startUI[j]);
		printf("\n");
	}
	SetCurrentCursorPos(START_ORIGIN_X + START_WIDTH / 2, START_ORIGIN_Y + START_HEIGHT / 2);
	printf("Press P To Start");
	SetCurrentCursorPos(START_ORIGIN_X + START_WIDTH / 2, 30);
	RemoveCursor();

	while (1) {
		key = _getch();
		if (key == bp || key == sp) {
			//stopBGM();
			break;
		}
	}
	system("cls");
	Sleep(100);
}

void ProcessKeyInput()
{
	int key, i;
	key = sleep_input(1);
	switch (key) {
	case LEFT:
		ShiftLeft();
		left = 1;
		right = 0;
		up = 0;
		down = 0;
		zerocnt++;
		break;
	case RIGHT:
		ShiftRight();
		left = 0;
		right = 1;
		up = 0;
		down = 0;
		zerocnt++;
		break;
	case UP:
		ShiftUp();
		left = 0;
		right = 0;
		up = 1;
		down = 0;
		zerocnt++;
		break;
	case DOWN:
		ShiftDown();
		left = 0;
		right = 0;
		up = 0;
		down = 1;
		zerocnt++;
		break;
	case SPACEBAR:
		Item_Dis();
		break;
	}
	if (zerocnt == 8) {
	Zero(3, 3);
	zerocnt++;
	}
}

int main()
{
	DrawStartDisplay();
	RemoveCursor();
	GetCurrentCursorPos(player);
	Prologue();

	currentRoomNumber = 0; //현재 방 번호
	DrawGameUI(Map);

	SetCurrentCursorPos(player.X, player.Y);
	printf("▲");
	Interface();
	Zero(0, 2);
	L = create_Linklist_h();

	Quiz1_Random(); //숫자퀴즈 랜덤생성
	Quiz2_Random(); //암호학 랜덤생성
	mini5_key = random_keynumber(); //타임어택 랜덤 키 생성
	SoundTrack(currentRoomNumber);

	while (1) {
		DrawRoomNumber();
		if (currentRoomNumber != 7) Check_exit_door();
		if (currentRoomNumber == 7 || currentRoomNumber == 10 || currentRoomNumber == 11)
			Check_entrance_door();
		if (currentRoomNumber == 4) {
			mini_lock_door();
			if (LifeLimit == 0) DrawGameOverDisplay();
		}
		if (currentRoomNumber == 5) mini_lock_door();
		if (currentRoomNumber == 8) {
			mini_lock_door();
			if (!clock_reset) { //시간 초기화
				clock_reset = 1;
				start = clock();
			}
			timer();
		}
		//재시작: 갈래길로 리로딩
		if (mini4_load || mini5_load) { //재시작: 갈래길로 리로딩
			currentRoomNumber = 10;
			Interface();
			DrawGameUI(Map);
			if (mini4_load) {
				player.X = 40;
				player.Y = 12;
				SetCurrentCursorPos(player.X, player.Y);
				printf("◀");
				mini4_load = 0;
			}
			if (mini5_load) {
				player.X = 48;
				player.Y = 12;
				SetCurrentCursorPos(player.X, player.Y);
				printf("▶");
				mini5_load = 0;
			}
		}
		if (currentRoomNumber == 9) {
			if (finish) { //게임 완전히 클리어
				RemoveCursor();
				system("cls"); //화면 초기화
				Epilogue();
			}
		}
		ProcessKeyInput();
	}
	return 0;
}


//함수 재정의
void DrawGameOverDisplay()//게임 오버 그리는 함수
{
	int i, j;
	int x, y;
	int key;
	LN* lp = NULL;

	system("title soRRow"); //콘솔창 제목
	system("mode con cols=112 lines=33"); //콘솔창 크기 조절: cols=칸/행(가로)   lines=줄/열(세로)
	//게임 실행 인터페이스
	Gray();
	for (j = 0; j < 20; j++) {
		SetCurrentCursorPos(START_WIDTH / 2, START_HEIGHT / 2 + j - allign);
		printf(GameOverUI[j]);
		printf("\n");
	}
	Sleep(1000);
	//재시작할건지 띄우기
	SetCurrentCursorPos(START_ORIGIN_X + START_WIDTH / 2 - 14, 3);
	printf("다시 도전하시겠습니까?: Y/N (키보드를 눌러주세요)");
	while (1) {
		key = _getch();
		if (key == by || key == sy) {//재시작 누른다면
			if (mini4_die) { //4번방에서 죽었으면
				//4번방 변수 모두 초기화
				talk_to_superior = 0; //상사와 대화 했는지 확인(이전과 이후 대화창 출력 다르게)
				mini4_finish = 0; //4번방 게임 통과 여부 확인
				mini4_door = 0; //4번방 들어가면 문 잠그기
				mini4_come = 0; //4번방에 완전히 들어왔는지 확인
				mini4_print = 0; //들어갈 수 없다 출력했는지 확인
				mini4_disappear = 0; //문잠금 삭제했는지 확인
				mini4_die = 0; //4번방에서 죽었는지 확인
				LifeLimit = 2; //목숨
				mini4_load = 0; //4번방 앞으로 로드하기
				Map[4][13][17] = 0; //문잠금 초기화
				Map[4][13][18] = 0; //문잠금 초기화
				mini4_load = 1; //4번방 앞으로 로드하기
			}
			if (mini5_die) { //8번방에서 죽었으면
				//5번+8번방 변수 모두 초기화
				mini5_finish = 0; //5번방 게임 통과 여부 확인
				mini5_door = 0; //5번방 들어가면 문 잠그기
				mini5_come = 0; //5번방에 완전히 들어왔는지 확인
				mini5_print = 0; //들어갈 수 없다 출력했는지 확인
				mini5_disappear = 0; //문잠금 삭제했는지 확인
				mini5_start = 0; //미니게임 시작 전 열쇠 주우면 1로 변경
				mini5_end = 0; //미니게임 중 열쇠 획득하면 1로 변경
				mini5_die = 0; //8번방에서 죽었는지 확인
				times = 20; //타임어택 시간(15초)
				time_start = 0; //타임어택 시간초 시작
				mini5_key = 0; //랜덤 키 생성
				mini5_notcome = 0; //게임클리어 이후 다시 8번방 진입하지 못하도록
				mini5_load = 1; //5번방 앞으로 로드하기
				clock_check = 0; //시간 확인
				clock_reset = 0; //시간 초기화
				lp = searchNode(8, 0); //8번방 열쇠 리스트 초기화
				DeleteNode(lp);
				DeleteList();
				lp = searchNode(5, 0); //5번방 열쇠 리스트 초기화
				DeleteNode(lp);
				DeleteList();
				PrintList();
				Map[8][6][27] = 0;
				Map[8][7][27] = 0;
			}
			system("cls");
			break;
		}
		if (key == bn || key == sn) {//재시작 안누르면
			system("cls");
			exit(0);
		}
	}
}