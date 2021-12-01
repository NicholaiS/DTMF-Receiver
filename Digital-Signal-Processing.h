#ifndef DIGITALSIGNALPROCESSING_H
#define DIGITALSIGNALPROCESSING_H
#include "SFML/Audio.hpp"
#include <iostream>
#include <fstream>
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
    std::vector<double> GoertzelAlgorithm(int BufferSplitterSampleCount, int TargetFreq, std::vector<std::vector<double>> Data);
    double GoertzelAlgorithmForStartBit(int SampelSize, int TargetFreq, const sf::Int16* Data);
    bool StartBitTest();
    bool DTMFTest(int LowFreq, int HighFreq, int BufferCounter);
    std::string RecordDSPLoop();
    void FreqPerceiver();
    void PlaybackTest();
    void SingleBufferTest();
    void FilSkriver();

private:
    sf::SoundBufferRecorder recorder;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    const sf::Int16* samples;
    std::size_t samplecount;
    int SamplingRate = 44100;
    double BackgroundNoiseCap = 0.2;
    double StartBitBackgroundNoiseCap = 200.0;
};

#endif // DIGITALSIGNALPROCESSING_H
