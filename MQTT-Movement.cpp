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
    fasterslow=currentspeed;
    angle=currentangle;
    currentspeed=FB/10;
    currentangle=SS/10;
    if(currentspeed>-0.701 && currentspeed<0.701 && currentangle>=-0.701 && currentangle<=0.701)
    {
            json f = {
                {"linear", {{"x", currentspeed}, {"y",0},{"z",0}}},
                {"angular", {{"x", 0}, {"y",0},{"z", currentangle}}}
            };
            std::cout<<f<<std::endl;

            return f;
    }

    else if(currentangle<-0.7) //currentangle testen chekker om inputtet er -8 hvor fasterslow checker om den må kører langsommere
    {
        json f =slower();
        return f;
    }
    else if(currentspeed<-0.7) //currentspeed testen checker om inputtet er -8 hvor fasterslow ser om den må kører hurtigere
    {
        json f = faster();
        return f;
    }
    else
    {
        std::cout << "forstod intet (MQTT)" << std::endl;
    }
}

json MQTT::faster()
{
    if(fasterslow<0.7)
    {
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
        currentangle=angle;
        return f;
    }
    else
    {
        json f = {
            {"linear", {{"x", fasterslow}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", angle}}}
        };
        messageBot(f);
        std::cout<<f<<std::endl;
        currentspeed=fasterslow;
        currentangle=angle;
        throw(2);
    }
}

json MQTT::slower()
{
    if(fasterslow>-0.7)
    {
        fasterslow=fasterslow*10;
        fasterslow=fasterslow-1;
        fasterslow=fasterslow/10;
        json f = {
            {"linear", {{"x", fasterslow}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", angle}}}
        };
        std::cout<<f<<std::endl;
        currentspeed=fasterslow;
        currentangle=angle;
        return f;
    }
    else
    {
        json f = {
            {"linear", {{"x", fasterslow}, {"y",0},{"z",0}}},
            {"angular", {{"x", 0}, {"y",0},{"z", angle}}}
        };
        messageBot(f);
        std::cout<<f<<std::endl;
        currentspeed=fasterslow;
        currentangle=angle;
    }
}

void MQTT::run(MQTT ex)
{
        bool connected = ex.connect();
        std::cout << "Connected: " << connected << std::endl;
        DSP dsp;
        std::string wtf;
        dsp.FindMic();
        std::cout << "Ready to run." << std::endl;
        do
        {
            wtf = falseChar(dsp.RecordDSPLoop());
            if(errorcheck(wtf))
            {
                int side =inty(decode(falseChar(wtf)));
                int frem =intx(decode(falseChar(wtf)));
                int afstand = abs(frem)*10;
                std::cout << afstand << frem <<std::endl;
                for(int i=0;i<afstand;i++)
                {
                    ex.messageBot(styr(frem,side));
                    usleep(100000);
                }
            }
        } while(1/*test<40*/);
        ex.messageBot(styr(0,0));
}
