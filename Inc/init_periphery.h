/*
 * init_periphery.h
 *
 *  Created on: Oct 24, 2026
 *      Author: Ej
 */

#ifndef INIT_PERIPHERY_H_
#define INIT_PERIPHERY_H_

#include "stm32f10x_conf.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "uc1601s.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define button1_pin		GPIO_Pin_10
#define button2_pin		GPIO_Pin_9
#define button3_pin		GPIO_Pin_8
#define button4_pin		GPIO_Pin_15
#define lcd_led			GPIO_Pin_11

void init(void);
void init_gpio(void);

#endif /* INIT_PERIPHERY_H_ */
