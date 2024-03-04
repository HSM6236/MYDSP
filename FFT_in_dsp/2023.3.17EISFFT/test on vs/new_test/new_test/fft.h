

#ifndef APP_FFT_H_
#define APP_FFT_H_

#define N_SIGN 128


typedef struct EIS_STRUCT
{
    float fre, amp, ang;
}EIS;


typedef struct COMPLEX
{
    float real, imag;
}Complex;

void run_fft(float* input, float f, EIS* output);


#endif
#pragma once
