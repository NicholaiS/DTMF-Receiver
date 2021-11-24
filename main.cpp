#include <iostream>
#include "Digital-Signal-Processing.h"
#include "MQTT-Movement.h"
#include "encoder.h"
void setupAndRun()
{
    const std::string ADDRESS { "tcp://172.20.10.7:1883" };
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
    setupAndRun();
}

