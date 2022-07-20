#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#define LCD_RW GPIO_Pin_13
#define LCD_RS GPIO_Pin_14 // Chan chon thanh ghi RS = 0 cho phep ghi RS = 1 thanh ghi doc 
#define LCD_EN GPIO_Pin_15 // chan cho phep hoat dong LCD va tao xung 
#define LCD_D4 GPIO_Pin_0
#define LCD_D5 GPIO_Pin_1
#define LCD_D6 GPIO_Pin_2
#define LCD_D7 GPIO_Pin_3

void LCD_Configure(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin      = LCD_RW | LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
	GPIO_InitStructure.GPIO_Speed    = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void delay_ms(unsigned int time){
	while(time){
		SysTick  -> LOAD   = 71999;
		SysTick  -> VAL    = 0;
		SysTick  -> CTRL   = 5;
	  while(!(SysTick -> CTRL & ( 1 << 16))){
		}
		-- time;
	}
}

void delay_us(unsigned int time){
	while(time){
		SysTick -> LOAD   = 72 - 1;
		SysTick -> VAL    = 0;
		SysTick -> CTRL   = 5;
		while(!(SysTick -> CTRL & (1 << 16))){
		}
		-- time;
	}
}

void LCD_Enable(){
	GPIO_SetBits(GPIOB, LCD_EN);
	delay_us(3);
	GPIO_ResetBits(GPIOB, LCD_EN);
	delay_us(50);
}

void LCD_Send4Bit(unsigned int data){
	GPIO_WriteBit(GPIOB, LCD_D4, data & 0x01);
	GPIO_WriteBit(GPIOB, LCD_D5, (data >> 1) & 1);
	GPIO_WriteBit(GPIOB, LCD_D6, (data >> 2) & 1);
	GPIO_WriteBit(GPIOB, LCD_D7, (data >> 3) & 1);
}
void LCD_SendCommand(unsigned int data){
	LCD_Send4Bit(data >> 4);
	LCD_Enable();
	LCD_Send4Bit(data);
	LCD_Enable();
}
void LCD_Init(){
	LCD_Configure();
	LCD_Send4Bit(0x00);
	delay_ms(20);
	GPIO_WriteBit(GPIOB, LCD_RS, 0);
	GPIO_WriteBit(GPIOB, LCD_RW, 0);
	LCD_Send4Bit(0x03);//mac dinh
	LCD_Enable();
	delay_ms(5);
	LCD_Enable();
	delay_us(100);
	LCD_Send4Bit(0x02);//nhap nhay con tro
	LCD_Enable();
	LCD_SendCommand(0x28);//giao tiep mac dinh 4 bit
	LCD_SendCommand(0x0C);//bat ta con tro 
	LCD_SendCommand(0x06);//tu dong di chuyn con tro to vi tri ke tiep
	LCD_SendCommand(0x01);// xoa toan man hinh
}
void LCD_Clear(){
	LCD_SendCommand(0x01);
	delay_ms(10);
}
void LCD_Gotoxy(unsigned int x, unsigned int y){
	unsigned int address;
	if(!y){
		address = (0x80 + x);
	}
	else address = (0xC0 +  x);
	delay_us(1000);
	LCD_SendCommand(address);
	delay_us(50);
}
void LCD_PutChar(unsigned char data){
	GPIO_WriteBit(GPIOB, LCD_RS, 1);
	LCD_SendCommand(data);
	GPIO_WriteBit(GPIOB, LCD_RS, 0);
}
void LCD_Puts(char *s){
	while(*s){
		LCD_PutChar(*s);
		s++;
	}
}
int main(){
	LCD_Init();
	delay_ms(20);
	while(1){
		LCD_Gotoxy(0,0);
		LCD_Puts(" Vuong Viet Thao");
	}
}