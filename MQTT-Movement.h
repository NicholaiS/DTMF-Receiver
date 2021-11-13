#ifndef MQTTMOVEMENT_H
#define MQTTMOVEMENT_H

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <csignal>
#include <vector>
#include <string>
#include "mqtt/async_client.h"
#include "json.hpp"

using json = nlohmann::json;

enum direction
{
    FORWARD, BACKWARDS, LEFT, RIGHT, STOP
};

class MQTT
{
public:
    MQTT (){}
    MQTT(mqtt::async_client& client, mqtt::topic& message) : cli(&client), mes(&message){}
    bool connect();
    void messageBot(json j);
//    void DirectionDecider(std::string d);
    json movement(direction d);
    void run(MQTT ex);
    ~MQTT (){}

private:
    mqtt::async_client* cli;
    mqtt::topic* mes;
    mqtt::token_ptr tok;
};

#endif // MQTTMOVEMENT_H
