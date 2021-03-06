#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define USER 1;
#define AI 2;

// 선공 선방 선택하는 룰 만들기
char g_stone[5][5] = {0};
int g_save[128] = {0};
int g_AIPoint = 0;
int g_USERPoint = 0;
int recentPlayer = 0;
int infoCnt = 0;
int saveCnt = 0;

void display(char *p_message);
void addStone();
void deleteStone();
void makePlatform();
void robot();
int findStone(int g_stoneNum, int x, int y, int notGo, int *locationInfo);
int findHouse(int *location);
int isFar(int lastX, int lastY, int lastX_2nd, int lastY_2nd);

// 메인 ============================================================================
// ===============================================================================
void main() {
	int i = 0, k = 0;

	for (i = 0; i<5; i++) {
		for (k = 0; k<5; k++) {
			g_stone[i][k] = 0;
		}
	}
	makePlatform();

	while (1) {
		addStone();
		delStone();
		makePlatform();

		robot();
		makePlatform();
		delStone();
		makePlatform();
	}
	for (i = 0; i<5; i++) {
		for (k = 0; k<5; k++) {
			g_stone[i][k] = 0;
		}
	}

}


void display(char *p_message) {
	fputs("\n===================================\n",stderr);
	fputs(p_message, stderr);
	fputc('\n', stderr);
	fputs("===================================\n", stderr);
}


void addStone() {
	int x = -1;
	int y = -1;
	//static int i = 0;

	recentPlayer = USER;

	//for (; i<50; i++) {
	while (1) {
		printf(" X좌표를 입력하세요 : ");
		scanf("%d", &x);
		printf(" Y좌표를 입력하세요 : ");
		scanf("%d", &y);


		if (x == 6 && y == 6) {
			display("사용자가 종료를 선택했습니다.");
			exit(1);
		}

		if (g_stone[x][y] == 0) {
			if (x >= 0 && x<5 && y >= 0 && y<5) {
				g_stone[x][y] = 1;
				g_save[saveCnt++] = 1000 + x * 100 + y * 10 + 1;
				break;
			}
		}
		else continue;
	}
	//}

	//if (i >= 50) {
	//	display("최대 턴 초과로 인한 종료");
	//	exit(1);
	//}
}


void deleteStone() {
	int i, k, isHouse, round, del, tmp;
	int locationInfo[25] = { -1 };

	int com = AI;
	int user = USER;

	for (round = 1; round <= 2; round++) { // 돌 둔 사람을 먼저 처리해주고 나머지를 처리해주므로 총 2번 처리해야함 
		for (i = 0; i<5; i++) {
			for (k = 0; k<5; k++) {

				if (g_stone[i][k] == 0) continue; // 빈칸이 선택되었다면 계산할 가치가 없다.
				if (g_stone[i][k] != recentPlayer) continue; // 지금 돌을 막 둔사람의 돌을 먼저 처리해줌


				if (g_stone[i][k] != 0) {

					findStone(g_stone[i][k], i, k, 0, locationInfo); // 해당 위치의 돌색과 같으면서 연결된 돌들의 위치를 배열에 저장해줌.
				}

				/*
				for(tmp=0; tmp<25; tmp++)
				printf("[%d]\n", locationInfo[tmp]);
				*/

				isHouse = findHouse(locationInfo); // 그곳이 집인지 판단
				//printf("********************* %d (1집발견 0 집못발견) ***************\n", isHouse);

				printf("\n\n");

				if (isHouse == 1) { // 집인경우

					for (del = 0; del<25; del++) { // 해당 리스트에 있는 돌들을 삭제

						if (locationInfo[del] == -1) continue;

						if (g_stone[locationInfo[del] / 10][locationInfo[del] % 10] == 1)
							g_AIPoint++;
							//comGet++;
						if (g_stone[locationInfo[del] / 10][locationInfo[del] % 10] == 2)
							g_USERPoint++;
							//userGet++;

						g_stone[locationInfo[del] / 10][locationInfo[del] % 10] = 0;
						g_save[saveCnt++] = 1000 + (locationInfo[del] / 10) * 100 + (locationInfo[del] % 10) * 10 + 0;



					}

				}



				for (tmp = 0; tmp<25; tmp++) // 다음 작업을 위한 초기화
					locationInfo[tmp] = -1;
				infoCnt = 0;

			}
		}

		if (recentPlayer == user)
			recentPlayer = com;
		else
			recentPlayer = user;

	}
}


void makePlatform() {
	int platformWidth = 10;
	int platformHeight = 10;
	int i = 0, j = 0, k = 0;

	system("cls");
	display("\t  <종료방법>\n\tX : 9 Y :9를 입력");
	fprintf(stderr, "\t    <점수>\n\t당신 : %d AI : %d", g_USERPoint, g_AIPoint);
	fputs("\n===================================\n\n", stderr);

	printf(" X → 0   1   2   3   4\n");
	for (i = 0; i<platformWidth + 2; i++) {
		if (i == 0) printf(" Y┌");
		if (i == platformWidth + 1) {
			printf("┐");
			break;
		}
		if (i % 2 == 1) printf("┬");
		else {
			printf("─");
		}

	}

	printf("\n"); printf("↓│");
	for (k = 0; k<platformWidth + 3; k++) {
		if (k % 2 == 1)
			printf("│");
		else
			printf("  ");
	}

	printf("\n");
	for (i = 0; i<platformHeight/2; i++) {
		printf(" %d├", i);
		for (j = 0; j<platformHeight/2; j++) {
			printf("─");

			if (g_stone[j][i] == 0) {
				printf("┼");
			}
			else if (g_stone[j][i] == 1) {
				printf("○");
			}
			else {
				printf("●");
			}
		}
		printf("─┤\n");

		printf("  │");
		for (k = 0; k<platformWidth + 3; k++) {
			if (k % 2 == 1)
				printf("│");
			else
				printf("  ");
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
		else {
			printf("─");
		}
	}
	printf("\n");
}


void robot() {
	int lastX = (g_save[saveCnt - 1] / 100) % 10; // 1052 : x, y, type(0:빈칸,1:흑,2:백) 순서 1000은 더미
	int lastY = (g_save[saveCnt - 1] / 10) % 10;

	int lastX_2nd = -1;
	int lastY_2nd = -1;
	int farChecked = 0;
	int i = 0, j = 0;
	int blackCnt = 0;
	int maxBlackInfo = 100;
	// ㄴ 125 : 2위치까지 검은색이 5개 있었다는 의미. 
	//    9칸의 정사각형 바둑판 기준으로, 가장 왼쪽위가 1번째자리를 시작으로 반시계반향으로 돌면서 숫자가 커짐. 최대치 8
	long seed = (long)time(NULL);
	int ranX = -1, ranY = -1;
	int w;

	recentPlayer = AI;
	Sleep(1000);

	if (saveCnt - 3 >= 0) {
		lastY_2nd = (g_save[saveCnt - 3] / 10) % 10;
		lastX_2nd = (g_save[saveCnt - 3] / 100) % 10;
	}

	if (saveCnt != 1) { // 동 떨어진덴다가 둘 경우의 사전 설정
		if (isFar(lastX, lastY, lastX_2nd, lastY_2nd)) {
			lastX = lastX_2nd;
			lastY = lastY_2nd;
			farChecked = 1;
		}
	}

	if (saveCnt == 1 || farChecked == 1) { // 맨 처음이거나 사람이 동떨어진데다 둘 경우의 처리
		if (lastX - 1 >= 0 && g_stone[lastX - 1][lastY] == 0) {
			g_stone[lastX - 1][lastY] = 2;
			g_save[saveCnt++] = 1000 + (lastX - 1) * 100 + (lastY)* 10 + 2;
			return;
		}
		if (lastY - 1 >= 0 && g_stone[lastX][lastY - 1] == 0) {
			g_stone[lastX][lastY - 1] = 2;
			g_save[saveCnt++] = 1000 + (lastX)* 100 + (lastY - 1) * 10 + 2;
			return;
		}
		if (lastX + 1 <= 4 && g_stone[lastX + 1][lastY] == 0) {
			g_stone[lastX + 1][lastY] = 2;
			g_save[saveCnt++] = 1000 + (lastX + 1) * 100 + (lastY)* 10 + 2;
			return;
		}
		if (lastY + 1 <= 4 && g_stone[lastX][lastY + 1] == 0) {
			g_stone[lastX][lastY + 1] = 2;
			g_save[saveCnt++] = 1000 + (lastX)* 100 + (lastY + 1) * 10 + 2;
			return;
		}
	}

	if (saveCnt != 1 && farChecked == 0) { // 이전의 둔 곳과 비슷한 곳에 둔 경우의 처리 
		// 최근의 컴퓨터가 둔 돌 기준으로, 사람이 둔 돌이 많은 쪽 옆에다가 돌을 두도록 한다. 
		if (saveCnt - 2 >= 0) {
			lastY_2nd = (g_save[saveCnt - 2] / 10) % 10;
			lastX_2nd = (g_save[saveCnt - 2] / 100) % 10;

			if (g_stone[lastX_2nd - 1][lastY_2nd - 1] == 1 && g_stone[lastX_2nd - 1][lastY_2nd - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 10 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd - 1][lastY_2nd] == 1 && g_stone[lastX_2nd - 1][lastY_2nd] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 20 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd - 1][lastY_2nd + 1] == 1 && g_stone[lastX_2nd - 1][lastY_2nd + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 30 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd][lastY_2nd + 1] == 1 && g_stone[lastX_2nd][lastY_2nd + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 40 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd + 1][lastY_2nd + 1] == 1 && g_stone[lastX_2nd + 1][lastY_2nd + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 50 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd + 1][lastY_2nd] == 1 && g_stone[lastX_2nd + 1][lastY_2nd] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 60 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd + 1][lastY_2nd - 1] == 1 && g_stone[lastX_2nd + 1][lastY_2nd - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 70 + blackCnt;
			}
			else blackCnt = 0;
			if (g_stone[lastX_2nd][lastY_2nd - 1] == 1 && g_stone[lastX_2nd][lastY_2nd - 1] != 2) {
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
				g_stone[lastX_2nd - 1][lastY_2nd] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd - 1) * 100 + (lastY_2nd)* 10 + 2;
				return;
			case 2:
				g_stone[lastX_2nd - 1][lastY_2nd + 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd - 1) * 100 + (lastY_2nd + 1) * 10 + 2;
				return;
			case 3:
				g_stone[lastX_2nd][lastY_2nd + 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd)* 100 + (lastY_2nd + 1) * 10 + 2;
				return;
			case 4:
				g_stone[lastX_2nd + 1][lastY_2nd + 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd + 1) * 100 + (lastY_2nd + 1) * 10 + 2;
				return;
			case 5:
				g_stone[lastX_2nd + 1][lastY_2nd] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd + 1) * 100 + (lastY_2nd)* 10 + 2;
				return;
			case 6:
				g_stone[lastX_2nd + 1][lastY_2nd - 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd + 1) * 100 + (lastY_2nd - 1) * 10 + 2;
				return;
			case 7:
				g_stone[lastX_2nd][lastY_2nd - 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd)* 100 + (lastY_2nd - 1) * 10 + 2;
				return;
			case 8:
				g_stone[lastX_2nd - 1][lastY_2nd - 1] = 2;
				g_save[saveCnt++] = 1000 + (lastX_2nd - 1) * 100 + (lastY_2nd - 1) * 10 + 2;
				return;

			}
		}
	}

	// 이도저도 아닌 경우, 랜덤한, 빈칸의, 새로운 위치에 돌을 둔다.
	srand(seed);

	for (w = 0; w<25; w++) {
		ranX = rand() % 5;
		ranY = rand() % 5;

		if (g_stone[ranX][ranY] == 0) {
			if (g_stone[ranX - 1][ranY] != 1 && g_stone[ranX][ranY - 1] != 1 && g_stone[ranX][ranY + 1] != 1 && g_stone[ranX + 1][ranY] != 1) {
				g_stone[ranX][ranY] = 2;
				g_save[saveCnt++] = 1000 + (ranX)* 100 + (ranY)* 10 + 2;
				return;
			}
		}
	}

}


int findStone(int g_stoneNum, int x, int y, int notGo, int *locationInfo) {

	// g_stoneNum : 돌색(1 or 2)
	// x, y : 좌표
	// notGo : 재귀 이용했으므로 갔던길을 되돌아 오지 않도록 하는 변수.
	//    4 
	//  1   3  으로 지정하여 처리함
	//    2
	// *location : 갔던길을 저장하는 변수

	locationInfo[infoCnt++] = x * 10 + y;

	if (x - 1 >= 0 && notGo != 1) {
		if (g_stone[x - 1][y] == g_stoneNum) {
			findStone(g_stone[x - 1][y], x - 1, y, 3, locationInfo);
		}
	}

	if (y + 1 < 5 && notGo != 2) {
		if (g_stone[x][y + 1] == g_stoneNum) {
			findStone(g_stone[x][y + 1], x, y + 1, 4, locationInfo);
		}
	}

	if (x + 1 < 5 && notGo != 3) {
		if (g_stone[x + 1][y] == g_stoneNum) {
			findStone(g_stone[x + 1][y], x + 1, y, 1, locationInfo);
		}
	}

	if (y - 1 >= 0 && notGo != 4) {
		if (g_stone[x][y - 1] == g_stoneNum) {
			findStone(g_stone[x][y - 1], x, y - 1, 2, locationInfo);
		}
	}
	return 0;

}


int findHouse(int *location) {
	int i;
	int tmpX = -1, tmpY = -1;
	int myType, yourType;

	for (i = 0; i<25; i++) {

		if (location[i] == -1)  // 배열의 끝이면 다음거 진행
			continue;

		tmpX = location[i] / 10;
		tmpY = location[i] % 10;
		myType = g_stone[tmpX][tmpY];
		if (myType == 1) yourType = 2;
		if (myType == 2) yourType = 1;

		if (g_stone[tmpX - 1][tmpY] == 0 && tmpX - 1 >= 0) // 4방향중 0(=빈칸)이 존재한다면 그것은 집이 아니다.
			return 0;
		if (g_stone[tmpX + 1][tmpY] == 0 && tmpX + 1 < 5)
			return 0;
		if (g_stone[tmpX][tmpY - 1] == 0 && tmpY - 1 >= 0)
			return 0;
		if (g_stone[tmpX][tmpY + 1] == 0 && tmpY + 1 < 5)
			return 0;

	}
	return 1; // 집인경우는 1을 리턴

}


int isFar(int lastX, int lastY, int lastX_2nd, int lastY_2nd) {

	if (lastX - 1 != lastX_2nd || lastX + 1 != lastX_2nd || lastY - 1 != lastY_2nd || lastY + 1 != lastY_2nd) { // 사람이 두 돌을 인접하게 두지 않았다면
		return 1; // 멀리 둠
	}
	else
		return 0; // 가까이 둠


}