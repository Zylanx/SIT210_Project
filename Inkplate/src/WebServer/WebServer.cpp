#include "WebServer/WebServer.hpp"

Manager *webServerGuiManager;

void setWebServerManager(Manager *manager)
{
    webServerGuiManager = manager;
}

void setupMyServer()
{
    // Setup the server as a separate task.
    Serial.println("Creating server task... ");
    // We pass:
    // serverTask - the function that should be run as separate task
    // "https443" - a name for the task (mainly used for logging)
    // 6144       - stack size in byte. If you want up to four clients, you should
    //              not go below 6kB. If your stack is too small, you will encounter
    //              Panic and stack canary exceptions, usually during the call to
    //              SSL_accept.
    xTaskCreatePinnedToCore(serverTask, "https443", 20000, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

void serverTask(void *params)
{
    // In the separate task we first do everything that we would have done in the
    // setup() function, if we would run the server synchronously.

    // Note: The second task has its own stack, so you need to think about where
    // you create the server's resources and how to make sure that the server
    // can access everything it needs to access. Also make sure that concurrent
    // access is no problem in your sketch or implement countermeasures like locks
    // or mutexes.

    // Create nodes
    ResourceNode *node404 = new ResourceNode("", "GET", &handle404);
    ResourceNode *nodeTwitchAPI = new ResourceNode("/", "POST", &handleTwitchAPI);

    // Add nodes to the server
    webServer.setDefaultNode(node404);
    webServer.registerNode(nodeTwitchAPI);

    Serial.println("Starting server...");
    webServer.start();
    if (webServer.isRunning())
    {
        Serial.println("Server ready!");

        // "loop()" function of the separate task
        while (true)
        {
            // This call will let the server do its work
            webServer.loop();

            // Other code would go here...
            delay(1);
        }
    }
}

void handle404(HTTPRequest *req, HTTPResponse *res)
{
    // Discard request body, if we received any
    // We do this, as this is the default node and may also server POST/PUT requests
    req->discardRequestBody();

    // Set the response status
    res->setStatusCode(404);
    res->setStatusText("Not Found");

    // Set content type of the response
    res->setHeader("Content-Type", "text/html");

    // Write a tiny HTTP page
    res->println("<!DOCTYPE html>");
    res->println("<html>");
    res->println("<head><title>Not Found</title></head>");
    res->println("<body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body>");
    res->println("</html>");
}

void handleTwitchAPI(HTTPRequest *request, HTTPResponse *response)
{
    // Parse the JSON content of the body
    const size_t capacity = 5120;
    DynamicJsonDocument jsonData(capacity);

    // Create buffer to read request
    char *buffer = new char[capacity + 1];
    memset(buffer, 0, capacity + 1);

    // Try to read request into buffer
    size_t idx = 0;
    while (!request->requestComplete() && idx < capacity)
    {
        idx += request->readChars(buffer + idx, capacity - idx);
    }

    Serial.println(buffer);

    // If the request is still not read completely, we cannot process it.
    if (!request->requestComplete())
    {
        response->setStatusCode(413);
        response->setStatusText("Request entity too large");
        response->println("413 Request entity too large");
        // Clean up
        delete[] buffer;
        return;
    }

    // Parse the object
    deserializeJson(jsonData, buffer);
    delete[] buffer;

    Serial.println(jsonData["event"]["user_name"].as<const char *>());

    // Now direct off to the correct function based off it
    std::string messageType = request->getHeader("Twitch-Eventsub-Message-Type");
    if (messageType == "webhook_callback_verification")
    {
        handleTwitchRegistration(request, response, jsonData);
    }
    else if (messageType == "notification")
    {
        handleTwitchCallback(request, response, jsonData);
    }
    else
    {
        // return a fail code
    }

    response->setStatusCode(200);
    response->print("");
}

void handleTwitchRegistration(HTTPRequest *request, HTTPResponse *response, DynamicJsonDocument &data)
{
    response->setStatusCode(200);

    response->setHeader("Content-Type", "text/plain");

    response->print(data["challenge"].as<const char *>());
}

void handleTwitchCallback(HTTPRequest *request, HTTPResponse *response, DynamicJsonDocument &data)
{
    webServerGuiManager->Notify(data["subscription"]["type"], data);
}