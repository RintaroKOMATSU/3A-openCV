#pragma once

//window setting
#define WINDOW_X (960)
#define WINDOW_Y (540)
#define WINDOW_NAME "main"
#define FRAME_RATE 30

//game lane setting
#define LANE1 -3.0
#define LANE2 -1.0
#define LANE3 1.0
#define LANE4 3.0
#define LANE_WIDTH 2.0

#define BALL_RADIUS 0.65*LANE_WIDTH/2
#define BALL_COLOR {180, 0.5, 1}
#define BALL_SHADOW_COLOR {30, 0.5, 0.5}

#define OBSTACLE_COLOR {0, 0.7, 1}

#define LOAD_COLOR {30, 0.5, 1}
#define LINE_COLOR {0, 0, 1}
#define WORLD_COLOR {100, 0.5, 1}