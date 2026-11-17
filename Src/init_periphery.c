#include "init_periphery.h"

void init(void)
{
	init_gpio();
	LcdInit();
}

void init_gpio(void)
{
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init.GPIO_Pin = button1_pin | button2_pin | button3_pin;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_init);

	gpio_init.GPIO_Pin = button4_pin;
	GPIO_Init(GPIOB, &gpio_init);

	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin = lcd_led;
	GPIO_Init(GPIOB, &gpio_init);

	GPIOB->BSRR = GPIO_Pin_11;
}
