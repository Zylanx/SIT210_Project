#include "Widget.hpp"

/** Check if we have multiple cores */
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define TEXT_SIZE 3

#define FLASHER_PADDING 3
#define FLASH_PERIOD 1000

Widget::Widget(Inkplate *display, int x, int y, int width, int height)
    : _display(display),
      _position(x, y, width, height),
      _flasherArea(x + FLASHER_PADDING, y + FLASHER_PADDING, x + (width * 0.10f), height - FLASHER_PADDING * 2)
{
}

void Widget::Update()
{
    // Blank for now
    Serial.println(_position.x);
    Serial.println(_position.y);
    Serial.println(_position.width);
    Serial.println(_position.height);

    Serial.println(_flasherArea.x);
    Serial.println(_flasherArea.y);
    Serial.println(_flasherArea.width);
    Serial.println(_flasherArea.height);
}

void Widget::DrawChanged()
{
    // Clear the area
    _display->fillRect(_position.x, _position.y, _position.width, _position.height, 0);

    // Draw the frame
    _display->drawRect(_position.x, _position.y, _position.width, _position.height, 1);

    // Draw the flasher
    if (!_flasherState)
    {
        _display->drawRect(_flasherArea.x, _flasherArea.y, _flasherArea.width, _flasherArea.height, 1);
    }
    else
    {
        _display->fillRect(_flasherArea.x, _flasherArea.y, _flasherArea.width, _flasherArea.height, 1);
    }
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

typedef struct FlasherParams
{
    Widget *widget;
    std::function<void()> callback;
} FlasherParams;

void Widget::AnimateFlasher(FlasherParams *params)
{
    Widget *widget = params->widget;
    std::function<void()> callback = params->callback;
    delete params;

    for (int i = 0; i < (3 * 2); i++)
    {
        widget->_flasherState = !widget->_flasherState;
        widget->_changed = true;

        vTaskDelay(FLASH_PERIOD / portTICK_PERIOD_MS);
    }

    widget->_flasherState = false;
    widget->_changed = true;

    widget->_flasherTask = nullptr;
    vTaskDelete(NULL);

    if (callback != nullptr)
    {
        callback();
    }

    widget->OnFlasherEnd();
}

void Widget::StartFlasher()
{
    StartFlasher(std::function<void()>());
}

void Widget::StartFlasher(std::function<void()> callback)
{
    if (_flasherTask != nullptr)
    {
        eTaskState flasherTaskState = eTaskGetState(_flasherTask);
        Serial.println(flasherTaskState);

        if (flasherTaskState == eDeleted)
        {
            _flasherTask = nullptr;
        }
        else
        {
            vTaskDelete(_flasherTask);
        }
    }

    FlasherParams *params = new FlasherParams{this, callback};

    xTaskCreatePinnedToCore((TaskFunction_t)AnimateFlasher, "FlasherAnimate", 1024, params, 1, &_flasherTask, ARDUINO_RUNNING_CORE);
}

void Widget::OnFlasherEnd()
{
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