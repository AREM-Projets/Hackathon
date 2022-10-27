################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/gpio.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c 

CPP_SRCS += \
../Core/Src/BlocMoteurs.cpp \
../Core/Src/ControleurPID.cpp \
../Core/Src/DefileurTaches.cpp \
../Core/Src/EtatDynamiqueRobot.cpp \
../Core/Src/FileTaches.cpp \
../Core/Src/HardEncodeurs.cpp \
../Core/Src/Odometrie.cpp \
../Core/Src/Trajet.cpp \
../Core/Src/main.cpp \
../Core/Src/sonar.cpp 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/gpio.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d 

OBJS += \
./Core/Src/BlocMoteurs.o \
./Core/Src/ControleurPID.o \
./Core/Src/DefileurTaches.o \
./Core/Src/EtatDynamiqueRobot.o \
./Core/Src/FileTaches.o \
./Core/Src/HardEncodeurs.o \
./Core/Src/Odometrie.o \
./Core/Src/Trajet.o \
./Core/Src/adc.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/sonar.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o 

CPP_DEPS += \
./Core/Src/BlocMoteurs.d \
./Core/Src/ControleurPID.d \
./Core/Src/DefileurTaches.d \
./Core/Src/EtatDynamiqueRobot.d \
./Core/Src/FileTaches.d \
./Core/Src/HardEncodeurs.d \
./Core/Src/Odometrie.d \
./Core/Src/Trajet.d \
./Core/Src/main.d \
./Core/Src/sonar.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/BlocMoteurs.d ./Core/Src/BlocMoteurs.o ./Core/Src/BlocMoteurs.su ./Core/Src/ControleurPID.d ./Core/Src/ControleurPID.o ./Core/Src/ControleurPID.su ./Core/Src/DefileurTaches.d ./Core/Src/DefileurTaches.o ./Core/Src/DefileurTaches.su ./Core/Src/EtatDynamiqueRobot.d ./Core/Src/EtatDynamiqueRobot.o ./Core/Src/EtatDynamiqueRobot.su ./Core/Src/FileTaches.d ./Core/Src/FileTaches.o ./Core/Src/FileTaches.su ./Core/Src/HardEncodeurs.d ./Core/Src/HardEncodeurs.o ./Core/Src/HardEncodeurs.su ./Core/Src/Odometrie.d ./Core/Src/Odometrie.o ./Core/Src/Odometrie.su ./Core/Src/Trajet.d ./Core/Src/Trajet.o ./Core/Src/Trajet.su ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sonar.d ./Core/Src/sonar.o ./Core/Src/sonar.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su

.PHONY: clean-Core-2f-Src

