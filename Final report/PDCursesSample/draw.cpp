#include "head.h"




void paintOption() {


	start_color();	// ÉJÉâÅ[ÇÃê›íË
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_BLACK, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);
}



void  paint(int color, coordinate current, const char* letter) {
	attrset(COLOR_PAIR(color));
	mvaddstr(current.y, current.x, letter);
}

