################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/profiler/profiler.c 

OBJS += \
./source/profiler/profiler.o 

C_DEPS += \
./source/profiler/profiler.d 


# Each subdirectory must supply rules for building sources it contributes
source/profiler/%.o: ../source/profiler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\board" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\source" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\drivers" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\CMSIS" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\utilities" -I"C:\Users\hemanth\Documents\MCUXpressoIDE_11.4.1_6260\PBKDF2\PBKDF2\startup" -O0 -fno-common -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


