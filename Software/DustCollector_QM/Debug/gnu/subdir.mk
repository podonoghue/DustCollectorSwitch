################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/opt/qp/qpcpp/ports/arm-cm/qv/gnu/qv_port.cpp 

CPP_DEPS += \
./gnu/qv_port.d 

OBJS += \
./gnu/qv_port.o 


# Each subdirectory must supply rules for building sources it contributes
gnu/qv_port.o: /opt/qp/qpcpp/ports/arm-cm/qv/gnu/qv_port.cpp gnu/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-gnu

clean-gnu:
	-$(RM) ./gnu/qv_port.d ./gnu/qv_port.o

.PHONY: clean-gnu

