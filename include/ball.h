#pragma once

#include <cstdlib>
#include <vector>
#include <iostream>
#include "draw_mesh.h"
#include "obstacle.h"
#include "utility.h"
#include "sound.h"

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

    void move(double y0) {
        if (y0 !=  y) {
            playMusic("SE/se1.mp3");
        }
        y = y0;
        
    }

    inline void draw() {
        draw_sphere(x, y, radius, radius, color);
        double shadow_color[3] = LOAD_COLOR;
        shadow_color[2] *= 0.8;
        draw_circle(x, y, 0.015, 0.9*radius, shadow_color);
    }
};
 
void ball_init(ball& left_ball, ball& right_ball) {
    left_ball.x = 0;
    left_ball.y = LANE3;
    right_ball.x = 0;
    right_ball.y = LANE2;
}

void ball_reset(ball& left_ball, ball& right_ball) {
    left_ball.x = 0;
    left_ball.y = LANE3;
    right_ball.x = 0;
    right_ball.y = LANE2;
}

void ball_move(const int left_eye_state, const int right_eye_state, ball& left_ball, ball& right_ball) {
    if (left_eye_state == -1) {
        left_ball.move(LANE4);
    } else if (left_eye_state == 1) {
        left_ball.move(LANE3);
    }
    if (right_eye_state == -1) {
        right_ball.move(LANE1);
    } else if (right_eye_state == 1) {
        right_ball.move(LANE2);
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

class ball_fragment
{
  
public:
    vec3 position;
    vec3 direction;
    double angle_x, angle_y, angle_z;
    double color[3] =  BALL_COLOR;
    double verocity = 18;
    const double size = 0.6*BALL_RADIUS;
    double radius = 0.3*BALL_RADIUS;
    double distance;

    ball_fragment(vec3 pos){
        angle_x = std::rand() % 360;
        angle_y = std::rand() % 360;
        angle_z = std::rand() % 360;
        double angle1 = (double)std::rand()/(double)RAND_MAX*M_PI*2;
        double angle2 = (double)std::rand()/(double)RAND_MAX*M_PI;
        direction = vec3(sin(angle2)*cos(angle1), sin(angle2)*sin(angle1), cos(angle2));
        position = pos + radius*direction;
    };
    ~ball_fragment() = default;

    void move() {
        position += (verocity/FRAME_RATE)*direction;
    }

    inline void draw() {
        draw_fragent(position, angle_x, angle_y, angle_z, size, color);
    }
};
 
class fragment_list
{
public:
    std::vector<ball_fragment> fragments;
    unsigned int number = 200;
    vec3 position;

    fragment_list() {}
    fragment_list(double x0, double y0, double z0) {
        position = vec3(x0, y0, z0);
        for ( int i = 0; i < number; i++) {
            ball_fragment fragment(position);
            fragments.push_back(fragment);
        }
    }

    void init(double x0, double y0, double z0) {
        clear();
        position = vec3(x0, y0, z0);
        for ( int i = 0; i < number; i++) {
            ball_fragment fragment(position);
            fragments.push_back(fragment);
        }
    }

    void move() {
        for ( int i = 0; i < fragments.size(); i++) {
            fragments[i].move();
        }
    }

    void draw() {
        for ( int i = 0; i < fragments.size(); i++) {
            fragments[i].draw();
        }
    }
    void clear() {
        fragments.clear();
    }

    int size() {
        return fragments.size();
    }

};