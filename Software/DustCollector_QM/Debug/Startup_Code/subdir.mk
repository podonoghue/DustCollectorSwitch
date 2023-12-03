################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Startup_Code/console.cpp \
../Startup_Code/ics.cpp \
../Startup_Code/system-gcc.cpp \
../Startup_Code/system.cpp \
../Startup_Code/vectors.cpp 

C_SRCS += \
../Startup_Code/newlib_stubs.c \
../Startup_Code/security.c 

S_UPPER_SRCS += \
../Startup_Code/startup_ARMLtdGCC.S 

CPP_DEPS += \
./Startup_Code/console.d \
./Startup_Code/ics.d \
./Startup_Code/system-gcc.d \
./Startup_Code/system.d \
./Startup_Code/vectors.d 

C_DEPS += \
./Startup_Code/newlib_stubs.d \
./Startup_Code/security.d 

OBJS += \
./Startup_Code/console.o \
./Startup_Code/ics.o \
./Startup_Code/newlib_stubs.o \
./Startup_Code/security.o \
./Startup_Code/startup_ARMLtdGCC.o \
./Startup_Code/system-gcc.o \
./Startup_Code/system.o \
./Startup_Code/vectors.o 


# Each subdirectory must supply rules for building sources it contributes
Startup_Code/%.o: ../Startup_Code/%.cpp Startup_Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

Startup_Code/%.o: ../Startup_Code/%.c Startup_Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpc/ports/arm-cm/qv/gnu" -I"/opt/qp/qpc/include" -std=c17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

Startup_Code/%.o: ../Startup_Code/%.S Startup_Code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -Wall -Wextra -x assembler-with-cpp -DDEBUG_BUILD -I"../Project_Headers" -Wall -Wextra -c -fmessage-length=0  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Startup_Code

clean-Startup_Code:
	-$(RM) ./Startup_Code/console.d ./Startup_Code/console.o ./Startup_Code/ics.d ./Startup_Code/ics.o ./Startup_Code/newlib_stubs.d ./Startup_Code/newlib_stubs.o ./Startup_Code/security.d ./Startup_Code/security.o ./Startup_Code/startup_ARMLtdGCC.o ./Startup_Code/system-gcc.d ./Startup_Code/system-gcc.o ./Startup_Code/system.d ./Startup_Code/system.o ./Startup_Code/vectors.d ./Startup_Code/vectors.o

.PHONY: clean-Startup_Code

