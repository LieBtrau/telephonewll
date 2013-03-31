#include <iostream>
#include <cstdio>
#include <cmath>
#include "../ADPCM_test/oki_adpcm.h"

using namespace std;
#define PI 3.14159265

int main()
{
    oki_adpcm_state_t okiEncoder, okiDecoder;
    int dataIn, dataOut;
    unsigned char okiByte;

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
        //Generate 12bit data
        dataIn=sin(i*PI/180) * 32768;

        okiByte=oki_encode(&okiEncoder, dataIn);
        dataOut=oki_decode(&okiDecoder, okiByte)<<4;
        printf("%X %d %d\n",okiByte, dataIn, dataOut);
    }
    return 0;
}

