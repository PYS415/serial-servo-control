#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <Serial.h>
#include <steering_control.h>
#include "delay.h"


void Host2SCS(uint8_t  *DataL, uint8_t * DataH, uint16_t  Data)
{
		*DataL = (uint8_t)(Data>>8);
		*DataH = (uint8_t)(Data&0xff);
}

void UART_Servo_SendWork_3(char ID, uint16_t location,int speed,int time)//速度<2700
{
	uint8_t   DataL_speed;
	uint8_t   DataH_speed;
	
	uint8_t   DataL_time;
	uint8_t   DataH_time;	
//	DataL_speed=(speed>>8);	DataH_speed=(speed&0xff);
//	DataL_time=(time>>8);		DataH_time=(time&0xff);
  Host2SCS(&DataL_speed,	&DataH_speed, speed);
  Host2SCS(&DataL_time, &DataH_time, time);	
	char Check_Sum;
	char temp_location = (location >> 8);
	
	uint16_t temp_sum = ID + 0x09 + 0x03 + 0x2A + location + temp_location + DataL_time + DataH_time + DataL_speed + DataH_speed ;

	//char temp_sum2;
//	if(temp_sum > 255)
//	{
	temp_sum = temp_sum & 0x00000FF;
//	}
	Check_Sum = ~(temp_sum);

	Serial_SendByte(0xFF);

	Serial_SendByte(0xFF);

	Serial_SendByte(ID);

	//写数据长度
	Serial_SendByte(0x09);

	//命令类型
	Serial_SendByte(0x03);

	//写入位置
	Serial_SendByte(0x2A);

	//写位置
	Serial_SendByte(location);

	Serial_SendByte(temp_location);

	//写时间
	Serial_SendByte(DataL_time);

	Serial_SendByte(DataH_time);

	//写速度
	Serial_SendByte(DataL_speed);

	Serial_SendByte(DataH_speed);
		
		
	Serial_SendByte(Check_Sum);

}

void UART_Servo_SendWork_4(char ID, uint16_t order)//舵机模式
{
	char Check_Sum;
	uint16_t temp_sum = ID + 0x09 + 0x03 + 0x2A + order ;
	//char temp_sum2;
//	if(temp_sum > 255)
//	{
	temp_sum = temp_sum & 0x00000FF;
//	}
	Check_Sum = ~(temp_sum);
	
	Serial_SendByte(0xFF);
//	UART_transmitData(EUSCI_A2_BASE, 0xFF);
	Serial_SendByte(0xFF);
//	UART_transmitData(EUSCI_A2_BASE, 0xFF);
	Serial_SendByte(ID);
//	UART_transmitData(EUSCI_A2_BASE, ID);

	//写数据长度
	Serial_SendByte(0x04);
//	UART_transmitData(EUSCI_A2_BASE, 0x04);

	//命令类型
	Serial_SendByte(0x03);
//	UART_transmitData(EUSCI_A2_BASE, 0x03);

	//写入位置
	Serial_SendByte(0x21);
//	UART_transmitData(EUSCI_A2_BASE, 0x21);

	//写运行模式
	Serial_SendByte(order);
//	UART_transmitData(EUSCI_A2_BASE, order );
	
	Serial_SendByte(Check_Sum);
//	UART_transmitData(EUSCI_A2_BASE, Check_Sum);

}

void UART_Servo_SendWork(char ID, uint16_t location)//速度1000每秒
{

	char Check_Sum;
	char temp_location = (location >> 8);
	uint16_t temp_sum = ID + 0x09 + 0x03 + 0x2A + location + temp_location + 0x00 + 0x00 + 0xE8 + 0X03;
	
	if(temp_sum > 255)
	{
		 temp_sum = temp_sum & 0x00000FF;
	}
	Check_Sum = ~(temp_sum);
	
	Serial_SendByte(0xFF);	
//	UART_transmitData(EUSCI_A2_BASE, 0xFF);

	Serial_SendByte(0xFF);	
//	UART_transmitData(EUSCI_A2_BASE, 0xFF);

	Serial_SendByte(ID);	
//	UART_transmitData(EUSCI_A2_BASE, ID);

	Serial_SendByte(0x09);	
//	UART_transmitData(EUSCI_A2_BASE, 0x09);

	Serial_SendByte(0x03);	
//	UART_transmitData(EUSCI_A2_BASE, 0x03);
	
	Serial_SendByte(0x2A);	
//	UART_transmitData(EUSCI_A2_BASE, 0x2A);
	
	Serial_SendByte(location);	
//	UART_transmitData(EUSCI_A2_BASE, location );

	Serial_SendByte(temp_location);	
//	UART_transmitData(EUSCI_A2_BASE, temp_location);

	Serial_SendByte(0x00);	
//	UART_transmitData(EUSCI_A2_BASE, 0x00);

	Serial_SendByte(0x00);	
//	UART_transmitData(EUSCI_A2_BASE, 0x00);

	Serial_SendByte(0xE8);	
//	UART_transmitData(EUSCI_A2_BASE, 0xE8);

	Serial_SendByte(0x03);	
//	UART_transmitData(EUSCI_A2_BASE, 0x03);

	Serial_SendByte(Check_Sum);	
//	UART_transmitData(EUSCI_A2_BASE, Check_Sum);

}

int abs_value(int Location) {
	
	
	if(Location >= 0) { 
		return Location;
	} else { return -Location;}
	
}

int _value(uint16_t Act_Location, uint16_t Tar_Location) {
	
	if(Tar_Location > Act_Location) {
			return 1;
	} else {return 0;}
}

void move_ctr(uint16_t Act_value, uint16_t Tar_value, uint16_t tim, uint8_t speed) {
	
	
	int dir = _value(Act_value, Tar_value); // 鎺у埗鑸垫満鏂瑰悜
	int steps = abs_value(Act_value - Tar_value);
	
	if(dir) {
		for (int i = 0; (speed * i) < steps; i++) {
			
			UART_Servo_SendWork_3(1, Act_value + (speed * i),1,0);
			Delay_ms(tim);
		}
	}
	if(!dir) {
		
		for(int i = 0; (speed * i) < steps; i++) {
			
			UART_Servo_SendWork_3(1, Act_value - (speed * i),1,0);
			Delay_ms(tim);
		}
	}
}

int abs_value1(int Location) {
	
	
	if(Location >= 0) { 
		return Location;
	} else { return -Location;}
	
}

int _value1(uint16_t Act_Location, uint16_t Tar_Location) {
	
	if(Tar_Location > Act_Location) {
			return 1;
	} else {return 0;}
}

void move_ctr1(uint16_t Act_value, uint16_t Tar_value, uint16_t tim, uint8_t speed) {
	
	
	int dir = _value(Act_value, Tar_value); // 鎺у埗鑸垫満鏂瑰悜
	int steps = abs_value(Act_value - Tar_value);
	
	if(dir) {
		for (int i = 0; (speed * i) < steps; i++) {
			
			UART_Servo_SendWork_3(2, Act_value + (speed * i),1,0);
			Delay_ms(tim);
		}
	}
	if(!dir) {
		
		for(int i = 0; (speed * i) < steps; i++) {
			
			UART_Servo_SendWork_3(2, Act_value - (speed * i),1,0);
			Delay_ms(tim);
		}
	}
}

void move_servo_smoothly(int sx1, int sy1, int sx2, int sy2, int steps, int delay_ms)
{
    for (int i = 0; i <= steps; i++)
    {

        float t = i / (float)steps;
        int sx = sx1 + (sx2 - sx1) * t;
        int sy = sy1 + (sy2 - sy1) * t;
        UART_Servo_SendWork_3(1, sx, 1, 0);
        UART_Servo_SendWork_3(2, sy, 1, 0);
        Delay_ms(delay_ms);  // ?????????
    }
}

