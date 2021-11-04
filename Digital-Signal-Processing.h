#ifndef DIGITALSIGNALPROCESSING_H
#define DIGITALSIGNALPROCESSING_H
#include "SFML/Audio.hpp"
#include <iostream>

class DSP
{
public:
    DSP();
    void FindMic();
    void StartRecording();
    void StopRecording();
    void PlaybackTest();

private:
    sf::SoundBufferRecorder recorder;
    sf::SoundBuffer buffer;
    sf::Sound sound;
};

#endif // DIGITALSIGNALPROCESSING_H
