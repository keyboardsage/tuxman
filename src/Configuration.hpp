#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <vector>

typedef std::vector<int> BonusTimes;
typedef struct {
    int tile_size;
    int pixel_depth;
    int map_width;
    int map_height;
    std::string bonus_type;
    BonusTimes* bonus_times;
    int tuxman_position;
    int lives;
    int real_tile_width;
    int real_tile_height;
    int begin_stage_wait;
} Configuration;

#endif
