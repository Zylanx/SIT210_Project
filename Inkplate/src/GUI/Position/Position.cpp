#include "Position.hpp"

Position::Position(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

int16_t Position::Right()
{
    return x + width;
}

int16_t Position::Bottom()
{
    return y + height;
}