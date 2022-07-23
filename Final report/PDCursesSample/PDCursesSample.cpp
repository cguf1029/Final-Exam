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


int count;
int mode;
double sum1, sum2,sum3;

void CSV2Array(char* fileName, int TATE);


int stage[45][20];


struct coordinate {
	int x;
	int y;
};

struct coordinate my, enemy[20], bullet[6];




void Key_Action() {
	int key;
	//noecho();
	//cbreak();
	keypad(stdscr, TRUE);
	key = getch();
	if (key == KEY_RIGHT) {
		if (my.x < 44) {
			my.x++;
		}
		erase();
	}
	else if (key == KEY_LEFT) {
		if (my.x > 0) {
			my.x--;
		}
		erase();
	}
	else if (key == KEY_UP) {
		if (my.y > 0) {
			my.y--;
		}
		erase();
	}
	else if (key == KEY_DOWN) {
		if (my.y < 19) {
			my.y++;
		}
		erase();
	}
	//echo();
	//nocbreak();
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
				if (n < 25) {			//敵の数がオーバーしないようにする
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
			enemy[i].x++;
			paint(3, enemy[i], " ");
		}
		count++;
		if (count == 3) {
			count = 0;
			mode = 1;
		}
	}
	else {
		for (int i = 0; i < 20; i++) {
			enemy[i].x--;
			paint(3, enemy[i], " ");
		}
		count++;
		if (count == 3) {
			count = 0;
			mode = 0;
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
	
	count = 0;
	mode = 0;
	my.x = 5;
	my.y = 5;
	while (true) {

		enemyMove();
		paint(2, my, " ");

		

		
		Key_Action();			//key操作
		
		//std::thread paintMy();
		//std::thread paintBullet();

		//paintMy.join();
		//paintBullet.join();
		
		erase();

		refresh();
	}
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

