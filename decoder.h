#ifndef DECODER_H
#define DECODER_H
#include <string>
#include <bitset>
#include <vector>
#include "sound_generator.h"

class decoder : SoundGenerator
{
public:
    decoder();
    std::string encode(std::string i);
    std::string decode(std::string i);
    int intx(std::string i);
    int inty(std::string i);
    bool encheckstring(std::string i);
    bool parityCheck1(std::string p);
    bool parityCheck2(std::string p);
    std::string falseChar(std::string p);
    bool decodecheck;
    bool errorcheck(std::string i);
    std::string datawords(int i)
    {
        return dataword[i];
    }
    std::string kodewords(int i)
    {
        return kodeword[i];
    }
private:
    std::string dataword[16] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"};
    std::string kodeword[16] =  {"00000","00011","00101","00110","01001","01010","01100","01111","10001","10010","10100","10111","11000","11011","11101","11110"};
    int n=4;
    int k=5;
    int fejl;
    int fb;
    int ss;    
};

#endif // DECODER_H
