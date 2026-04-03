#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include "utility.h"

#include "vec2.h"

#define THRESHOLD_CLOSE  0.18
#define THRESHOLD_OPEN 0.2


int eval_left_eye_close (const std::vector<vec2> eye_landmarks) {
    static int eye_stat_prev = -2;
    static int eye_stat = -2;
    static int eye_stat_next = -2;
    eye_stat_prev = eye_stat;
    eye_stat = eye_stat_next;

    vec2 p0 = eye_landmarks[0];
    vec2 p1 = eye_landmarks[1];
    vec2 p2 = eye_landmarks[2];
    vec2 p3 = eye_landmarks[3];
    vec2 p4 = eye_landmarks[4];
    vec2 p5 = eye_landmarks[5];

    double A = distance(p1, p5);
    double B = distance(p2, p4);
    double C = distance(p0, p3);
    
    double eye_ratio = (A+B) /(2.0*C);
    if (eye_ratio <THRESHOLD_CLOSE) {
        eye_stat_next = -1; //close
    } else {
        eye_stat_next = 1; //open
    }
    if (eye_stat == -2) {
        eye_stat = eye_stat_next;
        eye_stat_prev = eye_stat_next;
    }
    if (eye_stat_prev == eye_stat_next && eye_stat != eye_stat_next) {
        eye_stat = eye_stat_prev;
    }
    return eye_stat;
}

int eval_right_eye_close (const std::vector<vec2> eye_landmarks) {
    static int eye_stat_prev = -2;
    static int eye_stat = -2;
    static int eye_stat_next = -2;
    eye_stat_prev = eye_stat;
    eye_stat = eye_stat_next;

    vec2 p0 = eye_landmarks[0];
    vec2 p1 = eye_landmarks[1];
    vec2 p2 = eye_landmarks[2];
    vec2 p3 = eye_landmarks[3];
    vec2 p4 = eye_landmarks[4];
    vec2 p5 = eye_landmarks[5];

    double A = distance(p1, p5);
    double B = distance(p2, p4);
    double C = distance(p0, p3);
    
    double eye_ratio = (A+B) /(2.0*C);
    if (eye_ratio <THRESHOLD_CLOSE) {
        eye_stat_next = -1; //close
    } else {
        eye_stat_next = 1; //open
    }
    if (eye_stat == -2) {
        eye_stat = eye_stat_next;
        eye_stat_prev = eye_stat_next;
    }
    if (eye_stat_prev == eye_stat_next && eye_stat != eye_stat_next) {
        eye_stat = eye_stat_prev;
    }
    return eye_stat;
}

int eval_eyes_state(const std::vector<vec2> left_eye_landmarks, const std::vector<vec2> right_eye_landmarks, int& left_eye_stat, int& right_eye_stat) {
    int eyes_stat = 0;
    left_eye_stat = eval_left_eye_close(left_eye_landmarks);
    right_eye_stat = eval_right_eye_close(right_eye_landmarks);
    if (right_eye_stat == 1 && left_eye_stat == -1) {
        eyes_stat = -1; //left close
    } else if (right_eye_stat == -1 && left_eye_stat == 1) {
        eyes_stat = 1;//right close
    } else if  (right_eye_stat == -1 && left_eye_stat == -1) {
        eyes_stat = 2;//both close
    }
    
    return eyes_stat;
}


void print_eyes_state(const int eyes_stat) {
    switch (eyes_stat) {
    case 0:
        std::cout << "open both  \r";
        break;
    case 1:
        std::cout << "left wink  \r";
        break;
    case -1:
        std::cout << "right wink  \r";
        break;
    case 2:
        std::cout << "close both   \r";
        break;
    }
}