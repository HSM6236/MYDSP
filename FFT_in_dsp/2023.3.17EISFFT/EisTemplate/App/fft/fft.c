#include  <stdio.h>
#include  <math.h>
#include "fft.h"

/*********************************************************
傅里叶变换:           ( ∫为积分符号，从负无穷到正无穷)

        F(w)=         ∫f(t)exp(-i*w*t)dt
        f(t)= (1/2/pi)∫F(w)exp(i*w*t)dw

        F(s)= ∫f(t)exp(-i*2*pi*s*t)dt
        f(t)= ∫F(s)exp(i*2*pi*s*t)ds

离散傅里叶变换(DFT)   ( ∑为连加符 , WN=exp(-i*2*pi/N))

        F(sj) =     ∑k Ak*(WN)^(jk)
        Ak = (1/N)* j∑ F(sj)*(WN)^(-jk)

        Ak=[f(t(k-N)) + f(tk)] * delta_t

        delta_t为采样时域间隔,tk=k*delta_t
        delta_s为F(sj)频谱对应频率sj的间隔,sj=j*delta_s

        上式中Ak并不是原时序序列，故频谱也不是频谱序列，再经变换，可得

        F(sj)=(1/N)*∑k f(tk)WN^(jk)
        f(tk)=      ∑j F(sj)WN^(-jk)

        这里f(tk)即为原输入时间序列，F(sj)为对应频谱

        为了计算方便，假设如下

        F(j)= ∑k f(tk)WN^(jk)
        计算完F(j)后，再乘以(1/N)即得到F(sj)


*********************************************************/


#define pi 3.141593       // float小数点后6位
#define N_SIGN 128        // 信号合成和采样点数,直接影响delta_t

// 周期越数越多，采样速度越低，采样时域间隔越大。
// 当采样速率 f 不满足奈奎斯特定律(f >= 2 * max frequence of signal)时，无法还原

int N_FFT = N_SIGN;         // FFT点数(频域和时域点数一样)


Complex WNP;                   // 定义蝶形旋转因子
Complex WNP_XJB;               // 存放旋转因子与F(J+B)(即蝶形的下方入口)的乘积
Complex input_complex[N_SIGN]; // 采样输入的实数转化为复数


/************     复乘函数  ************/
Complex MUL_Complex( Complex a, Complex b)
{
    Complex c;
    c.real = a.real * b.real - a.imag * b.imag;
    c.imag = a.real * b.imag + a.imag * b.real;
    return(c);
}

/********************************
功能：计算复数的模
形参：*sample指向需要取模的复数结构体
      N为取模点数
      *output存放取模后数值的数组
*********************************/
void ModelComplex(Complex* sample, int N, EIS* output)
{
    int i;
    for (i = 0; i < N; i++)
    {
        output[i].amp = (float)0;
        if(i==0){
            output[i].amp = sqrt(sample[i].real * sample[i].real + sample[i].imag * sample[i].imag)  / N;
            // 直流分量在FFT变换中并没有被正负谱均分
            // 在MATLAB中的官方FFT.m中得到验证
            // Debug产生的谱，即在output.amp中也可发现，其余频率点都有与其对称的频率点幅值与其相同
            // 但在f=0.0点未发现有对称点
        }
        else{
            output[i].amp = sqrt(sample[i].real * sample[i].real + sample[i].imag * sample[i].imag) * 2 / N;
            // 乘2因为正负谱的需要，乘(1/N)是因为要从F(j)求F(sj),F(sj)才为真实的频谱
        }
    }
}

void PhaseComplex(Complex* sample, int N, EIS* output)
{
    int i;
    for (i = 0; i < N; i++)
    {
        output[i].ang = (float)0;
        output[i].ang = atan2(sample[i].imag,sample[i].real);
        if(output[i].ang<0){
            output[i].ang=output[i].ang+2*pi;  //弧度范围转化为0-2pi
        }
        //返回atan2（虚部/实部），即为对应频率点的相位弧度制，但是弧度范围为
        //**********pi-----0**********
        //**********-pi----0**********
        //转化为：
        //**********pi-----0**********
        //**********pi----2pi*********

    }
}

/****************   Cyrus Cheung的基2FFT算法解析   *************
基2FFT原理:
    对于N=2^M个点的FFT，共有L=M级运算，每级有2^(M-1)即N/2个蝶形运算组成；
    旋转因子为WNP,WNP=exp(-i*2*pi*p/N);
    P=J*2^(M-L),J=0,1......2^(L-1)-1，即每级有 2^(L-1)个不同的旋转因子；

    对于同一P，参与本级蝶形运算的次数为2^M-L，B=2^(L-1)为同一蝶形的两输入点间的距离，即距离B等于不同旋转因子的个数；
    每级第一次调用WNP的蝶形结第一个输入节点的位置为J,第二个输入节点的位置为J+B=J+2^(L-1)；
    每级第二次调用WNP的蝶形结第一个输入节点的位置为J+2^L,第二个输入节点的位置为J+2^L+B=J+2^L+2^(L-1)；
    ......如此类推
    每级第2^M-L次调用WNP的蝶形结第一个输入节点的位置为J+2^M-2^L,第二个输入节点位置为J+2^M-2^L+B=J+2^M-2^L+2^(L-1)
    然后计算同一级的其他P，直到完成本级所有的蝶形运算

    另外可求得WNP=exp(-i*2*pi*p/N) =exp(-i*2*pi*J / 2^L) =exp(-i*pi*J/B)

    由上可见，基2FFT一般使用三层嵌套的方式实现FFT功能
    三层循环的功能是：
    最里的一层循环完成相同WNP的蝶形运算，
    中间的一层循环完成因子WNP的变化，
    而最外的一层循环则是完成M次迭代过程。


以下为同址基2FFT具体代码
因为输入f(tk)与输出Fj共用一个Complex数组，故变量记为ftk_Fj
正序列f(tk)输入，正序列Fj输出，内含倒序功能

!!注意一点，该FFT求出来的ftk_Fj[j]序列的序号j意义并非是频率，
  而是频率sj=j*delta_s的序号，即有以下的幅度-频率对应关系
  F(s0)=(1/N)*ftk_Fj[0]       <->  s0=0*delta_s
  F(s1)=(1/N)*ftk_Fj[1]       <->  s1=1*delta_s
  ......
  F(sj)=(1/N)*ftk_Fj[j]       <->  sj=j*delta_s
  ......
  F(smax)=(1/N)*ftk_Fj[N_FFT-1] <->  smax=(N_FFT-1)*delta_s

  其中根据DFT性质
  delta_s = 1/(delta_t * N_FFT) , 即 delta_t=1/(delta_s * N_FFT)

************************************************************/

void B2FFT_CHEUNG(Complex* ftk_Fj, int N)
{
    int L = 0;       // 级间运算层
    int J = 0;       // 级内运算层
    int K = 0, KB = 0;  // 蝶形运算层
    int M = 0;       // N=2^M，即级数
    int N_temp = 0;  // 计算级数M需要用的变量
    float B = 0;     // 蝶形运算两输入数据间隔

    /* 以下是为倒序中使用的局部变量*/
    int I = 0;       // 正序序号
    int J_invert = 0;// 倒序序号
    int J_start = 0; // 倒序起始序号
    int K_temp = 0;  // 倒序号排列的控制变量
    Complex C_temp; //倒序交换中的临时变量


    /*以下是将输入序列倒序*/
    J_start = N / 2;           // LH=N/2
    J_invert = J_start;      // 倒序号J_invert从J_start=N/2开始

    for (I = 1; I <= N - 2; I++)   // 正序序号从1到=N-2;
    {
        if (I < J_invert)
        {
            C_temp = ftk_Fj[I];
            ftk_Fj[I] = ftk_Fj[J_invert];
            ftk_Fj[J_invert] = C_temp;
        }

        K_temp = J_start;       // 倒序号J_invert的序号是从二进制最高位开始进位的

        while (J_invert >= K_temp)
        {
            J_invert -= K_temp;
            K_temp = K_temp / 2;  // 从高位到低位逐位判断
        }

        J_invert += K_temp;
    }
    /*倒序完成*/


    /* 以下计算出级数M */
    N_temp = N;
    while (N_temp != 1)          // 计算出N的以2为底数的幂M
    {
        M++;
        N_temp = N_temp / 2;
    }

    /* 三层循环 B2FFT计算 */
    for (L = 1; L <= M; L++)         // 级间循环
    {
        B = pow(2, (L - 1));       //次幂函数计算每级蝶形两输入的距离
        for (J = 0; J < B; J++)      // 级内有B个不同蝶形
        {
            //因为WNP=exp(-i*2*pi*p/N) =exp(-i*2*pi*J / 2^L) =exp(-i*pi*J/B)
            WNP.real = cos(pi * J / B);
            WNP.imag = -sin(pi * J / B);
            for (K = J; K < N; K += 2 * B)  // 同一蝶形的所有计算,同一个蝶形的步进值为2^L=2*B
            {
                KB = K + B;
                WNP_XJB = MUL_Complex(ftk_Fj[KB], WNP);
                ftk_Fj[KB].real = ftk_Fj[K].real - WNP_XJB.real;//同址计算，正序输出
                ftk_Fj[KB].imag = ftk_Fj[K].imag - WNP_XJB.imag;
                ftk_Fj[K].real = ftk_Fj[K].real + WNP_XJB.real;
                ftk_Fj[K].imag = ftk_Fj[K].imag + WNP_XJB.imag;
            }
        }
    }
}

void run_fft (float* input,float f, EIS* output)
{
    for (int k = 0; k < N_FFT; k++)   // 输入实数信号转换为复数
    {
        input_complex[k].real = input[k];
        input_complex[k].imag = 0;
    }

    B2FFT_CHEUNG(input_complex, N_FFT);          // FFT
    ModelComplex(input_complex, N_FFT, output);  // 求模
    PhaseComplex(input_complex, N_FFT, output);  //求相角
    // 求出来的output[j]数组的大小为频谱
    // 而序号j不是频率，只是sj的序号，对应频率为i / N_FFT * f，且只有一半是有用的

    for (int i = 0; i < N_FFT; i++)
    {
        float s = (float)i / N_FFT * f;
        output[i].fre = s;
    }
}


