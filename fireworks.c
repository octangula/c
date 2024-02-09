// my second c program, some pretty coloured fireworks - essentially extended from the parallax

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define max 400
#define x 280
#define y 72
#define maxbuildings (x / 5)

char *chars[13] = {" ", ".", "*", "@", "^", "|", "=", "■", "x", "#", "@", ".", "$"};
int allcolours[12] = {0, 31, 32, 33, 34, 35, 36, 41, 42, 44, 45, 46};
int display[x][y][2];
int pcount = 0;
int bcount = 0;
int character[max];
int type[max]; // 0 for firework, 1 for normal
float xpos[max], ypos[max], xvel[max], yvel[max]; // particle stuff
int colour[max];
int i, j, k;

int epoch() { // epoch but in milliseconds yes
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void create_particle(float xp, float yp, float xv, float yv, int c, int t, int clr) {
	if (pcount < max) {
		xpos[pcount] = xp;
		ypos[pcount] = yp;
		xvel[pcount] = xv;
		yvel[pcount] = yv;
		character[pcount] = c;
		type[pcount] = t;
		colour[pcount] = clr;
		pcount++;
	}
}

void remove_particle(int idx) { // move last one to fill the gap and let it overwrite the last space
	xpos[idx] = xpos[pcount - 1];
	ypos[idx] = ypos[pcount - 1];
	xvel[idx] = xvel[pcount - 1];
	yvel[idx] = yvel[pcount - 1];
	character[idx] = character[pcount - 1];
	type[idx] = type[pcount - 1];
	colour[idx] = colour[pcount - 1];
	pcount = pcount - 1;
}

void update() {
	i = 0;
	while (i < pcount) { // probably can't for loop cuz the size changes lole
		if (yvel[i] > 0 && type[i] == 0) { // explode at highest point
			for (j = 0; j < 50; j++) { // make lil particles
				create_particle(xpos[i], ypos[i], (float)(rand() % 200 - 100) / 150, (float)(rand() % 200 - 100) / 170, rand() % 2 + 1, 1, rand() % 6 + 1);
			}
			remove_particle(i);
			i++;
			continue;
		}
		
		display[(int)round(xpos[i])][(int)round(ypos[i])][0] = 0; // remove old
		
		xpos[i] = xpos[i] + xvel[i];
		ypos[i] = ypos[i] + yvel[i];
		
		if (ypos[i] < 1 || xpos[i] < 1 || ypos[i] > y - 2 || xpos[i] > x - 2) {
			remove_particle(i);
			i++;
			continue;
		}
		
		yvel[i] = yvel[i] + 0.005;
		
		display[(int)round(xpos[i])][(int)round(ypos[i])][0] = character[i];
		display[(int)round(xpos[i])][(int)round(ypos[i])][1] = colour[i];
		
		i++;
	}
}

void display_frame() {
	printf("\033[%iA", y); // move cursor back to top ansi
	
	for (j = 0; j < y; j++) {
		printf("\n");
		for (i = 0; i < x; i++) {
			printf("\x1B[%im%s", allcolours[display[i][j][1]], chars[display[i][j][0]]);
		}
	}
}

int main() {
	srand(time(NULL));

	memset(display, 0, x*y*2*sizeof(int));
	memset(character, 0, max*sizeof(int));
	memset(colour, 0, max*sizeof(int));
	memset(xpos, 0, max*sizeof(int));
	memset(ypos, 0, max*sizeof(int));
	memset(xvel, 0, max*sizeof(int));
	memset(yvel, 0, max*sizeof(int));
	
	for (i = 0; i < x; i++) {
		display[i][0][0] = 6;
		display[i][y-1][0] = 4; // floor
	}
	
	printf("\033[2J"); // clear screen ansi
	printf("\033[?25l"); // hide cursor ansi
	
	long int lastupdate = epoch();
	long int lastfirework = epoch();
	for (;;) {
		if (epoch() > lastupdate + 10) {
			update();
			display_frame();
			lastupdate = epoch();
		}
		if (epoch() > lastfirework + 1000) {
			create_particle((rand() % (x - 50)) + 25, y - 2, ((float)(rand() % 100) / 200) - 0.25, ((float)(rand() % 100) / 300) - 0.7, 3, 0, rand() % 6 + 1);
			lastfirework = epoch();
		}
	}
}
