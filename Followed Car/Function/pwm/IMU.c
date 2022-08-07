#include "IMU.h"

#define Kp 	0.2f    // ????
#define Ki 	0.001f  // ????
#define halfT 0.001f//???
#define T	0.002f  // ???1ms
#define errINT_Limt 0.002f

// ===================================================================================
// 变量定义
static float q0 = 1.0f,  q1 = 0.0f,  q2 = 0.0f,q3 = 0.0f;     	// 四元数
static float exInt = 0, eyInt = 0, ezInt = 0;    	// 误差积分累计值 
static float rMat[3][3];//四元数的旋转矩阵

#define Radian_to_Angle	   57.2957795f  //弧转角
#define RawData_to_Angle	0.0610351f	//原转角
#define RawData_to_Radian	0.0010653f	//原转弧

//struct _acc Acc_Offset = {0};

float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{ 
	static unsigned char cnnt = 0;
	static float norm;
	static float ex, ey, ez;

	float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;
    static float q0_last = 0;
    static float q1_last = 0;
    static float q2_last = 0;
    static float q3_last = 0;
	//B--->N
	rMat[0][0]=1.0f-2.0f*q2q2-2.0f*q3q3;rMat[0][1] = 2.0f * (q1q2 + -q0q3)  ;rMat[0][2] = 2.0f * (q1q3 - -q0q2);
    rMat[1][0] = 2.0f * (q1q2 - -q0q3) ;rMat[1][1] =1.0f-2.0f*q1q1-2.0f*q3q3;rMat[1][2] = 2.0f * (q2q3 + -q0q1);
    rMat[2][0] = 2.0f * (q1q3 + -q0q2) ;rMat[2][1] = 2.0f * (q2q3 - -q0q1)   ;rMat[2][2]=1.0f-2.0f*q1q1-2.0f*q2q2;
	
	norm = invSqrt(ax*ax + ay*ay + az*az);
	
	static float aex, aey, aez;
	aex=ax*0.00244140625f;
	aey=ay*0.00244140625f;
	aez=az*0.00244140625f;//1.0f/4096.0f;
	
	ax = ax * norm;//?????
	ay = ay * norm;
	az = az * norm;
    //N--->B////????
	ex = (ay * rMat[2][2] - az * rMat[2][1]);
	ey = (az * rMat[2][0] - ax * rMat[2][2]);
	ez = (ax * rMat[2][1] - ay * rMat[2][0]);

	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;
    if(exInt>= errINT_Limt) exInt= errINT_Limt;
    if(exInt<=-errINT_Limt) exInt=-errINT_Limt;
    if(eyInt>= errINT_Limt) eyInt= errINT_Limt;
    if(eyInt<=-errINT_Limt) eyInt=-errINT_Limt;
	//PI
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	//??????
	q0_last = q0;
	q1_last = q1;
	q2_last = q2;
	q3_last = q3;
	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 
	//???
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
    if(!cnnt)
	{nacc.x=rMat[0][0]*aex+rMat[0][1]*aey+rMat[0][2]*aez;
	nacc.y=rMat[1][0]*aex+rMat[1][1]*aey+rMat[1][2]*aez;
	nacc.z=rMat[2][0]*aex+rMat[2][1]*aey+rMat[2][2]*aez;}
	
	if(gyro.z>2 || gyro.z<-2)
	out_angle.yaw-=(filter_gyro.z * RawData_to_Angle * 0.001f);
}
void Get_Eulerian_Angle(struct _out_angle *angle)//所得的只是 pitch roll
{
	angle->roll = -atan2(2.0f*(q0*q1 + q2*q3),q0*q0 - q1*q1 - q2*q2 + q3*q3)*Radian_to_Angle;
	angle->pitch  =  asin (2.0f*(q0*q2 - q1*q3))*Radian_to_Angle;
}
