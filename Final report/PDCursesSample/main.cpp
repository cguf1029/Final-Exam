// PDCursesSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//



#include "head.h"


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
int stage[45][20];
struct coordinate my, enemy[20], bulletEnemy[20], bulletMy;



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
		keyAction();			//key操作
		colision();
		erase();
		gameEnd();
		//refresh();
	}
	fileOut();

	return 0;
}


