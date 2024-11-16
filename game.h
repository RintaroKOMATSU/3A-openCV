#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include "utility.h"


void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void draw_cube(double x_size , double y_size , double z_size , double* color );
void* game(void*args);

// グローバル変数
double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
int n;
int* eyes_state;


typedef struct game_thread_args{
    int argc;
    char** argv;
    int *eye_state;
    pthread_mutex_t* eye_state_lock;
}GameThreadArgs;

void* game(void*args) {
    GameThreadArgs* game_args = (GameThreadArgs*)args;
	eyes_state = game_args ->eye_state;
	init_GL(game_args->argc, game_args->argv);
	/* このプログラム特有の初期化 */
	init();

	/* コールバック関数の登録 */
	set_callback_functions();

	/* メインループ */
	glutMainLoop(); 
	
	return 0;
}

void init_GL(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA |GLUT_DEPTH| GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow(WINDOW_NAME);
}

void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
	
void set_callback_functions(){
	glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
    glutMouseFunc(glut_mouse);
    glutMotionFunc(glut_motion);
    glutPassiveMotionFunc(glut_motion);
	glutIdleFunc(glut_idle);
}

void glut_keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q':
	case 'Q':
	case '\033':
		exit(0);
	}

	glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_UP){
			g_isLeftButtonOn = false;
		}else if(state == GLUT_DOWN){
			g_isLeftButtonOn = true;
		}
	}

	if(button == GLUT_RIGHT_BUTTON){
		if(state == GLUT_UP){
			g_isRightButtonOn = false;
		}else if(state == GLUT_DOWN){
			g_isRightButtonOn = true;
		}
	}
}

void glut_motion(int x, int y){
	static int px = -1, py = -1;
	if(g_isLeftButtonOn == true){
		if(px >= 0 && py >= 0){
			g_angle1 += (double)-(x - px)/1.5;
			g_angle2 += (double)(y - py)/1.5;
		}
		px = x;
		py = y;
	}else if(g_isRightButtonOn == true){
		if(px >= 0 && py >= 0){
			g_distance += (double)(y - py)/20;
		}
		px = x;
		py = y;
	}else{
		px = -1;
		py = -1;
	}
	glutPostRedisplay();
}

void glut_display(){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, 1.0, 0.1, 100);
	gluLookAt(g_distance, g_distance, g_distance,0, 0, 0, 0, 0, 1);
    glRotatef(-1*g_angle1, 0, 0, 1);
    glRotatef(-1*g_angle2, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	double color[3] = {1, 1, 1};
    draw_cube(1, 2+ *eyes_state, 2, color);
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

void glut_idle() {
	glutPostRedisplay();
}


void hsv2rgb(double *hsv, double *rgb) {
	double h = hsv[0], s = hsv[1], v = hsv[2];
	double h0 = (fmod(h, 360))/60;
	double c = s;
	double x = c*(1-abs(fmod(h0, 2) -1));
	double r = v-c, g = v-c, b = v-c;
	switch (((int)h0))
	{
	case 0:
		r += c;g+=x;
		break;
	case 1:
		r +=x;g+=c;
		break;
	case 2:
		g +=c;b+=x;
		break;
	case 3:
		g+=x;b+=c;
		break;
	case 4:
		r+=x;b+=c;
		break;
	case 5:
		r+=c;b+=x;
		break;
	}
	rgb[0] = r; rgb[1] = g; rgb[2] = b;
}

void setColorRGB(double *rgb) {
	glColor3d(rgb[0], rgb[1], rgb[2]);
}

void setColorHSV(double *hsv) {
	double rgb[3];
	hsv2rgb(hsv, rgb);
	setColorRGB(rgb);
}

void draw_cube(double x_size, double y_size , double z_size , double* color ) {
	if (color == nullptr) {
		double color0[3] = {1.0, 1.0, 1.0};
		color = color0; 
	}
    double *hsv = color;
    double x0 = x_size/2;
    double y0 = y_size/2;
    double z0 = z_size/2;
	GLdouble pointA[] = {x0, y0, z0};
	GLdouble pointB[] = {-1*x0, y0, z0};
	GLdouble pointC[] = {-1*x0, -1*y0, z0};
	GLdouble pointD[] = {x0, -1*y0, z0};
    GLdouble pointE[] = {x0, y0, -1*z0};
	GLdouble pointF[] = {-1*x0, y0, -1*z0};
	GLdouble pointG[] = {-1*x0, -1*y0, -1*z0};
	GLdouble pointH[] = {x0, -1*y0, -1*z0};

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glEnd();

    glBegin(GL_POLYGON);
	glVertex3dv(pointA);
	glVertex3dv(pointB);
	glVertex3dv(pointF);
	glVertex3dv(pointE);
	glEnd();

    glBegin(GL_POLYGON);
	glVertex3dv(pointB);
	glVertex3dv(pointC);
	glVertex3dv(pointG);
	glVertex3dv(pointF);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointC);
	glVertex3dv(pointD);
	glVertex3dv(pointH);
	glVertex3dv(pointG);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointD);
	glVertex3dv(pointA);
	glVertex3dv(pointE);
	glVertex3dv(pointH);
	glEnd();

    setColorHSV(hsv);
    glBegin(GL_POLYGON);
	glVertex3dv(pointE);
	glVertex3dv(pointF);
	glVertex3dv(pointG);
	glVertex3dv(pointH);
	glEnd();

}