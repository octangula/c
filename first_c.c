// my first real c program! a little parallax star thingy

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define amount 200
#define x 280 // replace with your own window size
#define y 72

char *chars[6] = {" ", ".", "*", "@", "-", "|"};
int display[x][y];
int type[amount];
float xpos[amount];
int ypos[amount];
int i;
int j;

int epoch() { // epoch but in milliseconds yes
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void update() {
	for (i = 0; i < amount; i++) {
		display[(int)round(xpos[i]) % (x - 2) + 1][(int)round(ypos[i]) % (y - 2) + 1] = 0; // remove old 
		xpos[i] = xpos[i] + (float)type[i] / 2;
		display[(int)round(xpos[i]) % (x - 2) + 1][(int)round(ypos[i]) % (y - 2) + 1] = type[i];
	}
}

void display_frame() {
	printf("\033[%iA", y); // move cursor back to top ansi
	
	for (j = 0; j < y; j++) { // for each line
		for (i = 0; i < x; i++) { // for each char in line because its going along x axis
			printf("%s", chars[display[i][j]]);
		}
		printf("\n");
	}
}

int main() {
	srand(time(NULL)); // rand() sucks

	memset(display, 0, x*y*sizeof(int));
	memset(type, 0, amount*sizeof(int));
	memset(xpos, 0.0, amount*sizeof(int));
	memset(ypos, 0, amount*sizeof(int));
	
	for (i = 0; i < amount; i++) {
		type[i] = rand() % 3 + 1;
		xpos[i] = rand() % (x - 2) + 1;
		ypos[i] = rand() % (y - 2) + 1;
	}
	
	for (i = 0; i < y; i++) { // left and right borders
		display[0][i] = 5;
		display[x-1][i] = 5; // why does negative index not work in c
	}
	
	for (i = 0; i < x; i++) { // top and bottom borders
		display[i][0] = 4;
		display[i][y-1] = 4;
	}
	
	printf("\033[2J"); // clear screen ansi
	printf("\33[?25l"); // hide cursor ansi
	
	long int lastupdate = epoch();
	for (;;) {
		if (epoch() > lastupdate + 100) { // this is so stupid why am i doing this
			update();
			display_frame();
			lastupdate = epoch();
		}
	}
}