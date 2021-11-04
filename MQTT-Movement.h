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
mqtt::async_client* cli;
mqtt::topic* mes;
mqtt::token_ptr tok;

void signalhandler(int s)
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
    MQTT(mqtt::async_client& client, mqtt::topic& message);
    bool connect();
    void messageBot(json j);
//    void signalhandler(int s);
    void movement(direction d);
    void run();


private:
    const std::string ADDRESS { "tcp://localhost:1883" };
    const std::string TOPIC { "cmd_vel" };
    const int QOS = 1;
};

#endif // MQTTMOVEMENT_H