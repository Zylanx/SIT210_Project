#ifndef FOLLOW_WIDGET_H
#define FOLLOW_WIDGET_H

#include <string>

#include <Inkplate.h>

#include "GUI/position/Position.hpp"
#include "GUI/Widgets/EventWidget/EventWidget.hpp"

class FollowWidget : public EventWidget
{
public:
    FollowWidget(Inkplate *display, int x, int y, int width, int height);

    std::string NotificationType()
    {
        return "channel.follow";
    }
};

#endif