cmake_minimum_required(VERSION 3.5)

project(DTMF-Receiver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the PahoMqttCpp package
find_package(PahoMqttCpp REQUIRED)

include_directories("/usr/include/nlohmann/" "usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")

add_executable(DTMF-Receiver main.cpp MQTT-Movement.cpp MQTT-Movement.h Digital-Signal-Processing.cpp Digital-Signal-Processing.h)
target_link_libraries(DTMF-Receiver PahoMqttCpp::paho-mqttpp3 libsfml-audio.so)
