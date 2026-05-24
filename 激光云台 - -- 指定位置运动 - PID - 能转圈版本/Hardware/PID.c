#include "stm32f10x.h"
#include "PID.h"

// 添加X轴PID结构体
PIDczx xPID = {
    .Kp = 0.36,
    .Ki = 0.43,
    .Kd = 0,
    .OutMax = 2300,
    .OutMin = 1800,
	  .Out = 2047,
};


//X轴云台
void PID_czx_x(PIDczx *p)
{
	
	p->error2 = p->error1;//上上次误差
	p->error1 = p->error0;//上次误差
	p->error0 = p->Target- p->Actual;//本次误差
	

	p->Out += p->Kp * (p->error0 - p->error1) 
				 +  p->Ki *  p->error0 
	       +  p->Kd * (p->error0 - 2 * p->error1 + p->error2);
	
	if(p->Out > p->OutMax) p->Out = p->OutMax;
	if(p->Out < p->OutMin) p->Out = p->OutMin;

}

// 添加Y轴PID结构体
PIDczx yPID = {
    .Kp = 0.36,
    .Ki = 0.43,
    .Kd = 0,
    .OutMax = 2300,
    .OutMin = 1800,
    .Out = 2047,
};

// Y轴云台PID控制函数
void PID_czx_y(PIDczx *p)
{
    p->error2 = p->error1;  // 上上次误差
    p->error1 = p->error0;  // 上次误差
    p->error0 = p->Target - p->Actual;  // 本次误差
    
    p->Out += p->Kp * (p->error0 - p->error1) 
             + p->Ki * p->error0 
             + p->Kd * (p->error0 - 2 * p->error1 + p->error2);
    
    if(p->Out > p->OutMax) p->Out = p->OutMax;
    if(p->Out < p->OutMin) p->Out = p->OutMin;
}

