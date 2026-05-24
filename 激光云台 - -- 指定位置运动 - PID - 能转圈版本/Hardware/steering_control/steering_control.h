#ifndef __STEERING_CONTROL_H
#define __STEERING_CONTROL_H

#include <stdio.h>
extern volatile uint8_t key_interrupt_flag;
void UART_Servo_SendWork_3(char ID, uint16_t location,int speed,int time);//速度<2700
void UART_Servo_SendWork_4(char ID, uint16_t order);//速度<2700
void Host2SCS(uint8_t  *DataL, uint8_t * DataH, uint16_t Data);
void UART_Servo_SendWork(char ID, uint16_t location);
void move_ctr(uint16_t Act_value, uint16_t Tar_value, uint16_t tim, uint8_t speed);
void move_ctr1(uint16_t Act_value, uint16_t Tar_value, uint16_t tim, uint8_t speed);
void move_servo_smoothly(int sx1, int sy1, int sx2, int sy2, int steps, int delay_ms);

#endif
