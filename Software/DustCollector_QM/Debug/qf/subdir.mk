################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/opt/qp/qpcpp/src/qf/qep_hsm.cpp \
/opt/qp/qpcpp/src/qf/qep_msm.cpp \
/opt/qp/qpcpp/src/qf/qf_act.cpp \
/opt/qp/qpcpp/src/qf/qf_actq.cpp \
/opt/qp/qpcpp/src/qf/qf_defer.cpp \
/opt/qp/qpcpp/src/qf/qf_dyn.cpp \
/opt/qp/qpcpp/src/qf/qf_mem.cpp \
/opt/qp/qpcpp/src/qf/qf_ps.cpp \
/opt/qp/qpcpp/src/qf/qf_qact.cpp \
/opt/qp/qpcpp/src/qf/qf_qeq.cpp \
/opt/qp/qpcpp/src/qf/qf_qmact.cpp \
/opt/qp/qpcpp/src/qf/qf_time.cpp 

CPP_DEPS += \
./qf/qep_hsm.d \
./qf/qep_msm.d \
./qf/qf_act.d \
./qf/qf_actq.d \
./qf/qf_defer.d \
./qf/qf_dyn.d \
./qf/qf_mem.d \
./qf/qf_ps.d \
./qf/qf_qact.d \
./qf/qf_qeq.d \
./qf/qf_qmact.d \
./qf/qf_time.d 

OBJS += \
./qf/qep_hsm.o \
./qf/qep_msm.o \
./qf/qf_act.o \
./qf/qf_actq.o \
./qf/qf_defer.o \
./qf/qf_dyn.o \
./qf/qf_mem.o \
./qf/qf_ps.o \
./qf/qf_qact.o \
./qf/qf_qeq.o \
./qf/qf_qmact.o \
./qf/qf_time.o 


# Each subdirectory must supply rules for building sources it contributes
qf/qep_hsm.o: /opt/qp/qpcpp/src/qf/qep_hsm.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qep_msm.o: /opt/qp/qpcpp/src/qf/qep_msm.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_act.o: /opt/qp/qpcpp/src/qf/qf_act.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_actq.o: /opt/qp/qpcpp/src/qf/qf_actq.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_defer.o: /opt/qp/qpcpp/src/qf/qf_defer.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_dyn.o: /opt/qp/qpcpp/src/qf/qf_dyn.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_mem.o: /opt/qp/qpcpp/src/qf/qf_mem.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_ps.o: /opt/qp/qpcpp/src/qf/qf_ps.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_qact.o: /opt/qp/qpcpp/src/qf/qf_qact.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_qeq.o: /opt/qp/qpcpp/src/qf/qf_qeq.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_qmact.o: /opt/qp/qpcpp/src/qf/qf_qmact.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '

qf/qf_time.o: /opt/qp/qpcpp/src/qf/qf_time.cpp qf/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -g3 -fcallgraph-info=su -Og -ffunction-sections -fdata-sections -fno-rtti -Wall -Wextra -DDEBUG_BUILD -I"../Sources" -I"../Project_Headers" -I"/opt/qp/qpcpp/ports/arm-cm/qv/gnu" -I"/opt/qp/qpcpp/include" -fno-exceptions -std=c++17 -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -o "$@" $<
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-qf

clean-qf:
	-$(RM) ./qf/qep_hsm.d ./qf/qep_hsm.o ./qf/qep_msm.d ./qf/qep_msm.o ./qf/qf_act.d ./qf/qf_act.o ./qf/qf_actq.d ./qf/qf_actq.o ./qf/qf_defer.d ./qf/qf_defer.o ./qf/qf_dyn.d ./qf/qf_dyn.o ./qf/qf_mem.d ./qf/qf_mem.o ./qf/qf_ps.d ./qf/qf_ps.o ./qf/qf_qact.d ./qf/qf_qact.o ./qf/qf_qeq.d ./qf/qf_qeq.o ./qf/qf_qmact.d ./qf/qf_qmact.o ./qf/qf_time.d ./qf/qf_time.o

.PHONY: clean-qf

