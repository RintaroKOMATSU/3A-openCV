#pragma once

#include "draw_mesh.h"

void draw_world() {
    double load_color[3] = LOAD_COLOR;
    double line_color[3] = LINE_COLOR;
    double world_color[3] = WORLD_COLOR;
    draw_plane(-50, 0, 0, 150, 4*LANE_WIDTH, load_color);
    draw_plane(-50, 0, -0.1, 150, 200, world_color);
    double y = LANE1 - LANE_WIDTH/2;
    for (int i = 0; i < 5; i ++) {
        draw_plane(-50, y, 0.01, 150, 0.1*LANE_WIDTH, line_color);
        y += LANE_WIDTH;
    }
    
}