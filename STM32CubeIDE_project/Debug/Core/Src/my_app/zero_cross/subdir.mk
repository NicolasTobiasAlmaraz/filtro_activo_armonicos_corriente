################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/my_app/zero_cross/zero_cross.c 

OBJS += \
./Core/Src/my_app/zero_cross/zero_cross.o 

C_DEPS += \
./Core/Src/my_app/zero_cross/zero_cross.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/my_app/zero_cross/%.o Core/Src/my_app/zero_cross/%.su Core/Src/my_app/zero_cross/%.cyclo: ../Core/Src/my_app/zero_cross/%.c Core/Src/my_app/zero_cross/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-my_app-2f-zero_cross

clean-Core-2f-Src-2f-my_app-2f-zero_cross:
	-$(RM) ./Core/Src/my_app/zero_cross/zero_cross.cyclo ./Core/Src/my_app/zero_cross/zero_cross.d ./Core/Src/my_app/zero_cross/zero_cross.o ./Core/Src/my_app/zero_cross/zero_cross.su

.PHONY: clean-Core-2f-Src-2f-my_app-2f-zero_cross

