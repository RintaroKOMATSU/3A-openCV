#include "game_gl.h"
#include "game_cv.h"
#include <pthread.h>

int main() {
	std::vector<unsigned char> pixelData(WINDOW_X * WINDOW_Y * 3, 255);
	int game_mode = 0;
	int score = 0;
	args_game args = {pixelData, game_mode, score};
	pthread_t thread1, thread2;
	pthread_create(&thread1, nullptr, thread_gl, &args);
	pthread_create(&thread2, nullptr, thread_cv, &args);
	pthread_join(thread1, nullptr);
	pthread_join(thread2, nullptr);
	return 0;
}