#include "uc1601s.h"
#include "symbtable.h"


uint8_t lcd_buffer[LCD_BUFFER_SIZE];

void LcdPortInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	SPI_InitTypeDef spi_struct;
	GPIO_InitTypeDef gpio_struct;

	gpio_struct.GPIO_Pin = LCD_MOSI_PIN | LCD_SCK_PIN;
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &gpio_struct);

	gpio_struct.GPIO_Pin = LCD_RESET_PIN | LCD_CD_PIN | LCD_CS_PIN;
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &gpio_struct);

	spi_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spi_struct.SPI_CPHA = SPI_CPHA_1Edge;
	spi_struct.SPI_CPOL = SPI_CPOL_Low;
	//spi_struct.SPI_CRCPolynomial = SPI_CRCPR_CRCPOLY;
	spi_struct.SPI_DataSize = SPI_DataSize_8b;
	spi_struct.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_struct.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_struct.SPI_Mode = SPI_Mode_Master;
	spi_struct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(LCD_SPI, &spi_struct);
	SPI_Cmd(LCD_SPI, ENABLE);
}

void SPI_SEND_BYTE(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET);
	LCD_SPI->DR = data;
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) == SET);
}
void Delay(uint32_t tick)
{
	volatile uint32_t count=0;
	while(tick--)
	{
		while(count < 7200){count++;}
	}
}
void LcdSendCom(uint8_t command)
{
	LCD_COMMAND;
	SPI_SEND_BYTE(command);
}
void LcdSendData(uint8_t data)
{
	LCD_DATA;
	SPI_SEND_BYTE(data);
}
void LCD_SET_BYTE(void)
{
		LcdSendCom(0b00000000);//Set Column address LSB
		LcdSendCom(0b00010000);//Set Column address MSB
		LcdSendCom(0b10110000);//Set Page address
		LCD_DATA;
		for(uint8_t x=0; x<1024; x++)
		{
			SPI_SEND_BYTE(255);
		}
}
void DMA1_Init(void)
{

}
void LcdInit(void)
{
	LcdPortInit();
	Delay(100);
	LCD_CS_L;
	LCD_RESET_L;
	Delay(100);
	LCD_RESET_H;
	Delay(100);
	LcdSendCom(0b11100010);//System Reset
	Delay(100);
	LcdSendCom(0b00100111);//Set temperature compensation 0b001001##
	LcdSendCom(0b11000010);//Set lcd Mapping control 0b11000##0
	LcdSendCom(0b10001001);//Set RAM Address Control 0b10001###
	LcdSendCom(0b10100000);//Set Frame Rate 0b1010000#
	LcdSendCom(0b11101011);//Set LCD_Bias Ratio 0b111010##
	LcdSendCom(0b10000001);//Set Vbias Potentiometer
	LcdSendCom(170);//Value potentiometer 0-255
	LcdSendCom(0b11110001);//Set COM end
	LcdSendCom(0b00111111);//63
	LcdSendCom(0b00000000);//Set Column address LSB
	LcdSendCom(0b00010000);//Set Column address MSB
	LcdSendCom(0b10110000);//Set Page address
	LcdSendCom(0b10101111);//Set Display Enable
	Delay(100);
	LcdGraphicsClear(LCD_Color_White);
	LcdGraphicsRedraw();
}
void LcdInitHello(void)
{
	uint8_t buff[32];
	for(uint8_t c=9; c>0; c--)
	{
		sprintf(buff, "%d", c);
		LcdSizePutStr(buff, 0, 0, 5, 5, LCD_Color_Black);
		LcdGraphicsRedraw();
		Delay(1000000);
	}
}
//----------------------------------------------------------------------------------------------
void LcdGraphicsRedraw(void)
{
		LcdSendCom(0b00000000);//Set Column address LSB
		LcdSendCom(0b00010000);//Set Column address MSB
		LcdSendCom(0b10110000);//Set Page address
		LCD_DATA;
		for(uint16_t x=0; x<1052; x++)
		{
			SPI_SEND_BYTE(lcd_buffer[x]);
		}
}
void LcdGraphicsRedrawDMA(void)
{
	/*
    // Ждём, пока предыдущая DMA-передача закончится
    while (HAL_DMA_GetState(&hdma_spi1_tx) != HAL_DMA_STATE_READY){}
    // Устанавливаем начальный адрес RAM
    LcdSendCom(0x00);       // Column address LSB = 0
    LcdSendCom(0x10);       // Column address MSB = 0
    LcdSendCom(0xB0);       // Page address = 0

    LCD_DATA;

    // Запускаем передачу буфера через SPI + DMA
    HAL_SPI_Transmit_DMA(&hspi1, lcd_buffer, buff_size);
    */
}
void LcdGraphicsClear(uint8_t LCD_Color)
{
	for(uint16_t c=0; c<LCD_BUFFER_SIZE; c++)
	{
		lcd_buffer[c]=LCD_Color;
	}
}
void LcdDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	if (color>1)
	{
		lcd_buffer[x+(LCD_X*(y/8))] |= 1<<y%8;	//���� pixel=1 �� ������ ����� �������
	}
	else
	{
		lcd_buffer[x+(LCD_X*(y/8))] &= ~(1<<y%8);	//���� pixel !=0 �� ������ ������ �������
	}
}
void LcdDrawLine(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t color)
{
	int dx=x1-x0;
	int dy=y1-y0;
	int stepx,stepy;

	if (dx<0) {dx=-dx;stepx=-1;} else {stepx=1;}
	if (dy<0) {dy=-dy;stepy=-1;} else {stepy=1;}

	dx<<=1;
	dy<<=1;

	LcdDrawPixel(x0, y0, color);

	if (dx>dy)
	{
		int fraction=dy-(dx>>1);
		while (x0!=x1)
		{
			if (fraction>=0)
			{
				y0+=stepy;
				fraction-=dx;
			}
			x0+=stepx;
			fraction+=dy;
			LcdDrawPixel(x0, y0, color);
		}
	}
	else
	{
		int fraction=dx-(dy>>1);
		while (y0!=y1)
		{
			if (fraction>=0)
			{
				x0+=stepx;
				fraction-=dy;
			}
			y0+=stepy;
			fraction+=dx;
			LcdDrawPixel(x0, y0, color);
		}
	}

}
void LcdDrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color)
{
	LcdDrawLine(x0,y0,x1,y0,color);
	LcdDrawLine(x1,y0,x1,y1,color);
	LcdDrawLine(x1,y1,x0,y1,color);
	LcdDrawLine(x0,y1,x0,y0,color);
}
void LcdDrawShadeRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
	for(uint8_t x=0; x<x1-x0 ; x++)
	{
		for(uint8_t y=0; y<y1-y0; y++)
		{
			LcdDrawPixel(x0+x,y0+y,color);
		}
	}
}
void LcdDrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color)
{
	int f=1-radius;
	int ddF_x=0;
	int ddF_y=-2*radius;
	int x=0;
	int y=radius;

	LcdDrawPixel(x0,y0+radius, color);
	LcdDrawPixel(x0,y0-radius, color);
	LcdDrawPixel(x0+radius,y0, color);
	LcdDrawPixel(x0-radius,y0, color);

	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x+1;
		LcdDrawPixel(x0+x,y0+y, color);
		LcdDrawPixel(x0-x,y0+y, color);
		LcdDrawPixel(x0+x,y0-y, color);
		LcdDrawPixel(x0-x,y0-y, color);
		LcdDrawPixel(x0+y,y0+x, color);
		LcdDrawPixel(x0-y,y0+x, color);
		LcdDrawPixel(x0+y,y0-x, color);
		LcdDrawPixel(x0-y,y0-x, color);
	}
}
void LcdSizePutChar(char ch,uint16_t x, uint16_t y, uint8_t pixel_width, uint8_t pixel_height, uint8_t color)
{
	if (ch<0x20) return;		//���� ������� �� ������� ������� �������� �� ���������
	uint8_t sym=0, polu_sym;	//������ � ������� ��������
	if (ch>128) sym=ch-0x60; else sym=ch-0x20;//������� ��� �������� ������� � �������� �����
	uint16_t x1,y1;
	for (x1 = 0; x1 < 5; x1++)
	{
		polu_sym = symboltable[sym][x1];
		for (y1 = 0; y1 < 7; y1++ )
		{
			if ((polu_sym >> y1) & 1)
			{
				LcdDrawShadeRect((x1*pixel_width)+x,(y1*pixel_height)+y,(x1*pixel_width)+pixel_width+x,(y1*pixel_height)+pixel_height+y, color);
			}
			else
			{
				LcdDrawShadeRect((x1*pixel_width)+x,(y1*pixel_height)+y,(x1*pixel_width)+pixel_width+x,(y1*pixel_height)+pixel_height+y, ~color);
			}
		}
	}
}
void LcdSizePutStr(const char *pString, uint16_t  x, uint16_t  y, uint8_t pixel_width, uint8_t pixel_height, uint8_t color)
{
	while (*pString != 0x00)//������� ������� ���� �� ����� �� ������� ����� ������
	{
		if (x+pixel_width*6>479)//���� ��������� ������ ������ �� ��������� �� ��������� ������
		{
			x=0;
			y+= (pixel_height*7)+1;
		}
		if (*pString == '\r') {x=0; y+= (pixel_height*7)+1;}//���� ��������� ������ \r ��������� �� ����. ������
		LcdSizePutChar(*pString++, x, y, pixel_width, pixel_height, color);//��� ������
		x+=(pixel_width*5)+1;//��������� �� 5 �������� ������ ���� ���� ������� ����� ����� ���������
		if (y > 479)
		{
			break;//������� �� ����� ������ ������ �� �����
		}
	}
}
void LcdDrawGraph(uint8_t pos_x, uint8_t pos_y, uint8_t lenght_pix, uint8_t height_pix, uint16_t *buff, uint16_t size_buff, uint8_t color)
{
	float l_coef = size_buff / lenght_pix;
	LcdDrawRect(pos_x, pos_y, pos_x+lenght_pix, pos_y+height_pix, color);
	for(uint16_t i=1; i< lenght_pix; i++)
	{
		LcdDrawPixel(i+pos_x, (*(buff+(uint16_t)(i*l_coef))/(4096/height_pix))+pos_y, color);
	}
}
void LcdDrawLogGraph(uint8_t pos_x, uint8_t pos_y, uint8_t lenght_pix, uint8_t height_pix, uint8_t value,  uint8_t color)
{
	static uint8_t temp_buff[128];
	static uint8_t count_val=0;
	static uint8_t index=0;
	index = count_val;
	temp_buff[count_val++] = height_pix - value;
	if(count_val>128){count_val=0;}
	for(uint8_t x=0; x<128; x++)
	{
		LcdDrawPixel(x, temp_buff[index--], LCD_Color_Black);
		if(index==0){index=128;}
	}
}
void LcdDrawLogGraphStart(void)
{
	extern uint8_t _flag_log_graph;
	_flag_log_graph = 1;
}
