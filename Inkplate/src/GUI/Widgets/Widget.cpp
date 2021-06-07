#include "Widget.hpp"

/** Check if we have multiple cores */
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define TEXT_SIZE 3

Widget::Widget(Inkplate *display, int x, int y, int width, int height)
    : _display(display),
      _position(x, y, width, height)
{
}

void Widget::Update()
{
}

void Widget::DrawChanged()
{
    // Clear the area
    _display->fillRect(_position.x, _position.y, _position.width, _position.height, 0);

    // Draw the frame
    _display->drawRect(_position.x, _position.y, _position.width, _position.height, 1);
}

// TODO: Add text wrapping
bool Widget::Draw()
{
    // Now do I make this draw itself or do I have it return true to let it know something has changed?
    if (_changed)
    {
        _changed = false;

        this->DrawChanged();

        return true;
    }

    return false;
}

int16_t Widget::Left()
{
    return _position.x;
}

int16_t Widget::Top()
{
    return _position.y;
}

int16_t Widget::Right()
{
    return _position.Right();
}

int16_t Widget::Bottom()
{
    return _position.Bottom();
}

std::string Widget::NotificationType()
{
    return "";
}

void Widget::Notify(DynamicJsonDocument &data)
{
    ;
}