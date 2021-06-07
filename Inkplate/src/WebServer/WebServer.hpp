#ifndef WEB_SERVER_H
#define WEB_SERVER_H

/** Check if we have multiple cores */
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

// Includes for the server
#include <HTTPServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <HTTPHeader.hpp>

// Include JSON for body interpretation
#include <ArduinoJson.h>

// Include the GUI manager to allow registering callbacks etc
#include "GUI/Manager/Manager.hpp"

// The HTTPS Server comes in a separate namespace. For easier use, include it here.
using namespace httpsserver;

// We need a global reference to a GUI manager
extern Manager *webServerGuiManager;

void setWebServerManager(Manager *manager);

// Create an SSL-enabled server that uses the certificate
extern HTTPServer webServer;

void setupMyServer();

// Declare some handler functions for the various URLs on the server
void handleRoot(HTTPRequest *req, HTTPResponse *res);
void handle404(HTTPRequest *req, HTTPResponse *res);

// Twitch related functions
void handleTwitchAPI(HTTPRequest *request, HTTPResponse *response);
void handleTwitchCallback(HTTPRequest *request, HTTPResponse *response, DynamicJsonDocument &data);
void handleTwitchRegistration(HTTPRequest *request, HTTPResponse *response, DynamicJsonDocument &data);

void handleNewFollower();
void handleNewSubscriber();

// We declare a function that will be the entry-point for the task that is going to be
// created.
void serverTask(void *params);

#endif