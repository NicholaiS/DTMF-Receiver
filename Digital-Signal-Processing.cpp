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

std::vector<std::vector<double>> DSP::BufferSplitter()
{
    int bufferSize = (buffer.getSampleCount()/2)/10;
    std::vector<std::vector<double>> res;
    std::vector<double> loader;

    for(int i = buffer.getSampleCount()/2; i < buffer.getSampleCount(); i++)
    {
        loader.push_back(samples[i]);
        if(loader.size() == bufferSize)
        {
            res.push_back(loader);
            loader.erase(loader.begin(), loader.end());
        }
    }
//    BufferSplitterSampleCount = res.size();
    return res;
}

//Kør trekantet vindues funktion før der kørers Goertzel.

double DSP::GoertzelAlgorithmForStartBit(int SampelSize, int TargetFreq, const sf::Int16* Data)
{
    int k;
    double DoubleSampleSize;
    double omega, sine, cosine, coeff, s_n, s_nMinus1, s_nMinus2, magnitude, real, imag;

    double scalingFactor = SampelSize / 2.0;

    DoubleSampleSize = (double) SampelSize;
    k = (int) (0.5 + ((DoubleSampleSize * TargetFreq) / SamplingRate));
    omega = (2.0 * M_PI * k) / DoubleSampleSize;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    s_n = 0;
    s_nMinus1 = 0;
    s_nMinus2 = 0;


    for(int i = 0; i < SampelSize; i++)
    {
        s_n = Data[i] + coeff * s_nMinus1 - s_nMinus2;
        s_nMinus2 = s_nMinus1;
        s_nMinus1 = s_n;
    }
    real = (s_nMinus1 - s_nMinus2 * cosine) / scalingFactor;
    imag = (s_nMinus2 * sine) / scalingFactor;

    magnitude = sqrtf(real*real + imag*imag);

    return magnitude;
}

std::vector<double> DSP::GoertzelAlgorithm(int BufferSplitterSampleCount, int TargetFreq, std::vector<std::vector<double>> Data)
{
    std::vector<double> VectorOfMagnitudes;
    int k;
    double DoubleSampleSize;
    double omega, sine, cosine, coeff, s_n, s_nMinus1, s_nMinus2, magnitude, real, imag;

    double scalingFactor = BufferSplitterSampleCount / 2.0;

    DoubleSampleSize = (double) BufferSplitterSampleCount;
    k = (int) (0.5 + ((DoubleSampleSize * TargetFreq) / SamplingRate));
    omega = (2.0 * M_PI * k) / DoubleSampleSize;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    s_n = 0;
    s_nMinus1 = 0;
    s_nMinus2 = 0;

    for(int i = 0; i < Data.size(); i++)
    {
        for(int j = 0; j < Data[i].size(); j++)
        {
            s_n = (Data[i][j] * (0.54-0.46*cos(2*M_PI*i/Data[i].size()))) + coeff * s_nMinus1 - s_nMinus2;
            s_nMinus2 = s_nMinus1;
            s_nMinus1 = s_n;
        }
        real = (s_nMinus1 - s_nMinus2 * cosine) / scalingFactor;
        imag = (s_nMinus2 * sine) / scalingFactor;

        magnitude = sqrtf(real*real + imag*imag);
        VectorOfMagnitudes.push_back(magnitude);
    }
    return VectorOfMagnitudes;
}

bool DSP::StartBitTest()
{
    std::cout << "Waiting for start bit" << std::endl;
    double DTMF1Mag;
    double DTMF5Mag;
    double DTMF9Mag;
    double DTMFDMag;
    do
    {
        usleep(75000);
        StopRecording();
        //DTMFTest For Start Bittet:
        DTMF1Mag = GoertzelAlgorithmForStartBit(samplecount, 697, samples) + GoertzelAlgorithmForStartBit(samplecount, 1209, samples);
        DTMF5Mag = GoertzelAlgorithmForStartBit(samplecount, 770, samples) + GoertzelAlgorithmForStartBit(samplecount, 1336, samples);
        DTMF9Mag = GoertzelAlgorithmForStartBit(samplecount, 852, samples) + GoertzelAlgorithmForStartBit(samplecount, 1477, samples);
        DTMFDMag = GoertzelAlgorithmForStartBit(samplecount, 941, samples) + GoertzelAlgorithmForStartBit(samplecount, 1633, samples);
        if(DTMF1Mag > DTMF5Mag && DTMF1Mag > DTMF9Mag && DTMF1Mag > DTMFDMag && DTMF1Mag > BackgroundNoiseCap)
        {
            return true;
            break;
        }
//        else
//        {
//            return false;
//        }

        sound.resetBuffer();
        StartRecording();
    } while(1);
}

bool DSP::DTMFTest(int LowFreq, int HighFreq, int BufferCounter)
{
    int input = LowFreq + HighFreq;
    double DTMF1Mag = GoertzelAlgorithm(samplecount, 697, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1209, BufferSplitter())[BufferCounter];
    double DTMF5Mag = GoertzelAlgorithm(samplecount, 770, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1336, BufferSplitter())[BufferCounter];
    double DTMF9Mag = GoertzelAlgorithm(samplecount, 852, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1477, BufferSplitter())[BufferCounter];
    double DTMFDMag = GoertzelAlgorithm(samplecount, 941, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1633, BufferSplitter())[BufferCounter];

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
    std::vector<char> DirectionsInstructions;
    std::string returString = "";
    if(StartBitTest())
    {
        std::cout << "Startbit fundet" << std::endl;
            StartRecording();
            usleep(6000000);
            StopRecording();
            for(int i = 0; i < 11; i++)
            {
                if(DTMFTest(852, 1477, i))
                {
                    DirectionsInstructions.push_back('1');
                }
                if(DTMFTest(941, 1633, i))
                {
                    DirectionsInstructions.push_back('0');
                }
//                if(DTMFTest(770, 1336, i))
//                    break;
            }
    }

    std::cout << "Loop stopped" << std::endl;

    for(int i = 0; i < DirectionsInstructions.size(); i++)
    {
        returString += DirectionsInstructions[i];
    }
    return returString;
}

//TEST FUNKTIONER:

void DSP::PlaybackTest()
{
    recorder.stop();
    buffer = recorder.getBuffer();
    sound.setBuffer(buffer);
    samples = buffer.getSamples();
    samplecount = buffer.getSampleCount();
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
            for(int i = 0; i < 10; i++){
            std::cout << "Magnitude 697: " << GoertzelAlgorithm(samplecount, 697, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 770: " << GoertzelAlgorithm(samplecount, 770, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 852: " << GoertzelAlgorithm(samplecount, 852, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 941: " << GoertzelAlgorithm(samplecount, 941, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1209: " << GoertzelAlgorithm(samplecount, 1209, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1336: " << GoertzelAlgorithm(samplecount, 1336, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1477: " << GoertzelAlgorithm(samplecount, 1477, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1633: " << GoertzelAlgorithm(samplecount, 1633, BufferSplitter())[i] << std::endl;
            std::cout << "------------------------------------" << std::endl;
            }
            break;

        case 'x':
            StartRecording();
            usleep(8000000);
            StopRecording();
            for(int i = 0; i < 10; i++){
            std::cout << "Magnitude 697: " << GoertzelAlgorithm(samplecount, 697, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 770: " << GoertzelAlgorithm(samplecount, 770, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 852: " << GoertzelAlgorithm(samplecount, 852, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 941: " << GoertzelAlgorithm(samplecount, 941, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1209: " << GoertzelAlgorithm(samplecount, 1209, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1336: " << GoertzelAlgorithm(samplecount, 1336, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1477: " << GoertzelAlgorithm(samplecount, 1477, BufferSplitter())[i] << std::endl;
            std::cout << "Magnitude 1633: " << GoertzelAlgorithm(samplecount, 1633, BufferSplitter())[i] << std::endl;
            std::cout << "------------------------------------" << std::endl;
            }
            sound.resetBuffer();
        }

    } while(input != '0');
}
