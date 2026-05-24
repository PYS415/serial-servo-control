#ifndef __KEY_H
#define __KEY_H

extern volatile uint8_t key_interrupt_flag;
void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif
