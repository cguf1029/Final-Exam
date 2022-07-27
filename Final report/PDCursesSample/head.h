#pragma once

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




struct coordinate {
	int x;
	int y;
};



void getGurrentDirectory(char* currentDirectory);
void CSV2Array(char* fileName, int TATE);
void fileOut();
void scanCSV();
void  paint(int color, coordinate current, const char* letter);
void paintOption();
void saveStart();
void burst();
void keyAction();
void bulletMyself();
void enemyMove();
void bulletFlag();
void bulletEnemies();
void colision();
void gameEnd();