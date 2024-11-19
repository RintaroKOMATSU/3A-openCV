#include "game_gl.h"
#include <pthread.h>

int main() {
	std::vector<unsigned char> pixelData(WINDOW_X * WINDOW_Y * 3, 255);
	int game_mode;
	int score;
	args_game args = {pixelData, game_mode, score};
	pthread_t thread1, thread2;
	pthread_create(&thread1, nullptr, thread_gl, );
	pthread_join(thread1, nullptr);
	return 0;
}