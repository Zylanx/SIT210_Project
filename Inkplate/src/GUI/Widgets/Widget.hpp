#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <functional>

#include <Inkplate.h>
#include <ArduinoJson.h>

#include "GUI/position/Position.hpp"

struct FlasherParams;

class Widget
{
protected:
    Inkplate *_display;

    bool _changed = true;

    uint8_t _maxTextLength;
    Position _position;

    virtual void DrawChanged();

public:
    Widget(Inkplate *display, int x, int y, int width, int height);

    virtual void Update();
    bool Draw();

    virtual std::string NotificationType();
    virtual void Notify(DynamicJsonDocument &data);

    int16_t Left();
    int16_t Top();
    int16_t Right();
    int16_t Bottom();
};

#endif