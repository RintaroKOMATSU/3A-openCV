#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "utility.h"
#include "draw_mesh.h"
#include "obstacle.h"

void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void glut_time(int value);
void* game(void*args);

// グローバル変数
double g_angle1 = 0.0;
double g_angle2 = 0.0;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
int n;
int* eyes_state;
int* right_eye_state;
int* left_eye_state;


obstacle_list obs_list = obstacle_list();

typedef struct game_thread_args{
    int argc;
    char** argv;
    int *eye_state;
	int *left_eye_state;
	int *right_eye_state;
    pthread_mutex_t* eye_state_lock;
}GameThreadArgs;

void* game(void*args) {
    GameThreadArgs* game_args = (GameThreadArgs*)args;
	eyes_state = game_args ->eye_state;
	right_eye_state = game_args->right_eye_state;
	left_eye_state = game_args->left_eye_state;
	init_GL(game_args->argc, game_args->argv);
	/* このプログラム特有の初期化 */
	init();

	/* コールバック関数の登録 */
	set_callback_functions();

	/* メインループ */
	glutMainLoop(); 
	shm_cleanup();
	return nullptr;
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
	glutTimerFunc(int(1000/FRAME_RATE), glut_time, 0);
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
	double color[3] = {100, 1, 0.5};
    draw_sphere(0,-3-*left_eye_state, 0, 0.9, color);
	std::cout << -3-*left_eye_state << '\n';
	draw_sphere(0,3+*right_eye_state, 0, 0.9, color);
	obs_list.draw();
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

void glut_idle() {
	glutPostRedisplay();
}

void glut_time(int value) {
	static int obs_interval_count = 0;
	static double obs_interval = 1;//second
	static int obs_interval_count_max = int(FRAME_RATE*obs_interval);
	
	if (obs_interval_count == obs_interval_count_max) {
		obs_list.add(obstacle());
		obs_list.del_obstacle();
		obs_interval_count = 0;
	}
	obs_interval_count ++;
	obs_list.move();

	glutTimerFunc(int(1000 / FRAME_RATE), glut_time, 0);
}
