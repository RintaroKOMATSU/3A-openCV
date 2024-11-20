#include <opencv2/opencv.hpp>
#include <iostream>
#include "game_gl.h"
#include "utility.h"
#include "shm.h"

args_game* args_cv;
cv::Mat frame;
int key;

void draw_score();
void draw_home();

void* thread_cv(void* args) {
    cv::namedWindow("game", 1);
    args_cv = (args_game*)args;
    while (true) {
        cv::Mat frame_tmp = cv::Mat(WINDOW_Y, WINDOW_X, CV_8UC3, args_cv->pixels.data());
        cv::flip(frame_tmp, frame, 0);
        if (args_cv->game_mode == 0) {

        } else if (args_cv->game_mode == 1) {
            draw_home();
        } else if (args_cv->game_mode == 2) {
            draw_score();
        } else if (args_cv->game_mode == 3) {
        }
        cv::imshow("game", frame);
        key = cv::waitKey(int(1000/2/FRAME_RATE));
        if (key == 'q') {
            shm_write_data(0.0);
            exit(0);
        }
    }
    return nullptr;
}

void draw_score() {
    cv::Point position(100, 45);
    // フォント、サイズ、色の指定
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;   
    double fontScale = 1.0;                 
    cv::Scalar color(255, 255, 255);                
    std::string text = "Score "+std::to_string(args_cv->score);
    // テキストを画像に描画
    cv::putText(frame, text,position, fontFace, fontScale, color, 2, cv::LINE_AA);
}

void draw_home() {
    if (key == 'a') {
        args_cv->game_mode = 2;
    }
}
