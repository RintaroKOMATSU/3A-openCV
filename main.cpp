#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include "vec2.h"
#include "eye.h"
#include "shm.h"
#include "game.h"


int main(int argc, char *argv[]){
	pthread_mutex_t eye_state_mutex = PTHREAD_MUTEX_INITIALIZER;
	int eye_state = 0;
	int right_eye_state = 0;
	int left_eye_state = 0;
	GameThreadArgs game_args = {argc, argv, &eye_state, &left_eye_state, &right_eye_state, &eye_state_mutex};
	ShmTreadArgs shm_args = {&eye_state,  &left_eye_state, &right_eye_state, &eye_state_mutex};
	pthread_t shm_thread, game_thread; 
	pthread_create(&game_thread, nullptr, game, &game_args);
	pthread_create(&shm_thread, nullptr, shm, &shm_args);

	pthread_join(game_thread, nullptr);
	pthread_join(shm_thread, nullptr);

	return 0;
}



