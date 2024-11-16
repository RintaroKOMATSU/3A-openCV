#include <iostream>
#include <vector>
#include "vec2.h"
#include "eye.h"
#include "shm.h"

#define FRAME_RATE 60


int shm() {

    shm_init();
    std::vector<vec2> right_landmarks(6, vec2(0.0, 0.0));
    std::vector<vec2> left_landmarks(6, vec2(0.0, 0.0));
    while(true) {
        shm_fetch_data(right_landmarks, left_landmarks);
        int eyes_state = eval_eyes_state(left_landmarks, right_landmarks);
        print_eyes_state(eyes_state);
        usleep(int(1000/FRAME_RATE));
    }
    shm_cleanup();
}