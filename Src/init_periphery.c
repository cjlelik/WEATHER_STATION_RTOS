#include "init_periphery.h"

void init_gpio(void)
{
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_11;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio_init);

	GPIO_SetBits(GPIOB,  GPIO_Pin_11);
}
