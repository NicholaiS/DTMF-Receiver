#ifndef DIGITALSIGNALPROCESSING_H
#define DIGITALSIGNALPROCESSING_H
#include "SFML/Audio.hpp"
#include <iostream>
#include <cmath>

class DSP
{
public:
    DSP();
    void FindMic();
    void StartRecording();
    void StopRecording();
    double GoertzelAlgorithm(int SampleSize, int TargetFreq, int SamplingRate, const sf::Int16* Data);
    void RecordDSPLoop();
    void PlaybackTest();

private:
    sf::SoundBufferRecorder recorder;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    const sf::Int16* samples;
    std::size_t samplecount;
};

#endif // DIGITALSIGNALPROCESSING_H
