#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"
#include "encoder.h"

int main()
{
    const std::string ADDRESS { "tcp://172.20.10.7:1883" };
        const std::string TOPIC { "cmd_vel" };
        const int QOS = 1;
        std::cout << "Initializing for server '" << ADDRESS << "'..." << std::endl;
        mqtt::async_client cli(ADDRESS, "");
        mqtt::topic mes(cli, TOPIC, QOS);

        MQTT ex(cli, mes);
        ex.run(ex);
    //DSP TEST:
//    char input;
//    DSP dsp;
//    MQTT mqtt;
//    int testswitch=1;
//    if(testswitch==0)
//        mqtt.run();
//    else
//    {
//    encoder e;  //kun til brug a test
//    std::string k="1000101111"; //kun til brug a test
//    std::string q="0110001111";
//    std::string b="0001110001";
//    if(e.errorcheck(q))
//        mqtt.styr(e.inty(e.decode(q)),e.intx(e.decode(q)));
//    if(e.errorcheck(k)) //kun til brug a test
//    {
//        mqtt.styr(e.inty(e.decode(k)),e.intx(e.decode(k))); //kun til brug a test
//        //mqtt.faster();  //kun til brug a test
//        //mqtt.slower();  //kun til brug a test
//        //mqtt.faster();  //kun til brug a test

//    }
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));
//    if(e.errorcheck(b))
//        mqtt.styr(e.inty(e.decode(b)),e.intx(e.decode(b)));

//    if(e.errorcheck(k))
//        mqtt.styr(e.inty(e.decode(k)),e.intx(e.decode(k)));
//    if(e.errorcheck(k))
//        mqtt.styr(e.inty(e.decode(k)),e.intx(e.decode(k)));
    }
//    //for(int i=0;i<16;i++)
    //    std::cout<<e.datawords(i)<<std::endl;;
    //
//    encoder encoderen;
    //mqtt.connect();
//    dsp.FindMic();

//    do
//    {
//        std::cout << "w for optagelse." << std::endl;
//        std::cout << "s for playback test." << std::endl;
//        std::cout << "x for kontinuert DTMF genkendelses loop (DTMF 0 lukker looped)" << std::endl;
//        std::cout << "0 for at lukke ned" << std::endl;
//        std::cin >> input;

//        switch(input)
//        {
//        case 'w':
//            dsp.StartRecording();
//            break;

//        case 's':
//            dsp.PlaybackTest(); //Kan bruges til at teste om din mic virker.
//            break;

//        case 'x':
//            std::cout << dsp.RecordDSPLoop() << std::endl; //En kontinuerlig test om der bliver hørt DTMF 0 til 9 hvor DTMF 0 stopper looped.
//            break;
//        }
//    } while(input!='0');
//    std::string wtf=dsp.RecordDSPLoop();
//    mqtt.styr(encoderen.inty(wtf),encoderen.intx(wtf));

    //testest

    //MQTT TEST:
    //Replace "localhost" with the ip of the robot.
//    const std::string ADDRESS { "tcp://172.20.10.7:1883" };
//    const std::string TOPIC { "cmd_vel" };
//    const int QOS = 1;
//    std::cout << "Initializing for server '" << ADDRESS << "'..." << std::endl;
//    mqtt::async_client cli(ADDRESS, "");
//    mqtt::topic mes(cli, TOPIC, QOS);

//    MQTT ex(cli, mes);
//    ex.run(ex);

