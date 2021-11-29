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
        throw(1);

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
        throw(3);
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
        ex.messageBot(styr(0,0));
        char test;
        std::cin>>test;
        if(test=='1')
        {
            char input;
            do
            {
                std::cin>>input;
                if(input=='w')
                    dsp.StartRecording();
                else if(input=='s')
                    dsp.StopRecording();
                else if(input=='d')
                    dsp.PlaybackTest();
            }while(input!='0');
        }
        else{
        do
        {
            try
            {
                wtf = dsp.RecordDSPLoop();

                if(errorcheck(wtf))
                {
                    int frem =inty(decode(falseChar(wtf)));
                    int side =intx(decode(falseChar(wtf)));
                    int afstand =currentspeed*10;
                    for(int i=0;i<afstand;i++)
                    {
                        ex.messageBot(styr(side,frem));
                        std::cout <<" jaaa taaaak " << std::endl;
                    }
                    ex.messageBot(styr(side,frem));
//                    ex.messageBot(styr(side,frem));
                    //test++;
                }
                else
                {
                    ex.messageBot(styr(0,0));
                    break;
                }
            }
            catch(int x)
            {
                switch(x)
                {
                case 1:
                    std::cout<<"forstod intet"<<std::endl;
                    break;

                case 2:
                    std::cout<<"kan ikke kører hurtigere"<<std::endl;
                    break;

                case 3:
                    std::cout<<"kan ikke kører langsommere"<<std::endl;
                    break;
                }
            }



        } while(1/*test<40*/);
        }
        ex.messageBot(styr(0,0));
}


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
