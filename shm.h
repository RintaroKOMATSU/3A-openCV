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

const char* shm_name = "landmarks";
const size_t shm_size = 24 * sizeof(double);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int shm_fd;
double* data;

typedef struct shm_thread_args{
    int *eye_state;
    pthread_mutex_t* eye_state_lock;
}ShmTreadArgs;

//initializer
bool shm_init() {
    shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cerr << "Failed to open shared memory\n";
        return false;
    }

    data = (double*)mmap(0, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        std::cerr << "Failed to map shared memory\n";
        close(shm_fd);
        return false;
    }
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
    shm_init();
    std::vector<vec2> right_landmarks(6, vec2(0.0, 0.0));
    std::vector<vec2> left_landmarks(6, vec2(0.0, 0.0));
    while(true) {
        shm_fetch_data(right_landmarks, left_landmarks);
        int eyes_state = eval_eyes_state(left_landmarks, right_landmarks);
        print_eyes_state(eyes_state);
        usleep(int(1000/FRAME_RATE));
        pthread_mutex_lock(args ->eye_state_lock);
        *args->eye_state = eyes_state;
        pthread_mutex_unlock(args->eye_state_lock);
    }
    shm_cleanup();
}