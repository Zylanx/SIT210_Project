#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include <string>

#include <Inkplate.h>

#include "GUI/position/Position.hpp"
#include "GUI/widgets/Widget.hpp"

#define TEXT_SIZE 3

#define STATUS_AREA_PADDING 20
#define FOLLOWER_AREA_PADDING 20

#define FLASH_PERIOD 1000

class EventWidget : public Widget
{
protected:
    std::string _statusText;
    std::string _currentText = "";

    Position _statusArea;
    Position _textArea;

    void DrawChanged();

    void FlasherEnd();

public:
    EventWidget(Inkplate *display, int x, int y, int width, int height);

    void Update();
    void UpdateText(std::string text);

    virtual void Notify(DynamicJsonDocument &datas);
};

#endif