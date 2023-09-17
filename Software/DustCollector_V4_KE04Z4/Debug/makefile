################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup_Code/subdir.mk
-include Sources/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := DustCollector_V4_KE04Z4
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
MAP_FILE_OUTPUTS += \
DustCollector_V4_KE04Z4.map \

ARCHIVES += \
libUSER_OBJS.a \

SIZE_FILE_OUTPUTS += \
DustCollector_V4_KE04Z4.size \

SYMBOL_LIST_FILE_OUTPUTS += \
DustCollector_V4_KE04Z4.sym \


# All Target
all: main-build

# Main-build Target
main-build: DustCollector_V4_KE04Z4.elf secondary-outputs

# Tool invocations
DustCollector_V4_KE04Z4.elf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -Og -Wl,--gc-sections -L"../Project_Settings/Linker_Files" --specs=nano.specs -Wl,--no-warn-rwx-segments -T "MemoryMap.ld" -Xlinker -Map="DustCollector_V4_KE04Z4.map"  -o "DustCollector_V4_KE04Z4.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DustCollector_V4_KE04Z4.map: DustCollector_V4_KE04Z4.elf $(OBJS) $(USER_OBJS)

libUSER_OBJS.a: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: ARM Archiver'
	arm-none-eabi-ar -r   "libUSER_OBJS.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building: $@'
	@echo ' '

DustCollector_V4_KE04Z4.size: DustCollector_V4_KE04Z4.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: ARM Sizer'
	arm-none-eabi-size "DustCollector_V4_KE04Z4.elf" --format=sysv --radix=10   >"DustCollector_V4_KE04Z4.size"
	@echo 'Finished building: $@'
	@echo ' '

DustCollector_V4_KE04Z4.sym: DustCollector_V4_KE04Z4.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: ARM Symbol Table'
	arm-none-eabi-nm "DustCollector_V4_KE04Z4.elf" --demangle --defined-only --extern-only --numeric-sort   >"DustCollector_V4_KE04Z4.sym"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) DustCollector_V4_KE04Z4.elf DustCollector_V4_KE04Z4.map DustCollector_V4_KE04Z4.size DustCollector_V4_KE04Z4.sym libUSER_OBJS.a
	-@echo ' '

secondary-outputs: $(LIST_FILE_OUTPUTS) $(SIZE_FILE_OUTPUTS) $(SYMBOL_LIST_FILE_OUTPUTS)

.PHONY: all clean dependents main-build

-include ../makefile.targets
