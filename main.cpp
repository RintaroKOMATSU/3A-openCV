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
	game(argc, argv);
	return 0;
}



