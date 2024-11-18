#pragma once

#include <vector>
#include <fcntl.h>    // shm_open
#include <sys/mman.h> // mmap
#include <unistd.h>   // close
#include <pthread.h>
#include "vec2.h"     // vec2クラスが定義されているヘッダー
#include "eye.h"
#include "utility.h"
#include <iostream>

#define DURATION_TIME 0.01 //(second) 
int duration_count_max = int(FRAME_RATE*DURATION_TIME);
int duration_count = 0;
int prev_eye_state;

const char* shm_name = "landmarks";
const size_t shm_size = 24 * sizeof(double);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int shm_fd;
double* data;

typedef struct shm_thread_args{
    int *eye_state;
    int *left_eye_state;
	int *right_eye_state;
    pthread_mutex_t* eye_state_lock;
}ShmTreadArgs;

//initializer
bool shm_init() {
    shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cout << "Failed to open shared memory\r";
        return false;
    }

    data = (double*)mmap(0, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        std::cout << "Failed to map shared memory\r";
        close(shm_fd);
        return false;
    }
    std::cout << "                                     \r";
    return true;
}

//cleanup
void shm_cleanup() {
    munmap(data, shm_size);
    close(shm_fd);
    shm_unlink(shm_name);
}


//fetch data from shared memory
void shm_fetch_data(std::vector<vec2>& right_landmarks, std::vector<vec2>& left_landmarks) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 6; i++) {
        right_landmarks[i] = vec2(data[2 * i], data[2 * i + 1]);
        left_landmarks[i] = vec2(data[2 * i + 12], data[2 * i + 13]);
    }
    pthread_mutex_unlock(&lock);
}


//shm thread function
void *shm(void *arg) {
    ShmTreadArgs* args = static_cast<ShmTreadArgs*>(arg);
    bool shm_init_success = shm_init();
    while (!shm_init_success) {
        shm_init_success = shm_init();
        sleep(1);
    }
    std::vector<vec2> right_landmarks(6, vec2(0.0, 0.0));
    std::vector<vec2> left_landmarks(6, vec2(0.0, 0.0));
    while(true) {
        if (duration_count == 0) {
            shm_fetch_data(right_landmarks, left_landmarks);
            int left_eye_stat, right_eye_stat;
            int eyes_stat = eval_eyes_state(left_landmarks, right_landmarks, *args ->left_eye_state, *args -> right_eye_state);
            print_eyes_state(eyes_stat);
            usleep(int(1000/FRAME_RATE));
            pthread_mutex_lock(args ->eye_state_lock);
            *args->eye_state = eyes_stat;
            *args->right_eye_state = right_eye_stat;
            *args->left_eye_state = left_eye_stat;
            pthread_mutex_unlock(args->eye_state_lock);
        }
        if (duration_count == duration_count_max) {
            duration_count = 0;
            continue;
        }
        duration_count ++;
    }
}