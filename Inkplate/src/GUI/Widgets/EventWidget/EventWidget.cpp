#include "EventWidget.hpp"

/** Check if we have multiple cores */
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define FLASHER_PADDING 3
#define FLASH_PERIOD 1000

EventWidget::EventWidget(Inkplate *display, int x, int y, int width, int height)
    : Widget(display, x, y, width, height),
      _flasherArea(x + FLASHER_PADDING, y + FLASHER_PADDING, x + (width * 0.10f), height - FLASHER_PADDING * 2),
      _statusArea(_flasherArea.Right() + STATUS_AREA_PADDING, y + STATUS_AREA_PADDING, width - (_flasherArea.Right() + STATUS_AREA_PADDING), STATUS_AREA_PADDING),
      _textArea(_flasherArea.Right() + FOLLOWER_AREA_PADDING, _statusArea.Bottom() + FOLLOWER_AREA_PADDING, width - (_flasherArea.Right() + FOLLOWER_AREA_PADDING), height - (_statusArea.Bottom() + FOLLOWER_AREA_PADDING))
{
}

// TODO: Add text wrapping
void EventWidget::DrawChanged()
{
    // Now do I make this draw itself or do I have it return true to let it know something has changed?
    // Call the base DrawChanged
    Widget::DrawChanged();

    // Draw the flasher
    if (!_flasherState)
    {
        _display->drawRect(_flasherArea.x, _flasherArea.y, _flasherArea.width, _flasherArea.height, 1);
    }
    else
    {
        _display->fillRect(_flasherArea.x, _flasherArea.y, _flasherArea.width, _flasherArea.height, 1);
    }

    // Draw the status
    this->_display->setTextSize(TEXT_SIZE);
    this->_display->setCursor(_statusArea.x, _statusArea.y);
    this->_display->print(_statusText.c_str());

    // Draw the followers name
    this->_display->setCursor(_textArea.x, _textArea.y);
    this->_display->print(_currentText.c_str());
}

void EventWidget::UpdateText(std::string text)
{
    _changed = true;
    _currentText = text;

    StartFlasher();
}

void EventWidget::Notify(DynamicJsonDocument &data)
{
    UpdateText(data["event"]["user_name"]);
}

typedef struct FlasherParams
{
    EventWidget *widget;
    std::function<void()> callback;
} FlasherParams;

void EventWidget::AnimateFlasher(FlasherParams *params)
{
    EventWidget *widget = params->widget;
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

void EventWidget::StartFlasher()
{
    StartFlasher(std::function<void()>());
}

void EventWidget::StartFlasher(std::function<void()> callback)
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

void EventWidget::OnFlasherEnd()
{
}