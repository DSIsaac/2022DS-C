#include "Filter.h"
                                           //通带截止频率   阻带起始频率    阻带最大衰减30或60    通带衰减
//struct Butterworth_Filter Butter1 = {0,      20,           50,            30,                   3};
//5.9改
struct Butterworth_Filter Butter1 = {0,      20,           100,            30,                   3};

double as[11] = { 0 };
double az[11] = { 0 };
double bs[11] = { 0 };
double bz[11] = { 0 };

#define RawData_to_Radian	0.0010653f	//原转弧

struct filter_data acc_filter[3];
struct filter_data gyro_filter[3];


struct complex Complex_Multiple(struct complex* x, struct complex* y)
{
	struct complex result = { 0 };
	result.re = x->re * y->re - x->im * y->im;
	result.im = x->im * y->re + x->re * y->im;
	return result;
}

int Ceil(double input)
{
	if (input != (int)input) return ((int)input) + 1;
	else return ((int)input);
}

void Create_Butter_Filter(struct Butterworth_Filter* Butter_n)
{
	struct complex pole[10] = { 0 };
	struct complex Res[10] = { 0 };
	struct complex Res_save[10] = { 0 };
	double dk = 0;

	int count = 0, count_1 = 0;

	Butter_n->wp = Butter_n->wp * 2 * pi / 1000;
	Butter_n->ws = Butter_n->ws * 2 * pi / 1000;  //计算滤波器参数时使用的是角频率,500是采样频率，要对频率进行归一化
	Butter_n->wp = 2 * tan((Butter_n->wp) / 2);   //[red/sec]
	Butter_n->ws = 2 * tan((Butter_n->ws) / 2);   //[red/sec]
	Butter_n->n = Ceil(0.5 * (log10(pow(10, Butter_n->as / 10) - 1) / log10(Butter_n->ws / Butter_n->wp)));  //计算滤波器阶数
	if (Butter_n->n % 2 == 0)
		dk = 0.5;
	else
		dk = 0;
	for (int k = 0, i = 0; k < 2 * Butter_n->n - 1; k++) {
		if (Butter_n->wp * cos(((double)k + dk) * pi / (double)Butter_n->n) < 0){
			pole[i].re = -(double)Butter_n->wp * cos(((double)k + dk) * (pi / Butter_n->n));
			pole[i].im = -(double)Butter_n->wp * sin(((double)k + dk) * (pi / Butter_n->n));
			i++;
			if (i >= Butter_n->n)
				break;
		}
	}
	/*for (int i = 0; i < Butter_n->n; i++) {
		printf("P%d = %f + %fi \n", i, -pole[i].re, -pole[i].im);
	}*/

	Res[0].re = pole[0].re;
	Res[0].im = pole[0].im;

	Res[1].re = 1;
	Res[1].im = 0;

	for (count_1 = 0; count_1 < Butter_n->n - 1; count_1++){
		for (count = 0; count <= count_1 + 2; count++){
			if (0 == count){
				Res_save[count] = Complex_Multiple(&Res[count], &pole[count_1 + 1]);
			}
			else if ((count_1 + 2) == count){
				Res_save[count].re += Res[count - 1].re;
				Res_save[count].im += Res[count - 1].im;
			}
			else{
				Res_save[count] = Complex_Multiple(&Res[count], &pole[count_1 + 1]);
				Res_save[count].re += Res[count - 1].re;
				Res_save[count].im += Res[count - 1].im;
			}
		}

		for (count = 0; count <= Butter_n->n; count++){
			Res[count].re = Res_save[count].re;
			Res[count].im = Res_save[count].im;
			as[Butter_n->n - count]= Res[count].re;
		}
	}
	bs[Butter_n->n] = as[Butter_n->n];
}


void Bilinear(struct Butterworth_Filter* Butter_n)
{
	int Count = 0, Count_1 = 0, Count_2 = 0, Count_Z = 0;
	double Res[10];
	double Res_Save[10];

	for (Count = 0; Count <= Butter_n->n; Count++){
		for (Count_Z = 0; Count_Z <= Butter_n->n; Count_Z++){
			Res[Count_Z] = 0;
			Res_Save[Count_Z] = 0;
		}
		Res_Save[0] = 1;
		for (Count_1 = 0; Count_1 < Butter_n->n - Count; Count_1++){
			for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++){
				if (Count_2 == 0){
					Res[Count_2] += Res_Save[Count_2];
				}

				else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)){
					Res[Count_2] += -Res_Save[Count_2 - 1];
				}

				else{
					Res[Count_2] += Res_Save[Count_2] - Res_Save[Count_2 - 1];
				}
			}
			for (Count_Z = 0; Count_Z <= Butter_n->n; Count_Z++){
				Res_Save[Count_Z] = Res[Count_Z];
				Res[Count_Z] = 0;    
			}
		}

		for (Count_1 = (Butter_n->n - Count); Count_1 < Butter_n->n; Count_1++){
			for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++){
				if (Count_2 == 0){
					Res[Count_2] += Res_Save[Count_2];
				}
				else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)){
					Res[Count_2] += Res_Save[Count_2 - 1];
				}
				else{
					Res[Count_2] += Res_Save[Count_2] + Res_Save[Count_2 - 1];
				}
			}
			for (Count_Z = 0; Count_Z <= Butter_n->n; Count_Z++)
			{
				Res_Save[Count_Z] = Res[Count_Z];
				Res[Count_Z] = 0;
    
			}
		}

		for (Count_Z = 0; Count_Z <= Butter_n->n; Count_Z++)
		{
			az[Count_Z] += pow(2, (double)Butter_n->n - Count) * as[Count] * Res_Save[Count_Z];
			bz[Count_Z] += (bs[Count]) * Res_Save[Count_Z];       
		}

	}



	for (Count_Z = Butter_n->n; Count_Z >= 0; Count_Z--)
	{
		bz[Count_Z] = (bz[Count_Z]) / az[0];
		az[Count_Z] = (az[Count_Z]) / az[0];
	}

}


float IIRLowPass(int x, struct filter_data *data_in, struct Butterworth_Filter* Butter_n)
{
	//运算之前Buf向前移动一个位置，以保存之前Buf的数据；
	int i;
	for(i = Butter_n->n; i > 0; i--){
		data_in->outBuf[i] = data_in->outBuf[i - 1];
		data_in->inBuf[i] = data_in->inBuf[i - 1];
	}
	data_in->inBuf[0] = x;
	data_in->outBuf[0] = 0;
	for(i = 1; i < Butter_n->n + 1;i++){
		data_in->outBuf[0] = data_in->outBuf[0] + bz[i] * data_in->inBuf[i];
		data_in->outBuf[0] = data_in->outBuf[0] - az[i] * data_in->outBuf[i];
	}
	data_in->outBuf[0] = data_in->outBuf[0] + bz[0] * data_in->inBuf[0];
	return data_in->outBuf[0];
}

void ACC_IIR_Filter(struct _acc *Acc_in, struct _acc *Acc_out, struct Butterworth_Filter* Butter_n)
{ //将新得的数据和上次输出的数据进行处理
	Acc_out->x = IIRLowPass(Acc_in -> x, &acc_filter[0], Butter_n); 
	Acc_out->y = IIRLowPass(Acc_in -> y, &acc_filter[1], Butter_n);  
	Acc_out->z = IIRLowPass(Acc_in -> z, &acc_filter[2], Butter_n); 
}

void GYRO_IIR_Filter(struct _gyro *Gyro_in, struct _gyro *Gyro_out, struct Butterworth_Filter* Butter_n)
{ //将新得的数据和上次输出的数据进行处理
	Gyro_out->x = IIRLowPass(Gyro_in->x, &gyro_filter[0], Butter_n); 
	Gyro_out->y = IIRLowPass(Gyro_in->y, &gyro_filter[1], Butter_n);
	Gyro_out->z = IIRLowPass(Gyro_in->z, &gyro_filter[2], Butter_n); 
}

void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
{
	Gyro_out->x = (float)(Gyro_in->x * RawData_to_Radian);
	Gyro_out->y = (float)(Gyro_in->y * RawData_to_Radian);
	Gyro_out->z = (float)(Gyro_in->z * RawData_to_Radian);
	//注意这里的Gyro_out->x 变量的定义是在void Get_Radian(struct _gyro *Gyro_in,struct _SI_float *Gyro_out)
	//我们可以注意到我们实际上运算后是将数据放到了 “Gyro_in“”Gyro_out“ 这两个地址里面
}

