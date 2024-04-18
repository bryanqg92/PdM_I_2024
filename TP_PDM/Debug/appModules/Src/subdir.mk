################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../appModules/Src/LCD_Manager.c \
../appModules/Src/UART_Manager.c \
../appModules/Src/tankLevel.c \
../appModules/Src/valveControl.c 

OBJS += \
./appModules/Src/LCD_Manager.o \
./appModules/Src/UART_Manager.o \
./appModules/Src/tankLevel.o \
./appModules/Src/valveControl.o 

C_DEPS += \
./appModules/Src/LCD_Manager.d \
./appModules/Src/UART_Manager.d \
./appModules/Src/tankLevel.d \
./appModules/Src/valveControl.d 


# Each subdirectory must supply rules for building sources it contributes
appModules/Src/%.o appModules/Src/%.su appModules/Src/%.cyclo: ../appModules/Src/%.c appModules/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/CMSIS/Include" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/Core/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/API" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/API/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/Drivers/API/Src" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/appModules" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/appModules/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/repo/TP_PDM/appModules/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-appModules-2f-Src

clean-appModules-2f-Src:
	-$(RM) ./appModules/Src/LCD_Manager.cyclo ./appModules/Src/LCD_Manager.d ./appModules/Src/LCD_Manager.o ./appModules/Src/LCD_Manager.su ./appModules/Src/UART_Manager.cyclo ./appModules/Src/UART_Manager.d ./appModules/Src/UART_Manager.o ./appModules/Src/UART_Manager.su ./appModules/Src/tankLevel.cyclo ./appModules/Src/tankLevel.d ./appModules/Src/tankLevel.o ./appModules/Src/tankLevel.su ./appModules/Src/valveControl.cyclo ./appModules/Src/valveControl.d ./appModules/Src/valveControl.o ./appModules/Src/valveControl.su

.PHONY: clean-appModules-2f-Src

