################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lcd8bit/lcd.c 

OBJS += \
./lcd8bit/lcd.o 

C_DEPS += \
./lcd8bit/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
lcd8bit/%.o lcd8bit/%.su lcd8bit/%.cyclo: ../lcd8bit/%.c lcd8bit/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"C:/Users/alifn/STM32CubeIDE/workspace_1.19.0/LCD_LIBRARY/lcd8bit" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lcd8bit

clean-lcd8bit:
	-$(RM) ./lcd8bit/lcd.cyclo ./lcd8bit/lcd.d ./lcd8bit/lcd.o ./lcd8bit/lcd.su

.PHONY: clean-lcd8bit

