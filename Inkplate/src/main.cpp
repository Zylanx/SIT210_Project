// We will use wifi
#include <WiFi.h>

// Inkplate
#include <Inkplate.h>

#include "GUI/Manager/Manager.hpp"
#include "WebServer/WebServer.hpp"
#include "GUI/Widgets/EventWidget/FollowWidget.hpp"
#include "GUI/Widgets/EventWidget/SubscribeWidget.hpp"

// WiFi Config
#define WIFI_SSID "WiFi-EF6B"
#define WIFI_PSK "85756182"

Inkplate display = Inkplate(INKPLATE_1BIT);
Manager guiManager = Manager(&display);

HTTPServer webServer = HTTPServer();

void setup()
{
    // Set the GUI Manager reference for the web server
    setWebServerManager(&guiManager);

    // For logging
    Serial.begin(115200);

    // Connect to WiFi
    Serial.println("Setting up WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PSK);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.print("Connected. IP=");
    Serial.println(WiFi.localIP());

    // Register the GUI Widgets
    guiManager.RegisterWidget(new FollowWidget(&display, 10, 10, 380, 125));
    guiManager.RegisterWidget(new SubscribeWidget(&display, 10, 145, 380, 125));

    // Set up the display and draw the initial frame
    display.begin();
    guiManager.Draw();
    display.setCursor(700, 500);
    display.setTextSize(1);
    display.print(WiFi.localIP());
    display.display();

    setupMyServer();
}

void loop()
{
    guiManager.Draw();

    delay(100);
}