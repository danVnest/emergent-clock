#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

#define PI 3.14159265

#define X_COUNT 10 // must be greater than Y_COUNT
#define Y_COUNT 10
#define COUNT (X_COUNT*Y_COUNT)
#define BORDER 0.1
#define SEPERATION ((1.0 - 2*BORDER) / X_COUNT) 
#define LENGTH ((SEPERATION / 1) * 0.9)

double direction[COUNT] = {0};

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	for (uint8_t i = 1; i < X_COUNT; i++) {
		for (uint8_t j = 1; j < Y_COUNT; j++) {
			direction[i*X_COUNT + j] += PI/180 * (i*X_COUNT + j) / COUNT;
			glVertex2f(BORDER + i*SEPERATION, BORDER + j*SEPERATION);
			glVertex2f((BORDER + i*SEPERATION) + LENGTH*sin(direction[i*X_COUNT + j]), (BORDER + j*SEPERATION) + LENGTH*cos(direction[i*X_COUNT + j]));
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width >= height) gluOrtho2D(0.0, (double)width/height, 0.0, 1.0);
	else gluOrtho2D(0.0, 1.0, 0.0, (double)height/width);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key,int x,int y) {
	  exit(0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Emergent Clock - Simulation");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 1;
}
