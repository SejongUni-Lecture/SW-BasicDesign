#pragma once
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>

void startBGM() //DrawStartDisplay()�ȿ� ����-�泲
{
	//���� ����Ǿ� �ִ� ���� ��η� �����ؼ� �غ��� �� �� �����ϵ�
	//���� �����Ǿ� �ִ� �� �ּ�ó���ϰ� �Ʒ� �ּ��� �Լ� �����ؼ� ������ּ���-����

	//PlaySound(TEXT("C:\\Users\\�迹��\\Desktop\\�ֽź�\\�ֽź�\\Game Project2\\Debug\\Wadjdas.wav"),NULL,SND_ASYNC|SND_LOOP); //����
	PlaySound(TEXT("Wadjdas.wav"), NULL, SND_ASYNC | SND_LOOP); //�泲(�����ذ� �״�� �����ѰŶ�� ��� �ȹٲ㵵 �˴ϴٿ�)
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