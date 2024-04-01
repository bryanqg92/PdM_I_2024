################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_Delay.c \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_uart.c 

OBJS += \
./Drivers/API/Src/API_Delay.o \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_uart.o 

C_DEPS += \
./Drivers/API/Src/API_Delay.d \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/CMSIS/Include" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/Core/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/API" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/API/Inc" -I"/home/lean/Documents/UBA/CESE/I/PDM/PRACTICA 5/Drivers/API/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_Delay.cyclo ./Drivers/API/Src/API_Delay.d ./Drivers/API/Src/API_Delay.o ./Drivers/API/Src/API_Delay.su ./Drivers/API/Src/API_debounce.cyclo ./Drivers/API/Src/API_debounce.d ./Drivers/API/Src/API_debounce.o ./Drivers/API/Src/API_debounce.su ./Drivers/API/Src/API_uart.cyclo ./Drivers/API/Src/API_uart.d ./Drivers/API/Src/API_uart.o ./Drivers/API/Src/API_uart.su

.PHONY: clean-Drivers-2f-API-2f-Src

