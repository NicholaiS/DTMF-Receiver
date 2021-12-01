#include "encoder.h"
#include <string>
#include <iostream>

encoder::encoder()
{

}

std::string encoder::encode(std::string i){
if(encheckstring(i)){

    std::string sh1;
    std::string sh2;

    std::string fs;

    for(int j=0; j<(2*n);j++){
    if(j<n){
        sh1+=i[j];
    } else {
        sh2+=i[j];
    }
    }

    for(int j=0; j<(n*n);j++){
        if(sh1==dataword[j]){
            fs+=kodeword[j];
        }
    }

    for(int j=0; j<(n*n);j++){
        if(sh2==dataword[j]){
            fs+=kodeword[j];
        }
    }
    return fs;

} else {
    std::cout << "string ikke 8 lang eller 1/0" << std::endl;
    return "string ikke 8 lang eller 1/0";
}
}

std::string encoder::decode(std::string i)
{
        std::string sh1, sh2, fs;

        int tael=0;

        for(int j=0; j<(2*k);j++)
        {
            if(j<k){
                sh1+=i[j];
            }
            else
            {
                sh2+=i[j];
            }
        }

        for(int j=0; j<(n*n);j++)
        {
            if(sh1==kodeword[j])
            {
                fs+=dataword[j];
            }
            else {tael++;}
        }

        for(int j=0; j<(n*n);j++)
        {
            if(sh2==kodeword[j])
            {
                fs+=dataword[j];
            }
            else {tael++;}
        }

        fejl=tael-(n*n*2-2);

        if(fejl==0)
        {
            return fs;
            decodecheck=true;
        }
        else
        {
            std::string fejlstr = sh1.substr(0,4);
            fejlstr += sh2.substr(0,4);
            return fejlstr;
            decodecheck=false;
        }
}

int encoder::inty(std::string i)
{
    std::string str;
    if(encheckstring(i))
    {
        for(int j=0;j!=4;j++)
        {
            str+=i[j];
        }
        if(str[0]=='0')
        {
            return std::stoi(str,0,2);
        }
        else
        {
            str.erase(str.begin());
            return std::stoi(str,0,2)-8;
        }
    }
}

int encoder::intx(std::string i){
    std::string str;
    if(encheckstring(i)){
        for(int j=4;j!=8;j++){
            str+=i[j];
        }

        if(str[0]=='0'){
        return std::stoi(str,0,2);
        } else {
            str.erase(str.begin());
            return std::stoi(str,0,2)-8;
        }
    }
}

bool encoder::encheckstring(std::string i){
    bool tjek1=false, tjek2=false;
    if(i.size()==(2*n)){
        tjek1=true;
    }

    int h=0;
    for(int j=0; j<(2*n);j++){

        if(i[j]=='0'||i[j]=='1'){
            h++;
        }
    }

    if(h==(2*n)){
        tjek2=true;
    }

    if(tjek1&&tjek2){
        return true;
    } else return false;
}

bool encoder::parityCheck1(std::string p)
{
    int paritychek=0;

    for(int i=0;i<p.size()/2;i++)
    {
        if(p[i]=='1')
            paritychek++;
    }

    if(paritychek%2==0)
        return false;
    return true;

}
bool encoder::parityCheck2(std::string p)
{
        int paritychek=0;

        for(int i=p.size()/2;i<p.size();i++)
        {
            if(p[i]=='1')
                paritychek++;
        }

        if(paritychek%2==0)
            return false;
        return true;
}

std::string encoder::falseChar(std::string p)
{
    std::cout << p << std::endl;
    int amountOfF1=0;
    int amountOfF2=0;
    int placeOfF1;
    int placeOfF2;
    for(int i=0; i<p.size();i++)
    {
        if(p[i]=='f')
        {

            if(i<p.size()/2)
            {
                amountOfF1++;
                placeOfF1=i;
            }
            else
            {
                amountOfF2++;
                placeOfF2=i;
            }
        }
    }

    if(amountOfF1>1 || amountOfF2>1)
    {
        return "burst bits";
    }

    if(amountOfF1==1)
    {
        if(parityCheck1(p))
        {
            p[placeOfF1]='1';
        }

        else
            p[placeOfF1]='0';
    }

    if(amountOfF2==1)
    {
        if(parityCheck2(p))
        {
            p[placeOfF2]='1';
        }
        else
            p[placeOfF2]='0';

    }
    return p;
}

bool encoder::errorcheck(std::string i)
{
    std::cout << i <<std::endl;
    std::string f1;
    std::string f2;
        if(i=="burst bits")
        {
            PlaySingle(852,1477);
            std::cout << "Burstbit"<<std::endl;
            return false;
        }

//        if(i.size()!=2*k)
//        {
//            if(i.size()<10)
//            {
//                std::cout << "Modtog et for kort signal" <<std::endl;
//                return false;
//            }

//            else
//            {
//                std::cout << "Modtog et for langt signal" << std::endl;
//                return false;
//            }
//        }

        for(int j=0; j<i.size();j++)
        {
            if(i[j]=='f')
            {
                return false;
            }

            if(j<k)
            {
                f1+=i[j];
            }

            else
            {
                f2+=i[j];
            }
        }

        if(parityCheck1(i)||parityCheck2(i))
        {
                std::cout << "Parity check error" << std::endl;
                PlaySingle(941,1633);
                return false;
        }

        int b1=0;
        int b2=0;
        for(int l=0;l<16;l++)
        {
            if(f1==kodewords(l))
                b1++;
            if(f2==kodewords(l))
                b2++;
        }

        if(b1!=1&&b2!=1)
        {
            std::cout <<"Koden findes ikke på listen"<<std::endl;
            return false;
        }

        PlaySingle(697,1209); //DTMF 1
        std::cout << "snilt" <<std::endl;  
        return true;
}

