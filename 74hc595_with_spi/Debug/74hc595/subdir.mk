################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../74hc595/74hc595.c 

OBJS += \
./74hc595/74hc595.o 

C_DEPS += \
./74hc595/74hc595.d 


# Each subdirectory must supply rules for building sources it contributes
74hc595/%.o 74hc595/%.su 74hc595/%.cyclo: ../74hc595/%.c 74hc595/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/alifn/STM32CubeIDE/workspace_1.19.0/74hc595_with_spi/74hc595" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-74hc595

clean-74hc595:
	-$(RM) ./74hc595/74hc595.cyclo ./74hc595/74hc595.d ./74hc595/74hc595.o ./74hc595/74hc595.su

.PHONY: clean-74hc595

