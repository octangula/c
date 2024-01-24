// my second c program, some pretty coloured fireworks - essentially extended from the parallax

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define max 4000
#define x 280 // replace with your own window size
#define y 72

char *chars[6] = {" ", ".", "*", "@", "^"};
int allcolours[7] = {0, 31, 32, 33, 34, 35, 36};
int display[x][y][2];
int count = 0;
int character[max];
int type[max]; // 0 for firework, 1 for normal
float xpos[max];
float ypos[max];
float xvel[max];
float yvel[max];
int colour[max];
int i, j;

int epoch() { // epoch but in milliseconds yes
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void create_particle(float xp, float yp, float xv, float yv, int c, int t, int clr) {
	if (count < max) {
		xpos[count] = xp;
		ypos[count] = yp;
		xvel[count] = xv;
		yvel[count] = yv;
		character[count] = c;
		type[count] = t;
		colour[count] = clr;
		count++;
	}
}

void remove_particle(int idx) { // move last one to fill the gap and let it overwrite the last space
	display[(int)round(xpos[idx]) % (x - 1)][(int)round(ypos[idx]) % (y - 1)][0] = 0; // gotta remove it here i think
	xpos[idx] = xpos[count - 1];
	ypos[idx] = ypos[count - 1];
	xvel[idx] = xvel[count - 1];
	yvel[idx] = yvel[count - 1];
	character[idx] = character[count - 1];
	type[idx] = type[count - 1];
	colour[idx] = colour[count - 1];
	count = count - 1;
}

void update() {
	i = 0;
	while (i < count) { // probably can't for loop cuz the size changes lole
		if (yvel[i] > -0.1 && type[i] == 0) { // explode near its highest point
			for (j = 0; j < 50; j++) { // make lil particles
				create_particle(xpos[i], ypos[i], (float)(rand() % 200 - 100) / 150, (float)(rand() % 200 - 100) / 170, rand() % 2 + 1, 1, rand() % 6 + 1);
			}
			remove_particle(i);
			i++;
			continue;
		}
		
		display[(int)round(xpos[i]) % (x - 1)][(int)round(ypos[i]) % (y - 1)][0] = 0; // remove old
		
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
	
	for (i = 0; i < x; i++) { // floor
		display[i][y-1][0] = 4;
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