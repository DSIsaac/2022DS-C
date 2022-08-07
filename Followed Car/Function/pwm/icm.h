#ifndef icm_h
#define icm_h

#include "driverlib.h"
#include "spi.h"

struct _acc
{
  short int x;
  short int y;
  short int z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
struct _gyro
{
  short int x;
  short int y;
  short int z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;

struct _SI_float
{
	float x;
	float y;
	double  z;
};
extern struct _SI_float   nacc;//惯导加速度
extern struct _SI_float SI_acc;
extern struct _SI_float SI_gyro;
struct _out_angle
{
	double  yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

void ICM20602_Updata_get(void); //读取数据
unsigned char ICM20602_Init(void); //初始化
#endif
