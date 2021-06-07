#include "FollowWidget.hpp"

FollowWidget::FollowWidget(Inkplate *display, int x, int y, int width, int height)
    : EventWidget(display, x, y, width, height)
{
    _statusText = "Latest Follower";
}