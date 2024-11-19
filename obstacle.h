#pragma once


#include <cstdlib>
#include "utility.h"
#include "draw_mesh.h"
#include <deque>

class obstacle
{
private:
    const double size = 0.9*LANE_WIDTH;

    double init_left_y() {

        int rand_val = int(std::rand()%2);
        if (rand_val == 0) {
            return LANE3;
        }
        else {
            return LANE4;
        }
    }
    double init_right_y() {
        int rand_val = int(std::rand()%2);
        if (rand_val == 0) {
            return LANE2;
        }
        else {
            return LANE1;
        }
    }
public:
    double verocity;
    double x;
    double x_min = -80;
    double x_max = 100;
    double obstacle_color[3] = OBSTACLE_COLOR;
    double left_y;
    double right_y;
    
    obstacle():left_y(init_left_y()), right_y(init_right_y()), verocity(20){
        x = x_min;
    }
    obstacle(double verocity): left_y(init_left_y()), right_y(init_right_y()), verocity(verocity) {
        x = x_min;
    }
    ~obstacle() = default;
    void move_obstacle(int& score) {
        double verocity_per_frame = verocity/FRAME_RATE;
        double x_before = x;
        x += verocity_per_frame;
        if (x_before < BALL_RADIUS && x > BALL_RADIUS) {
            score += 1;
        }
    }
    
    void draw_obstacle() {
        draw_cube(x, right_y, size/2, size/10, size, size, obstacle_color);
        draw_cube(x, left_y, size/2, size/10, size, size, obstacle_color);
    }

    
};

class obstacle_list
{
public:
    std::deque<obstacle> obs_list;
    obstacle_list() {}
    ~obstacle_list() = default;

    obstacle operator[](int i) const { 
        return obs_list[i]; 
    }
    obstacle& operator[](int i) {
        return obs_list[i]; }

    void clear() {
        obs_list.clear();
    }
    int size() {
        return obs_list.size();
    }

    void add(obstacle obs) {
        obs_list.push_back(obs);
    }
    void draw() {
        for (obstacle& obs : obs_list) {
            obs.draw_obstacle();
        }
    }
    void move(int& score) {
        for (obstacle& obs : obs_list) {
            obs.move_obstacle(score);
        }
    }

    void del_obstacle() {
        while(true) {
            obstacle obs = obs_list.front();
            if (obs.x > obs.x_max) {
                obs_list.pop_front();
            } else {
                break;
            }
        }
    }


};



