#pragma once
#include "vec2.h"

bool shm_init() ;
void shm_cleanup();
void shm_write_data(double flag) ;
void shm_fetch_data(std::vector<vec2>& right_landmarks, std::vector<vec2>& left_landmarks);
void get_eyes_state(int& eyes_state, int& left_eye_state, int& right_eye_state);