################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/DustCollector.cpp \
../Sources/Sampler.cpp \
../Sources/bsp.cpp \
../Sources/delay.cpp \
../Sources/hardware.cpp \
../Sources/i2c.cpp \
../Sources/main.cpp \
../Sources/spi.cpp \
../Sources/usbdmError.cpp 

CPP_DEPS += \
./Sources/DustCollector.d \
./Sources/Sampler.d \
./Sources/bsp.d \
./Sources/delay.d \
./Sources/hardware.d \
./Sources/i2c.d \
./Sources/main.d \
./Sources/spi.d \
./Sources/usbdmError.d 

OBJS += \
./Sources/DustCollector.o \
./Sources/Sampler.o \
./Sources/bsp.o \
./Sources/delay.o \
./Sources/hardware.o \
./Sources/i2c.o \
./Sources/main.o \
./Sources/spi.o \
./Sources/usbdmError.o 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp Sources/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Sources

clean-Sources:
	-$(RM) ./Sources/DustCollector.d ./Sources/DustCollector.o ./Sources/Sampler.d ./Sources/Sampler.o ./Sources/bsp.d ./Sources/bsp.o ./Sources/delay.d ./Sources/delay.o ./Sources/hardware.d ./Sources/hardware.o ./Sources/i2c.d ./Sources/i2c.o ./Sources/main.d ./Sources/main.o ./Sources/spi.d ./Sources/spi.o ./Sources/usbdmError.d ./Sources/usbdmError.o

.PHONY: clean-Sources

