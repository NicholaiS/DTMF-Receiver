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
        std::cout << sf::SoundBufferRecorder::getDefaultDevice() << std::endl;

        std::vector<std::string> devices;
               devices= sf::SoundBufferRecorder::getAvailableDevices();
               for (int i = 0; i < devices.size(); i++)
               {
                   std::cout <<devices[i] <<std::endl;
               }
    }
}



void DSP::StartRecording()
{
    unsigned int sampleRate = 44100;
    recorder.start(sampleRate);
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
    int bufferSize = (buffer.getSampleCount()*0.73167045)/10;
    std::vector<std::vector<double>> res;
    std::vector<double> loader;

    for(int i = buffer.getSampleCount()*0.26832955; i < buffer.getSampleCount(); i++)
    {
        loader.push_back(samples[i]);
        if(loader.size() == bufferSize)
        {
            res.push_back(loader);
            loader.erase(loader.begin(), loader.end());
        }
    }
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

//Udfase Buffersplitter og bare kør med SFML bufferen. Dobbelt for-looped løser opsplitningen.
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

    for(int i = 0; i < Data.size(); i++)
    {
        s_n = 0;
        s_nMinus1 = 0;
        s_nMinus2 = 0;
        real = 0;
        imag = 0;

        for(int j = 4000; j < Data[i].size() - 2000; j++)
        {
            s_n = (Data[i][j] * (0.54 - 0.46 * cos(2*M_PI*i/Data[i].size()))) + coeff * s_nMinus1 - s_nMinus2;
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
    double DTMF9Mag;
    double DTMFDMag;
    do
    {
        usleep(75000);
        StopRecording();
        //DTMFTest For Start Bittet:
        DTMF1Mag = GoertzelAlgorithmForStartBit(samplecount, 697, samples) + GoertzelAlgorithmForStartBit(samplecount, 1209, samples);
        DTMF9Mag = GoertzelAlgorithmForStartBit(samplecount, 852, samples) + GoertzelAlgorithmForStartBit(samplecount, 1477, samples);
        DTMFDMag = GoertzelAlgorithmForStartBit(samplecount, 941, samples) + GoertzelAlgorithmForStartBit(samplecount, 1633, samples);
        if(DTMF1Mag > DTMF9Mag && DTMF1Mag > DTMFDMag && DTMF1Mag > StartBitBackgroundNoiseCap)
        {
            return true;
            break;
        }

        sound.resetBuffer();
        StartRecording();
    } while(1);
}

bool DSP::DTMFTest(int LowFreq, int HighFreq, int BufferCounter)
{
    int input = LowFreq + HighFreq;
    double DTMF1Mag = GoertzelAlgorithm(samplecount, 697, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1209, BufferSplitter())[BufferCounter];
    double DTMF9Mag = GoertzelAlgorithm(samplecount, 852, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1477, BufferSplitter())[BufferCounter];
    double DTMFDMag = GoertzelAlgorithm(samplecount, 941, BufferSplitter())[BufferCounter] +
            GoertzelAlgorithm(samplecount, 1633, BufferSplitter())[BufferCounter];

    switch(input)
    {
    case 1906:
        if(DTMF1Mag > DTMF9Mag && DTMF1Mag > DTMFDMag && DTMF1Mag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    case 2329:
        if(DTMF9Mag > DTMF1Mag && DTMF9Mag > DTMFDMag && DTMF9Mag > BackgroundNoiseCap)
            return true;
        else
            return false;
        break;

    case 2574:
        if(DTMFDMag > DTMF1Mag && DTMFDMag > DTMF9Mag && DTMFDMag > BackgroundNoiseCap)
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
            usleep(3000000);
            StopRecording();
            FilSkriver();
            for(int i = 0; i < 10; i++)
            {
                if(DTMFTest(852, 1477, i))
                {
                    DirectionsInstructions.push_back('1');
                }
                else if(DTMFTest(941, 1633, i))
                {
                    DirectionsInstructions.push_back('0');
                }
                else
                {
                    DirectionsInstructions.push_back('f');
                }
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
            usleep(3000000);
            StopRecording();
            for(int i = 0; i < 10; i++){
                std::cout << "Test: " << i << std::endl;
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

        case 'q':
            StopRecording();
                std::cout << "Magnitude 697: " << GoertzelAlgorithmForStartBit(samplecount, 697, samples) << std::endl;
                std::cout << "Magnitude 770: " << GoertzelAlgorithmForStartBit(samplecount, 770, samples) << std::endl;
                std::cout << "Magnitude 852: " << GoertzelAlgorithmForStartBit(samplecount, 852, samples) << std::endl;
                std::cout << "Magnitude 941: " << GoertzelAlgorithmForStartBit(samplecount, 941, samples) << std::endl;
                std::cout << "Magnitude 1209: " << GoertzelAlgorithmForStartBit(samplecount, 1209, samples) << std::endl;
                std::cout << "Magnitude 1336: " << GoertzelAlgorithmForStartBit(samplecount, 1336, samples) << std::endl;
                std::cout << "Magnitude 1477: " << GoertzelAlgorithmForStartBit(samplecount, 1477, samples) << std::endl;
                std::cout << "Magnitude 1633: " << GoertzelAlgorithmForStartBit(samplecount, 1633, samples) << std::endl;
                std::cout << "------------------------------------" << std::endl;
            sound.resetBuffer();
        }

    } while(input != '0');
}


void DSP::FilSkriver()
{
    std::vector<double> holderfortiddomain;

        for(int i = 0; i < buffer.getSampleCount() - 1; i++)
        {

            holderfortiddomain.push_back(samples[i]);


        }

        const char *path="/home/nicholai/Time_Frequency-Domain_test/TimeDomain";
        std::ofstream file(path);



        for(int i = 0; i < holderfortiddomain.size(); i++)
        {
            file << i << ", " << holderfortiddomain[i] << "\n";
        }


        file.close();
}
