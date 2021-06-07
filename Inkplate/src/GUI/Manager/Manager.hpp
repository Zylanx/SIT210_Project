#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <memory>

#include <Inkplate.h>
#include <ArduinoJson.h>

#include "GUI/Widgets/Widget.hpp"

class Manager
{
private:
    std::vector<std::unique_ptr<Widget>> widgetList;

    Inkplate *display;

public:
    Manager(Inkplate *display);

    void RegisterWidget(Widget *widget);

    void Update();
    void Draw();

    void Notify(std::string type, DynamicJsonDocument &data);
};

#endif