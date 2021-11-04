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
        std::cin >> input;

        switch(input)
        {
        case 'w':
            dsp.StartRecording();
            break;

        case 's':
            dsp.PlaybackTest();
            break;
        }
    } while(input!='0');

    //MQTT TEST:
    const std::string ADDRESS { "tcp://localhost:1883" };
    const std::string TOPIC { "cmd_vel" };
    const int QOS = 1;
    signal(SIGINT, signalHandler);
    std::cout << "Initializing for server '" << ADDRESS << "'..." << std::endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic mes(cli, TOPIC, QOS);

    MQTT ex(cli, mes);
    ex.run(ex);
}
