#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include <Inkplate.h>

#include "GUI/Widgets/Widget.hpp"

class ButtonWidget : public Widget
{
private:
    std::string buttonText;

    bool state;

    uint8_t touchpadButton;

public:
    ButtonWidget(Inkplate *display, int x, int y, int width, int height, std::string buttonText, uint8_t touchpadButton);
    void Update();
    void DrawChanged();
};

#endif