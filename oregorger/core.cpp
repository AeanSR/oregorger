/*
	Oregorger Simulator?
	Aean, 2015.2.9
*/

#include "stdafx.h"
#include "oregorger.h"

int roadmap[9][9] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0 },
	{ 0, 1, 0, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

int bossx = 3;
int bossy = 2;
int charx = 3;
int chary = 3;
int boxx = 3;
int boxy = 4;

double speedf = 250.0;
int speed = 250;
int fine = 1;
int rolling = 0;
int score = 0;
int scoredelta = 1;

void init(){
	srand((unsigned int)time(NULL));
	fine = 1;
	rolling = 0;
	score = 0;
	bossx = 3;
	bossy = 2;
	charx = 3;
	chary = 3;
	speedf = 250.0;
	speed = 250;
	refresh();
}

void wait(int t){
    std::chrono::milliseconds dur(speed);
    std::this_thread::sleep_for(dur); 
}

void down(){
	bossx++;
	refresh();
	if (bossx == charx && bossy == chary) fine = 0;
	wait(speed);
}
void left(){
	bossy--;
	refresh();
	if (bossx == charx && bossy == chary) fine = 0;
	wait(speed);
}
void right(){
	bossy++;
	refresh();
	if (bossx == charx && bossy == chary) fine = 0;
	wait(speed);
}
void up(){
	bossx--;
	refresh();
	if (bossx == charx && bossy == chary) fine = 0;
	wait(speed);
}

void go(){
	rolling = 1;
	if (bossx == 3 && bossy == 2){
		if (rand() & 1){
			left();
			down();
			down();
			down();
			right();
			right();
		}
		else{
			right();
			right();
			right();
		}
	}
	else if (bossx == 3 && bossy == 3){
		if (rand() & 1){
			left();
			left();
			down();
			down();
			down();
			right();
			right();
		}
		else{
			right();
			right();
		}
	}
	else if (bossx == 3 && bossy == 5){
		if (rand() & 1){
			up();
			up();
			left();
			left();
			left();
			down();
			down();
		}
		else{
			down();
			down();
		}
	}
	else if (bossx == 2 && bossy == 5){
		if (rand() & 1){
			up();
			left();
			left();
			left();
			down();
			down();
		}
		else{
			down();
			down();
			down();
		}
	}
	else if (bossx == 5 && bossy == 3){
		if (rand() & 1){
			down();
			down();
			right();
			right();
			right();
			up();
			up();
		}
		else{
			up();
			up();
		}
	}
	else if (bossx == 6 && bossy == 3){
		if (rand() & 1){
			down();
			right();
			right();
			right();
			up();
			up();
		}
		else{
			up();
			up();
			up();
		}
	}
	else if (bossx == 5 && bossy == 5){
		if (rand() & 1){
			right();
			right();
			up();
			up();
			up();
			left();
			left();
		}
		else{
			left();
			left();
		}
	}
	else if (bossx == 5 && bossy == 6){
		if (rand() & 1){
			right();
			up();
			up();
			up();
			left();
			left();
		}
		else{
			left();
			left();
			left();
		}
	}
	rolling = 0;
}

void bossroutine(){
	startcallback();
	while (fine){
		if (bossx == charx && bossy == chary){ fine = 0; break; }
		score++;
		scoredelta++;
		go();
		wait(speed * 10);
		speedf = speedf * 0.99;
		speed = static_cast<int>(speedf);
	}
	endcallback();
}

void charmove(int direct){
	if (!fine) return;
	int newx = charx;
	int newy = chary;
	switch (direct){
	case 0: newx --; break;
	case 1: newy --; break;
	case 2: newx ++; break;
	case 3: newy ++; break;
	}
	if (roadmap[newx][newy]){
		charx = newx;
		chary = newy;
	}
	if (rolling && charx == bossx && chary == bossy) fine = 0;
	if (boxx == charx && boxy == chary){
		score += scoredelta;
		do{
			boxx = rand() % 9;
			boxy = rand() % 9;
		} while (!roadmap[boxx][boxy]);
	}
	refresh();
}