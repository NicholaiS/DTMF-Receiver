#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"
#include "encoder.h"
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
//    setupAndRun();
//    std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
//    for(int i=0; i<availableDevices.size();i++)
//        std::cout <<availableDevices.at(i) << std::endl;
    encoder t;
//    if(t.parityCheck1("01f0100000"))
//        std::cout<<"nice"<<std::endl;
//    t.parityCheck2("0110100000");






























    std::cout<<t.falseChar("0101100f10")<<std::endl;













}

