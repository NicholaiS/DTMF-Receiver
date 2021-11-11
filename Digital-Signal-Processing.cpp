#include "Digital-Signal-Processing.h"

DSP::DSP()
{

}

void DSP::FindMic()
{
    if(!sf::SoundBufferRecorder::isAvailable())
    {
        std::cout << "No microphone is available." << std::endl;
    }
    else
    {
        std::cout << "Microphone found" << std::endl;
    }
}

void DSP::StartRecording()
{
    recorder.start();
    std::cout << "Recording.." << std::endl;
}

void DSP::StopRecording()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    sound.setBuffer(buffer);
    samples = buffer.getSamples();
    samplecount = buffer.getSampleCount();
}

double DSP::GoertzelAlgorithm(int SampleSize, int TargetFreq, int SamplingRate, const sf::Int16* Data)
{
    int      k,i;
    double   DoubleSampleSize;
    double   omega, sine, cosine, coeff, q0, q1, q2, magnitude, real, imag;

    double   scalingFactor = SampleSize / 2.0;

    DoubleSampleSize = (double) SampleSize;
    k = (int) (0.5 + ((DoubleSampleSize * TargetFreq) / SamplingRate));
    omega = (2.0 * M_PI * k) / DoubleSampleSize;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    q0=0;
    q1=0;
    q2=0;

    for(i=0; i<SampleSize; i++)
    {
        q0 = coeff * q1 - q2 + Data[i];
        q2 = q1;
        q1 = q0;
    }

    // calculate the real and imaginary results
    // scaling appropriately
    real = (q1 - q2 * cosine) / scalingFactor;
    imag = (q2 * sine) / scalingFactor;

    magnitude = sqrtf(real*real + imag*imag);
    return magnitude;
}

void DSP::RecordDSPLoop()
{
    StopRecording();
    std::cout << "Test for DTMF 2 (begge værdier skal være størren end 10):" << std::endl;
    std::cout << "Udslag på 697 Hz: " << GoertzelAlgorithm(samplecount, 697, 44100, samples) << std::endl;
    std::cout << "Udslag på 1336 Hz: " << GoertzelAlgorithm(samplecount, 1336, 44100, samples) << std::endl;
}

void DSP::PlaybackTest()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    buffer.saveToFile("DTMF-Receiver-Sound-Buffer.ogg");
    sound.setBuffer(buffer);
    std::cout << "Playing back ..." << std::endl;
    sound.play();
    std::cout << "Done ..." << std::endl;
}
