#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

class Position
{
public:
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;

    Position(int x, int y, int width, int height);

    int16_t Right();
    int16_t Bottom();
};

#endif