################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/display/display_driver/display_driver.c 

OBJS += \
./app/display/display_driver/display_driver.o 

C_DEPS += \
./app/display/display_driver/display_driver.d 


# Each subdirectory must supply rules for building sources it contributes
app/display/display_driver/%.o app/display/display_driver/%.su app/display/display_driver/%.cyclo: ../app/display/display_driver/%.c app/display/display_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-display-2f-display_driver

clean-app-2f-display-2f-display_driver:
	-$(RM) ./app/display/display_driver/display_driver.cyclo ./app/display/display_driver/display_driver.d ./app/display/display_driver/display_driver.o ./app/display/display_driver/display_driver.su

.PHONY: clean-app-2f-display-2f-display_driver

