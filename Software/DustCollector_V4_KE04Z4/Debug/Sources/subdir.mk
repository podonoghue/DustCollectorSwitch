################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/delay.cpp \
../Sources/hardware.cpp \
../Sources/main.cpp \
../Sources/usbdmError.cpp 

CPP_DEPS += \
./Sources/delay.d \
./Sources/hardware.d \
./Sources/main.d \
./Sources/usbdmError.d 

OBJS += \
./Sources/delay.o \
./Sources/hardware.o \
./Sources/main.o \
./Sources/usbdmError.o 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp Sources/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -O0 -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Sources

clean-Sources:
	-$(RM) ./Sources/delay.d ./Sources/delay.o ./Sources/hardware.d ./Sources/hardware.o ./Sources/main.d ./Sources/main.o ./Sources/usbdmError.d ./Sources/usbdmError.o

.PHONY: clean-Sources

