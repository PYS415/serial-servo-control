#ifndef __SERIAL234_H
#define __SERIAL234_H

#include <stdio.h>

extern char Serial_RxPacket1[];
extern uint8_t Serial_RxFlag1;

void Serial_Init1(void);
void Serial_SendByte1(uint8_t Byte);
void Serial_SendArray1(uint8_t *Array, uint16_t Length);
void Serial_SendString1(char *String);
void Serial_SendNumber1(uint32_t Number, uint8_t Length);
void Serial_Printf1(char *format, ...);

#endif
