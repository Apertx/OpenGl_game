#include <GL/glut.h>
#include "bm_load.h"

typedef struct {
	float x;
	float y;
	float scale;
	float x_scale;
	float y_scale;
	float angle;
	float matrix[16];
} model;

struct {
	float x;
	float y;
	float x_scale;
	float y_scale;
	int button;
	int state;
} tap;

float vert[30] = {-1, -1, 0, 1,  1, 1, 1, 0,  -1, 1, 0, 0,  1, 1, 1, 0,  -1, -1, 0, 1,  1, -1, 1, 1};
model miku_model;
bm_texture miku_texture;

unsigned char timer_stop;
unsigned int time;
int win_width;
int win_height;
float win_ratio;
float _mtx[16] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glScalef(miku_model.x_scale, miku_model.y_scale, 1);
	glTranslatef(miku_model.x, miku_model.y, 0);
	//glRotatef(time * 4, 1, 0, 1);
	glScalef(miku_model.scale, miku_model.scale, 0);
	glBindTexture(GL_TEXTURE_2D, miku_texture.id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glutSwapBuffers();
}

void timer(int value) {
	if(!timer_stop) glutTimerFunc(50, timer, value);
	time++;
	display();
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	win_width = w;
	win_height = h;
	win_ratio = (float)w / h;
	if(win_ratio > 1) {
		tap.x_scale = 2 * win_ratio;
		tap.y_scale = 2;
	} else {
		tap.x_scale = 2;
		tap.y_scale = 2 * h / w;
	}
	if(win_ratio > miku_texture.ratio) {
		miku_model.x_scale = miku_texture.ratio / win_ratio;
		miku_model.y_scale = 1;
	} else {
		miku_model.x_scale = 1;
		miku_model.x_scale = win_ratio / miku_texture.ratio;
	}
}

void mouse_motion(int x, int y) {
	tap.x = ((float)x / win_width - 0.5) * tap.x_scale;
	tap.y = (0.5 - (float)y / win_height) * tap.y_scale;
	miku_model.x = tap.x;
	miku_model.y = tap.y;
}

void mouse(int b, int s, int x, int y) {
	tap.button = b;
	tap.state = s;
	mouse_motion(x, y);
}

void motion(int x, int y) {
	mouse_motion(x, y);
}

void keyboard(unsigned char c, int x, int y) {
	x = x;
	y = y;
	switch(c) {
		case 27:
			glutDestroyWindow(glutGetWindow());
			exit(EXIT_SUCCESS);
			break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("OpenGL Game");
	glutFullScreen();
	glutDisplayFunc(display);
	glutTimerFunc(50, timer, 0);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(0);

	miku_model.scale = 0.25;
	miku_texture = loadBitmap(".\\miku.bmp");

	glDisable(GL_DITHER);
	glEnable(GL_CULL_FACE);
	glClearColor(0.125, 0.125, 0.25, 1.0);
	glVertexPointer(2, GL_FLOAT, 16, &vert[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 16, &vert[2]);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glutMainLoop();
	return EXIT_SUCCESS;
}
