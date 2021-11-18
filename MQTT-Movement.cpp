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

json MQTT::styr(double FB, double SS)
{
    //std::cout<<currentspeed<<std::endl;
    fasterslow=currentspeed;
        //std::cout<<fasterslow<<" "<<angle<< "hey1" <<std::endl;
    angle=currentangle;
    //std::cout<<fasterslow<<" "<<angle<< "hey2" <<std::endl;
    currentspeed=FB/10;
    currentangle=SS/10;
    //std::cout<<currentspeed<<" current speed"<<std::endl;
    //std::cout<<currentangle<<" current angle"<<std::endl;
    if(currentspeed>-0.7 && currentspeed<0.7 && currentangle>=-0.7 && currentangle<=0.7)
    {
            json f = {
                {"linear", {{"x", currentspeed}, {"y",0},{"z",0}}},
                {"angular", {{"x", 0}, {"y",0},{"z", currentangle}}}
            };
            std::cout<<f<<std::endl;
            return f;


    }

    else if(currentangle<-0.7 && fasterslow>-0.7)
        slower();
    else if(currentspeed<-0.7 && fasterslow<0.7)
        faster();

}

void MQTT::faster()
{
    //std::cout<<"hastigheden før: "<<currentspeed<<std::endl;
    if(currentspeed<0.7  )
    {
        //std::cout<<angle<<" vinkel "<<fasterslow<<" hastighed "<<std::endl;
        fasterslow=fasterslow*10;
        fasterslow=fasterslow+1;
        fasterslow=fasterslow/10;
        json f = {
            {"linear", {{"x", fasterslow}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", angle}}}
        };
        messageBot(f);
        std::cout<<f<<std::endl;
        currentspeed=fasterslow;
    }
}

void MQTT::slower()
{
    if(currentangle<0.7 && angle>-0.7)
    {
        fasterslow=fasterslow*10;
        fasterslow=fasterslow-1;
        fasterslow=fasterslow/10;
        json f = {
            {"linear", {{"x", fasterslow}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", angle}}}
        };
        std::cout<<f<<std::endl;
        messageBot(f);
        currentspeed=fasterslow;
        currentangle=angle;
    }
    //std::cout<<currentspeed<<std::endl;
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


void MQTT::run(MQTT ex)
{
    bool connected = ex.connect();
    std::cout << "Connected: " << connected << std::endl;
    DSP dsp;
    std::string wtf;
    dsp.FindMic();
    std::cout << "Ready to run." << std::endl;
    styr(0,0);
    //std::cout << "w, a, s, d to control, q to stop the robot and 0 to exit." << std::endl;
//    char input;
    do
    {
        //wtf="11011011000";
//        std::cin>>input;
        wtf = dsp.RecordDSPLoop();

        if(errorcheck(wtf))
        {

            ex.messageBot(styr(inty(decode(wtf)),intx(decode(wtf))));

        }
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
