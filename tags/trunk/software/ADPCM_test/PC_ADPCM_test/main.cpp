#include <iostream>
#include <cstdio>
#include <cmath>
#include "../ADPCM_test/oki_adpcm.h"

using namespace std;
#define PI 3.14159265

typedef unsigned char byte;

int main()
{
    oki_adpcm_state_t okiEncoder, okiDecoder;
    short dataIn, dataOut;
    byte okiByte;

    //Reset OKI-ADPCM encoder & decoder
    okiEncoder.last=0;//half-scale
    okiDecoder.last=0;//half-scale
    okiDecoder.step_index=0;//step=16
    okiEncoder.step_index=0;//step=16
    for(int i=0;i<24;i++){
        oki_decode(&okiEncoder, 0x08);
        oki_decode(&okiDecoder, 0x08);
    }
    for(int i=0;i<24;i++){
        oki_decode(&okiEncoder, 0x80);
        oki_decode(&okiDecoder, 0x80);
    }

    for(int i=-90;i<90;i++){
        byte yDataIn=(sin(i*PI/180)+1)*128;

        //Convert byte data to data for OKI-encoder
        dataIn=(yDataIn-128)<<8;
        okiByte=oki_encode(&okiEncoder, dataIn);
        dataOut=oki_decode(&okiDecoder, okiByte)<<4;
        //Convert output of OKI-decoder back to bytes
        byte yDataOut=(dataOut>>8)+128;

        printf("%d %d %d %d\n",yDataIn, dataIn, dataOut, yDataOut);
    }
    return 0;
}

