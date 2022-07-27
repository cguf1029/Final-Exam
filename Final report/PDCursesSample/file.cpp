#include "head.h"

extern int score;
extern int stage[45][20];


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

void CSV2Array(char* fileName, int TATE) {

	char* p1; // char型ポインタワーク
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
		fprintf(fp, "score:%d", score);
		fclose(fp);
	}
}
