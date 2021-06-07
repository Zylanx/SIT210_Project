#include "EventWidget.hpp"

/** Check if we have multiple cores */
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

EventWidget::EventWidget(Inkplate *display, int x, int y, int width, int height)
    : Widget(display, x, y, width, height),
      _statusArea(_flasherArea.Right() + STATUS_AREA_PADDING, y + STATUS_AREA_PADDING, width - (_flasherArea.Right() + STATUS_AREA_PADDING), STATUS_AREA_PADDING),
      _textArea(_flasherArea.Right() + FOLLOWER_AREA_PADDING, _statusArea.Bottom() + FOLLOWER_AREA_PADDING, width - (_flasherArea.Right() + FOLLOWER_AREA_PADDING), height - (_statusArea.Bottom() + FOLLOWER_AREA_PADDING))
{
}

void EventWidget::Update()
{
    Widget::Update();

    Serial.println(_statusArea.x);
    Serial.println(_statusArea.y);
    Serial.println(_statusArea.width);
    Serial.println(_statusArea.height);

    Serial.println(_textArea.x);
    Serial.println(_textArea.y);
    Serial.println(_textArea.width);
    Serial.println(_textArea.height);
}

// TODO: Add text wrapping
void EventWidget::DrawChanged()
{
    // Now do I make this draw itself or do I have it return true to let it know something has changed?
    // Call the base DrawChanged
    Widget::DrawChanged();

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