#ifndef SUBSCRIBE_WIDGET_H
#define SUBSCRIBE_WIDGET_H

#include <string>

#include <Inkplate.h>

#include "GUI/position/Position.hpp"
#include "GUI/Widgets/EventWidget/EventWidget.hpp"

class SubscribeWidget : public EventWidget
{
public:
    SubscribeWidget(Inkplate *display, int x, int y, int width, int height);

    std::string NotificationType()
    {
        return "channel.subscribe";
    }
};

#endif