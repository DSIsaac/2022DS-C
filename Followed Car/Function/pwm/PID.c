/*
 * PID.c
 *
 *  Created on: 2022年7月29日
 *      Author: HASEE
 */
#include "PID.h"

 _PID_Position PID_Pos_1;
 _PID_Position PID_Pos_2;
 _PID_Position PID_Pos_3;
 _PID_Position PID_Pos_4;

 float Pid_Value_Array [4][6]=
 {
     //内环
     //KP KI KD INT_UP INT_DOWN       target//参数含义
     {500,  5, 10,   10000,     -10000,      0 },
     {500,  5, 10,   10000,     -10000,      0 },
     //外环
     {1.8f,  0, 0.15,   10000,     -10000,      0 },
     {0,  0, 0,   10000,     -10000,      0 },

 };

void PID_Init(void)
{
    PID_Pos_1.KP = Pid_Value_Array[0][0];
    PID_Pos_1.KI = Pid_Value_Array[0][1];
    PID_Pos_1.KD = Pid_Value_Array[0][2];
    PID_Pos_1.INT_UP = Pid_Value_Array[0][3];
    PID_Pos_1.INT_DOWN = Pid_Value_Array[0][4];
    PID_Pos_1.Target = Pid_Value_Array[0][5];

    PID_Pos_2.KP = Pid_Value_Array[1][0];
    PID_Pos_2.KI = Pid_Value_Array[1][1];
    PID_Pos_2.KD = Pid_Value_Array[1][2];
    PID_Pos_2.INT_UP = Pid_Value_Array[1][3];
    PID_Pos_2.INT_DOWN = Pid_Value_Array[1][4];
    PID_Pos_2.Target = Pid_Value_Array[0][5];

    PID_Pos_3.KP = Pid_Value_Array[2][0];
    PID_Pos_3.KI = Pid_Value_Array[2][1];
    PID_Pos_3.KD = Pid_Value_Array[2][2];
    PID_Pos_3.INT_UP = Pid_Value_Array[2][3];
    PID_Pos_3.INT_DOWN = Pid_Value_Array[2][4];
    PID_Pos_3.Target = Pid_Value_Array[2][5];

    PID_Pos_4.KP = Pid_Value_Array[3][0];
    PID_Pos_4.KI = Pid_Value_Array[3][1];
    PID_Pos_4.KD = Pid_Value_Array[3][2];
    PID_Pos_4.INT_UP = Pid_Value_Array[3][3];
    PID_Pos_4.INT_DOWN = Pid_Value_Array[3][4];
    PID_Pos_4.Target = Pid_Value_Array[3][5];
}

void Position_PID_Speed(int Value, _PID_Position *PID_Pos_n)
{
    PID_Pos_n->Error = PID_Pos_n->Target - Value;
    PID_Pos_n->Integral += PID_Pos_n->Error;
    PID_Pos_n->Integral = PID_Pos_n->Integral < PID_Pos_n->INT_UP ? PID_Pos_n->Integral : PID_Pos_n->INT_UP;
    PID_Pos_n->Integral = PID_Pos_n->Integral > PID_Pos_n->INT_DOWN ? PID_Pos_n->Integral : PID_Pos_n->INT_DOWN;

    PID_Pos_n->PID_Out = PID_Pos_n->KP * PID_Pos_n->Error + PID_Pos_n->KI * PID_Pos_n->Integral + PID_Pos_n->KD * (PID_Pos_n->Error-PID_Pos_n->Last_Error);
    PID_Pos_n->PID_Out = PID_Pos_n->PID_Out < PID_Pos_n->INT_UP ? PID_Pos_n->PID_Out : PID_Pos_n->INT_UP;
    PID_Pos_n->PID_Out = PID_Pos_n->PID_Out > PID_Pos_n->INT_DOWN ? PID_Pos_n->PID_Out : PID_Pos_n->INT_DOWN;
    PID_Pos_n->Last_Error = PID_Pos_n->Error;
//    if(PID_Pos_n->PID_Out < 0)  PID_Pos_n->PID_Out = 0;
//    PID_Pos_n->PID_Out = -PID_Pos_n->PID_Out;
}
