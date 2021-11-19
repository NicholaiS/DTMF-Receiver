#ifndef DIGITALSIGNALPROCESSING_H
#define DIGITALSIGNALPROCESSING_H
#include "SFML/Audio.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unistd.h>

class DSP
{
public:
    DSP();
    void FindMic();
    void StartRecording();
    void StopRecording();
    std::vector<std::vector<double> > BufferSplitter();
    double GoertzelAlgorithm(int SampleSize, int TargetFreq, const sf::Int16* Data);
    bool StartBitTest();
    bool DTMFTest(int LowFreq, int HighFreq);
    std::string RecordDSPLoop();
    void FreqPerceiver();
    void PlaybackTest();
    void SingleBufferTest();

private:
    sf::SoundBufferRecorder recorder;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    const sf::Int16* samples;
    std::size_t samplecount;
    int SamplingRate = 44100;
//    double HighFrequencyBackgroundNoiseCap = 25.0;
//    double LowFrequencyBackgroundNoiseCap = 2.5;
    double BackgroundNoiseCap = 500;
};

#endif // DIGITALSIGNALPROCESSING_H
