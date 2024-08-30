################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/my_app/inject_simulator/inject_simulator.c 

OBJS += \
./Core/Src/my_app/inject_simulator/inject_simulator.o 

C_DEPS += \
./Core/Src/my_app/inject_simulator/inject_simulator.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/my_app/inject_simulator/%.o Core/Src/my_app/inject_simulator/%.su Core/Src/my_app/inject_simulator/%.cyclo: ../Core/Src/my_app/inject_simulator/%.c Core/Src/my_app/inject_simulator/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-my_app-2f-inject_simulator

clean-Core-2f-Src-2f-my_app-2f-inject_simulator:
	-$(RM) ./Core/Src/my_app/inject_simulator/inject_simulator.cyclo ./Core/Src/my_app/inject_simulator/inject_simulator.d ./Core/Src/my_app/inject_simulator/inject_simulator.o ./Core/Src/my_app/inject_simulator/inject_simulator.su

.PHONY: clean-Core-2f-Src-2f-my_app-2f-inject_simulator

