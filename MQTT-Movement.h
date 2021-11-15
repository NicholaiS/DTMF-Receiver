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
#include "encoder.h"
#include "Digital-Signal-Processing.h"

using json = nlohmann::json;

enum direction
{
    FORWARD, BACKWARDS, LEFT, RIGHT, STOP
};

class MQTT : encoder//, DSP
{
public:
    MQTT (){}
    MQTT(mqtt::async_client& client, mqtt::topic& message) : cli(&client), mes(&message){}
    bool connect();
    void messageBot(json j);
//    void DirectionDecider(std::string d);
    json movement(direction d);
    void run();
    void styr(int FB, int SS);
    void faster();
    void slower();
    ~MQTT (){}

private:
    mqtt::async_client* cli;
    mqtt::topic* mes;
    mqtt::token_ptr tok;
    double currentspeed;
    double currentangle;
};

#endif // MQTTMOVEMENT_H
