#include "head.h"

extern int bulletCount;
extern int bulletMyFlag;
extern int bulletEnemyFlag[20];
extern int keyFlag;
extern int enemyFlag[20];
extern int timeCount;
extern int endNumber;
extern int count;
extern int mode;
extern int score;
extern int stage[45][20];
extern struct coordinate my, enemy[20], bulletEnemy[20], bulletMy;


void saveStart() {
	int n = 0;
	for (int i = 0; i < 45; i++) {
		for (int j = 0; j < 20; j++) {
			if (stage[i][j] == 1) {
				if (n < 20) {			//敵の数がオーバーしないようにする
					enemy[n].x = i;
					enemy[n].y = j;
					paint(3, enemy[n], " ");
					n++;
				}
			}
		}
	}
}


void enemyMove() {
	if (mode == 0) {
		for (int i = 0; i < 20; i++) {
			if (enemyFlag[i] == 1) {
				enemy[i].x++;
				paint(3, enemy[i], " ");
			}
		}
		count++;
		if (count == 4) {
			count = 0;
			mode = 1;
		}
	}
	else {
		for (int i = 0; i < 20; i++) {
			if (enemyFlag[i] == 1) {
				enemy[i].x--;
				paint(3, enemy[i], " ");
			}
		}
		count++;
		if (count == 4) {
			count = 0;
			mode = 0;
		}
	}
	timeCount++;
	napms(10);
	if (timeCount == 20) {
		timeCount = 0;
		for (int i = 0; i < 20; i++) {
			enemy[i].y++;
		}
	}
}



void burst() {
	for (int i = 0; i < 20; i++) {
		enemy[i].y = 0;
	}
}



void keyAction() {
	int key;
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	key = getch();
	timeout(1);
	if (key == KEY_RIGHT) {
		if (keyFlag == 0) {
			if (my.x < 44) {
				my.x++;
				keyFlag = 1;
			}
		}

	}
	else if (key == KEY_LEFT) {
		if (keyFlag == 0) {
			if (my.x > 0) {
				my.x--;
				keyFlag = 1;
			}
		}

	}
	else if (key == KEY_UP) {
		if (keyFlag == 0) {
			if (bulletMyFlag == 0) {
				bulletMyFlag = 1;
			}
			else {
				bulletMyFlag = 0;
			}
		}
		keyFlag = 1;
	}
	else if (key == KEY_DOWN) {
		if (score >= 50) {
			if (keyFlag == 0) {
				burst();
				score = score - 50;
			}
		}
	}
	else {
		keyFlag = 0;
	}
	echo();
	nocbreak();
}

void bulletMyself() {
	if (bulletMyFlag == 0) {
		bulletMy.x = my.x;
		bulletMy.y = my.y;
	}
	else if (bulletMyFlag == 1) {
		bulletMy.y--;
		paint(2, bulletMy, " ");
		if (bulletMy.y < 0) {
			bulletMyFlag = 0;
		}
	}
	napms(100);
}

void bulletEnemies() {
	for (int i = 0; i < 20; i++) {
		if (bulletEnemyFlag[i] == 0) {
			bulletEnemy[i].x = enemy[i].x;
			bulletEnemy[i].y = enemy[i].y;
		}
		else {

			bulletEnemy[i].y++;
			paint(7, bulletEnemy[i], " ");
			if (bulletEnemy[i].y > 20) {
				bulletEnemyFlag[i] = 0;
				bulletCount--;
			}

		}
	}
}

void bulletFlag() {
	int r;
	if (bulletCount < 10) {
		if (rand() < 5000) {
			r = rand() % 20;
			if (enemyFlag[r] == 1) {
				bulletEnemyFlag[r] = 1;
				bulletCount++;
			}
		}
	}
}

void colision() {
	for (int i = 0; i < 20; i++) {
		if (bulletMy.y == enemy[i].y) {
			if (bulletMy.x == enemy[i].x) {
				enemyFlag[i] = 0;
				bulletMyFlag = 0;
				score = score + 100;
			}

		}
		if (bulletEnemy[i].y == my.y) {
			if (bulletEnemy[i].x == my.x) {
				bulletEnemyFlag[i] = 0;
				if (score > 100) {
					score = score - 100;
				}
				else {
					score = 0;
				}
			}
		}
	}
}

void gameEnd() {
	int endCount;
	endNumber = 1;
	for (int i = 0; i < 20; i++) {
		if (enemyFlag[i] == 1) {
			endNumber = 0;
			break;
		}
	}
}