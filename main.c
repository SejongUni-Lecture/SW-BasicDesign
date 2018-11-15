#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"tutorial.h"

#define UP 119 //W
#define LEFT 97 //A
#define DOWN 115 //S
#define RIGHT 100 //D

COORD player = { 10,5 };

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

void DrawGameUI(char wall[][16])
{
	int i, j;
	for (i = 0; i < 16; i++){
		for (j = 0; j < 11; j++){
			if (wall[j][i] == 1){
				SetCurrentCursorPos(6 + i * 2, 2 + j);
				printf("¡á");
			}
			if (wall[j][i] == 2) {
				SetCurrentCursorPos(6 + i * 2, 2 + j);
				printf("¢Ã");
			}
			if (wall[j][i] == 3) {
				SetCurrentCursorPos(6 + i * 2, 2 + j);
				printf("¡Ï");
			}
		}
	}
}

void ShiftLeft()
{
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X -= 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("¢¸");
}

void ShiftRight()
{
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.X += 2;
	SetCurrentCursorPos(player.X, player.Y);
	printf("¢º");
}

void ShiftUp()
{
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y -= 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("¡ã");
}

void ShiftDown()
{
	SetCurrentCursorPos(player.X, player.Y);
	printf("  ");
	player.Y += 1;
	SetCurrentCursorPos(player.X, player.Y);
	printf("¡å");
}

void ProcessKeyInput()
{
	//int key = _getch();
	int key;
	int i;
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
}

int main()
{
	RemoveCursor();
	DrawGameUI(tutorial);
	GetCurrentCursorPos(player);
	while (1) {
		ProcessKeyInput();
		DrawGameUI(tutorial);
	}
	return 0;
}