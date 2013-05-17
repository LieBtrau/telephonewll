#include <stdio.h>
#include <math.h>

typedef unsigned char byte;
void loopy();
byte CIC_n2_r8_m1(byte sampleIn);

typedef unsigned char byte;
const double PI=3.14159;

int main(void)
{
    loopy();
    return 0;
}

void loopy()
{
    int sample, i, j;
    for(i=-45;i<45;i++)
    {
        sample=(sin(i*PI/18)+1)*128;
        for(j=0;j<8;j++)
        {
            if(j==0)
            {
                printf("%d \t %d\n",sample, CIC_n2_r8_m1(sample));
            }else
            {
                printf("%d \t %d\n",sample, CIC_n2_r8_m1(0));
            }
        }
    }
    //while(1);
}

//CIC-filter implementation:
//N=2
//R=8
//M=1
byte CIC_n2_r8_m1(byte ySampleIn)
{
    static byte upsampleClock=0;
    static int comb1, comb2, integrator1, integrator2, comb2Out;
    int comb1Out=0;
    int iSampleIn=ySampleIn;

    if((upsampleClock & 0x7)==0)
    {
        //Input data should be symmetrical around 0, otherwise the
        //integrators will keep increasing.  Eventually overflow will
        //occur, but that might take some time.
        //So adding a -128 offset.
        iSampleIn=((int)ySampleIn)-128;
        //modulo 8: 2 comb filters run at original, slow datarate
        comb1Out=iSampleIn-comb1;
        comb2Out=comb1Out-comb2;

        //Update registers
        comb1=iSampleIn;
        comb2=comb1Out;
        iSampleIn=comb2Out;
    }

    //integrators runs at upsampled datarate
    integrator1=iSampleIn+integrator1;
    integrator2=integrator1+integrator2;

    upsampleClock++;

    //Removing the offset again.
    return (integrator2>>3)+128;
}
