#ifndef MQTTMOVEMENT_H
#define MQTTMOVEMENT_H

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <csignal>
//#include <ncurses.h>
#include "mqtt/async_client.h"
#include "json.hpp"

using json = nlohmann::json;

void signalHandler(int s)
{
    json stop_msg = {{"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
    {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
    };
    std::cout << "CTRL + C pressed, exiting.." << std::endl;
    tok = mes->publish(stop_msg.dump());
    tok->wait();
    exit(s);
}

enum direction
{
    FORWARD, BACKWARDS, LEFT, RIGHT
};

class MQTT
{
public:
    MQTT(mqtt::async_client& client, mqtt::topic& message) : mes(&message), cli(&client){}
    bool connect();
    void messageBot(json j);
//    void signalHandler(int s);
    json movement(direction d);
    void run(MQTT ex);

private:
    mqtt::async_client* cli;
    mqtt::topic* mes;
    mqtt::token_ptr tok;
};

#endif // MQTTMOVEMENT_H
