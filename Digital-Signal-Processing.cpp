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


//Kør trekantet vindues funktion før der kørers Goertzel.
double DSP::GoertzelAlgorithm(int SampleSize, int TargetFreq, const sf::Int16* Data)
{
    Data = samples;
    int      k,i;
    double   DoubleSampleSize;
    double   omega, sine, cosine, coeff, s_n, s_nMinus1, s_nMinus2, magnitude, real, imag;

    double   scalingFactor = SampleSize / 2.0;

    DoubleSampleSize = (double) SampleSize;
    k = (int) (0.5 + ((DoubleSampleSize * TargetFreq) / SamplingRate));
    omega = (2.0 * M_PI * k) / DoubleSampleSize;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    s_n=0;
    s_nMinus1=0;
    s_nMinus2=0;

    for(i = 0; i < SampleSize; i++)
    {
        s_n = Data[i] + coeff * s_nMinus1 - s_nMinus2;
        s_nMinus2 = s_nMinus1;
        s_nMinus1 = s_n;
    }

    // calculate the real and imaginary results
    // scaling appropriately
    real = (s_nMinus1 - s_nMinus2 * cosine) / scalingFactor;
    imag = (s_nMinus2 * sine) / scalingFactor;

    magnitude = sqrtf(real*real + imag*imag);
    return magnitude;
}

bool DSP::StartBitTest()
{
    std::cout << "Waiting for start bit" << std::endl;
    do
    {
        usleep(7500);
        StopRecording();
        //DTMF 1 (Start bit):
        if(DTMFTest(697, 1209))
        {
            return true;
            break;
        }
        sound.resetBuffer();
        StartRecording();
    } while(1);
}

bool DSP::DTMFTest(int LowFreq, int HighFreq)
{
    int input = LowFreq + HighFreq;
    double DTMF1Mag = GoertzelAlgorithm(samplecount, 697, samples) + GoertzelAlgorithm(samplecount, 1209, samples);
    double DTMF5Mag = GoertzelAlgorithm(samplecount, 770, samples) + GoertzelAlgorithm(samplecount, 1336, samples);
    double DTMF9Mag = GoertzelAlgorithm(samplecount, 852, samples) + GoertzelAlgorithm(samplecount, 1477, samples);
    double DTMFDMag = GoertzelAlgorithm(samplecount, 941, samples) + GoertzelAlgorithm(samplecount, 1633, samples);

    switch(input)
    {
    case 1906:
        if(DTMF1Mag > DTMF5Mag && DTMF1Mag > DTMF9Mag && DTMF1Mag > DTMFDMag && DTMF1Mag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    case 2106:
        if(DTMF5Mag > DTMF1Mag && DTMF5Mag > DTMF9Mag && DTMF5Mag > DTMFDMag && DTMF5Mag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    case 2329:
        if(DTMF9Mag > DTMF1Mag && DTMF9Mag > DTMF5Mag && DTMF9Mag > DTMFDMag && DTMF9Mag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    case 2574:
        if(DTMFDMag > DTMF1Mag && DTMFDMag > DTMF5Mag && DTMFDMag > DTMF9Mag && DTMFDMag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    default:
        return false;
        break;
    }
}

// Hvis der forsat bliver fejlgenkendt lyde, så lav en if test på de høje frekvenser
// og test om den du gerne vil finde giver et højere udslag end den som den tror den har fundet.
std::string DSP::RecordDSPLoop()
{
    bool idleTest = true;
    std::vector<char> DirectionsInstructions;
    std::string retur = "";
    if(StartBitTest())
    {
        std::cout << "RecordDSPLoop begyndt" << std::endl;
        do
        {
            StopRecording();
            //DTMF 5 (Stop bit):
            if(DTMFTest(770, 1336))
            {
                std::cout << "Stop bit (DTMF 5)" << std::endl;
                break;
            }
            //DTMF 1 (Idle bit):
            if(DTMFTest(697, 1209))
            {
                std::cout << "Idle bit (DTMF 1)" << std::endl;
                idleTest = true;
            }
            //DTMF 9 (1):
            if(DTMFTest(852, 1477) && idleTest)
            {
                std::cout << "DTMF 9" << std::endl;
                idleTest = false;
                DirectionsInstructions.push_back('1');
            }
            //DTMF D (0):
            if(DTMFTest(941, 1633) && idleTest)
            {
                std::cout << "DTMF D" << std::endl;
                idleTest = false;
                DirectionsInstructions.push_back('0');
            }
            sound.resetBuffer();
            StartRecording();
            usleep(150000);
        } while (1);
        sound.resetBuffer();
    }

    std::cout << "Loop stopped" << std::endl;

    for(int i = 0; i < DirectionsInstructions.size(); i++)
    {
        retur += DirectionsInstructions[i];
    }
    return retur;
}

//TEST FUNKTIONER:

void DSP::PlaybackTest()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    sound.setBuffer(buffer);
    std::cout << "Playing back ..." << std::endl;
    sound.play();
    std::cout << "Done ..." << std::endl;
}

void DSP::SingleBufferTest()
{
    char input;
    do
    {
        std::cin >> input;

        switch(input)
        {
        case 'w':
            StartRecording();
            break;

        case 's':
            StopRecording();
            std::cout << "Magnitude 697: " << GoertzelAlgorithm(samplecount, 697, samples) << std::endl;
            std::cout << "Magnitude 770: " << GoertzelAlgorithm(samplecount, 770, samples) << std::endl;
            std::cout << "Magnitude 852: " << GoertzelAlgorithm(samplecount, 852, samples) << std::endl;
            std::cout << "Magnitude 941: " << GoertzelAlgorithm(samplecount, 941, samples) << std::endl;
            std::cout << "Magnitude 1209: " << GoertzelAlgorithm(samplecount, 1209, samples) << std::endl;
            std::cout << "Magnitude 1336: " << GoertzelAlgorithm(samplecount, 1336, samples) << std::endl;
            std::cout << "Magnitude 1477: " << GoertzelAlgorithm(samplecount, 1477, samples) << std::endl;
            std::cout << "Magnitude 1633: " << GoertzelAlgorithm(samplecount, 1633, samples) << std::endl;
            break;

        case 'x':
            StartRecording();
            usleep(150000);
            StopRecording();
            std::cout << "Magnitude 697: " << GoertzelAlgorithm(samplecount, 697, samples) << std::endl;
            std::cout << "Magnitude 770: " << GoertzelAlgorithm(samplecount, 770, samples) << std::endl;
            std::cout << "Magnitude 852: " << GoertzelAlgorithm(samplecount, 852, samples) << std::endl;
            std::cout << "Magnitude 941: " << GoertzelAlgorithm(samplecount, 941, samples) << std::endl;
            std::cout << "Magnitude 1209: " << GoertzelAlgorithm(samplecount, 1209, samples) << std::endl;
            std::cout << "Magnitude 1336: " << GoertzelAlgorithm(samplecount, 1336, samples) << std::endl;
            std::cout << "Magnitude 1477: " << GoertzelAlgorithm(samplecount, 1477, samples) << std::endl;
            std::cout << "Magnitude 1633: " << GoertzelAlgorithm(samplecount, 1633, samples) << std::endl;
            sound.resetBuffer();
        }

    } while(input != '0');
}
