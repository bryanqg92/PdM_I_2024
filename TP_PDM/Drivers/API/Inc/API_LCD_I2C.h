/*
 * API_I2C.h
 *
 *  Created on: Apr 7, 2024
 *      Author: lean
 */

#ifndef API_INC_API_LCD_I2C_H_
#define API_INC_API_LCD_I2C_H_


#define I2Cx                            I2C1
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE();
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define I2Cx_FORCE_RESET()             __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()           __HAL_RCC_I2C1_RELEASE_RESET()

#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1


#define LCD_ADDRESS 0x4E


#include <stdbool.h>

typedef bool bool_t;

bool_t I2C_Init(void);


bool_t Lcd_Init(void);
void Lcd_Send_Cmd(char cmd);
void Lcd_Send_Char(char data);
void Lcd_Send_String(char *str);
void Lcd_Set_Cursor(int row, int col);
void Lcd_Clear(void);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Blink(void);
void Lcd_NoBlink(void);
void Lcd_CGRAM_CreateChar(unsigned char pos, const char*msg);
void Lcd_CGRAM_WriteChar(char pos);





#endif /* API_INC_API_LCD_I2C_H_ */
