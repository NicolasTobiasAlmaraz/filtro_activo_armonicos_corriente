################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/my_app/zero_cross/subdir.mk
-include Core/Src/my_app/timer_api/subdir.mk
-include Core/Src/my_app/signal_analyzer/subdir.mk
-include Core/Src/my_app/inject_simulator/subdir.mk
-include Core/Src/my_app/display/display_driver/subdir.mk
-include Core/Src/my_app/display/subdir.mk
-include Core/Src/my_app/current_sensor/subdir.mk
-include Core/Src/my_app/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := filtro_activo_armonicos_corriente
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
filtro_activo_armonicos_corriente.elf \

MAP_FILES += \
filtro_activo_armonicos_corriente.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
filtro_activo_armonicos_corriente.list \


# All Target
all: main-build

# Main-build Target
main-build: filtro_activo_armonicos_corriente.elf secondary-outputs

# Tool invocations
filtro_activo_armonicos_corriente.elf filtro_activo_armonicos_corriente.map: $(OBJS) $(USER_OBJS) C:\Users\User\Documents\Workspaces\STMCubeIDE\filtro_activo_armonicos_corriente\STM32F429ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "filtro_activo_armonicos_corriente.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\User\Documents\Workspaces\STMCubeIDE\filtro_activo_armonicos_corriente\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="filtro_activo_armonicos_corriente.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

filtro_activo_armonicos_corriente.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "filtro_activo_armonicos_corriente.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout filtro_activo_armonicos_corriente.elf filtro_activo_armonicos_corriente.list filtro_activo_armonicos_corriente.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
