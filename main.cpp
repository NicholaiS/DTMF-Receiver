#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"
#include "decoder.h"
void setupAndRun()
{
    const std::string ADDRESS { "tcp://192.168.43.132" };
    const std::string TOPIC { "cmd_vel" };
    const int QOS = 1;
    std::cout << "Initializing for server '" << ADDRESS << "'..." << std::endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic mes(cli, TOPIC, QOS);

    MQTT ex(cli, mes);
    ex.run(ex);
}


int main()
{
    while(1)
    {
        setupAndRun();
    }
    //DSP TEST:
//        char input;
//        DSP dsp;


//        do
//        {
//            std::cout << "w for optagelse." << std::endl;
//            std::cout << "s for playback test." << std::endl;
//            std::cout << "x for kontinuert DTMF genkendelses loop (DTMF A lukker looped)" << std::endl;
//            std::cout << "e for singleBuffer test" << std::endl;
//            std::cout << "0 for at lukke ned" << std::endl;
//            std::cin >> input;

//            switch(input)
//            {
//            case 'w':
//                dsp.FindMic();
//                dsp.StartRecording();
//                break;

//            case 's':
//                dsp.PlaybackTest(); //Kan bruges til at teste om din mic virker.
//                break;

//            case 'x':
//                dsp.FindMic();
//                std::cout << dsp.RecordDSPLoop() << std::endl; //En kontinuerlig test om der bliver hørt DTMF 0 til 9 hvor DTMF 0 stopper looped.
//    //            std::cout << dsp.RecordDSPLoop().length() << std::endl;
//    //            std::cout << dsp.BufferSplitter().size() << std::endl;
//                break;

//            case 'e':
//                dsp.FindMic();
//                dsp.SingleBufferTest();
//                break;
//            case 'r':
//                setupAndRun();
//                std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
//                for(int i=0; i<availableDevices.size();i++)
//                    std::cout <<availableDevices.at(i) << std::endl;
//            }
//        } while(input!='0');
}

