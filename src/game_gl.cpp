

#include <cmath>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "utility.h"
#include "obstacle.h"
#include "vec2.h"
#include "ball.h"
#include "shm.h"
#include "world.h"
#include "game_gl.h"
#include "sound.h"

using namespace std::chrono;
//opengl setting
void init_GL(int argc, char *argv[]);
void init();
void set_callback_functions();

void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void glut_time(int value);
void glut_time_game();//game screen
void glut_time_home();//home screen
void game_over();
void game_over1();



double g_distance = 7.0;

int eyes_state;
int right_eye_state;
int left_eye_state;

bool shm_initialized = false;

obstacle_list obs_list = obstacle_list();
fragment_list left_fragments = fragment_list();
fragment_list right_fragments = fragment_list();
ball left_ball, right_ball;
double obs_verocity = 20.0;



args_game* args_gl;

void* thread_gl(void* args) {
	args_gl = (args_game*)args;
	char** argv;
	init_GL(0, argv);
	/* このプログラム特有の初期化 */
	init();

	/* コールバック関数の登録 */
	set_callback_functions();
	glutPostRedisplay();
	/* メインループ */
	glutMainLoop(); 
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
	//initialize shared memory
	shm_initialized = shm_init();
	ball_init(left_ball, right_ball);
	initAudio();
}
	
void set_callback_functions(){
	glutDisplayFunc(glut_display);
	glutTimerFunc(int(1000/FRAME_RATE), glut_time, 0);
}

void glut_display(){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)WINDOW_X/(double)WINDOW_Y, 0.1, 120);
	gluLookAt(7, 0, 6,-7, 0, 0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
	draw_world();
	if (args_gl ->game_mode == 0 ||args_gl ->game_mode == 1 ||args_gl ->game_mode == 2) {
		ball_draw(left_ball, right_ball);
	} else if (args_gl ->game_mode == 3) {
		right_fragments.draw();
		left_fragments.draw();
	}
	obs_list.draw();
    glDisable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

void glut_time(int value) {
	auto t1 = system_clock::now();
	if (args_gl-> game_mode == 0) {
		if (!shm_initialized) {
			shm_initialized =  shm_init();
		} else {
			args_gl -> game_mode = 1;
		}
	} else if (args_gl -> game_mode == 1) {
		glut_time_home();
	} else if (args_gl -> game_mode == 2) {
		glut_time_game();
	} else if (args_gl -> game_mode == 3) {
		game_over();
	} else if (args_gl ->game_mode == 5) {
		game_over1();
	}
	glutPostRedisplay();
	glReadPixels(0, 0, WINDOW_X, WINDOW_Y, GL_BGR, GL_UNSIGNED_BYTE, args_gl->pixels.data());
	auto t2 = system_clock::now();
	int duration = int(duration_cast<milliseconds>(t2-t1).count());
	glutTimerFunc(fmax(1, int(1000/(double)FRAME_RATE)-duration), glut_time, 0);
}



void glut_time_game() {
	static int obs_interval_count = 0;
	static double obs_interval = (double)obs_verocity/40;//second
	static int obs_interval_count_max = int((double)FRAME_RATE*obs_interval);
	//collision
	if (collosion(left_ball, right_ball, obs_list) >= 0) {
		args_gl->game_mode = 3;
		playMusic("SE/se2.mp3");
		return ;
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
	obs_list.move(args_gl ->score);
}

void glut_time_home() {
	//move balls
	ball_move(left_eye_state, right_eye_state, left_ball, right_ball);

}

void game_over() {
	//reset
	static int count_game_over = 0;
	if (count_game_over== 0) {
		left_fragments.init(left_ball.x, left_ball.y, left_ball.radius);
		right_fragments.init(right_ball.x, right_ball.y, right_ball.radius);
	} else if (count_game_over == 4*FRAME_RATE) {
		args_gl ->game_mode = 4;
		count_game_over = 0;
		return;
	} else if (count_game_over == 1*FRAME_RATE) {
		right_fragments.clear();
		left_fragments.clear();
		ball_reset(left_ball, right_ball);
	}
	count_game_over += 1;
	left_fragments.move();
	right_fragments.move();
	glReadPixels(0, 0, WINDOW_X, WINDOW_Y, GL_BGR, GL_UNSIGNED_BYTE, args_gl->pixels.data());
}

void game_over1() {
	obs_list.clear();
	args_gl ->game_mode = 2;
}

void game_quit() {
    shm_write_data(0.0);
	shm_cleanup();
	cleanupAudio();
    exit(0);
}