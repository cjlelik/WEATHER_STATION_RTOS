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



void init(void);
void init_gpio(void);

#endif /* INIT_PERIPHERY_H_ */
