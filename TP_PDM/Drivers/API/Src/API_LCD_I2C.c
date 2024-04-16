/*
 * API_I2C.c
 *
 *  Created on: Apr 7, 2024
 *      Author: lean
 */

#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "API_LCD_I2C.h"
#include "stm32f4xx_nucleo_144.h"

static const uint8_t I2C_TIMEOUT = 100;
static const uint8_t I2C_CHAR_SIZE = 4;

I2C_HandleTypeDef I2C1Handle;

static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

bool_t I2C_Init(void)
{
	I2C1Handle.Instance = I2Cx;
	I2C1Handle.Init.ClockSpeed = 100000;
	I2C1Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C1Handle.Init.OwnAddress1 = 0;
	I2C1Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C1Handle.Init.OwnAddress2 = 0;
	I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C1Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&I2C1Handle) != HAL_OK)
	{
		Error_Handler();
	}

	return true;
}

void Lcd_Send_Cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xF0);
	data_l = ((cmd << 4) & 0xF0);
	data_t[0] = data_u | 0x0C;
	data_t[1] = data_u | 0x08;
	data_t[2] = data_l | 0x0C;
	data_t[3] = data_l | 0x08;
	HAL_I2C_Master_Transmit(&I2C1Handle, LCD_ADDRESS, (uint8_t *)data_t, I2C_CHAR_SIZE, I2C_TIMEOUT);
}

void Lcd_Send_Char(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xF0);
	data_l = ((data << 4) & 0xF0);
	data_t[0] = data_u | 0x0D;
	data_t[1] = data_u | 0x09;
	data_t[2] = data_l | 0x0D;
	data_t[3] = data_l | 0x09;
	HAL_I2C_Master_Transmit(&I2C1Handle, LCD_ADDRESS, (uint8_t *)data_t, I2C_CHAR_SIZE, I2C_TIMEOUT);
}

bool_t Lcd_Init(void)
{
	HAL_Delay(60);
	Lcd_Send_Cmd(0x30);
	HAL_Delay(5);
	Lcd_Send_Cmd(0x30);
	HAL_Delay(1);
	Lcd_Send_Cmd(0x30);
	HAL_Delay(10);
	Lcd_Send_Cmd(0x20); // 4-bit mode
	HAL_Delay(10);
	Lcd_Send_Cmd(0x28); // 4-bit, 2 lines, 5x8 font
	HAL_Delay(1);
	Lcd_Send_Cmd(0x08); // Display off
	HAL_Delay(1);
	Lcd_Send_Cmd(0x01); // Clear display
	HAL_Delay(2);
	Lcd_Send_Cmd(0x06); // Increment cursor
	HAL_Delay(1);
	Lcd_Send_Cmd(0x0C); // Display on

	return true;
}

void Lcd_Clear(void){

	Lcd_Send_Cmd(0x01);
	HAL_Delay(2);
}

void Lcd_Set_Cursor(int row, int col){
	uint8_t address;
	switch (row)
	{
	case 1:
		address = col - 1;
		break;
	case 2:
		address = 0x40 + col - 1;
		break;
	case 3:
		address = 0x14 + col - 1;
		break;
	case 4:
		address = 0x54 + col - 1;
		break;
	}
	Lcd_Send_Cmd(0x80 | address);
}

void Lcd_Send_String(char *str){
	while (*str)
		Lcd_Send_Char(*str++);
}

void Lcd_Shift_Right(void){
	Lcd_Send_Cmd(0x1C);
}

void Lcd_Shift_Left(void){
	Lcd_Send_Cmd(0x18);
}

void Lcd_Blink(void){
	Lcd_Send_Cmd(0x0F);
}

void Lcd_NoBlink(void){
	Lcd_Send_Cmd(0x0C);
}

void Lcd_CGRAM_CreateChar(unsigned char pos, const char *msg){

	if (pos < 8){

		Lcd_Send_Cmd(0x40 + (pos * 8));

		for (unsigned char i = 0; i < 8; i++){
			Lcd_Send_Char(msg[i]);
		}
	}
}

void Lcd_CGRAM_WriteChar(char pos){
	Lcd_Send_Char(pos);
}
