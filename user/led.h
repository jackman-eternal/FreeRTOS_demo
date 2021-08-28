#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "usart.h"

#define LED0 PAout(8)	// PA8
#define LED1 PAout(2)	// PD2	

void LED_INIT(int flag);
//void EXTI_PA4_init(void);
//extern TaskHandle_t Task2_Handler;
#endif
