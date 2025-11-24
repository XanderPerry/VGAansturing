################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ub_lib/stm32_ub_vga_screen.c 

OBJS += \
./Core/ub_lib/stm32_ub_vga_screen.o 

C_DEPS += \
./Core/ub_lib/stm32_ub_vga_screen.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ub_lib/%.o Core/ub_lib/%.su Core/ub_lib/%.cyclo: ../Core/ub_lib/%.c Core/ub_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Xande/EE cursusmateriaal/3B/SofOnt/VGAansturingRepo/VGA/Core/ub_lib" -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ub_lib

clean-Core-2f-ub_lib:
	-$(RM) ./Core/ub_lib/stm32_ub_vga_screen.cyclo ./Core/ub_lib/stm32_ub_vga_screen.d ./Core/ub_lib/stm32_ub_vga_screen.o ./Core/ub_lib/stm32_ub_vga_screen.su

.PHONY: clean-Core-2f-ub_lib

