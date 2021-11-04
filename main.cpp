#include <iostream>
#include "Digital-Signal-Processing.h"

int main()
{
    char input;
    DSP dsp;
    dsp.FindMic();
    do
    {
        std::cin >> input;

        switch(input)
        {
        case 'w':
            dsp.StartRecording();
            break;

        case 's':
            dsp.PlaybackTest();
            break;
        }
    } while(input!='0');
}
