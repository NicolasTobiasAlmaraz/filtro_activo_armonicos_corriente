################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/my_app/signal_analyzer/signal_analyzer.c 

OBJS += \
./Core/Src/my_app/signal_analyzer/signal_analyzer.o 

C_DEPS += \
./Core/Src/my_app/signal_analyzer/signal_analyzer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/my_app/signal_analyzer/%.o Core/Src/my_app/signal_analyzer/%.su Core/Src/my_app/signal_analyzer/%.cyclo: ../Core/Src/my_app/signal_analyzer/%.c Core/Src/my_app/signal_analyzer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-my_app-2f-signal_analyzer

clean-Core-2f-Src-2f-my_app-2f-signal_analyzer:
	-$(RM) ./Core/Src/my_app/signal_analyzer/signal_analyzer.cyclo ./Core/Src/my_app/signal_analyzer/signal_analyzer.d ./Core/Src/my_app/signal_analyzer/signal_analyzer.o ./Core/Src/my_app/signal_analyzer/signal_analyzer.su

.PHONY: clean-Core-2f-Src-2f-my_app-2f-signal_analyzer

