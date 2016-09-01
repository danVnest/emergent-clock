#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

// Clock Parameters
#define NUMBER_WIDTH 4
#define NUMBER_HEIGHT 6
#define COLON_SIZE 2
#define X_EXTRA 1 // number of extra columns on either side not used for time
#define Y_EXTRA 1 // number of extra rows at the top and bottom not used for time
#define X_COUNT (4 * NUMBER_WIDTH + COLON_SIZE + 2*X_EXTRA)
#define Y_COUNT (NUMBER_HEIGHT + 2*Y_EXTRA)
#define COUNT (X_COUNT * Y_COUNT)
#define BORDER 0.1 // proportion of the clock width or height which is border
#define GAP 0.5 // >0.5 for no hand overlap, >0.2929 for no diagonal overlap

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

double direction[COUNT] = {0};
int placeMask[COUNT] = {0};
int indexMask[COUNT] = {0};
double number9[] = {-1,-1,0,3, -1,-1,0,2, 0,3,3,2, 0,0,3,2, 0,1,2,2, 1,1,1,2};
double colon[] = {1,0, 2,3, 1,0, 2,3};

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
	int a = 0;
	for (uint8_t j = 0; j < Y_COUNT; j++) {
		for (uint8_t i = 0; i < X_COUNT; i++) {
			if (placeMask[a] == 1) {
				if (number9[indexMask[a]] != -1) direction[a] = number9[indexMask[a]] * M_PI/2;
				else direction[a] += M_PI/180 * a /COUNT;
			}
			else if (placeMask[a] == 2) direction[a] = colon[indexMask[a]] * M_PI/2;
			else direction[a] += M_PI/180 * a / COUNT;
			glVertex2f(X_BORDER + i*SEPERATION, Y_BORDER + j*SEPERATION);
			glVertex2f((X_BORDER + i*SEPERATION) + LENGTH*sin(direction[a]), (Y_BORDER + j*SEPERATION) + LENGTH*cos(direction[a]));
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

void keyboard(unsigned char key,int x,int y) {
	exit(0);
}

void initEmergentClock(void) {
	for (uint8_t j = Y_EXTRA; j < Y_EXTRA + NUMBER_HEIGHT; j++) {
		for (uint8_t i = X_EXTRA; i < X_EXTRA + 2*NUMBER_WIDTH; i++) {
			placeMask[i + j*X_COUNT] = 1;
			placeMask[X_COUNT/2+i + j*X_COUNT] = 1;
			indexMask[i + j*X_COUNT] = (i - X_EXTRA)%4 + (j - Y_EXTRA)*4;
			indexMask[X_COUNT/2+i + j*X_COUNT] = (i - X_EXTRA)%4 + (j - Y_EXTRA)*4;
		}
	}
	for (uint8_t j = Y_COUNT/2 - COLON_SIZE; j < Y_COUNT/2 + COLON_SIZE; j++) {
		for (uint8_t i = X_COUNT/2 - COLON_SIZE/2; i < X_COUNT/2 + COLON_SIZE/2; i++) {
			placeMask[i + j*X_COUNT] = 2;
			indexMask[i + j*X_COUNT] = i - (X_COUNT/2 - COLON_SIZE/2) + (j - (Y_COUNT/2 - COLON_SIZE))*COLON_SIZE;
		}
	}
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
