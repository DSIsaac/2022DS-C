#ifndef _FILTER_H
#define _FILTER_H

#include "stdlib.h"
#include "usart.h" 
#include "icm.h"
#include "math.h"

#define pi 3.14159265

struct Butterworth_Filter
{
	int n;
	double wp;//截止频率
	double ws;//阻带起始频率
	unsigned int as;//阻带衰减
	unsigned int ap;
};
extern struct Butterworth_Filter Butter1;
struct filter_data{
	double inBuf[11];
	double outBuf[11];
};

struct complex{
	double re;//实部
        double im;//虚部
};



int Ceil(double input);
void Create_Butter_Filter(struct Butterworth_Filter *Butter_n);
void Bilinear(struct Butterworth_Filter* Butter_n);
float IIRLowPass(int x, struct filter_data *data_in, struct Butterworth_Filter* Butter_n);

void ACC_IIR_Filter(struct _acc *Acc_in, struct _acc *Acc_out, struct Butterworth_Filter* Butter_n);
void GYRO_IIR_Filter(struct _gyro *Gyro_in, struct _gyro *Gyro_out, struct Butterworth_Filter* Butter_n);
void SPL_IIR_Filter(float x, float *spl_out, struct Butterworth_Filter* Butter_n);
struct complex Complex_Multiple(struct complex* x, struct complex* y);

void Matrix_Multiply(float a[3][3], float b[3], float c[3], int i);


int signal(void);

void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out);

double sort(double arr[]);

#endif
