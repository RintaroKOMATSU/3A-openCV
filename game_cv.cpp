#include <opencv2/opencv.hpp>
#include <iostream>
#include "game_gl.h"
#include "utility.h"

args_game* args_cv;
cv::Mat frame;

void draw_score();

void* thread_cv(void* args) {
    cv::namedWindow("game", 1);
    args_cv = (args_game*)args;
    while (true) {
        cv::Mat frame_tmp = cv::Mat(WINDOW_Y, WINDOW_X, CV_8UC3, args_cv->pixels.data());
        cv::flip(frame_tmp, frame, 0);
        
        if (args_cv->game_mode == 2) {
            draw_score();
        }
        cv::imshow("game", frame);
        int key = cv::waitKey(int(1000/2/FRAME_RATE));
        if (key == 'q') {
            exit(0);
        }
    }
    return nullptr;
}

void draw_score() {
    cv::Point position(100, 250);
    // フォント、サイズ、色の指定

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;   // シンプルなサンセリフ体
    double fontScale = 1.0;                     // フォントのサイズ
    cv::Scalar color(0, 255, 0);                // 緑色（BGR）
    std::string text = "Score "+std::to_string(args_cv->score);
    // テキストを画像に描画
    cv::putText(frame, text,position, fontFace, fontScale, color, 2, cv::LINE_AA);

}