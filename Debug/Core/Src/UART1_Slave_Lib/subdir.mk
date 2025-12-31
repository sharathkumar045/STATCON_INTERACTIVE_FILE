################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.c \
../Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.c 

OBJS += \
./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.o \
./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.o 

C_DEPS += \
./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.d \
./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/UART1_Slave_Lib/%.o Core/Src/UART1_Slave_Lib/%.su Core/Src/UART1_Slave_Lib/%.cyclo: ../Core/Src/UART1_Slave_Lib/%.c Core/Src/UART1_Slave_Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DSTM32F030x8 -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DHSI_VALUE=8000000 -DLSI_VALUE=40000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=0 -DDATA_CACHE_ENABLE=0 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/UART1_Slave_Lib -I../Core/Src/UART2_Slave_Lib -I../Core/Src/UART_Debugging -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-UART1_Slave_Lib

clean-Core-2f-Src-2f-UART1_Slave_Lib:
	-$(RM) ./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.cyclo ./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.d ./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.o ./Core/Src/UART1_Slave_Lib/UART1_Slave_Functions.su ./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.cyclo ./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.d ./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.o ./Core/Src/UART1_Slave_Lib/UART1_Slave_Variables.su

.PHONY: clean-Core-2f-Src-2f-UART1_Slave_Lib

