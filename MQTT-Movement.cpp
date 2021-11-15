#include "MQTT-Movement.h"
#include "Digital-Signal-Processing.h"
#include "encoder.h"

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

void MQTT::styr(int FB, int SS)
{
    currentspeed=FB/10;
    currentangle=SS/10;
        json f = {
            {"linear", {{"x", FB}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", SS}}}
        };
        std::cout<<f<<std::endl;
}

void MQTT::faster()
{
    std::cout<<"hastigheden før: "<<currentspeed<<std::endl;
    if(currentspeed<7)
    {
        currentspeed++;
        json f = {
            {"linear", {{"x", currentspeed}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", currentangle}}}
        };
    }
    std::cout<<"hastigheden efter: "<<currentspeed<<std::endl;
}

void MQTT::slower()
{
    if(currentspeed>-7)
    {
        currentspeed--;
        json f = {
            {"linear", {{"x", currentspeed}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", currentangle}}}
        };
    }
}



//json MQTT::movement(direction d)
//{
//    switch(d)
//    {
//    case FORWARD:
//    {
//        json f = {
//            {"linear", {{"x", -0.2}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//        };
//        return f;
//        break;
//    }

//    case BACKWARDS:
//    {
//        json b = {
//            {"linear", {{"x", 0.2}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//        };
//        return b;
//        break;
//    }

//    case RIGHT:
//    {
//        json r = {
//            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", -0.5}}}
//        };
//        return r;
//        break;
//    }

//    case LEFT:
//    {
//        json l = {
//            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.5}}}
//        };
//        return l;
//        break;
//    }

//    case STOP:
//    {
//        json s = {
//            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//        };
//        return s;
//        break;
//    }

//    default:
//    {
//        json d = {
//            {"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//            {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//        };
//        return d;
//        break;
//    }
//    }
//}


void MQTT::run()
{
    //bool connected = ex.connect();
    //std::cout << "Connected: " << connected << std::endl;
    DSP dsp;
    std::string wtf;
    dsp.FindMic();
    std::cout << "Ready to run." << std::endl;
    //std::cout << "w, a, s, d to control, q to stop the robot and 0 to exit." << std::endl;
//    char input;
    do
    {
//        std::cin>>input;
        wtf = dsp.RecordDSPLoop();
        styr(inty(decode(wtf)),intx(decode(wtf)));

        break;
    } while(1);
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
