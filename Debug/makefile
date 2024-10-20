################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Session26.elf

dependents:
	-cd C:/Users/Braya/workspace/nucleo-l152re_hal_lib/Debug && $(MAKE) all

# Tool invocations
Session26.elf: $(OBJS) $(USER_OBJS) C:/Users/Braya/workspace/nucleo-l152re_hal_lib/Debug/libnucleo-l152re_hal_lib.a C:\Users\Braya\workspace\Session26/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -L"C:\Users\Braya\workspace\nucleo-l152re_hal_lib\Debug" -T"C:/Users/Braya/workspace/Session26/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "Session26.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "Session26.elf" "Session26.bin"
	arm-none-eabi-size "Session26.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Users/Braya/workspace/nucleo-l152re_hal_lib/Debug/libnucleo-l152re_hal_lib.a:

-include ../makefile.targets
