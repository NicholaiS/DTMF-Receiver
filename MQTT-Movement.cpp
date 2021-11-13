#include "MQTT-Movement.h"

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

json MQTT::movement(direction d)
{
    switch(d)
    {
    case FORWARD:
    {
        json f = {
            {"linear", {{"x", -0.2}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        return f;
        break;
    }

    case BACKWARDS:
    {
        json b = {
            {"linear", {{"x", 0.2}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        return b;
        break;
    }

    case RIGHT:
    {
        json r = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", -0.5}}}
        };
        return r;
        break;
    }

    case LEFT:
    {
        json l = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.5}}}
        };
        return l;
        break;
    }

    case STOP:
    {
        json s = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        return s;
        break;
    }

    default:
    {
        json d = {
            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
        };
        return d;
        break;
    }
    }
}


void MQTT::run(MQTT ex)
{
    bool connected = ex.connect();
    std::cout << "Connected: " << connected << std::endl;

    std::cout << "Ready to run." << std::endl;
    std::cout << "w, a, s, d to control, q to stop the robot and 0 to exit." << std::endl;
    char input;
    do
    {
        std::cin >> input;

        switch(input)
        {
        case 'w':
            ex.messageBot(movement(FORWARD));
            break;

        case 's':
            ex.messageBot(movement(BACKWARDS));
            break;

        case 'a':
            ex.messageBot(movement(LEFT));
            break;

        case 'd':
            ex.messageBot(movement(RIGHT));
            break;

        case 'q':
            ex.messageBot(movement(STOP));
            exit(0);
            break;
        }
    } while(input != '0');
}

//void MQTT::DirectionDecider(std::string d)
//{
//    std::cout << d << std::endl;
//}

//void MQTT::signalHandler(int s)
//{
//    json stop_msg = {{"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//    {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//    };
//    std::cout << "CTRL + C pressed, exiting.." << std::endl;
//    tok = mes->publish(stop_msg.dump());
//    tok->wait();
//    exit(s);
//}
