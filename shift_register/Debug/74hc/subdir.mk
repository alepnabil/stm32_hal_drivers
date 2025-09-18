################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../74hc/74hc.c 

OBJS += \
./74hc/74hc.o 

C_DEPS += \
./74hc/74hc.d 


# Each subdirectory must supply rules for building sources it contributes
74hc/%.o 74hc/%.su 74hc/%.cyclo: ../74hc/%.c 74hc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-74hc

clean-74hc:
	-$(RM) ./74hc/74hc.cyclo ./74hc/74hc.d ./74hc/74hc.o ./74hc/74hc.su

.PHONY: clean-74hc

