#include <stdint.h>
#include "init_periphery.h"

uint8_t hour=0,min=0,sec=0;
char sym_buff[32];

void button(void)
{
	if(GPIOB->IDR & button4_pin){hour++;}
	if(GPIOA->IDR & button3_pin){min++;}
	if(GPIOA->IDR & button2_pin){sec=0;}
	if(GPIOA->IDR & button1_pin){GPIOB->ODR ^=lcd_led;}
}

void BlinkLed(void *argument)
{
    while (1)
    {
    	button();
    	if(sec==60){sec=0;min++;}
    	if(min==60){min=0;hour++;}
    	if(hour==24){hour=0;}
    	sprintf(sym_buff, "%02d:%02d:%02d", hour, min, sec);
    	LcdSizePutStr(sym_buff, 0, 0, 3, 4, LCD_Color_Black);
    	LcdGraphicsRedraw();
    	sec++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void MainLogic(void *argument)
{
	while(1)
	{

    	vTaskDelay(pdMS_TO_TICKS(100));
	}
}

int main(void)
{
	init();
	xTaskCreate(BlinkLed,"Task1",128,NULL,1,NULL);
	xTaskCreate(MainLogic,"Task2",128,NULL,1,NULL);
	vTaskStartScheduler();
	for(;;);
}

