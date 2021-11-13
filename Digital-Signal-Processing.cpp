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
    std::cout << "Recording stopped" << std::endl;
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

bool DSP::StartBitTest()
{
    std::cout << "Waiting for start bit" << std::endl;
    do
    {
        StopRecording();
        //DTMF 4 (Start bit):
        if(GoertzelAlgorithm(samplecount, 770, samples) > BackgroundNoiseCap &&
                GoertzelAlgorithm(samplecount, 1209, samples) > BackgroundNoiseCap)
        {
            return true;
            break;
        }
        StartRecording();
        sleep(1);

    } while(1);
}

std::string DSP::RecordDSPLoop()
{
    std::vector<char> DirectionsInstructions;
    std::string retur = "";
    if(StartBitTest())
    {
        std::cout << "RecordDSPLoop begyndt" << std::endl;
        do
        {
            StopRecording();
            //DTMF 5 (Stop bit):
            if(GoertzelAlgorithm(samplecount, 770, samples) > BackgroundNoiseCap &&
                    GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
            {
                std::cout << "Stop bit (DTMF 5)" << std::endl;
                break;
            }
            //DTMF 8 (STOP ALT):
            else if(GoertzelAlgorithm(samplecount, 852, samples) > BackgroundNoiseCap &&
                    GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
            {
                std::cout << "Nødstop trukket (DTMF 8)" << std::endl;
                break;
            }
            //DTMF 9 (1):
            else if(GoertzelAlgorithm(samplecount, 852, samples) > BackgroundNoiseCap &&
                    GoertzelAlgorithm(samplecount, 1477, samples) > BackgroundNoiseCap)
            {
                std::cout << "DTMF 9" << std::endl;
                DirectionsInstructions.push_back('1');
            }
            //DTMF 0 (0):
            else if(GoertzelAlgorithm(samplecount, 941, samples) > BackgroundNoiseCap &&
                    GoertzelAlgorithm(samplecount, 1336, samples) > BackgroundNoiseCap)
            {
                std::cout << "DTMF 0" << std::endl;
                DirectionsInstructions.push_back('0');
            }

            StartRecording();
            sleep(1);

        } while (1);
    }

    std::cout << "Loop stopped" << std::endl;

    for(int i = 0; i < DirectionsInstructions.size(); i++)
    {
        retur += DirectionsInstructions[i];
    }
    return retur;
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
