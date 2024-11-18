#pragma once

#include "draw_mesh.h"

class ball 
{
private:
    const double radius = 0.85*LANE_WIDTH/2;
public:
    double x;
    double y;
    double color[3] = {180, 0.5, 1};

    ball(){};
    ball(double x, double y):x(x), y(y){};
    ~ball() = default;

    void move(double x0, double y0) {
        x = x0;
        y = y0;
    }

    void draw() {
        draw_sphere(x, y, radius, radius, color);
    }
};
 
void ball_init(ball& left_ball, ball& right_ball) {
    left_ball.x = 0;
    left_ball.y = LANE2;
    right_ball.x = 0;
    right_ball.y = LANE3;
}

void ball_move(const int left_eye_state, const int right_eye_state, ball& left_ball, ball& right_ball) {
    if (left_eye_state == -1) {
        left_ball.y = LANE1;
    } else if (left_eye_state == 1) {
        left_ball.y = LANE2;
    }
    if (right_eye_state == -1) {
        right_ball.y = LANE4;
    } else if (right_eye_state == 1) {
        right_ball.y = LANE3;
    }
}

void ball_draw(ball& left_ball, ball& right_ball) {
    left_ball.draw();
    right_ball.draw();
}

 
 