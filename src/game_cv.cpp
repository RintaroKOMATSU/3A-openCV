#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "game_gl.h"
#include "utility.h"
#include "shm.h"
#include "sound.h"

using namespace cv;
args_game* args_cv;
cv::Mat frame;
int key;
int fontFace = cv::FONT_HERSHEY_SIMPLEX;  
cv::Scalar fontColor(255, 255, 255);  

void draw_score();
void draw_home();
void darken_frame();
void draw_gameover();
void drawRoundRectangle(Mat& image, Point topLeft, Point bottomRight, Scalar color, int thickness, int radius);

void* thread_cv(void* args) {
    cv::namedWindow("game", 1);
    args_cv = (args_game*)args;
    while (true) {
        cv::Mat frame_tmp = cv::Mat(WINDOW_Y, WINDOW_X, CV_8UC3, args_cv->pixels.data());
        cv::flip(frame_tmp, frame, 0);
        if (args_cv->game_mode == 0) {
            darken_frame();
        } else if (args_cv->game_mode == 1) {
            draw_home();
        } else if (args_cv->game_mode == 2) {
            draw_score();
        } else if (args_cv->game_mode == 3) {
            draw_score();
        } else if (args_cv->game_mode == 4) {
            draw_gameover();
        }
        cv::imshow("game", frame);
        key = cv::waitKey(int(1000/2/FRAME_RATE));
        if (key == 'q') {
            playMusic("SE/se3.mp3");
            sleep(1);
            game_quit();
        }
    }
    return nullptr;
}

void draw_score() {                            
    std::string text = std::to_string(args_cv->score)+ "pt";
    cv::putText(frame, text, Point(100, 45), fontFace, 1.3, fontColor, 3, cv::LINE_AA);
    
}

void draw_home() {
    darken_frame();
    if (key == 'a') {
        playMusic("SE/se3.mp3");
        args_cv->game_mode = 2;
    }
    cv::putText(frame, "start:A", cv::Point(280, 300), fontFace, 1.5, fontColor, 4, cv::LINE_AA );
    cv::putText(frame, "quit:Q", cv::Point(500, 300), fontFace, 1.5, fontColor, 4, cv::LINE_AA );
    drawRoundRectangle(frame, Point(250, 250), Point(670, 320), fontColor, 4, 20);
}


void draw_gameover() {
    darken_frame();

    std::string text = std::to_string(args_cv->score);
    cv::putText(frame, text, cv::Point(420, 250), fontFace, 2.5, fontColor, 6, cv::LINE_AA);
    cv::putText(frame, "pt", cv::Point(480 +60*int(log10((double)args_cv->score)), 250), fontFace, 1.7, fontColor, 6, cv::LINE_AA);
    cv::putText(frame, "replay:A", cv::Point(300, 350), fontFace, 1.5, fontColor, 4, cv::LINE_AA );
    cv::putText(frame, "quit:Q", cv::Point(520, 350), fontFace, 1.5, fontColor, 4, cv::LINE_AA );
    drawRoundRectangle(frame, Point(270, 150), Point(690, 370), fontColor, 4, 20);
    if (key == 'a' || key == 'A') {
        playMusic("SE/se3.mp3");
        args_cv->game_mode = 5;
    }

}

void darken_frame() {
    unsigned char scalar = 60;
    cv::subtract(frame, cv::Scalar(scalar, scalar, scalar), frame);
}

void drawRoundRectangle(Mat& image, Point topLeft, Point bottomRight, Scalar color, int thickness, int radius) {
    // Extract rectangle coordinates
    int x1 = topLeft.x, y1 = topLeft.y;
    int x2 = bottomRight.x, y2 = bottomRight.y;

    // Draw the four rounded corners using ellipses
    ellipse(image, Point(x1 + radius, y1 + radius), Size(radius, radius), 180, 0, 90, color, thickness); // Top-left
    ellipse(image, Point(x2 - radius, y1 + radius), Size(radius, radius), 270, 0, 90, color, thickness); // Top-right
    ellipse(image, Point(x2 - radius, y2 - radius), Size(radius, radius), 0, 0, 90, color, thickness);   // Bottom-right
    ellipse(image, Point(x1 + radius, y2 - radius), Size(radius, radius), 90, 0, 90, color, thickness);  // Bottom-left

    // Draw the straight edges
    line(image, Point(x1 + radius, y1), Point(x2 - radius, y1), color, thickness); // Top edge
    line(image, Point(x1 + radius, y2), Point(x2 - radius, y2), color, thickness); // Bottom edge
    line(image, Point(x1, y1 + radius), Point(x1, y2 - radius), color, thickness); // Left edge
    line(image, Point(x2, y1 + radius), Point(x2, y2 - radius), color, thickness); // Right edge
}

