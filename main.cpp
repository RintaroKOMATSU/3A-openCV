
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include "utility.h"
#include "obstacle.h"
#include "vec2.h"
#include "ball.h"
#include "shm.h"
#include "world.h"

//opengl setting
void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_time(int value);
void glut_time_game();//game screen
void glut_time_home();//home screen

double g_distance = 7.0;

int eyes_state;
int right_eye_state;
int left_eye_state;

bool shm_initialized = false;

obstacle_list obs_list = obstacle_list();
ball left_ball, right_ball;
double obs_verocity = 20;

int game_mode = 0;



int main(int argc, char** argv) {
	init_GL(argc, argv);
	/* このプログラム特有の初期化 */
	init();

	/* コールバック関数の登録 */
	set_callback_functions();
	glutPostRedisplay();
	/* メインループ */
	glutMainLoop(); 
	shm_cleanup();
}

void init_GL(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA |GLUT_DEPTH| GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow(WINDOW_NAME);
}

void init(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//initialize shared memory
	shm_initialized = shm_init();
	ball_init(left_ball, right_ball);

}
	
void set_callback_functions(){
	glutDisplayFunc(glut_display);
    glutKeyboardFunc(glut_keyboard);
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

void glut_display(){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1.5, 0.1, 120);
	gluLookAt(g_distance, 0, g_distance/2,-30, 0, 0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	draw_world();
	ball_draw(left_ball, right_ball);
	obs_list.draw();
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

	

void glut_time(int value) {
	if (game_mode == 0) {
		if (!shm_initialized) {
			shm_initialized =  shm_init();
		} else {
			game_mode = 1;
		}
	} else if (game_mode == 1) {
		glut_time_home();
	} else if (game_mode == 2) {
		glut_time_game();
	}
	glutPostRedisplay();
	glutTimerFunc(int(1000 / FRAME_RATE), glut_time, 0);
}



void glut_time_game() {
	static int obs_interval_count = 0;
	static double obs_interval = obs_verocity/20;//second
	static int obs_interval_count_max = int(FRAME_RATE*obs_interval);

	//collision
	if (collosion(left_ball, right_ball, obs_list) > 0) {
		sleep(10);
	}
	//get eyes state from shared memory data
	get_eyes_state(eyes_state, left_eye_state, right_eye_state);
	//move balls
	ball_move(left_eye_state, right_eye_state, left_ball, right_ball);
	//update obstacles
	if (obs_interval_count == obs_interval_count_max) {
		obs_list.add(obstacle(obs_verocity));
		obs_list.del_obstacle();
		obs_interval_count = 0;
	}
	obs_interval_count ++;
	obs_list.move();
}