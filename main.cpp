#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"

int main()
{
    //DSP TEST:
    char input;
    DSP dsp;
    dsp.FindMic();
    do
    {
        std::cout << "w for optagelse." << std::endl;
        std::cout << "s for playback test." << std::endl;
        std::cout << "x for kontinuert DTMF genkendelses loop (DTMF 0 lukker looped)" << std::endl;
        std::cout << "0 for at lukke ned" << std::endl;
        std::cin >> input;

        switch(input)
        {
        case 'w':
            dsp.StartRecording();
            break;

        case 's':
            dsp.PlaybackTest(); //Kan bruges til at teste om din mic virker.
            break;

        case 'x':
            dsp.RecordDSPLoop(); //En kontinuerlig test om der bliver hørt DTMF 0 til 9 hvor DTMF 0 stopper looped.
            break;
        }
    } while(input!='0');

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
}
