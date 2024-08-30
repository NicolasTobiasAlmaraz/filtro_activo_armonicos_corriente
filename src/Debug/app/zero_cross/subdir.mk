################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/zero_cross/zero_cross.c 

OBJS += \
./app/zero_cross/zero_cross.o 

C_DEPS += \
./app/zero_cross/zero_cross.d 


# Each subdirectory must supply rules for building sources it contributes
app/zero_cross/%.o app/zero_cross/%.su app/zero_cross/%.cyclo: ../app/zero_cross/%.c app/zero_cross/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-zero_cross

clean-app-2f-zero_cross:
	-$(RM) ./app/zero_cross/zero_cross.cyclo ./app/zero_cross/zero_cross.d ./app/zero_cross/zero_cross.o ./app/zero_cross/zero_cross.su

.PHONY: clean-app-2f-zero_cross

