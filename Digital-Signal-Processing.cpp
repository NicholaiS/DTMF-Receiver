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
    std::cout << "Recording ..." << std::endl;
}

void DSP::StopRecording()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    sound.setBuffer(buffer);
    samples = buffer.getSamples();
    samplecount = buffer.getSampleCount();
}

double DSP::GoertzelAlgorithm(int SampleSize, int TargetFreq, const sf::Int16* Data)
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
    int BackgroundNoiseCap = 15;
    std::vector<int> DirectionsInstructions;

    do
    {
        StopRecording();
        //DTMF 1:
        if(GoertzelAlgorithm(samplecount, 697, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1209, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 1" << std::endl;
            DirectionsInstructions.push_back(1);
        }
        //DTMF 2:
        else if(GoertzelAlgorithm(samplecount, 697, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 2" << std::endl;
            DirectionsInstructions.push_back(2);
        }
        //DTMF 3:
        else if(GoertzelAlgorithm(samplecount, 697, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1477, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 3" << std::endl;
            DirectionsInstructions.push_back(3);
        }
        //DTMF 4:
        else if(GoertzelAlgorithm(samplecount, 770, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1209, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 4" << std::endl;
            DirectionsInstructions.push_back(4);
        }
        //DTMF 5:
        else if(GoertzelAlgorithm(samplecount, 770, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 5" << std::endl;
            DirectionsInstructions.push_back(5);
        }
        //DTMF 6:
        else if(GoertzelAlgorithm(samplecount, 770, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1477, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 6" << std::endl;
            DirectionsInstructions.push_back(6);
        }
        //DTMF 7:
        else if(GoertzelAlgorithm(samplecount, 852, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1209, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 7" << std::endl;
            DirectionsInstructions.push_back(7);
        }
        //DTMF 8:
        else if(GoertzelAlgorithm(samplecount, 852, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 8" << std::endl;
            DirectionsInstructions.push_back(8);
        }
        //DTMF 9:
        else if(GoertzelAlgorithm(samplecount, 852, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1477, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 9" << std::endl;
            DirectionsInstructions.push_back(9);
        }
        //DTMF 0 (lukke signal):
        else if(GoertzelAlgorithm(samplecount, 941, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
        {
            std::cout << "DTMF 0" << std::endl;
            break;
        }

        StartRecording();
//        std::cout << "Recording ..." << std::endl;
        std::cout << "Sleeping for 1 secs" << std::endl;
        sleep(1);
    } while (1);

    std::cout << "Loop stopped" << std::endl;
}


void DSP::PlaybackTest()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    sound.setBuffer(buffer);
    std::cout << "Playing back ..." << std::endl;
    sound.play();
    std::cout << "Done ..." << std::endl;
}
