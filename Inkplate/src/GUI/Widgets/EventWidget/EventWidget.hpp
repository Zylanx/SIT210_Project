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

    Position _flasherArea;
    Position _statusArea;
    Position _textArea;

    TaskHandle_t _flasherTask = nullptr;
    volatile bool _flasherState = false;

    static void AnimateFlasher(FlasherParams *params);
    void StartFlasher();
    void StartFlasher(std::function<void()> callback);
    virtual void OnFlasherEnd();

    void DrawChanged();

    void FlasherEnd();

public:
    EventWidget(Inkplate *display, int x, int y, int width, int height);

    virtual void UpdateText(std::string text);

    virtual void Notify(DynamicJsonDocument &datas);
};

#endif