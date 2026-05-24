#include "stm32f10x.h"
#include "OLED.h"
#include "Serial.h"
#include "steering_control.h"
#include "Key.h"
#include "Timer.h"
#include "string.h"
#include "Serial234.h"
#include "stdio.h"
#include "Delay.h"
#include "Pid.h"

uint8_t KeyNum;
uint8_t mode = 0, EN = 0, flag = 0;
int mode2flag = 0;

// 舵机起点位置
uint16_t qidian_positions[2] = {2050, 2050};

// 屏幕四个角位置
uint16_t servo_rect[4][2] = {
    {1890, 1900}, // A 左上
    {2201, 1900}, // B 右上
    {2201, 2205}, // C 右下
    {1887, 2205}  // D 左下
};

// 定义接受矩形5个点的数组，激光点的实际值和目标值
int pixel_rect[5][3];
int laser_x_act = -1, laser_y_act = -1, laser_x_tar = -1, laser_y_tar = -1;

// 飞特协议位置换算：
// 位置值范围 0~4095，对应舵机转角 0°~360°
// 当前基准位置（qidian_positions[0]=2050）是相对于中位(2048)的偏移
// 90度对应的绝对位置值 = 2048 + (4096/360)*90 ≈ 2048 + 1024 = 3072

#define SERVO_ID_1   5    // X轴舵机ID
#define SERVO_ID_2   3    // Y轴舵机ID
#define SERVO_SPEED  500   // 运行速度 (<2700)
#define POS_90DEG    1024 // 90度对应位置值
#define POS_180DEG   2048// 90度对应位置值

int main(void) {

    // ========== 硬件初始化 ==========
    Serial_Init();      // 串口初始化（USART1，TX=PA9，连接飞特舵机）
    OLED_Init();        // OLED屏幕初始化
    Key_Init();         // 按键初始化
    Timer_Init();       // 定时器1初始化（按键扫描，1ms周期）
    Timer_Init1();      // 定时器2初始化（PID控制，1ms周期）

    // ========== 舵机初始化 ==========
    // 让舵机以较快速度移动到中位(2048)，稳定后再执行后续动作
    UART_Servo_SendWork_3(SERVO_ID_1,0, SERVO_SPEED, 500); // X轴回中位
    Delay_ms(600);      // 等待舵机到达中位（约500ms运行时间 + 余量）
    UART_Servo_SendWork_3(SERVO_ID_2,0, SERVO_SPEED, 500); // Y轴回中位
    Delay_ms(600);      // 等待舵机到达中位（约500ms运行时间 + 余量）

    // ========== 让X轴舵机(SERVO_ID_1)旋转90度 ==========
    // 从当前位置(2048)移动到90度位置(3072)
    // 参数说明：UART_Servo_SendWork_3(ID, 目标位置, 速度, 运行时间)
    UART_Servo_SendWork_3(SERVO_ID_1, POS_90DEG, SERVO_SPEED, 1000);
    Delay_ms(200);      
    UART_Servo_SendWork_3(SERVO_ID_2, POS_180DEG, SERVO_SPEED, 1000);
    Delay_ms(600); 
		
    // 可选：在OLED上显示当前状态
    OLED_Clear();
    OLED_ShowString(0, 0, "Servo Test", 8);
    OLED_ShowString(0, 2, "X: 90deg -> 3072", 8);
    OLED_Update();

    while (1) {
        // 主循环可以添加其他逻辑
    }
}

void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) {
        Key_Tick();
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}


void TIM2_IRQHandler(void)
	{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) 
			{
        static uint16_t Count;  // 用于计次分频
        Count++;  // 计次自增
        if (Count >= 10)  // 如果计次25次，则if成立，即if每隔25ms进一次
        {
            Count = 0;  // 计次清零，便于下次计次
            
            // 只有在模式2且使能状态下才进行PID控制
            if (mode == 2 && EN == 1 && mode2flag == 1) 
            {
                // 设置PID实际值
                xPID.Actual = laser_x_act;
                yPID.Actual = laser_y_act;
                
                // 计算PID输出
                PID_czx_x(&xPID);
                PID_czx_y(&yPID);
                
                // 使用move_servo_smoothly控制舵机
                move_servo_smoothly(qidian_positions[0], qidian_positions[1], xPID.Out, yPID.Out, 1, 0);
//                UART_Servo_SendWork_3(1, xPID.Out, 1, 0);
//                UART_Servo_SendWork_3(2, yPID.Out, 1, 0);

                // 更新起点位置
                qidian_positions[0] = xPID.Out;
                qidian_positions[1] = yPID.Out;
							  mode2flag = 0;
            }
       }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
							
