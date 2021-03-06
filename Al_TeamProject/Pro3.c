#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define USER 1
#define AI 2
#define ROOM 3
#define NOROOM 4
#define FAR 5
#define NOFAR 6
#define END 7
#define YES 10
#define NO 11

char g_stone[7][7] = { 0 };
int g_save[128] = { 0 };
int g_AIPoint = 0;
int g_USERPoint = 0;
int g_recentPlayer = 0;
int g_stonPos = 0;
int g_saveNum = 0;

void display();
int addStone();
void updateStone();
void makePlatform();
void aiPlayer();
void feedBack();
int stonePos();
int findRoom();

// 메인 ==============================================================
// =================================================================
void main() {
	int i = 0, k = 0, select = 0;

	while (1) {
		system("cls");
		display("\t1. 바둑게임 시작");
		display("\t2. 복기 시작");
		display("\t3. 시스템 종료");

		fprintf(stderr, "당신의 선택은? : ");
		scanf("%d", &select);
		
		switch (select) {
		case 1:
			for (i = 0; i<7; i++) {
				for (k = 0; k<7; k++) {
					g_stone[i][k] = 0;
				}
			}
			while (1) {
				makePlatform();
				if (addStone() == END) break;
				updateStone();

				makePlatform();
				aiPlayer();
				updateStone();

				makePlatform();
			}
			break;
		case 2:
			for (i = 0; i<7; i++) {
				for (k = 0; k<7; k++) {
					g_stone[i][k] = 0;
				}
			}
			feedBack();
			break;
		case 3:
			exit(1);
			break;
		}
	}
	
}


void display(char *p_message) {
	fputs("===================================================\n", stderr);
	fputs(p_message, stderr);
	fputc('\n', stderr);
	fputs("===================================================\n", stderr);
}


int addStone() {
	int x = -1, y = -1, i = 0;
	static int backcount = 0;
	static int frist = YES;
	g_recentPlayer = USER;

	while (1) {
		printf(" X좌표를 입력하세요 : ");
		scanf("%d", &x);
		printf(" Y좌표를 입력하세요 : ");
		scanf("%d", &y);

		if (x == 9 && y == 9) {
			display("사용자가 종료를 선택했습니다.");
			return END;
		}

		if (frist == YES) {
			if (x != 0 && x != 6) continue;
			else if (y != 0 && y != 6) continue;
		}
		frist = NO;
		
		if (x == 8 && y == 8) {
			if (backcount == 2) continue;
			else{
				for (i = 0; i < 2; i++) {
					x = (g_save[--g_saveNum] / 100) % 10;
					y = (g_save[g_saveNum] / 10) % 10;
					g_stone[x][y] = 0;
					g_save[g_saveNum] = 0;
				}
				backcount++;
				updateStone();
				makePlatform();
				continue;
			}
		}

		backcount = 0;
		y = 6 - y;
		if (g_stone[x][y] == 0) {
			if (x >= 0 && x<7 && y >= 0 && y<7) {
				g_stone[x][y] = 1;
				g_save[g_saveNum++] = 1000 + x * 100 + y * 10 + 1;
				return 0;
			}
		}
		else continue;
	}
}


void updateStone() {
	int com = AI;
	int user = USER;
	int i = 0, j = 0, k = 0, round, del, data;
	int roomData[49] = { -1 };

	for (round = 1; round <= 2; round++) {  
		for (i = 0; i<7; i++) {
			for (k = 0; k<7; k++) {
				if (g_stone[i][k] == 0) continue; 
				if (g_stone[i][k] != g_recentPlayer) continue; 
				if (g_stone[i][k] != 0) stonePos(g_stone[i][k], i, k, roomData, 0); 

				//isRoom = findRoom(roomData); // 그곳이 집인지 판단
				if (findRoom(roomData) == ROOM) { // 집인경우
					for (del = 0; del<49; del++) { // 해당 리스트에 있는 돌들을 삭제
						if (roomData[del] == -1) continue;
						data = g_stone[roomData[del] / 10][roomData[del] % 10];
						switch (data) {
						case 1:
							g_AIPoint++; break;
						case 2:
							g_USERPoint++; break;
						}
						g_stone[roomData[del] / 10][roomData[del] % 10] = 0;
						g_save[g_saveNum++] = 1000 + (roomData[del] / 10) * 100 + (roomData[del] % 10) * 10 + 0;
					}
				}
				for (j = 0; j<49; j++) roomData[j] = -1;
				g_stonPos = 0;
			}
		}
		g_recentPlayer = (g_recentPlayer == user) ? com : user;
	}
}


void makePlatform() {
	int i = 0, j = 0, k = 0;
	int platformWidth = 14;
	int platformHeight = 14;

	system("cls");
	fprintf(stderr, "\n\t<점수> 당신 : %d AI : %d", g_USERPoint, g_AIPoint);
	fputs("\n===================================================\n", stderr);

	printf(" X → 0   1   2    3   4   5   6\n");
	for (i = 0; i<platformWidth + 2; i++) {
		if (i == 0) printf(" Y┌");
		if (i == platformWidth + 1) {
			printf("┐");
			break;
		}
		if (i % 2 == 1) printf("┬");
		else printf("─");
	}
	printf("\n"); printf("↓│");

	for (k = 0; k<platformWidth + 3; k++) {
		if (k % 2 == 1) printf("│");
		else printf("  ");
	}
	printf("\n");

	for (i = 0; i<platformHeight / 2; i++) {
	//for (i = platformHeight / 2-1; i>=0; i--) {
		printf(" %d├", 6 - i);
		for (j = 0; j<platformHeight / 2; j++) {
			printf("─");

			if (g_stone[j][i] == 0) printf("┼");
			else if (g_stone[j][i] == 1) printf("○");
			else printf("●");
		}
		printf("─┤\n");

		printf("  │");
		for (k = 0; k<platformWidth + 3; k++) {
			if (k % 2 == 1) printf("│");
			else printf("  ");
		}
		printf("\n");
	}

	for (i = 0; i<platformWidth + 2; i++) {
		if (i == 0) printf("  └");
		if (i == platformWidth + 1) {
			printf("┘");
			break;
		}
		if (i % 2 == 1) printf("┴");
		else printf("─");
	}
	printf("\n");
	display("\t<종료방법> (9, 9)를 입력 하세요.\n\t<무르기> (8, 8)을 입력 하세요.\n\t<게임룰> 첫수에 가장자리만 가능합니다.");
}


int stonePos(int p_stoneNum, int x, int y, int *roomData, int checkRoot) {

	// p_stoneNum : 돌색(1 or 2)
	// x, y : 좌표
	// checkRoot : 재귀 이용했으므로 갔던길을 되돌아 오지 않도록 하는 변수.
	//    4 
	//  1   3  으로 지정하여 처리함
	//    2
	// *location : 갔던길을 저장하는 변수

	roomData[g_stonPos++] = x * 10 + y;

	if (x - 1 >= 0 && checkRoot != 1) {
		if (g_stone[x - 1][y] == p_stoneNum) stonePos(g_stone[x - 1][y], x - 1, y, roomData, 3);
	}

	if (y + 1 < 7 && checkRoot != 2) {
		if (g_stone[x][y + 1] == p_stoneNum) stonePos(g_stone[x][y + 1], x, y + 1, roomData, 4);
	}

	if (x + 1 < 7 && checkRoot != 3) {
		if (g_stone[x + 1][y] == p_stoneNum) stonePos(g_stone[x + 1][y], x + 1, y, roomData, 1);
	}

	if (y - 1 >= 0 && checkRoot != 4) {
		if (g_stone[x][y - 1] == p_stoneNum) stonePos(g_stone[x][y - 1], x, y - 1, roomData, 2);
	}
	return 0;

}


int findRoom(int *p_roomPos) {
	int i = 0, x = -1, y = -1;
	int playerA, playerB;

	for (i = 0; i<49; i++) {
		if (p_roomPos[i] == -1)  continue;

		x = p_roomPos[i] / 10;
		y = p_roomPos[i] % 10;
		playerA = g_stone[x][y];

		playerB = (playerA == 1) ? 2 : 1;
		//if (myType == 1) yourType = 2;
		//if (myType == 2) yourType = 1;

		// 4방향중 0(=빈칸)이 존재한다면 그것은 집이 아니다.
		if ((g_stone[x - 1][y] == 0) && (x - 1 >= 0)) return NOROOM;
		if ((g_stone[x + 1][y] == 0) && (x + 1 < 5)) return NOROOM;
		if ((g_stone[x][y - 1] == 0) && (y - 1 >= 0)) return NOROOM;
		if ((g_stone[x][y + 1] == 0) && (y + 1 < 5)) return NOROOM;
	}
	return ROOM;
}

void aiPlayer() {
	int isFar = NOFAR;
	int blackCnt = 0;
	int maxBlackInfo = 100;
	// ㄴ 125 : 2위치까지 검은색이 5개 있었다는 의미. 
	//    9칸의 정사각형 바둑판 기준으로, 가장 왼쪽위가 1번째자리를 시작으로 반시계반향으로 돌면서 숫자가 커짐. 최대치 8
	long seed = (long)time(NULL);
	int ranX = -1, ranY = -1;
	int i = 0;

	int recentX = (g_save[g_saveNum - 1] / 100) % 10; // 1052 : x, y, type(0:빈칸,1:흑,2:백) 순서 1000은 더미
	int recentY = (g_save[g_saveNum - 1] / 10) % 10;
	int befroeRecentX = -1, befroeRecentY = -1;

	g_recentPlayer = AI;
	Sleep(2000);
	// 3개 이상의 수가 놓여 졌을 때
	if (g_saveNum - 3 >= 0) {
		befroeRecentX = (g_save[g_saveNum - 3] / 100) % 10;
		befroeRecentY = (g_save[g_saveNum - 3] / 10) % 10;
	}

	if (g_saveNum != 1) { // 동 떨어진덴다가 둘 경우의 사전 설정
		if (recentX - 1 != befroeRecentX || recentX + 1 != befroeRecentX || recentY - 1 != befroeRecentY || recentY + 1 != befroeRecentY) { // 사람이 두 돌을 인접하게 두지 않았다면
			recentX = befroeRecentX;
			recentY = befroeRecentY;
			isFar = FAR;
		}
	}

	if (g_saveNum == 1 || isFar == FAR) { // 맨 처음이거나 사람이 동떨어진데다 둘 경우의 처리
		if (recentX - 1 >= 0) {
			if (g_stone[recentX - 1][recentY] == 0) {
				g_stone[recentX - 1][recentY] = 2;
				g_save[g_saveNum++] = 1000 + (recentX - 1) * 100 + (recentY)* 10 + 2;
				return;
			}
			else if (g_stone[recentX][recentY - 1] == 0) {
				g_stone[recentX][recentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (recentX)* 100 + (recentY - 1) * 10 + 2;
				return;
			}
		}
		
		if (recentX + 1 <= 7) {
			if (g_stone[recentX + 1][recentY] == 0) {
				g_stone[recentX + 1][recentY] = 2;
				g_save[g_saveNum++] = 1000 + (recentX + 1) * 100 + (recentY)* 10 + 2;
				return;
			}
			else if (g_stone[recentX][recentY + 1] == 0) {
				g_stone[recentX][recentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (recentX)* 100 + (recentY + 1) * 10 + 2;
				return;
			}
		}
	}

	if (g_saveNum != 1 && isFar == NOFAR) { // 이전의 둔 곳과 비슷한 곳에 둔 경우의 처리 
		// 최근의 컴퓨터가 둔 돌 기준으로, 사람이 둔 돌이 많은 쪽 옆에다가 돌을 두도록 한다. 
		if (g_saveNum - 2 >= 0) {
			befroeRecentY = (g_save[g_saveNum - 2] / 10) % 10;
			befroeRecentX = (g_save[g_saveNum - 2] / 100) % 10;

			if (g_stone[befroeRecentX - 1][befroeRecentY - 1] == 1 && g_stone[befroeRecentX - 1][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 10 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX - 1][befroeRecentY] == 1 && g_stone[befroeRecentX - 1][befroeRecentY] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 20 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX - 1][befroeRecentY + 1] == 1 && g_stone[befroeRecentX - 1][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 30 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX][befroeRecentY + 1] == 1 && g_stone[befroeRecentX][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 40 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY + 1] == 1 && g_stone[befroeRecentX + 1][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 50 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY] == 1 && g_stone[befroeRecentX + 1][befroeRecentY] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 60 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY - 1] == 1 && g_stone[befroeRecentX + 1][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 70 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX][befroeRecentY - 1] == 1 && g_stone[befroeRecentX][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 80 + blackCnt;
			}
			else blackCnt = 0;
			// 해당돌 왼쪽부터 시작하여 아래쪽으로 한바퀴 돌면서 체크함

			switch ((maxBlackInfo / 10) % 10) {
			case 0:
				break;
			case 1:
				g_stone[befroeRecentX - 1][befroeRecentY] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY)* 10 + 2;
				return;
			case 2:
				g_stone[befroeRecentX - 1][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 3:
				g_stone[befroeRecentX][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX)* 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 4:
				g_stone[befroeRecentX + 1][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 5:
				g_stone[befroeRecentX + 1][befroeRecentY] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY)* 10 + 2;
				return;
			case 6:
				g_stone[befroeRecentX + 1][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY - 1) * 10 + 2;
				return;
			case 7:
				g_stone[befroeRecentX][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX)* 100 + (befroeRecentY - 1) * 10 + 2;
				return;
			case 8:
				g_stone[befroeRecentX - 1][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY - 1) * 10 + 2;
				return;

			}
		}
	}

	// 이도저도 아닌 경우, 랜덤한, 빈칸의, 새로운 위치에 돌을 둔다.
	srand(seed);

	for (i = 0; i<49; i++) {
		ranX = rand() % 5;
		ranY = rand() % 5;

		if (g_stone[ranX][ranY] == 0) {
			if (g_stone[ranX - 1][ranY] != 1 && g_stone[ranX][ranY - 1] != 1 && g_stone[ranX][ranY + 1] != 1 && g_stone[ranX + 1][ranY] != 1) {
				g_stone[ranX][ranY] = 2;
				g_save[g_saveNum++] = 1000 + (ranX)* 100 + (ranY)* 10 + 2;
				return;
			}
		}
	}
}


void feedBack() {
	int select = 0;
	int x = 0, y = 0, i = 0, k = 0, j = 0;
	
	while (select != 2) {
		for (j = 0; j<7; j++) {
			for (k = 0; k<7; k++) {
				g_stone[j][k] = 0;
			}
		}
		while (g_save[i] > 0) {
			x = (g_save[i] / 100) % 10;
			y = (g_save[i] / 10) % 10;
			g_stone[x][y] = g_save[i] % 10;
			
			makePlatform();
			Sleep(1000);
			i++;
		}
		display("1. 한번더 본다.\t2.그만 본다");
		fprintf(stderr, "당신의 선택은? : ");
		scanf("%d", &select);
	}

}
