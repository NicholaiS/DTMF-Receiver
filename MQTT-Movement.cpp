#include "MQTT-Movement.h"

MQTT::MQTT(mqtt::async_client &client, mqtt::topic &message)
{
    cli = &client;
    mes = &message;
}

bool MQTT::connect()
{
    try
    {
        std::cout << "Connecting ..." << std::endl;
        cli->connect()->wait();
        std::cout << "Connection established" << std::endl;
    }
    catch (const mqtt::exception& exc)
    {
        std::cerr << exc <<std::endl;
        return false;
    }
    return true;
}

void MQTT::messageBot(json j)
{
    try
    {
        tok = mes->publish(j.dump());
        tok ->wait();
    }
    catch(const mqtt::exception& exc)
    {
        std::cerr << exc << std::endl;
    }
}

void MQTT::movement(direction d)
{
    signal(SIGINT, signalhandler);
    std::cout << "Initializing for server '" << ADDRESS << "'..." << std::endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic mes(cli, TOPIC, QOS);

    MQTT ex(cli, mes);
    bool connected = ex.connect();
    std::cout << "Connected: " << connected << std::endl;

    switch(d)
    {
    case FORWARD:
    {
        json f = {
            {"linear", {{"x", 0.2}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        ex.messageBot(f);
        break;
    }

    case BACKWARDS:
    {
        json b = {
            {"linear", {{"x", -0.2}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        ex.messageBot(b);
        break;
    }

    case RIGHT:
    {
        json r = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.5}}}
        };
        ex.messageBot(r);
        break;
    }

    case LEFT:
    {
        json l = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", -0.5}}}
        };
        ex.messageBot(l);
        break;
    }
    }
}


void MQTT::run()
{
    std::cout << "Ready to run" << std::endl;
    std::cout << "w, a, s, d to control and q to stop" << std::endl;
    char input;
    do
    {
        std::cin >> input;

        switch(input)
        {
        case 'w':
            movement(FORWARD);
            break;

        case 's':
            movement(BACKWARDS);
            break;

        case 'a':
            movement(LEFT);
            break;

        case 'd':
            movement(RIGHT);
            break;
        }
    } while(input != 'q');
}
