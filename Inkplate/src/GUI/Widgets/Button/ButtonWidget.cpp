#include "ButtonWidget.hpp"

#include <HTTPClient.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

ButtonWidget::ButtonWidget(Inkplate *display, int x, int y, int width, int height, std::string buttonText, uint8_t touchpadButton)
    : Widget(display, x, y, width, height)
{
    this->state = false;
    this->buttonText = buttonText;
    this->touchpadButton = touchpadButton;
}

void UpdateServer(void *params)
{
    std::string address = "http://zylinx.au.ngrok.io/enabled?enabled=";

    bool *state = (bool *)params;

    Serial.println((address + (*state ? "false" : "true")).c_str());

    HTTPClient http;
    http.begin((address + (*state ? "false" : "true")).c_str());
    http.GET();

    vTaskDelete(NULL);
}

void ButtonWidget::Update()
{
    Widget::Update();

    if (this->_display->readTouchpad(this->touchpadButton))
    {
        this->state = !this->state;

        xTaskCreatePinnedToCore(UpdateServer, "UpdateServer", 6000, &(this->state), 1, NULL, ARDUINO_RUNNING_CORE);

        this->_changed = true;
    }
}

void ButtonWidget::DrawChanged()
{
    // The text functionality of this drawing library suuuuuucks
    // Instead of trying to handle wrapping etc, I will just work around it
    Widget::DrawChanged();

    this->_display->setCursor(_position.x + 10, _position.y + 10);
    this->_display->setTextSize(2);

    if (this->state)
    {
        this->_display->setTextColor(0);
        this->_display->fillRect(this->_position.x, this->_position.y, this->_position.width, this->_position.height, 1);
        this->_display->print(this->buttonText.c_str());
        this->_display->setTextColor(1);
    }
    else
    {
        this->_display->setTextColor(1);
        this->_display->print(this->buttonText.c_str());
    }
}
