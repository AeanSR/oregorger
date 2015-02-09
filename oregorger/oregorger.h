#pragma once

#include "resource.h"

void refresh();
void init();
void bossroutine();
void charmove(int direct);
extern int roadmap[9][9];
extern int score;
extern int bossx;
extern int bossy;
extern int charx;
extern int chary;
extern int boxx;
extern int boxy;
void endcallback();
void startcallback();
