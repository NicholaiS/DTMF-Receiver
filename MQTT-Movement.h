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
#include "decoder.h"
#include "Digital-Signal-Processing.h"
#include "sound_generator.h"

using json = nlohmann::json;

//enum direction
//{
//    FORWARD, BACKWARDS, LEFT, RIGHT, STOP
//};

class MQTT : decoder//, DSP
{
public:
    MQTT (){}
    MQTT(mqtt::async_client& client, mqtt::topic& message) : cli(&client), mes(&message)
    {

    }
    bool connect();
    void messageBot(json j);
//    json movement(direction d);
    void run(MQTT ex);
    json control(double FB, double SS);
    json faster();
    json slower();

    ~MQTT (){}

private:
    mqtt::async_client* cli;
    mqtt::topic* mes;
    mqtt::token_ptr tok;
    double currentspeed=0;
    double currentangle=0;
    double fasterslow=0;
    double angle=0;
};

#endif // MQTTMOVEMENT_H
