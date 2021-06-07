#include "SubscribeWidget.hpp"

SubscribeWidget::SubscribeWidget(Inkplate *display, int x, int y, int width, int height)
    : EventWidget(display, x, y, width, height)
{
    _statusText = "Latest Subscriber";
}