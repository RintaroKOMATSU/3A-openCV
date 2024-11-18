#pragma once


#include <cstdlib>
#include "utility.h"
#include "draw_mesh.h"
#include <deque>

class obstacle
{
private:

    double init_left_y() {

        int rand_val = int(std::rand()%2);
        if (rand_val == 0) {
            return 1.0;
        }
        else {
            return 3.0;
        }
    }
    double init_right_y() {
        int rand_val = int(std::rand()%2);
        if (rand_val == 0) {
            return -1.0;
        }
        else {
            return -3.0;
        }
    }
public:
    double verocity;
    double x;
    double obstacle_color[3] = {100, 0.5, 1};
    double left_y;
    double right_y;
    double size = 1.8;

    obstacle():x(-5), left_y(init_left_y()), right_y(init_right_y()), verocity(10) {}
    obstacle(double verocity): x(-30), left_y(init_left_y()), right_y(init_right_y()), verocity(verocity){}
    ~obstacle() = default;
    void move_obstacle() {
        double verocity_per_frame = verocity/FRAME_RATE;
        x += verocity_per_frame;
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

    void clear() {
        obs_list.clear();
    }
    void add(obstacle obs) {
        obs_list.push_back(obs);
    }
    void draw() {
        for (obstacle& obs : obs_list) {
            obs.draw_obstacle();
        }
    }
    void move() {
        for (obstacle& obs : obs_list) {
            obs.move_obstacle();
        }
    }

    void del_obstacle() {
        while(true) {
            obstacle obs = obs_list.front();
            if (obs.x > 100) {
                obs_list.pop_front();
            } else {
                break;
            }
        }
    }
};



