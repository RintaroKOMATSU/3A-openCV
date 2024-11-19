#pragma once

#include "draw_mesh.h"
#include "obstacle.h"
#include "utility.h"

class ball 
{
  
public:
    double x;
    double y;
    double color[3] =  BALL_COLOR;
    const double radius = BALL_RADIUS;

    ball(){};
    ball(double x, double y):x(x), y(y){};
    ~ball() = default;

    void move(double x0, double y0) {
        x = x0;
        y = y0;
    }

    void draw() {
        draw_sphere(x, y, radius, radius, color);
        double shadow_color[3] = LOAD_COLOR;
        shadow_color[2] *= 0.7;
        draw_circle(x, y, 0.015, 0.9*radius, shadow_color);
    }
};
 
void ball_init(ball& left_ball, ball& right_ball) {
    left_ball.x = 0;
    left_ball.y = LANE3;
    right_ball.x = 0;
    right_ball.y = LANE2;
}

void ball_move(const int left_eye_state, const int right_eye_state, ball& left_ball, ball& right_ball) {
    if (left_eye_state == -1) {
        left_ball.y = LANE4;
    } else if (left_eye_state == 1) {
        left_ball.y = LANE3;
    }
    if (right_eye_state == -1) {
        right_ball.y = LANE1;
    } else if (right_eye_state == 1) {
        right_ball.y = LANE2;
    }
}

void ball_draw(ball& left_ball, ball& right_ball) {
    left_ball.draw();
    right_ball.draw();
}

int collosion(ball& left_ball, ball& right_ball, obstacle_list& obs_list) {
    for (int i = 0; i < obs_list.size(); i++) {
        obstacle obs = obs_list[i];
        if (left_ball.x - left_ball.radius < obs.x && obs.x < left_ball.x + left_ball.radius) {
            if (left_ball.y == obs.left_y || right_ball.y == obs.right_y) {
                return i;
            }
        }
    }
    return -1;
}
