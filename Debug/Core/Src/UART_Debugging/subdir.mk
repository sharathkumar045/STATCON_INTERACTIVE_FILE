################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/UART_Debugging/UART_Debugging_Functions.c \
../Core/Src/UART_Debugging/UART_Debugging_Variables.c 

OBJS += \
./Core/Src/UART_Debugging/UART_Debugging_Functions.o \
./Core/Src/UART_Debugging/UART_Debugging_Variables.o 

C_DEPS += \
./Core/Src/UART_Debugging/UART_Debugging_Functions.d \
./Core/Src/UART_Debugging/UART_Debugging_Variables.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/UART_Debugging/%.o Core/Src/UART_Debugging/%.su Core/Src/UART_Debugging/%.cyclo: ../Core/Src/UART_Debugging/%.c Core/Src/UART_Debugging/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F030x8 -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/UART1_Slave_Lib -I../Core/Src/UART2_Slave_Lib -I../Core/Src/UART_Debugging -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-UART_Debugging

clean-Core-2f-Src-2f-UART_Debugging:
	-$(RM) ./Core/Src/UART_Debugging/UART_Debugging_Functions.cyclo ./Core/Src/UART_Debugging/UART_Debugging_Functions.d ./Core/Src/UART_Debugging/UART_Debugging_Functions.o ./Core/Src/UART_Debugging/UART_Debugging_Functions.su ./Core/Src/UART_Debugging/UART_Debugging_Variables.cyclo ./Core/Src/UART_Debugging/UART_Debugging_Variables.d ./Core/Src/UART_Debugging/UART_Debugging_Variables.o ./Core/Src/UART_Debugging/UART_Debugging_Variables.su

.PHONY: clean-Core-2f-Src-2f-UART_Debugging

