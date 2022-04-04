#include <GL/glut.h>
#include "bm_load.h"

#define FRAME_TIME 50//ms

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
	int width;
	int height;
	float ratio;
} window;

struct {
	float x;
	float y;
	float x_scale;
	float y_scale;
	int button;
	int state;
} tap;

float vert[30] = {-1, -1, 0, 1,  1, 1, 1, 0,  -1, 1, 0, 0,  1, 1, 1, 0,  -1, -1, 0, 1,  1, -1, 1, 1};
float _mtx[16] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
unsigned char paused;
unsigned int time;

model miku_model;
bm_texture miku_texture;

void render(){
	glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
			glScalef(miku_model.x_scale, miku_model.y_scale, 1);
			glTranslatef(miku_model.x, miku_model.y, 0);
			//glRotatef(time * 4, 1, 0, 1);
			glScalef(miku_model.scale, miku_model.scale, 0);
			glBindTexture(GL_TEXTURE_2D, miku_texture.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glLoadIdentity();
			glScalef(miku_model.x_scale, miku_model.y_scale, 1);
			glTranslatef(miku_model.x + miku_model.scale, miku_model.y + miku_model.scale, 0);
			//glRotatef(time * 4, 1, 0, 1);
			glScalef(miku_model.scale, miku_model.scale, 0);
			glBindTexture(GL_TEXTURE_2D, miku_texture.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glLoadIdentity();
			glScalef(miku_model.x_scale, miku_model.y_scale, 1);
			glTranslatef(miku_model.x + miku_model.scale + miku_model.scale, miku_model.y + miku_model.scale + miku_model.scale, 0);
			//glRotatef(time * 4, 1, 0, 1);
			glScalef(miku_model.scale, miku_model.scale, 0);
			glBindTexture(GL_TEXTURE_2D, miku_texture.id);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glutSwapBuffers();
}

void display() {
	if(!paused) render();
}

void timer(int value) {
	if(!paused) {
		glutTimerFunc(FRAME_TIME, timer, value);
		time++;
		render();
	}
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	window.width = w;
	window.height = h;
	window.ratio = (float)w / h;
	if(window.ratio > 1) {
		tap.x_scale = 2 * window.ratio;
		tap.y_scale = 2;
	} else {
		tap.x_scale = 2;
		tap.y_scale = 2 * h / w;
	}
	if(window.ratio > miku_texture.ratio) {
		miku_model.x_scale = miku_texture.ratio / window.ratio;
		miku_model.y_scale = 1;
	} else {
		miku_model.x_scale = 1;
		miku_model.x_scale = window.ratio / miku_texture.ratio;
	}
}

void mouse_motion(int x, int y) {
	tap.x = ((float)x / window.width - 0.5) * tap.x_scale;
	tap.y = (0.5 - (float)y / window.height) * tap.y_scale;
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
		case 32:
			paused = !paused;
			if(!paused) glutTimerFunc(FRAME_TIME, timer, 0);
			break;
	}
}

int main(int argc, char** argv) {
	// GLUT Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("OpenGL Game");
	glutFullScreen();
	glutDisplayFunc(display);
	glutTimerFunc(FRAME_TIME, timer, 0);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(0);

	miku_model.scale = 0.25;
	miku_texture = loadBitmap(".\\miku.bmp");

	// OpenGL Init
	glDisable(GL_DITHER);
	glEnable(GL_CULL_FACE);
	glClearColor(0.125, 0.125, 0.25, 1.0);
	glVertexPointer(2, GL_FLOAT, 16, &vert[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 16, &vert[2]);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glutMainLoop();
	return EXIT_SUCCESS;
}
