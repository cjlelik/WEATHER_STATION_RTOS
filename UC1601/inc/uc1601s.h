#ifndef _uc1601s_
#define _uc1601s_

#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"


#define LCD_X 132
#define LCD_Y 64

#define LCD_BUFFER_SIZE 1052

#define LCD_SPI SPI1

#define LCD_SCK_PIN		GPIO_Pin_5
#define LCD_MOSI_PIN	GPIO_Pin_7
#define LCD_RESET_PIN 	GPIO_Pin_2
#define LCD_CD_PIN 		GPIO_Pin_0
#define LCD_CS_PIN 		GPIO_Pin_10

//���������� ��������---------------------------------
#define LCD_RESET_L GPIOB->BSRR |= LCD_RESET_PIN << 16
#define LCD_RESET_H GPIOB->BSRR |= LCD_RESET_PIN

#define LCD_COMMAND GPIOB->BSRR |= LCD_CD_PIN << 16
#define LCD_DATA 	GPIOB->BSRR |= LCD_CD_PIN

#define LCD_CS_L 	GPIOB->BSRR |= LCD_CS_PIN << 16
#define LCD_CS_H 	GPIOB->BSRR |= LCD_CS_PIN
	
#define LCD_Color_White	0x00
#define LCD_Color_Black	0xFF

void SPI_SEND_BYTE(uint8_t data);

void LcdPortInit(void);
void Delay(uint32_t tick);
void LcdInit(void);
void LcdInitHello(void);
void DMA1_Init(void);
void LcdSendCom(uint8_t command);
void LcdSendData(uint8_t data);
void LcdGraphicsRedraw(void);
void LCD_SET_BYTE(void);
void LcdGraphicsRedrawDMA(void);
void LcdGraphicsClear(uint8_t LCD_Color);
void LcdLed(uint8_t value);
void LcdDrawPixel(uint8_t x, uint8_t y, uint8_t color);															//������ ������� �� ����������� x y
void LcdDrawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t color);								//������ ����� �� ����������� x y
void LcdDrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);						//������ ������� �� ����������� x y
void LcdDrawShadeRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void LcdDrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color);
void LcdSizePutChar(char ch,uint16_t x, uint16_t y, uint8_t pixel_width, uint8_t pixel_height, uint8_t color);
void LcdSizePutStr(const char *pString, uint16_t  x, uint16_t  y, uint8_t pixel_width, uint8_t pixel_height, uint8_t color);
void LcdDrawGraph(uint8_t pos_x, uint8_t pos_y, uint8_t lenght_pix, uint8_t height_pix, uint16_t *buff, uint16_t size_buff, uint8_t color);
void LcdDrawLogGraph(uint8_t pos_x, uint8_t pos_y, uint8_t lenght_pix, uint8_t height_pix, uint8_t value,  uint8_t color);
#endif
