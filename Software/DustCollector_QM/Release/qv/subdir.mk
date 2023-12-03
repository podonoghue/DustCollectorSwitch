################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/opt/qp/qpcpp/src/qv/qv.cpp 

CPP_DEPS += \
./qv/qv.d 

OBJS += \
./qv/qv.o 


# Each subdirectory must supply rules for building sources it contributes
qv/qv.o: /opt/qp/qpcpp/src/qv/qv.cpp qv/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -fcallgraph-info=su -O3 -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DRELEASE_BUILD -DNDEBUG -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-qv

clean-qv:
	-$(RM) ./qv/qv.d ./qv/qv.o

.PHONY: clean-qv

