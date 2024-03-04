#include  <stdio.h>
#include  <math.h>
#include "fft.h"

/*********************************************************
����Ҷ�任:           ( ��Ϊ���ַ��ţ��Ӹ����������)

        F(w)=         ��f(t)exp(-i*w*t)dt
        f(t)= (1/2/pi)��F(w)exp(i*w*t)dw

        F(s)= ��f(t)exp(-i*2*pi*s*t)dt
        f(t)= ��F(s)exp(i*2*pi*s*t)ds

��ɢ����Ҷ�任(DFT)   ( ��Ϊ���ӷ� , WN=exp(-i*2*pi/N))

        F(sj) =     ��k Ak*(WN)^(jk)
        Ak = (1/N)* j�� F(sj)*(WN)^(-jk)

        Ak=[f(t(k-N)) + f(tk)] * delta_t

        delta_tΪ����ʱ����,tk=k*delta_t
        delta_sΪF(sj)Ƶ�׶�ӦƵ��sj�ļ��,sj=j*delta_s

        ��ʽ��Ak������ԭʱ�����У���Ƶ��Ҳ����Ƶ�����У��پ��任���ɵ�

        F(sj)=(1/N)*��k f(tk)WN^(jk)
        f(tk)=      ��j F(sj)WN^(-jk)

        ����f(tk)��Ϊԭ����ʱ�����У�F(sj)Ϊ��ӦƵ��

        Ϊ�˼��㷽�㣬��������

        F(j)= ��k f(tk)WN^(jk)
        ������F(j)���ٳ���(1/N)���õ�F(sj)


*********************************************************/


#define pi 3.141593       // floatС�����6λ
#define N_SIGN 128        // �źźϳɺͲ�������,ֱ��Ӱ��delta_t

// ����Խ��Խ�࣬�����ٶ�Խ�ͣ�����ʱ����Խ��
// ���������� f �������ο�˹�ض���(f >= 2 * max frequence of signal)ʱ���޷���ԭ

int N_FFT = N_SIGN;         // FFT����(Ƶ���ʱ�����һ��)


Complex WNP;                   // ���������ת����
Complex WNP_XJB;               // �����ת������F(J+B)(�����ε��·����)�ĳ˻�
Complex input_complex[N_SIGN]; // ���������ʵ��ת��Ϊ����


/************     ���˺���  ************/
Complex MUL_Complex(Complex a, Complex b)
{
    Complex c;
    c.real = a.real * b.real - a.imag * b.imag;
    c.imag = a.real * b.imag + a.imag * b.real;
    return(c);
}

/********************************
���ܣ����㸴����ģ
�βΣ�*sampleָ����Ҫȡģ�ĸ����ṹ��
      NΪȡģ����
      *output���ȡģ����ֵ������
*********************************/
void ModelComplex(Complex* sample, int N, EIS* output)
{
    int i;
    for (i = 0; i < N; i++)
    {
        output[i].amp = (float)0;
        // ��2��Ϊ�����׵���Ҫ����(1/N)����ΪҪ��F(j)��F(sj),F(sj)��Ϊ��ʵ��Ƶ��
        output[i].amp = sqrt(sample[i].real * sample[i].real + sample[i].imag * sample[i].imag) * 2 / N;
    }
}

void PhaseComplex(Complex* sample, int N, EIS* output)
{
    int i;
    for (i = 0; i < N; i++)
    {
        output[i].ang = (float)0;
        // ��2��Ϊ�����׵���Ҫ����(1/N)����ΪҪ��F(j)��F(sj),F(sj)��Ϊ��ʵ��Ƶ��
        output[i].ang = atan(sample[i].imag / sample[i].real);

    }
}

/****************   Cyrus Cheung�Ļ�2FFT�㷨����   *************
��2FFTԭ��:
    ����N=2^M�����FFT������L=M�����㣬ÿ����2^(M-1)��N/2������������ɣ�
    ��ת����ΪWNP,WNP=exp(-i*2*pi*p/N);
    P=J*2^(M-L),J=0,1......2^(L-1)-1����ÿ���� 2^(L-1)����ͬ����ת���ӣ�

    ����ͬһP�����뱾����������Ĵ���Ϊ2^M-L��B=2^(L-1)Ϊͬһ���ε���������ľ��룬������B���ڲ�ͬ��ת���ӵĸ�����
    ÿ����һ�ε���WNP�ĵ��ν��һ������ڵ��λ��ΪJ,�ڶ�������ڵ��λ��ΪJ+B=J+2^(L-1)��
    ÿ���ڶ��ε���WNP�ĵ��ν��һ������ڵ��λ��ΪJ+2^L,�ڶ�������ڵ��λ��ΪJ+2^L+B=J+2^L+2^(L-1)��
    ......�������
    ÿ����2^M-L�ε���WNP�ĵ��ν��һ������ڵ��λ��ΪJ+2^M-2^L,�ڶ�������ڵ�λ��ΪJ+2^M-2^L+B=J+2^M-2^L+2^(L-1)
    Ȼ�����ͬһ��������P��ֱ����ɱ������еĵ�������

    ��������WNP=exp(-i*2*pi*p/N) =exp(-i*2*pi*J / 2^L) =exp(-i*pi*J/B)

    ���Ͽɼ�����2FFTһ��ʹ������Ƕ�׵ķ�ʽʵ��FFT����
    ����ѭ���Ĺ����ǣ�
    �����һ��ѭ�������ͬWNP�ĵ������㣬
    �м��һ��ѭ���������WNP�ı仯��
    �������һ��ѭ���������M�ε������̡�


����Ϊַͬ��2FFT�������
��Ϊ����f(tk)�����Fj����һ��Complex���飬�ʱ�����Ϊftk_Fj
������f(tk)���룬������Fj������ں�������

!!ע��һ�㣬��FFT�������ftk_Fj[j]���е����j���岢����Ƶ�ʣ�
  ����Ƶ��sj=j*delta_s����ţ��������µķ���-Ƶ�ʶ�Ӧ��ϵ
  F(s0)=(1/N)*ftk_Fj[0]       <->  s0=0*delta_s
  F(s1)=(1/N)*ftk_Fj[1]       <->  s1=1*delta_s
  ......
  F(sj)=(1/N)*ftk_Fj[j]       <->  sj=j*delta_s
  ......
  F(smax)=(1/N)*ftk_Fj[N_FFT-1] <->  smax=(N_FFT-1)*delta_s

  ���и���DFT����
  delta_s = 1/(delta_t * N_FFT) , �� delta_t=1/(delta_s * N_FFT)

************************************************************/

void B2FFT_CHEUNG(Complex* ftk_Fj, int N)
{
    int L = 0;       // ���������
    int J = 0;       // ���������
    int K = 0, KB = 0;  // ���������
    int M = 0;       // N=2^M��������
    int N_temp = 0;  // ���㼶��M��Ҫ�õı���
    float B = 0;     // �����������������ݼ��

    /* ������Ϊ������ʹ�õľֲ�����*/
    int I = 0;       // �������
    int J_invert = 0;// �������
    int J_start = 0; // ������ʼ���
    int K_temp = 0;  // ��������еĿ��Ʊ���
    Complex C_temp; //���򽻻��е���ʱ����


    /*�����ǽ��������е���*/
    J_start = N / 2;           // LH=N/2
    J_invert = J_start;      // �����J_invert��J_start=N/2��ʼ

    for (I = 1; I <= N - 2; I++)   // ������Ŵ�1��=N-2;
    {
        if (I < J_invert)
        {
            C_temp = ftk_Fj[I];
            ftk_Fj[I] = ftk_Fj[J_invert];
            ftk_Fj[J_invert] = C_temp;
        }

        K_temp = J_start;       // �����J_invert������ǴӶ��������λ��ʼ��λ��

        while (J_invert >= K_temp)
        {
            J_invert -= K_temp;
            K_temp = K_temp / 2;  // �Ӹ�λ����λ��λ�ж�
        }

        J_invert += K_temp;
    }
    /*�������*/


    /* ���¼��������M */
    N_temp = N;
    while (N_temp != 1)          // �����N����2Ϊ��������M
    {
        M++;
        N_temp = N_temp / 2;
    }

    /* ����ѭ�� B2FFT���� */
    for (L = 1; L <= M; L++)         // ����ѭ��
    {
        B = pow(2, (L - 1));       //���ݺ�������ÿ������������ľ���
        for (J = 0; J < B; J++)      // ������B����ͬ����
        {
            //��ΪWNP=exp(-i*2*pi*p/N) =exp(-i*2*pi*J / 2^L) =exp(-i*pi*J/B)
            WNP.real = cos(pi * J / B);
            WNP.imag = -sin(pi * J / B);
            for (K = J; K < N; K += 2 * B)  // ͬһ���ε����м���,ͬһ�����εĲ���ֵΪ2^L=2*B
            {
                KB = K + B;
                WNP_XJB = MUL_Complex(ftk_Fj[KB], WNP);
                ftk_Fj[KB].real = ftk_Fj[K].real - WNP_XJB.real;//ַͬ���㣬�������
                ftk_Fj[KB].imag = ftk_Fj[K].imag - WNP_XJB.imag;
                ftk_Fj[K].real = ftk_Fj[K].real + WNP_XJB.real;
                ftk_Fj[K].imag = ftk_Fj[K].imag + WNP_XJB.imag;
            }
        }
    }
}

void run_fft(float* input, float f, EIS* output)
{
    for (int k = 0; k < N_FFT; k++)   // ����ʵ���ź�ת��Ϊ����
    {
        input_complex[k].real = input[k];
        input_complex[k].imag = 0;
    }

    B2FFT_CHEUNG(input_complex, N_FFT);          // FFT
    ModelComplex(input_complex, N_FFT, output);  // ��ģ
    PhaseComplex(input_complex, N_FFT, output);  //�����
    // �������output[j]�������Ա�Ĵ�СΪƵ��
    // �����j����Ƶ�ʣ�ֻ��sj����ţ���ӦƵ��Ϊsj=j*delta_s

    for (int i = 0; i < N_FFT; i++)
    {
        float s = (float)i / N_FFT * f;
        output[i].fre = s;
    }
}


