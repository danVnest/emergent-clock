#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

// Clock Parameters
#define ASPECT (5.0 / 3.0)
#define X_COUNT 15
#define Y_COUNT ((int)(X_COUNT / ASPECT))
#define COUNT (X_COUNT * Y_COUNT)
#define BORDER 0.1 // proportion of the clock width or height which is border
#define GAP 0.5 // >0.5 for no hand overlap, >0.2929 for no diagonal overlap

// Simulation Definitions
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT ((int)(WINDOW_WIDTH / ASPECT))
#define X_MAX 1.0
#define Y_MAX (X_MAX / ASPECT)
#define SEPERATION ((X_MAX - 2*X_BORDER) / (X_COUNT - 1))
#define LENGTH ((SEPERATION * (1 - GAP)))
#define X_BORDER (BORDER * X_MAX)
#define Y_BORDER (BORDER * Y_MAX)

double direction[COUNT] = {0};

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
	for (uint8_t i = 0; i < X_COUNT; i++) {
		for (uint8_t j = 0; j < Y_COUNT; j++) {
			direction[i*X_COUNT + j] += M_PI/180 * (i*X_COUNT + j) / COUNT;
			glVertex2f(X_BORDER + i*SEPERATION, Y_BORDER + j*SEPERATION);
			glVertex2f((X_BORDER + i*SEPERATION) + LENGTH*sin(direction[i*X_COUNT + j]), (Y_BORDER + j*SEPERATION) + LENGTH*cos(direction[i*X_COUNT + j]));
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

int main(int argc, char **argv) {
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
