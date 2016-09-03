#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GLUT/glut.h>

// Clock Parameters
#define NUMBER_WIDTH 4
#define NUMBER_HEIGHT 6
#define NUMBER_SIZE (NUMBER_WIDTH * NUMBER_HEIGHT)
#define COLON_SIZE 2
#define X_EXTRA 1 // number of extra columns on either side not used for time
#define Y_EXTRA 1 // number of extra rows at the top and bottom not used for time
#define X_COUNT (4 * NUMBER_WIDTH + COLON_SIZE + 2*X_EXTRA)
#define Y_COUNT (NUMBER_HEIGHT + 2*Y_EXTRA)
#define COUNT (X_COUNT * Y_COUNT)
#define BORDER 0.1 // proportion of the clock width or height which is border
#define GAP 0.25 // >0.5 for no hand overlap, >0.2929 for no diagonal overlap

// Simulation Definitions
#define WINDOW_WIDTH 600
#define ASPECT ((double)X_COUNT / Y_COUNT)
#define WINDOW_HEIGHT ((int)(WINDOW_WIDTH / ASPECT))
#define X_MAX 1.0
#define Y_MAX (X_MAX / ASPECT)
#define SEPERATION ((X_MAX - 2*X_BORDER) / (X_COUNT - 1))
#define LENGTH ((SEPERATION * (1 - GAP)))
#define X_BORDER (BORDER * X_MAX)
#define Y_BORDER (BORDER * Y_MAX)
#define BLANK 10

double direction[COUNT] = {0};
int8_t placeMask[COUNT] = {[0 ... COUNT - 1] = -1};
int8_t indexMask[COUNT] = {[0 ... COUNT - 1] = -1};
int8_t digit[] = {1,0,0,4};
int8_t zero[] = {0,3,3,3, 0,0,3,2, 0,0,2,2, 0,0,2,2, 0,1,2,2, 1,1,1,2};
//int8_t one[] = {-1,-1,0,3, -1,-1,0,2, -1,-1,0,2, -1,-1,0,2, -1,-1,0,2, -1,-1,1,2};
int8_t one[] = {0,3,3,3, 1,0,1,2, -1,0,2,-1, -1,0,2,-1, 0,3,2,-1, 1,1,2,-1};
int8_t two[] = {0,3,3,3, 0,1,1,2, 0,2,3,3, 1,1,0,2, 0,3,3,2, 1,1,1,2};
int8_t three[] = {0,3,3,3, 1,1,0,2, 0,3,3,2, 1,1,0,2, 0,3,3,2, 1,1,1,2};
int8_t four[] = {-1,-1,0,3, -1,-1,0,2, 0,3,3,2, 0,1,0,2, 0,2,0,2, 1,2,1,2};
int8_t five[] = {0,3,3,3, 1,1,0,2, 0,3,3,2, 0,1,1,2, 0,2,3,3, 1,1,1,2};
int8_t six[] = {0,3,3,3, 0,0,3,2, 0,1,2,2, 0,1,1,2, 0,2,3,3, 1,1,1,2};
int8_t seven[] = {-1,-1,0,3, -1,-1,0,2, -1,-1,0,2, -1,-1,0,2, 0,3,3,2, 1,1,1,2};
int8_t eight[] = {0,3,3,3, 0,1,0,2, 0,2,3,2, 0,0,3,2, 0,1,2,2, 1,1,1,2};
int8_t nine[] = {-1,-1,0,3, -1,-1,0,2, 0,3,3,2, 0,0,3,2, 0,1,2,2, 1,1,1,2};
//int8_t nine[] = {0,3,3,3, 1,1,0,2, 0,3,3,2, 0,0,3,2, 0,1,2,2, 1,1,1,2};
int8_t blank[NUMBER_SIZE] = {[0 ... NUMBER_SIZE - 1] = -1};
int8_t colon[] = {1,0, 2,3, 1,0, 2,3};
int8_t* digitMask[] = {zero, one, two, three, four, five, six, seven, eight, nine, blank};

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, Y_MAX);
	glVertex2f(X_MAX, Y_MAX);
	glVertex2f(X_MAX, 0.0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	uint8_t a = 0;
	for (uint8_t j = 0; j < Y_COUNT; j++) {
		for (uint8_t i = 0; i < X_COUNT; i++) {
			if (placeMask[a] == 4) direction[a] = colon[indexMask[a]] * M_PI/2;
			else if (placeMask[a] != -1) {
				int8_t goal = digitMask[digit[placeMask[a]]][indexMask[a]];
				if (goal != -1) direction[a] = goal * M_PI/2;
				else direction[a] += M_PI/180 * a / COUNT;
			}
			else direction[a] += M_PI/180 * a / COUNT;
			double x = X_BORDER + i*SEPERATION;
			double y = Y_BORDER + j*SEPERATION;
			glVertex2f(x, y);
			glVertex2f(x + LENGTH*sin(direction[a]), y + LENGTH*cos(direction[a]));
			a++;
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ((double)width/height >= ASPECT) { 
		int insideWidth = height * ASPECT;
		glViewport((width - insideWidth) / 2, 0, insideWidth, height);
	}
	else {
		int insideHeight = width / ASPECT;
		glViewport(0, (height - insideHeight) / 2, width, insideHeight);
	}
	gluOrtho2D(0.0, X_MAX, 0.0, Y_MAX);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	exit(0);
}

void initEmergentClock(void) {
	for (uint8_t j = Y_EXTRA; j < Y_EXTRA + NUMBER_HEIGHT; j++) {
		for (uint8_t i = X_EXTRA; i < X_EXTRA + 2*NUMBER_WIDTH; i++) {
			uint8_t a1 = i + j*X_COUNT;
			uint8_t a2 = a1 + X_COUNT/2;
			placeMask[a1] = (i - X_EXTRA) / NUMBER_WIDTH;
			indexMask[a1] = (i - X_EXTRA)%4 + (j - Y_EXTRA)*4;
			placeMask[a2] = placeMask[a1] + 2;
			indexMask[a2] = indexMask[a1];
		}
	}
	uint8_t xStart = X_COUNT/2 - COLON_SIZE/2;
	uint8_t yStart = Y_COUNT/2 - COLON_SIZE;
	for (uint8_t j = yStart; j < yStart + 2*COLON_SIZE; j++) {
		for (uint8_t i = xStart; i < xStart + COLON_SIZE; i++) {
			placeMask[i + j*X_COUNT] = 4;
			indexMask[i + j*X_COUNT] = i - xStart + (j - yStart)*COLON_SIZE;
		}
	}
	time_t rawtime;
	time(&rawtime);
	struct tm *now = localtime(&rawtime);
	digit[0] = now->tm_hour / 10;
	digit[1] = now->tm_hour % 10;
	digit[2] = now->tm_min / 10;
	digit[3] = now->tm_min % 10;
	if (digit[0] == 0) digit[0] = BLANK;
}

int main(int argc, char **argv) {
	initEmergentClock();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 300);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Emergent Clock - Simulation");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 1;
}
