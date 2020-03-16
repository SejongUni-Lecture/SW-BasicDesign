#pragma once
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

void startBGM() //DrawStartDisplay()안에 있음-경남
{
	//각자 저장되어 있는 파일 경로로 수정해서 해보심 될 것 같습니돠
	//현재 설정되어 있는 건 주석처리하고 아래 주석된 함수 수정해서 사용해주세염-서영

	//PlaySound(TEXT("C:\\Users\\김예찐\\Desktop\\최신본\\최신본\\Game Project2\\Debug\\Wadjdas.wav"),NULL,SND_ASYNC|SND_LOOP); //서영
	PlaySound(TEXT("Wadjdas.wav"), NULL, SND_ASYNC | SND_LOOP); //경남(보내준거 그대로 실행한거라면 경로 안바꿔도 됩니다요)
}

void stopBGM()
{
	PlaySound(NULL, 0, 0);
}

void shotBGM()
{
	PlaySound(TEXT("gun_shot.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
}

void illusionBGM()
{
	PlaySound(TEXT("Ocean.wav"), NULL, SND_ASYNC | SND_LOOP);
}

void stageBGM()
{
	PlaySound(TEXT("Promise.wav"), NULL, SND_ASYNC | SND_LOOP);
}

void endBGM()
{
	PlaySound(TEXT("Promise2.wav"), NULL, SND_ASYNC | SND_LOOP);
	//PlaySound(TEXT("Promise2.wav"), NULL, SND_ASYNC | SND_LOOP);
}

void overBGM()
{
	PlaySound(TEXT("over.wav"), NULL, SND_ASYNC | SND_LOOP);
	//PlaySound(TEXT("over.wav"), NULL, SND_ASYNC | SND_LOOP);
}

void timeBGM()
{
	PlaySound(TEXT("basebeat.wav"), NULL, SND_ASYNC | SND_LOOP);
	//PlaySound(TEXT("over.wav"), NULL, SND_ASYNC | SND_LOOP);
}