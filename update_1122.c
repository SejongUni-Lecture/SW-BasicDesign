void DrawRoomFrame(char Map[][17][36],int x, int y)
{	//x=i, y=j

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
			if (currentRoomNumber == 0)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map,i,j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▣");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("♀");
				}
			}

			if (currentRoomNumber == 1)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("⊙");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("★");
				}


				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}

			if (currentRoomNumber == 2)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Ω");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("∮");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("￥");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("★");
				}

				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					//필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111111111111111111
					//각 방마다 크기가 다르니 중앙에 배치하도록 수식을 넣을 예정입니당 -혜원
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┘");
				}
			}

			if (currentRoomNumber == 3)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┬");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┼");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┴");
				}
				if (Map[currentRoomNumber][j][i] == 7 || Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("○");
				}
				if (Map[currentRoomNumber][j][i] == 9 || Map[currentRoomNumber][j][i] == 10 || Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("ㆀ");
				}
			}

			if (currentRoomNumber == 4) 
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Γ");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("●");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("◆");
				}
				if (Map[currentRoomNumber][j][i] == 8) {
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
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("□");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("Ф");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}
				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
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
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("↖");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("☎");
				}
			}

			if (currentRoomNumber == 9)
			{
				if (Map[currentRoomNumber][j][i] == 1) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					DrawRoomFrame(Map, i, j);
				}
				if (Map[currentRoomNumber][j][i] == 2) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("@");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▩");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▥");
				}

				if (Map[currentRoomNumber][j][i] == 7) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("▤");
				}

				if (Map[currentRoomNumber][j][i] == 8) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("★");
				}


				if (Map[currentRoomNumber][j][i] == 10) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 11) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 12) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 13) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 14) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("└");
				}
				if (Map[currentRoomNumber][j][i] == 15) {
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
					printf("┌");
				}
				if (Map[currentRoomNumber][j][i] == 3) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("─");
				}
				if (Map[currentRoomNumber][j][i] == 4) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("┐");
				}
				if (Map[currentRoomNumber][j][i] == 5) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
					printf("│");
				}
				if (Map[currentRoomNumber][j][i] == 6) {
					SetCurrentCursorPos(GBOARD_ORIGIN_X + 9 + i * 2, GBOARD_ORIGIN_Y + 2 + j);
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
