################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/my_app/display/display.c 

OBJS += \
./Core/Src/my_app/display/display.o 

C_DEPS += \
./Core/Src/my_app/display/display.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/my_app/display/%.o Core/Src/my_app/display/%.su Core/Src/my_app/display/%.cyclo: ../Core/Src/my_app/display/%.c Core/Src/my_app/display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-my_app-2f-display

clean-Core-2f-Src-2f-my_app-2f-display:
	-$(RM) ./Core/Src/my_app/display/display.cyclo ./Core/Src/my_app/display/display.d ./Core/Src/my_app/display/display.o ./Core/Src/my_app/display/display.su

.PHONY: clean-Core-2f-Src-2f-my_app-2f-display

