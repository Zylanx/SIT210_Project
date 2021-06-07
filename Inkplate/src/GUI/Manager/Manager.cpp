#include "Manager.hpp"

Manager::Manager(Inkplate *display)
{
    this->display = display;
}

void Manager::RegisterWidget(Widget *widget)
{
    this->widgetList.push_back(std::unique_ptr<Widget>(widget));
}

void Manager::Update()
{
    for (auto &&widget : widgetList)
    {
        widget->Update();
        Serial.println("");
    }
}

void Manager::Draw()
{
    bool changed = false;

    for (auto &&widget : widgetList)
    {
        if (widget->Draw())
        {
            changed = true;
        }
    }

    if (changed)
    {
        display->partialUpdate();
    }
}

void Manager::Notify(std::string type, DynamicJsonDocument &data)
{
    for (auto &&widget : widgetList)
    {
        if (type == widget->NotificationType())
        {
            widget->Notify(data);
        }
    }
}