#include <stdint.h>
#include "init_periphery.h"

void BlinkLed(void *argument)
{
    while (1)
    {
        // Твой код
    	GPIOB->ODR ^= GPIO_Pin_11;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
	init();
	   xTaskCreate(BlinkLed,"Task1",128,NULL,1,NULL
	    );

	    vTaskStartScheduler();
    /* Loop forever */
	for(;;);
}

