#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"
#include "fftw3.h"

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
//            dsp.PlaybackTest(); //Kan bruges til at teste om din mic virker.
            dsp.RecordDSPLoop(); //En test for udslag ved DTMF 2.
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
