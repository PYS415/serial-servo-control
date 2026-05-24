#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"                  // Device header
#include "math.h"

typedef struct
{
    // 目标值、实际值和输出值
    int Target;      // 目标值
    int Actual;     // 实际值
    int Out;      // 输出值

    // PID参数
    float Kp;          // 比例系数
    float Ki;          // 积分系数
    float Kd;          // 微分系数

    // 误差变量
	float error2; // 上上次误差
    float error1; // 上一次误差
    float error0;  // 当前误差

    // 输出限幅
    int OutMax; // 输出最大值
    int OutMin; // 输出最小值

} PIDczx;


// 添加Y轴PID控制函数声明

// 添加Y轴PID结构体声明
void PID_czx_x(PIDczx *p);
void PID_czx_y(PIDczx *p);

// 🔽 加上这两句：告诉其他文件这些变量是在别处定义的
extern PIDczx xPID;
extern PIDczx yPID;

#endif
