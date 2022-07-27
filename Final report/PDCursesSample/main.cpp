// PDCursesSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <Windows.h>

#include <string>
#include <sstream>
#include <iostream>

#include <thread>
#include <mutex>
#include <cstdio>


#define BUFFSIZE 1024

#define CHARBUFF 124

int bulletCount;
int bulletMyFlag;
int bulletEnemyFlag[20];
int keyFlag;
int enemyFlag[20];
int timeCount;
int endNumber;
int count;
int mode;
int score;

void CSV2Array(char* fileName, int TATE);
void fileOut();

int stage[45][20];


struct coordinate {
	int x;
	int y;
};

struct coordinate my, enemy[20], bulletEnemy[20],bulletMy;

void burst() {
	for (int i=0; i < 20; i++) {
		enemy[i].y = 0;
	}
}

void Key_Action() {
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
			if (bulletMyFlag == 0){ 
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



void  paint(int color, coordinate current, const char* letter) {
	attrset(COLOR_PAIR(color));
	mvaddstr(current.y, current.x, letter);
}



void getGurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}

void scanCSV() {
	std::ostringstream oss;

	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);
	char section[CHARBUFF];
	sprintf_s(section, "section1");
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, "keyword1");
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\setting.ini", currentDirectory);
	char keyValue[CHARBUFF];
	if (GetPrivateProfileString(section, keyWord, "none", keyValue, CHARBUFF, settingFile) != 0) {
		fprintf(stdout, "%s , %s\n", settingFile, keyValue);
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", settingFile, section, keyWord);
	}

	FILE* fp;
	char s[BUFFSIZE];

	errno_t error;
	error = fopen_s(&fp, keyValue, "r");
	if (error != 0)
		fprintf_s(stderr, "failed to open");
	else {
		int i = 0;
		while (fgets(s, BUFFSIZE, fp) != NULL) {

			CSV2Array(s, i);

			i++;
		}
		fclose(fp);
	}
}

void saveStart(){
	int n=0;
	for (int i = 0; i < 45;i++) {
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
			enemy[i].y ++;
		}
	}
}


void paintOption() {
	

	start_color();	// カラーの設定
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);
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
	for (int i = 0; i < 20;i++) {
		if (enemyFlag[i] == 1) {
			endNumber = 0;
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	

	scanCSV();
	
	
	

	// 初期化
	if (initscr() == NULL) {
		return 2;
	}

	paintOption();

	
	
	
	
	saveStart();
	
	for (int i = 0; i < 20; i++) {
		enemyFlag[i] = 1;
	}
	




	timeCount = 1;
	keyFlag = 0;
	bulletMyFlag = 0;
	for (int d = 0; d < 20; d++) {
		bulletEnemyFlag[d] = 0;
	}
	bulletCount = 0;
	count = 0;
	mode = 0;
	my.x = 22;
	my.y = 20;
	score = 0;

	endNumber = 0;

	while (endNumber==0) {

		attrset(COLOR_PAIR(1));
		std::string tmp = std::to_string(score);
		char const* scoreChar = tmp.c_str();
		mvaddstr(5, 50, "score:");
		mvaddstr(6, 50, scoreChar);
		enemyMove();
		paint(4, my, " ");

		bulletFlag();
		bulletEnemies();
		bulletMyself();
		Key_Action();			//key操作
		
		colision();

		erase();
		gameEnd();
		//refresh();
	}
	fileOut();

	return 0;
}


void CSV2Array(char* fileName, int TATE) {

	char ch1; // 変換後の取得値
	char* p1, * p2; // char型ポインタワーク
	double d;
	int i;
	char delim[] = ",";//デリミタ（複数渡せる）ここではカンマと空白
	char* ctx = 0;
	char* s = fileName;
	p1 = strtok_s(s, delim, &ctx);
	i = 0;
	while (p1 != NULL) {
		d = atof(p1);
		


		stage[i][TATE] = d;
		p1 = strtok_s(NULL, delim, &ctx); // 2個目以降の部分文字列取得
		i++;
	}
}

void fileOut() {
	FILE* fp;
	char s[BUFFSIZE];

	errno_t error;
	error = fopen_s(&fp, "result.txt", "w");
	if (error != 0)
		fprintf_s(stderr, "failed to open");
	else {
		fprintf(fp, "you win!\n");
		fprintf(fp, "score:%d",score);
		fclose(fp);
	}
}