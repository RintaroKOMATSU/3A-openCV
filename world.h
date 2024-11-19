#pragma once

#include "draw_mesh.h"

void draw_world() {
    double load_color[3] = {30, 0.5, 1};
    double line_color[3] = {0, 0, 1};
    draw_plane(0, 0, 0, 120, 4*LANE_WIDTH, load_color);
    double y = LANE1 - LANE_WIDTH/2;
    for (int i = 0; i < 5; i ++) {
        draw_plane(0, y, 0.01, 120, 0.1*LANE_WIDTH, line_color);
        y += LANE_WIDTH;
    }
    
}