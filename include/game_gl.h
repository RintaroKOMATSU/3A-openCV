#pragma once

#include <cmath>
#include <GL/glut.h>
#include <vector>
#include "utility.h"

typedef struct {
	std::vector<unsigned char> pixels;
	int game_mode;
    int score;
	//pixels(WINDOW_X * WINDOW_Y * 3, 0) {}
} args_game;

void* thread_gl(void* args);

void game_quit();